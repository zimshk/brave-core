/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens.h"

#include <utility>

#include "brave_base/random.h"
#include "net/http/http_status_code.h"
#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/privacy/unblinded_tokens/unblinded_token_info.h"
#include "bat/ads/internal/server/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens_url_request_builder.h"
#include "bat/ads/internal/time_util.h"

using std::placeholders::_1;

namespace ads {

RedeemUnblindedPaymentTokens::RedeemUnblindedPaymentTokens(
    AdsImpl* ads)
    : ads_(ads) {
  DCHECK(ads_);
}

RedeemUnblindedPaymentTokens::~RedeemUnblindedPaymentTokens() = default;

void RedeemUnblindedPaymentTokens::set_delegate(
    RedeemUnblindedPaymentTokensDelegate* delegate) {
  DCHECK(delegate);

  delegate_ = delegate;
}

void RedeemUnblindedPaymentTokens::RedeemAfterDelay(
    const WalletInfo& wallet) {
  if (retry_timer_.IsRunning()) {
    return;
  }

  wallet_ = wallet;
  if (!wallet_.IsValid()) {
    BLOG(0, "Failed to redeem unblinded payment tokens due to invalid wallet");
    return;
  }

  const uint64_t delay = CalculateTokenRedemptionDelay();

  const base::Time time = timer_.Start(delay,
      base::BindOnce(&RedeemUnblindedPaymentTokens::Redeem,
          base::Unretained(this)));

  BLOG(1, "Redeem unblinded payment tokens " << FriendlyDateAndTime(time));
}

uint64_t RedeemUnblindedPaymentTokens::get_token_redemption_timestamp() const {
  return token_redemption_timestamp_in_seconds_;
}

///////////////////////////////////////////////////////////////////////////////

void RedeemUnblindedPaymentTokens::Redeem() {
  BLOG(1, "RedeemUnblindedPaymentTokens");

  if (ads_->get_confirmations()->get_unblinded_payment_tokens()->IsEmpty()) {
    BLOG(1, "No unblinded payment tokens to redeem");
    ScheduleNextTokenRedemption();
    return;
  }

  BLOG(2, "PUT /v1/confirmation/payment/{payment_id}");

  const privacy::UnblindedTokenList unblinded_tokens =
      ads_->get_confirmations()->get_unblinded_payment_tokens()->GetAllTokens();
  RedeemUnblindedPaymentTokensUrlRequestBuilder
      url_request_builder(wallet_, unblinded_tokens);
  UrlRequestPtr url_request = url_request_builder.Build();
  BLOG(5, UrlRequestToString(url_request));

  auto callback = std::bind(&RedeemUnblindedPaymentTokens::OnRedeem, this, _1);
  ads_->get_ads_client()->UrlRequest(std::move(url_request), callback);
}

void RedeemUnblindedPaymentTokens::OnRedeem(
    const UrlResponse& url_response) {
  BLOG(1, "OnRedeemUnblindedPaymentTokens");

  BLOG(6, UrlResponseToString(url_response));

  if (url_response.status_code != net::HTTP_OK) {
    BLOG(1, "Failed to redeem unblinded payment tokens");
    OnRedeemUnblindedPaymentTokens(FAILED);
    return;
  }

  OnRedeemUnblindedPaymentTokens(SUCCESS);
}

void RedeemUnblindedPaymentTokens::OnRedeemUnblindedPaymentTokens(
    const Result result) {
  if (result != SUCCESS) {
    if (delegate_) {
      delegate_->OnFailedToRedeemUnblindedPaymentTokens();
    }

    const base::Time time = retry_timer_.StartWithPrivacy(
        kRetryRedeemUnblindedPaymentTokensAfterSeconds,
            base::BindOnce(&RedeemUnblindedPaymentTokens::OnRetry,
                base::Unretained(this)));

    BLOG(1, "Retry redeeming unblinded payment tokens "
        << FriendlyDateAndTime(time));

    return;
  }

  const TransactionList unredeemed_transactions =
      ads_->GetUnredeemedTransactions();
  ads_->get_ad_rewards()->SetUnreconciledTransactions(unredeemed_transactions);

  ads_->get_confirmations()->get_unblinded_payment_tokens()->RemoveAllTokens();

  ads_->UpdateAdRewards(true);

  retry_timer_.Stop();

  ScheduleNextTokenRedemption();

  if (delegate_) {
    delegate_->OnDidRedeemUnblindedPaymentTokens();
  }
}

void RedeemUnblindedPaymentTokens::ScheduleNextTokenRedemption() {
  UpdateNextTokenRedemptionDate();
  RedeemAfterDelay(wallet_);
}

void RedeemUnblindedPaymentTokens::OnRetry() {
  if (delegate_) {
    delegate_->OnDidRetryRedeemingUnblindedPaymentTokens();
  }

  Redeem();
}

uint64_t RedeemUnblindedPaymentTokens::CalculateTokenRedemptionDelay() {
  if (token_redemption_timestamp_in_seconds_ == 0) {
    UpdateNextTokenRedemptionDate();
  }

  const uint64_t now_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT());

  uint64_t delay;
  if (now_in_seconds >= token_redemption_timestamp_in_seconds_) {
    // Browser was launched after the token redemption date
    delay = 1 * base::Time::kSecondsPerMinute;
  } else {
    delay = token_redemption_timestamp_in_seconds_ - now_in_seconds;
  }

  return delay;
}

void RedeemUnblindedPaymentTokens::UpdateNextTokenRedemptionDate() {
  const uint64_t now_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT());

  uint64_t delay;

  if (!_is_debug) {
    delay = kNextTokenRedemptionAfterSeconds;
  } else {
    delay = kDebugNextTokenRedemptionAfterSeconds;
  }

  const uint64_t rand_delay = brave_base::random::Geometric(delay);

  token_redemption_timestamp_in_seconds_ = now_in_seconds + rand_delay;
  ads_->get_confirmations()->SaveState();
}

}  // namespace ads
