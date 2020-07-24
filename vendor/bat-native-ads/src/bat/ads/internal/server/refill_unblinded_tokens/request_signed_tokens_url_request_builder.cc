/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/refill_unblinded_tokens/request_signed_tokens_url_request_builder.h"

#include <utility>

#include "base/base64.h"
#include "base/json/json_writer.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/values.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/security/security_util.h"
#include "bat/ads/internal/server/ads_server_util.h"

namespace ads {

RequestSignedTokensUrlRequestBuilder::RequestSignedTokensUrlRequestBuilder(
    const WalletInfo& wallet,
    const std::vector<BlindedToken>& blinded_tokens)
    : wallet_(wallet),
      blinded_tokens_(blinded_tokens) {
  DCHECK(wallet_.IsValid());
  DCHECK(!blinded_tokens_.empty());
}

RequestSignedTokensUrlRequestBuilder::
~RequestSignedTokensUrlRequestBuilder() = default;

// POST /v1/confirmation/token/{payment_id}

UrlRequestPtr RequestSignedTokensUrlRequestBuilder::Build() {
  UrlRequestPtr request = UrlRequest::New();
  request->url = BuildUrl();
  const std::string body = BuildBody();
  request->headers = BuildHeaders(body);
  request->content = body;
  request->content_type = "application/json";
  request->method = UrlRequestMethod::POST;

  return request;
}

//////////////////////////////////////////////////////////////////////////////

std::string RequestSignedTokensUrlRequestBuilder::BuildUrl() const {
  return base::StringPrintf("%s/v1/confirmation/token/%s",
      server::GetDomain().c_str(), wallet_.payment_id.c_str());
}

std::vector<std::string> RequestSignedTokensUrlRequestBuilder::BuildHeaders(
    const std::string& body) const {
  const std::string digest_header_value = BuildDigestHeaderValue(body);
  const std::string digest_header =
      base::StringPrintf("digest: %s", digest_header_value.c_str());

  const std::string signature_header_value = BuildSignatureHeaderValue(body);
  const std::string signature_header =
      base::StringPrintf("signature: %s", signature_header_value.c_str());

  const std::string accept_header = "accept: application/json";

  return {
    digest_header,
    signature_header,
    accept_header
  };
}

std::string RequestSignedTokensUrlRequestBuilder::BuildDigestHeaderValue(
    const std::string& body) const {
  DCHECK(!body.empty());

  const std::vector<uint8_t> body_sha256 = security::Sha256Hash(body);
  const std::string body_sha256_base64 = base::Base64Encode(body_sha256);

  return base::StringPrintf("SHA-256=%s", body_sha256_base64.c_str());
}

std::string RequestSignedTokensUrlRequestBuilder::BuildSignatureHeaderValue(
    const std::string& body) const {
  DCHECK(!body.empty());

  const std::string digest_header_value = BuildDigestHeaderValue(body);

  return security::Sign({{"digest", digest_header_value}},
      "primary", wallet_.secret_key);
}

std::string RequestSignedTokensUrlRequestBuilder::BuildBody() const {
  base::Value list(base::Value::Type::LIST);

  for (const auto& blinded_token : blinded_tokens_) {
    const std::string blinded_token_base64 = blinded_token.encode_base64();
    base::Value blinded_token_base64_value = base::Value(blinded_token_base64);
    list.Append(std::move(blinded_token_base64_value));
  }

  base::Value dictionary(base::Value::Type::DICTIONARY);
  dictionary.SetKey("blindedTokens", base::Value(std::move(list)));

  std::string json;
  base::JSONWriter::Write(dictionary, &json);

  return json;
}

}  // namespace ads
