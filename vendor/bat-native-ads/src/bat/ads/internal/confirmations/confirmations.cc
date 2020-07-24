/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/confirmations/confirmations.h"

#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/logging.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace ads {

namespace {
const char kConfirmationsFilename[] = "confirmations.json";
}  // namespace

Confirmations::Confirmations(
    AdsImpl* ads)
    : ads_(ads),
      state_(std::make_unique<ConfirmationsState>(ads_)) {
  DCHECK(ads_);
}

Confirmations::~Confirmations() = default;

void Confirmations::Initialize(
    InitializeCallback callback) {
  callback_ = callback;

  LoadState();
}

CatalogIssuersInfo Confirmations::GetCatalogIssuers() const {
  return state_->get_catalog_issuers();
}

void Confirmations::SetCatalogIssuers(
    const CatalogIssuersInfo& catalog_issuers) {
  BLOG(1, "SetCatalogIssuers:");
  BLOG(1, "  Public key: " << catalog_issuers.public_key);
  BLOG(1, "  Issuers:");

  for (const auto& issuer : catalog_issuers.issuers) {
    BLOG(1, "    Name: " << issuer.name);
    BLOG(1, "    Public key: " << issuer.public_key);
  }

  const CatalogIssuersInfo current_catalog_issuers =
      state_->get_catalog_issuers();

  const bool public_key_was_rotated =
      !current_catalog_issuers.public_key.empty() &&
          current_catalog_issuers.public_key != catalog_issuers.public_key;

  state_->set_catalog_issuers(catalog_issuers);

  if (public_key_was_rotated) {
    state_->get_unblinded_tokens()->RemoveAllTokens();
  }

  SaveState();
}

ConfirmationList Confirmations::GetConfirmations() const {
  return state_->get_confirmations();
}

void Confirmations::SetConfirmations(
    const ConfirmationList& confirmations) {
  state_->set_confirmations(confirmations);
  SaveState();
}

void Confirmations::AppendConfirmationToRetryQueue(
    const ConfirmationInfo& confirmation) {
  state_->append_confirmation(confirmation);
  SaveState();

  BLOG(1, "Added confirmation id " << confirmation.id << ", creative instance "
      "id " << confirmation.creative_instance_id << " and "
          << std::string(confirmation.type) << " to the confirmations queue");

  RetryFailedConfirmationsAfterDelay();
}

void Confirmations::RemoveConfirmationFromRetryQueue(
    const ConfirmationInfo& confirmation) {
  if (!state_->remove_confirmation(confirmation)) {
    BLOG(0, "Failed to remove confirmation id " << confirmation.id
        << ", creative instance id " << confirmation.creative_instance_id
            << " and " << std::string(confirmation.type) << " from "
                "the confirmations queue");

    return;
  }

  BLOG(1, "Removed confirmation id " << confirmation.id << ", creative "
      "instance id " << confirmation.creative_instance_id << " and " <<
          std::string(confirmation.type) << " from the confirmations queue");

  SaveState();
}

TransactionList Confirmations::GetTransactions() const {
  return state_->get_transactions();
}

void Confirmations::SetTransactions(
    const TransactionList& transactions) {
  state_->set_transactions(transactions);
  SaveState();
}

void Confirmations::AppendTransaction(
    const double estimated_redemption_value,
    const ConfirmationType confirmation_type) {
  TransactionInfo transaction;

  transaction.timestamp_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT());
  transaction.estimated_redemption_value = estimated_redemption_value;
  transaction.confirmation_type = std::string(confirmation_type);

  state_->append_transaction(transaction);
  SaveState();

  ads_->get_ads_client()->OnAdRewardsChanged();
}

uint64_t Confirmations::GetNextTokenRedemptionTimestamp() const {
  return state_->get_next_token_redemption_timestamp();
}

void Confirmations::SetNextTokenRedemptionTimestamp(
    const uint64_t next_token_redemption_timestamp) {
  state_->set_next_token_redemption_timestamp(next_token_redemption_timestamp);
  SaveState();
}

privacy::UnblindedTokens* Confirmations::get_unblinded_tokens() {
  return state_->get_unblinded_tokens();
}

privacy::UnblindedTokens* Confirmations::get_unblinded_payment_tokens() {
  return state_->get_unblinded_payment_tokens();
}

///////////////////////////////////////////////////////////////////////////////

void Confirmations::SaveState() {
  if (!is_initialized_) {
    return;
  }

  BLOG(3, "Saving confirmations state");

  const std::string json = state_->ToJson();
  auto callback = std::bind(&Confirmations::OnStateSaved, this, _1);
  ads_->get_ads_client()->Save(kConfirmationsFilename, json, callback);
}

void Confirmations::OnStateSaved(
    const Result result) {
  if (result != SUCCESS) {
    BLOG(0, "Failed to save confirmations state");

    return;
  }

  BLOG(3, "Successfully saved confirmations state");
}

void Confirmations::LoadState() {
  BLOG(3, "Loading confirmations state");

  auto callback = std::bind(&Confirmations::OnStateLoaded, this, _1, _2);
  ads_->get_ads_client()->Load(kConfirmationsFilename, callback);
}

void Confirmations::OnStateLoaded(
    const Result result,
    const std::string& json) {
  if (result != SUCCESS) {
    BLOG(3, "Confirmations state does not exist, creating default state");

    is_initialized_ = true;

    state_.reset(new ConfirmationsState(ads_));
    SaveState();
  } else {
    if (!state_->FromJson(json)) {
      BLOG(0, "Failed to load confirmations state");

      BLOG(3, "Failed to parse confirmations state: " << json);

      callback_(FAILED);
      return;
    }

    BLOG(3, "Successfully loaded confirmations state");

    is_initialized_ = true;
  }

  callback_(SUCCESS);
}

void Confirmations::RetryFailedConfirmationsAfterDelay() {
  if (failed_confirmations_timer_.IsRunning()) {
    return;
  }

  const base::Time time = failed_confirmations_timer_.StartWithPrivacy(
      kRetryFailedConfirmationsAfterSeconds,
          base::BindOnce(&Confirmations::RetryFailedConfirmations,
              base::Unretained(this)));

  BLOG(1, "Retry failed confirmations " << FriendlyDateAndTime(time));
}

void Confirmations::RetryFailedConfirmations() {
  ConfirmationList confirmations = GetConfirmations();
  if (confirmations.empty()) {
    BLOG(1, "No failed confirmations to retry");
    return;
  }

  ConfirmationInfo confirmation(confirmations.front());
  RemoveConfirmationFromRetryQueue(confirmation);

  ads_->get_redeem_unblinded_token()->Redeem(confirmation);

  RetryFailedConfirmationsAfterDelay();
}

}  // namespace ads
