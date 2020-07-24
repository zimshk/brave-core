/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/unittest_util.h"

#include <stdint.h>

#include <limits>
#include <vector>

#include "base/base_paths.h"
#include "base/files/file_util.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/string16.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "net/http/http_status_code.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "third_party/re2/src/re2/re2.h"
#include "url/gurl.h"
#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/time_util.h"
#include "bat/ads/internal/url_util.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SetArgPointee;

namespace ads {

namespace {

static std::map<std::string, uint16_t> indexes;

const char kNowTagValue[] = "now";
const char kDistantPastTagValue[] = "distant_past";
const char kDistantFutureTagValue[] = "distant_future";
const char kFromSecondsTagValue[] = "seconds";
const char kFromMinutesTagValue[] = "minutes";
const char kFromHoursTagValue[] = "hours";
const char kFromDaysTagValue[] = "days";

bool ParseTimeDelta(
    const std::string& value,
    base::TimeDelta* time_delta) {
  DCHECK(time_delta);

  const std::vector<std::string> components = base::SplitString(value,
      " ", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  int offset;
  if (!base::StringToInt(components.at(0), &offset)) {
    return false;
  }

  const std::string period = components.at(1);
  if (period == kFromSecondsTagValue) {
    *time_delta = base::TimeDelta::FromSeconds(offset);
  } else if (period == kFromMinutesTagValue) {
    *time_delta = base::TimeDelta::FromMinutes(offset);
  } else if (period == kFromHoursTagValue) {
    *time_delta = base::TimeDelta::FromHours(offset);
  } else if (period == kFromDaysTagValue) {
    *time_delta = base::TimeDelta::FromDays(offset);
  } else {
    return false;
  }

  return true;
}

bool ParseTimeTag(
    std::string* value) {
  DCHECK(value);

  base::Time time;

  if (*value == kNowTagValue) {
    time = base::Time::Now();
  } else if (*value == kDistantPastTagValue) {
    time = base::Time::FromDoubleT(DistantPast());
  } else if (*value == kDistantFutureTagValue) {
    time = base::Time::FromDoubleT(DistantFuture());
  } else if (re2::RE2::FullMatch(*value,
      "[-+]?[0-9]*.*(seconds|minutes|hours|days)")) {
    base::TimeDelta time_delta;

    if (!ParseTimeDelta(*value, &time_delta)) {
      return false;
    }

    time = base::Time::Now() + time_delta;
  } else {
    return false;
  }

  base::Time::Exploded exploded;
  time.UTCExplode(&exploded);

  *value = base::StringPrintf("%04d-%02d-%02dT%02d:%02d:%02dZ",
      exploded.year, exploded.month, exploded.day_of_month, exploded.hour,
          exploded.minute, exploded.second);

  return true;
}

void ParseAndReplaceTags(
    std::string* text) {
  DCHECK(text);

  re2::StringPiece text_string_piece(*text);
  RE2 r("<(.*)>");

  std::string tag;

  while (RE2::FindAndConsume(&text_string_piece, r, &tag)) {
    tag = base::ToLowerASCII(tag);

    const std::vector<std::string> components = base::SplitString(tag, ":",
        base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

    if (components.size() != 2) {
      FAIL() << "Invalid tag: " << tag;
      return;
    }

    const std::string key = components.at(0);
    std::string value = components.at(1);

    if (key == "time") {
      if (!ParseTimeTag(&value)) {
        FAIL() << "Invalid tag: " << tag;
        return;
      }
    } else {
      FAIL() << "Unknown tag: " << tag;
      return;
    }

    const std::string enclosed_tag = base::StringPrintf("<%s>", tag.c_str());
    const std::string escaped_enclosed_tag = RE2::QuoteMeta(enclosed_tag);

    RE2::Replace(text, escaped_enclosed_tag, value);
  }
}

bool GetNextResponse(
    const std::string& url,
    const URLEndpoints& endpoints,
    URLResponse* response) {
  DCHECK(!url.empty());
  DCHECK(!endpoints.empty());
  DCHECK(response);

  const std::string path = GURL(url).PathForRequest();

  const auto iter = endpoints.find(path);
  if (iter == endpoints.end()) {
    // Failed due to unknown endpoint
    return false;
  }

  const URLResponses responses = iter->second;
  if (responses.empty()) {
    // Failed due as no mocked responses were provided
    return false;
  }

  const ::testing::TestInfo* const test_info =
      ::testing::UnitTest::GetInstance()->current_test_info();

  const std::string indexes_path = base::StringPrintf("%s:%s.%s", path.c_str(),
      test_info->test_suite_name(), test_info->name());

  uint16_t index;

  const auto indexes_iter = indexes.find(indexes_path);
  if (indexes_iter == indexes.end()) {
    index = 0;
    indexes.insert({indexes_path, index});
  } else {
    index = indexes_iter->second;
  }

  if (index == responses.size()) {
    // Failed due as out of mocked responses for endpoints
    return false;
  }

  *response = responses.at(index);

  indexes_iter->second++;

  return true;
}

}  // namespace

base::FilePath GetDataPath() {
  base::FilePath path;
  base::PathService::Get(base::DIR_SOURCE_ROOT, &path);
  path = path.AppendASCII("brave");
  path = path.AppendASCII("vendor");
  path = path.AppendASCII("bat-native-ads");
  path = path.AppendASCII("data");
  return path;
}

base::FilePath GetTestPath() {
  base::FilePath path = GetDataPath();
  path = path.AppendASCII("test");
  return path;
}

base::FilePath GetResourcesPath() {
  base::FilePath path = GetDataPath();
  path = path.AppendASCII("resources");
  return path;
}

void MockLoad(
    const std::unique_ptr<AdsClientMock>& mock) {
  ON_CALL(*mock, Load(_, _))
      .WillByDefault(Invoke([](
          const std::string& name,
          LoadCallback callback) {
        base::FilePath path = GetTestPath();
        path = path.AppendASCII(name);

        std::string value;
        if (!base::ReadFileToString(path, &value)) {
          callback(FAILED, value);
          return;
        }

        callback(SUCCESS, value);
      }));
}

void MockSave(
    const std::unique_ptr<AdsClientMock>& mock) {
  ON_CALL(*mock, Save(_, _, _))
      .WillByDefault(Invoke([](
          const std::string& name,
          const std::string& value,
          ResultCallback callback) {
        callback(SUCCESS);
      }));
}

void MockLoadUserModelForId(
    const std::unique_ptr<AdsClientMock>& mock) {
  ON_CALL(*mock, LoadUserModelForId(_, _))
      .WillByDefault(Invoke([](
          const std::string& id,
          LoadCallback callback) {
        base::FilePath path = GetTestPath();
        path = path.AppendASCII("user_models");
        path = path.AppendASCII(id);

        std::string value;
        if (!base::ReadFileToString(path, &value)) {
          callback(FAILED, value);
          return;
        }

        callback(SUCCESS, value);
      }));
}

void MockLoadResourceForId(
    const std::unique_ptr<AdsClientMock>& mock) {
  ON_CALL(*mock, LoadResourceForId(_))
      .WillByDefault(Invoke([](
          const std::string& id) -> std::string {
        base::FilePath path = GetResourcesPath();
        path = path.AppendASCII(id);

        std::string value;
        base::ReadFileToString(path, &value);

        return value;
      }));
}

void MockUrlRequest(
    const std::unique_ptr<AdsClientMock>& mock,
    const URLEndpoints& endpoints) {
  ON_CALL(*mock, UrlRequest(_))
      .WillByDefault(Invoke([&endpoints](
          const UrlRequest& request,
          UrlRequestCallback callback) {
        int status_code = -1;

        std::string body;

        const std::map<std::string, std::string> mapped_headers =
            HeadersToMap(headers);

        URLResponse response;
        if (!GetNextResponse(url, endpoints, &response)) {
          callback(status_code, body, mapped_headers);
          return;
        }

        status_code = response.first;
        if (status_code / 100 != 2) {
          callback(status_code, body, mapped_headers);
          return;
        }

        body = response.second;

        if (base::StartsWith(body, "/", base::CompareCase::INSENSITIVE_ASCII)) {
          const base::StringPiece filename =
              base::TrimString(body, "//", base::TrimPositions::TRIM_LEADING);

          const base::FilePath path = GetTestPath().AppendASCII(filename);
          ASSERT_TRUE(base::ReadFileToString(path, &body));
        }

        ParseAndReplaceTags(&body);

        callback(status_code, body, mapped_headers);
      }));
}

void MockRunDBTransaction(
    const std::unique_ptr<AdsClientMock>& mock,
    const std::unique_ptr<Database>& database) {
  ON_CALL(*mock, RunDBTransaction(_, _))
      .WillByDefault(Invoke([&database](
          DBTransactionPtr transaction,
          RunDBTransactionCallback callback) {
        DBCommandResponsePtr response = DBCommandResponse::New();

        if (!database) {
          response->status = DBCommandResponse::Status::RESPONSE_ERROR;
        } else {
          database->RunTransaction(std::move(transaction), response.get());
        }

        callback(std::move(response));
      }));
}

int64_t DistantPast() {
  return 0;  // Thursday, 1 January 1970 00:00:00 UTC
}

int64_t DistantFuture() {
  return 4102444799;  // Thursday, December 31 2099 23:59:59 UTC
}

}  // namespace ads
