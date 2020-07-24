/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens_url_request_builder.h"

#include <utility>

#include "base/json/json_writer.h"
#include "base/strings/stringprintf.h"
#include "wrapper.hpp"  // NOLINT
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/server/ads_server_util.h"

namespace ads {

using challenge_bypass_ristretto::VerificationKey;
using challenge_bypass_ristretto::VerificationSignature;
using challenge_bypass_ristretto::TokenPreimage;

RedeemUnblindedPaymentTokensUrlRequestBuilder::
RedeemUnblindedPaymentTokensUrlRequestBuilder(
    const WalletInfo& wallet,
    const privacy::UnblindedTokenList& unblinded_tokens)
    : wallet_(wallet),
      unblinded_tokens_(unblinded_tokens) {
  DCHECK(wallet_.IsValid());
  DCHECK(!unblinded_tokens_.empty());
}

RedeemUnblindedPaymentTokensUrlRequestBuilder::
~RedeemUnblindedPaymentTokensUrlRequestBuilder() = default;

// PUT /v1/confirmation/payment/{payment_id}

UrlRequestPtr RedeemUnblindedPaymentTokensUrlRequestBuilder::Build() {
  UrlRequestPtr request = UrlRequest::New();
  request->url = BuildUrl();
  request->headers = BuildHeaders();
  const std::string payload = CreatePayload();
  request->content = BuildBody(payload);
  request->content_type = "application/json";
  request->method = UrlRequestMethod::PUT;

  return request;
}

//////////////////////////////////////////////////////////////////////////////

std::string RedeemUnblindedPaymentTokensUrlRequestBuilder::BuildUrl() const {
  return base::StringPrintf("%s/v1/confirmation/payment/%s",
      server::GetDomain().c_str(), wallet_.payment_id.c_str());
}

std::vector<std::string>
RedeemUnblindedPaymentTokensUrlRequestBuilder::BuildHeaders() const {
  return {
    "accept: application/json"
  };
}

std::string RedeemUnblindedPaymentTokensUrlRequestBuilder::BuildBody(
    const std::string& payload) const {
  DCHECK(!payload.empty());

  base::Value dictionary(base::Value::Type::DICTIONARY);

  base::Value payment_request_dto = CreatePaymentRequestDTO(payload);
  dictionary.SetKey("paymentCredentials", std::move(payment_request_dto));

  dictionary.SetKey("payload", base::Value(payload));

  std::string json;
  base::JSONWriter::Write(dictionary, &json);

  return json;
}

std::string
RedeemUnblindedPaymentTokensUrlRequestBuilder::CreatePayload() const {
  base::Value payload(base::Value::Type::DICTIONARY);
  payload.SetKey("paymentId", base::Value(wallet_.payment_id));

  std::string json;
  base::JSONWriter::Write(payload, &json);

  return json;
}

base::Value
RedeemUnblindedPaymentTokensUrlRequestBuilder::CreatePaymentRequestDTO(
    const std::string& payload) const {
  DCHECK(!payload.empty());

  base::Value payment_request_dto(base::Value::Type::LIST);

  for (const auto& unblinded_token : unblinded_tokens_) {
    base::Value payment_credential(base::Value::Type::DICTIONARY);

    base::Value credential = CreateCredential(unblinded_token, payload);
    payment_credential.SetKey("credential", base::Value(std::move(credential)));

    payment_credential.SetKey("publicKey",
        base::Value(unblinded_token.public_key.encode_base64()));

    payment_request_dto.Append(std::move(payment_credential));
  }

  return payment_request_dto;
}

base::Value RedeemUnblindedPaymentTokensUrlRequestBuilder::CreateCredential(
    const privacy::UnblindedTokenInfo& unblinded_token,
    const std::string& payload) const {
  DCHECK(!payload.empty());

  base::Value credential(base::Value::Type::DICTIONARY);

  VerificationKey verification_key =
      unblinded_token.value.derive_verification_key();
  VerificationSignature verification_signature = verification_key.sign(payload);
  const std::string verification_signature_base64 =
      verification_signature.encode_base64();
  credential.SetKey("signature", base::Value(verification_signature_base64));

  TokenPreimage token_preimage = unblinded_token.value.preimage();
  const std::string token_preimage_base64 = token_preimage.encode_base64();
  credential.SetKey("t", base::Value(token_preimage_base64));

  return credential;
}

}  // namespace ads
