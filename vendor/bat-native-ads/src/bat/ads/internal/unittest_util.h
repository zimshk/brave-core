/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_UNITTEST_UTIL_H_
#define BAT_ADS_INTERNAL_UNITTEST_UTIL_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_path.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "bat/ads/database.h"
#include "bat/ads/result.h"

namespace ads {

// A list of endpoints where the response can be inline or read from the file
// system. Filenames should begin with forward slash. i.e.
//
//    {
//      "/foo/bar", {
//        {
//          net::HTTP_OK, "The quick brown fox jumps over the lazy dog"
//        }
//      }
//    }
//
// or
//
//    {
//      "/foo/bar", {
//        {
//           net::HTTP_OK, "/response.json"
//        },
//        {
//           net::HTTP_CREATED, "To me there's no creativity without boundaries"
//        }
//      }
//    }
//
// Inline responses can contain |<time:period>| tags for mocking timestamps,
// where |period| should be |now|, |distant_past|, |distant_future|, |+/-#
// seconds|, |+/-# minutes|, |+/-# hours| or |+/-# days|. i.e.
//
//    {
//      "/foo/bar", {
//        {
//          net::HTTP_OK, "An example response with a <time:+7 days> tag"
//        }
//      }
//    }
//
// The same endpoint can be added multiple times where responses are returned in
// the specified order

using URLResponse = std::pair<int, std::string>;
using URLResponses = std::vector<URLResponse>;
using URLEndpoints = std::map<std::string, URLResponses>;

class AdsClientMock;
class AdsImpl;

template<class T>
void Initialize(
    const T& object) {
  object->Initialize(
      [](const Result result) {
    ASSERT_EQ(Result::SUCCESS, result);
  });
}

base::FilePath GetTestPath();

base::FilePath GetResourcesPath();

void MockLoad(
    const std::unique_ptr<AdsClientMock>& mock);

void MockSave(
    const std::unique_ptr<AdsClientMock>& mock);

void MockLoadUserModelForId(
    const std::unique_ptr<AdsClientMock>& mock);

void MockLoadResourceForId(
    const std::unique_ptr<AdsClientMock>& mock);

void MockUrlRequest(
    const std::unique_ptr<AdsClientMock>& mock,
    const URLEndpoints& endpoints);

void MockRunDBTransaction(
    const std::unique_ptr<AdsClientMock>& mock,
    const std::unique_ptr<Database>& database);

int64_t DistantPast();

int64_t DistantFuture();

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_UNITTEST_UTIL_H_
