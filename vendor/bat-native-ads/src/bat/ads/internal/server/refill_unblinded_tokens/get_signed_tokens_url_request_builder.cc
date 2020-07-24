/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/refill_unblinded_tokens/get_signed_tokens_url_request_builder.h"

#include "base/strings/stringprintf.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/server/ads_server_util.h"

namespace ads {

GetSignedTokensUrlRequestBuilder::GetSignedTokensUrlRequestBuilder(
    const WalletInfo& wallet,
    const std::string& nonce)
    : wallet_(wallet),
      nonce_(nonce) {
  DCHECK(wallet_.IsValid());
  DCHECK(!nonce_.empty());
}

GetSignedTokensUrlRequestBuilder::~GetSignedTokensUrlRequestBuilder() = default;

// GET /v1/confirmation/token/{payment_id}?nonce={nonce}

UrlRequestPtr GetSignedTokensUrlRequestBuilder::Build() {
  UrlRequestPtr request = UrlRequest::New();
  request->url = BuildUrl();
  request->method = UrlRequestMethod::GET;

  return request;
}

//////////////////////////////////////////////////////////////////////////////

std::string GetSignedTokensUrlRequestBuilder::BuildUrl() const {
  return base::StringPrintf("%s/v1/confirmation/token/%s?nonce=%s",
      server::GetDomain().c_str(), wallet_.payment_id.c_str(), nonce_.c_str());
}

}  // namespace ads
