/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/guid.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/rand_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "brave/components/l10n/browser/locale_helper.h"
#include "brave/components/l10n/common/locale_util.h"
#include "url/gurl.h"
#include "bat/ads/ad_history.h"
#include "bat/ads/ads_client.h"
#include "bat/ads/internal/ad_conversions/ad_conversions.h"
#include "bat/ads/internal/ad_events/ad_notification_event_factory.h"
#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/bundle/bundle.h"
#include "bat/ads/internal/classification/page_classifier/page_classifier_user_models.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_classifier_user_models.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_signal_history.h"
#include "bat/ads/internal/eligible_ads/eligible_ads_filter_factory.h"
#include "bat/ads/internal/reports/event_type_blur_info.h"
#include "bat/ads/internal/reports/event_type_destroy_info.h"
#include "bat/ads/internal/reports/event_type_focus_info.h"
#include "bat/ads/internal/reports/event_type_load_info.h"
#include "bat/ads/internal/filters/ads_history_date_range_filter.h"
#include "bat/ads/internal/filters/ads_history_filter_factory.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/conversion_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/daily_cap_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/exclusion_rule.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/marked_as_inappropriate_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/marked_to_no_longer_receive_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/per_day_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/per_hour_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/subdivision_targeting_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/exclusion_rules/total_max_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/permission_rules/ads_per_day_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/permission_rules/ads_per_hour_frequency_cap.h"
#include "bat/ads/internal/frequency_capping/permission_rules/minimum_wait_time_frequency_cap.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/platform/platform_helper.h"
#include "bat/ads/internal/reports/reports.h"
#include "bat/ads/internal/search_engine/search_providers.h"
#include "bat/ads/internal/security/security_util.h"
#include "bat/ads/internal/server/get_subdivision/subdivision_targeting.h"
#include "bat/ads/internal/sorts/ads_history/ads_history_sort_factory.h"
#include "bat/ads/internal/static_values.h"
#include "bat/ads/internal/string_util.h"
#include "bat/ads/internal/time_util.h"
#include "bat/ads/internal/url_util.h"

#if defined(OS_ANDROID)
#include "base/android/build_info.h"
#include "base/system/sys_info.h"
#endif

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using challenge_bypass_ristretto::BlindedToken;
using challenge_bypass_ristretto::PublicKey;
using challenge_bypass_ristretto::Token;
using challenge_bypass_ristretto::UnblindedToken;

namespace ads {

namespace {

const char kCategoryDelimiter = '-';

std::string GetDisplayUrl(const std::string& url) {
  GURL gurl(url);
  if (!gurl.is_valid())
    return std::string();

  return gurl.host();
}

}  // namespace

AdsImpl::AdsImpl(AdsClient* ads_client)
    : is_foreground_(false),
      active_tab_id_(0),
      client_(std::make_unique<Client>(this)),
      confirmations_(std::make_unique<Confirmations>(this)),
      bundle_(std::make_unique<Bundle>(this)),
      get_catalog_(std::make_unique<GetCatalog>(this)),
      subdivision_targeting_(std::make_unique<SubdivisionTargeting>(this)),
      ad_conversions_(std::make_unique<AdConversions>(this)),
      database_(std::make_unique<database::Initialize>(this)),
      page_classifier_(std::make_unique<classification::PageClassifier>(this)),
      purchase_intent_classifier_(
          std::make_unique<classification::PurchaseIntentClassifier>()),
      is_initialized_(false),
      ad_notifications_(std::make_unique<AdNotifications>(this)),
      ads_client_(ads_client),
      ad_rewards_(std::make_unique<AdRewards>(this)),
      refill_unblinded_tokens_(std::make_unique<RefillUnblindedTokens>(this)),
      redeem_unblinded_token_(std::make_unique<RedeemUnblindedToken>(this)),
      redeem_unblinded_payment_tokens_(std::make_unique<
          RedeemUnblindedPaymentTokens>(this)) {
  set_ads_client_for_logging(ads_client_);

  redeem_unblinded_token_->set_delegate(this);
  redeem_unblinded_payment_tokens_->set_delegate(this);
  refill_unblinded_tokens_->set_delegate(this);
}

AdsImpl::~AdsImpl() = default;

AdsClient* AdsImpl::get_ads_client() const {
  return ads_client_;
}

Client* AdsImpl::get_client() const {
  return client_.get();
}

Confirmations* AdsImpl::get_confirmations() const {
  return confirmations_.get();
}

AdRewards* AdsImpl::get_ad_rewards() const {
  return ad_rewards_.get();
}

AdNotifications* AdsImpl::get_ad_notifications() const {
  return ad_notifications_.get();
}

SubdivisionTargeting* AdsImpl::get_subdivision_targeting() const {
  return subdivision_targeting_.get();
}

classification::PageClassifier* AdsImpl::get_page_classifier() const {
  return page_classifier_.get();
}

RedeemUnblindedToken* AdsImpl::get_redeem_unblinded_token() {
  return redeem_unblinded_token_.get();
}

RedeemUnblindedPaymentTokens* AdsImpl::get_redeem_unblinded_payment_tokens() {
  return redeem_unblinded_payment_tokens_.get();
}

Bundle* AdsImpl::get_bundle() const {
  return bundle_.get();
}

AdConversions* AdsImpl::get_ad_conversions() const {
  return ad_conversions_.get();
}

void AdsImpl::Initialize(
    InitializeCallback callback) {
  BLOG(1, "Initializing ads");

  initialize_callback_ = callback;

  if (IsInitialized()) {
    BLOG(1, "Already initialized ads");

    initialize_callback_(FAILED);
    return;
  }

  auto initialize_step_2_callback =
      std::bind(&AdsImpl::InitializeStep2, this, _1);
  database_->CreateOrOpen(initialize_step_2_callback);
}

void AdsImpl::InitializeStep2(
    const Result result) {
  if (result != SUCCESS) {
    BLOG(0, "Failed to initialize database: " << database_->get_last_message());
    initialize_callback_(FAILED);
    return;
  }

  const auto callback = std::bind(&AdsImpl::InitializeStep3, this, _1);
  client_->Initialize(callback);
}

void AdsImpl::InitializeStep3(
    const Result result) {
  if (result != SUCCESS) {
    initialize_callback_(FAILED);
    return;
  }

  const auto callback = std::bind(&AdsImpl::InitializeStep4, this, _1);
  confirmations_->Initialize(callback);
}

void AdsImpl::InitializeStep4(
    const Result result) {
  if (result != SUCCESS) {
    initialize_callback_(FAILED);
    return;
  }

  const auto callback = std::bind(&AdsImpl::InitializeStep5, this, _1);
  ad_notifications_->Initialize(callback);
}

void AdsImpl::InitializeStep5(
    const Result result) {
  if (result != SUCCESS) {
    initialize_callback_(FAILED);
    return;
  }

  const auto callback = std::bind(&AdsImpl::InitializeStep6, this, _1);
  ad_conversions_->Initialize(callback);
}

void AdsImpl::InitializeStep6(
    const Result result) {
  if (result != SUCCESS) {
    initialize_callback_(FAILED);
    return;
  }

  subdivision_targeting_->MaybeFetchForCurrentLocale();

  InitializeStep7(SUCCESS);
}

void AdsImpl::InitializeStep7(
    const Result result) {
  if (result != SUCCESS) {
    initialize_callback_(FAILED);
    return;
  }

  is_initialized_ = true;

  BLOG(1, "Successfully initialized ads");

  is_foreground_ = ads_client_->IsForeground();

  ads_client_->SetIdleThreshold(kIdleThresholdInSeconds);

  initialize_callback_(SUCCESS);

  UpdateAdRewards(true);

  redeem_unblinded_payment_tokens_->RedeemAfterDelay(wallet_);

  ad_conversions_->StartTimerIfReady();

  MaybeServeAdNotification(false);

#if defined(OS_ANDROID)
    // Ad notifications do not sustain a reboot or update, so we should remove
    // orphaned ad notifications
    RemoveAllAdNotificationsAfterReboot();
    RemoveAllAdNotificationsAfterUpdate();
#endif

  client_->UpdateAdUUID();

  if (PlatformHelper::GetInstance()->IsMobile()) {
    if (client_->GetNextCheckServeAdNotificationTimestampInSeconds() == 0) {
      StartDeliveringAdNotificationsAfterSeconds(
          2 * base::Time::kSecondsPerMinute);
    } else {
      StartDeliveringAdNotifications();
    }
  }

  const CatalogIssuersInfo catalog_issuers =
      confirmations_->GetCatalogIssuers();
  if (catalog_issuers.IsValid()) {
    RefillUnblindedTokensIfNecessary();
    confirmations_->RetryFailedConfirmationsAfterDelay();
  }

  get_catalog_->Download();
}

#if defined(OS_ANDROID)
void AdsImpl::RemoveAllAdNotificationsAfterReboot() {
  auto ads_shown_history = client_->GetAdsHistory();
  if (!ads_shown_history.empty()) {
    uint64_t ad_shown_timestamp =
        ads_shown_history.front().timestamp_in_seconds;
    uint64_t boot_timestamp =
        static_cast<uint64_t>(base::Time::Now().ToDoubleT() -
            static_cast<uint64_t>(base::SysInfo::Uptime().InSeconds()));
    if (ad_shown_timestamp <= boot_timestamp) {
      ad_notifications_->RemoveAll(false);
    }
  }
}

void AdsImpl::RemoveAllAdNotificationsAfterUpdate() {
  // Ad notifications do not sustain app update, so remove all ad notifications
  std::string current_version_code(
      base::android::BuildInfo::GetInstance()->package_version_code());
  std::string last_version_code = client_->GetVersionCode();
  if (last_version_code != current_version_code) {
    client_->SetVersionCode(current_version_code);
    ad_notifications_->RemoveAll(false);
  }
}
#endif

bool AdsImpl::IsInitialized() {
  if (!is_initialized_ || !ads_client_->IsEnabled()) {
    return false;
  }

  return true;
}

void AdsImpl::Shutdown(
    ShutdownCallback callback) {
  if (!is_initialized_) {
    BLOG(0, "Shutdown failed as not initialized");

    callback(FAILED);
    return;
  }

  ad_notifications_->RemoveAll(true);

  callback(SUCCESS);
}

bool AdsImpl::GetAdNotification(
    const std::string& uuid,
    AdNotificationInfo* notification) {
  return ad_notifications_->Get(uuid, notification);
}

void AdsImpl::OnForeground() {
  is_foreground_ = true;

  const Reports reports(this);
  const std::string report = reports.GenerateForegroundEventReport();
  BLOG(3, "Event log: " << report);

  if (PlatformHelper::GetInstance()->IsMobile() &&
      !ads_client_->CanShowBackgroundNotifications()) {
    StartDeliveringAdNotifications();
  }
}

void AdsImpl::OnBackground() {
  is_foreground_ = false;

  const Reports reports(this);
  const std::string report = reports.GenerateBackgroundEventReport();
  BLOG(3, "Event log: " << report);

  if (PlatformHelper::GetInstance()->IsMobile() &&
      !ads_client_->CanShowBackgroundNotifications()) {
    deliver_ad_notification_timer_.Stop();
  }
}

bool AdsImpl::IsForeground() const {
  return is_foreground_;
}

void AdsImpl::OnIdle() {
  BLOG(1, "Browser state changed to idle");
}

void AdsImpl::OnUnIdle() {
  if (!IsInitialized()) {
    BLOG(0, "OnUnIdle failed as not initialized");
    return;
  }

  BLOG(1, "Browser state changed to unidle");

  if (PlatformHelper::GetInstance()->IsMobile()) {
    return;
  }

  MaybeServeAdNotification(true);
}

void AdsImpl::OnMediaPlaying(
    const int32_t tab_id) {
  const auto iter = media_playing_.find(tab_id);
  if (iter != media_playing_.end()) {
    // Media is already playing for this tab
    return;
  }

  BLOG(2, "Started playing media for tab id " << tab_id);

  media_playing_.insert(tab_id);
}

void AdsImpl::OnMediaStopped(
    const int32_t tab_id) {
  const auto iter = media_playing_.find(tab_id);
  if (iter == media_playing_.end()) {
    // Media is not playing for this tab
    return;
  }

  BLOG(2, "Stopped playing media for tab id " << tab_id);

  media_playing_.erase(iter);
}

bool AdsImpl::IsMediaPlaying() const {
  const auto iter = media_playing_.find(active_tab_id_);
  if (iter == media_playing_.end()) {
    // Media is not playing in the active tab
    return false;
  }

  return true;
}

void AdsImpl::OnAdNotificationEvent(
    const std::string& uuid,
    const AdNotificationEventType event_type) {
  DCHECK(!uuid.empty());

  AdNotificationInfo info;
  if (!ad_notifications_->Get(uuid, &info)) {
    BLOG(1, "Failed to trigger ad event as an ad notification was not found "
        "for uuid " << uuid);

    return;
  }

  const auto ad_event = AdEventFactory::Build(this, event_type);
  ad_event->Trigger(info);
}

bool AdsImpl::ShouldNotDisturb() const {
  const PlatformType platform = PlatformHelper::GetInstance()->GetPlatform();
  if (platform != PlatformType::OS_ANDROID) {
    return false;
  }

  if (IsForeground()) {
    return false;
  }

  auto now = base::Time::Now();
  base::Time::Exploded now_exploded;
  now.LocalExplode(&now_exploded);

  if (now_exploded.hour >= kDoNotDisturbToHour &&
      now_exploded.hour <= kDoNotDisturbFromHour) {
    return false;
  }

  return true;
}

void AdsImpl::OnTabUpdated(
    const int32_t tab_id,
    const std::string& url,
    const bool is_active,
    const bool is_incognito) {
  if (is_incognito) {
    return;
  }

  if (is_active) {
    BLOG(2, "Tab id " << tab_id << " is visible");

    active_tab_id_ = tab_id;
    previous_tab_url_ = active_tab_url_;
    active_tab_url_ = url;

    const Reports reports(this);
    FocusInfo focus_info;
    focus_info.tab_id = tab_id;
    const std::string report = reports.GenerateFocusEventReport(focus_info);
    BLOG(3, "Event log: " << report);
  } else {
    BLOG(3, "Tab id " << tab_id << " is occluded");

    const Reports reports(this);
    BlurInfo blur_info;
    blur_info.tab_id = tab_id;
    const std::string report = reports.GenerateBlurEventReport(blur_info);
    BLOG(3, "Event log: " << report);
  }
}

void AdsImpl::OnTabClosed(
    const int32_t tab_id) {
  BLOG(2, "Tab id " << tab_id << " was closed");

  OnMediaStopped(tab_id);

  const Reports reports(this);
  DestroyInfo destroy_info;
  destroy_info.tab_id = tab_id;
  const std::string report = reports.GenerateDestroyEventReport(destroy_info);
  BLOG(3, "Event log: " << report);
}

void AdsImpl::OnWalletUpdated(
    const std::string& payment_id,
    const std::string& recovery_seed_base64) {
  const std::vector<uint8_t> secret_key =
      security::GenerateSecretKeyFromSeed(recovery_seed_base64);

  if (secret_key.empty()) {
    BLOG(0, "Invalid wallet secret key");
    return;
  }

  WalletInfo wallet;
  wallet.payment_id = payment_id;
  wallet.secret_key = BytesToHexString(secret_key);

  if (!wallet.IsValid()) {
    BLOG(0, "Invalid wallet");
    return;
  }

  if (wallet == wallet_) {
    return;
  }

  wallet_ = wallet;

  BLOG(1, "OnWalletUpdated:\n" << "  Payment id: " << wallet_.payment_id
      << "\n  Private key: ********");

  if (IsInitialized()) {
    UpdateAdRewards(true);
  }
}

void AdsImpl::RemoveAllHistory(
    RemoveAllHistoryCallback callback) {
  client_->RemoveAllHistory();

  callback(SUCCESS);
}

AdsHistory AdsImpl::GetAdsHistory(
    const AdsHistory::FilterType filter_type,
    const AdsHistory::SortType sort_type,
    const uint64_t from_timestamp,
    const uint64_t to_timestamp) {
  auto history = client_->GetAdsHistory();

  const auto date_range_filter = std::make_unique<AdsHistoryDateRangeFilter>();
  DCHECK(date_range_filter);
  if (date_range_filter) {
    history = date_range_filter->Apply(history, from_timestamp, to_timestamp);
  }

  const auto filter = AdsHistoryFilterFactory::Build(filter_type);
  DCHECK(filter);
  if (filter) {
    history = filter->Apply(history);
  }

  const auto sort = AdsHistorySortFactory::Build(sort_type);
  DCHECK(sort);
  if (sort) {
    history = sort->Apply(history);
  }

  AdsHistory ads_history;
  for (const auto& entry : history) {
    ads_history.entries.push_back(entry);
  }

  return ads_history;
}

AdContent::LikeAction AdsImpl::ToggleAdThumbUp(
    const std::string& creative_instance_id,
    const std::string& creative_set_id,
    const AdContent::LikeAction& action) {
  auto like_action =
      client_->ToggleAdThumbUp(creative_instance_id, creative_set_id, action);
  if (like_action == AdContent::LikeAction::kThumbsUp) {
    AdInfo ad;
    ad.creative_instance_id = creative_instance_id;
    ad.creative_set_id = creative_set_id;

    ConfirmAd(ad, ConfirmationType::kUpvoted);
  }

  return like_action;
}

AdContent::LikeAction AdsImpl::ToggleAdThumbDown(
    const std::string& creative_instance_id,
    const std::string& creative_set_id,
    const AdContent::LikeAction& action) {
  auto like_action =
      client_->ToggleAdThumbDown(creative_instance_id, creative_set_id, action);
  if (like_action == AdContent::LikeAction::kThumbsDown) {
    AdInfo ad;
    ad.creative_instance_id = creative_instance_id;
    ad.creative_set_id = creative_set_id;

    ConfirmAd(ad, ConfirmationType::kDownvoted);
  }

  return like_action;
}

CategoryContent::OptAction AdsImpl::ToggleAdOptInAction(
    const std::string& category,
    const CategoryContent::OptAction& action) {
  return client_->ToggleAdOptInAction(category, action);
}

CategoryContent::OptAction AdsImpl::ToggleAdOptOutAction(
    const std::string& category,
    const CategoryContent::OptAction& action) {
  return client_->ToggleAdOptOutAction(category, action);
}

bool AdsImpl::ToggleSaveAd(
    const std::string& creative_instance_id,
    const std::string& creative_set_id,
    const bool saved) {
  return client_->ToggleSaveAd(creative_instance_id, creative_set_id, saved);
}

bool AdsImpl::ToggleFlagAd(
    const std::string& creative_instance_id,
    const std::string& creative_set_id,
    const bool flagged) {
  auto flag_ad =
      client_->ToggleFlagAd(creative_instance_id, creative_set_id, flagged);
  if (flag_ad) {
    AdInfo ad;
    ad.creative_instance_id = creative_instance_id;
    ad.creative_set_id = creative_set_id;

    ConfirmAd(ad, ConfirmationType::kFlagged);
  }

  return flag_ad;
}

void AdsImpl::ChangeLocale(
    const std::string& locale) {
  subdivision_targeting_->MaybeFetchForLocale(locale);

  LoadPageClassificationUserModel(locale);
  LoadPurchaseIntentUserModel(locale);
}

void AdsImpl::LoadPageClassificationUserModel(
    const std::string& locale) {
  const std::string language_code = brave_l10n::GetLanguageCode(locale);

  const auto iter = kPageClassificationLanguageCodes.find(language_code);
  if (iter == kPageClassificationLanguageCodes.end()) {
    BLOG(1, language_code << " does not support page classification");
    return;
  }

  LoadPageClassificationUserModelForId(iter->second);
}

void AdsImpl::LoadPurchaseIntentUserModel(
    const std::string& locale) {
  const std::string country_code = brave_l10n::GetCountryCode(locale);

  const auto iter = kPurchaseIntentCountryCodes.find(country_code);
  if (iter == kPurchaseIntentCountryCodes.end()) {
    BLOG(1, country_code << " does not support purchase intent");
    return;
  }

  LoadPurchaseIntentUserModelForId(iter->second);
}

void AdsImpl::OnUserModelUpdated(
    const std::string& id) {
  if (kPageClassificationUserModelIds.find(id) !=
      kPageClassificationUserModelIds.end()) {
    LoadPageClassificationUserModelForId(id);
  } else if (kPurchaseIntentUserModelIds.find(id) !=
      kPurchaseIntentUserModelIds.end()) {
    LoadPurchaseIntentUserModelForId(id);
  } else {
    BLOG(1, "Unknown " << id << " user model");
  }
}

void AdsImpl::LoadPageClassificationUserModelForId(
    const std::string& id) {
  auto callback = std::bind(&AdsImpl::OnLoadPageClassificationUserModelForId,
      this, id, _1, _2);

  ads_client_->LoadUserModelForId(id, callback);
}

void AdsImpl::OnLoadPageClassificationUserModelForId(
    const std::string& id,
    const Result result,
    const std::string& json) {
  page_classifier_.reset(
      new classification::PageClassifier(this));

  if (result != SUCCESS) {
    BLOG(1, "Failed to load " << id << " page classification user model");
    return;
  }

  BLOG(1, "Successfully loaded " << id << " page classification user model");

  if (!page_classifier_->Initialize(json)) {
    BLOG(1, "Failed to initialize " << id << " page classification user model");
    return;
  }

  BLOG(1, "Successfully initialized " << id << " page classification user "
      "model");
}

void AdsImpl::LoadPurchaseIntentUserModelForId(
    const std::string& id) {
  auto callback = std::bind(&AdsImpl::OnLoadPurchaseIntentUserModelForId,
      this, id, _1, _2);

  ads_client_->LoadUserModelForId(id, callback);
}

void AdsImpl::OnLoadPurchaseIntentUserModelForId(
    const std::string& id,
    const Result result,
    const std::string& json) {
  purchase_intent_classifier_.reset(
      new classification::PurchaseIntentClassifier());

  if (result != SUCCESS) {
    BLOG(1, "Failed to load " << id << " purchase intent user model");
    return;
  }

  BLOG(1, "Successfully loaded " << id << " purchase intent user model");

  if (!purchase_intent_classifier_->Initialize(json)) {
    BLOG(1, "Failed to initialize " << id << " purchase intent user model");
    return;
  }

  BLOG(1, "Successfully initialized " << id << " purchase intent user model");
}

void AdsImpl::OnAdsSubdivisionTargetingCodeHasChanged() {
  subdivision_targeting_->MaybeFetchForCurrentLocale();
}

void AdsImpl::OnPageLoaded(
    const std::string& url,
    const std::string& content) {
  DCHECK(!url.empty());

  if (!IsInitialized()) {
    BLOG(1, "Failed to classify page as not initialized");
    return;
  }

  const bool is_supported_url = GURL(url).is_valid();

  if (is_supported_url) {
    ad_conversions_->Check(url);
  }

  ExtractPurchaseIntentSignal(url);

  if (SameSite(url, last_shown_ad_notification_.target_url)) {
    BLOG(1, "Visited URL matches the last shown ad notification");

    if (last_sustained_ad_notification_.creative_instance_id !=
        last_shown_ad_notification_.creative_instance_id) {
      last_sustained_ad_notification_ = AdNotificationInfo();
    }

    if (!SameSite(url, last_sustained_ad_notification_.target_url)) {
      last_sustained_ad_notification_ = last_shown_ad_notification_;

      StartSustainingAdNotificationInteraction();
    } else {
      if (sustain_ad_notification_interaction_timer_.IsRunning()) {
        BLOG(1, "Already sustaining ad for visited URL");
      } else {
        BLOG(1, "Already sustained ad for visited URL");
      }
    }

    return;
  }

  if (!last_shown_ad_notification_.target_url.empty()) {
    BLOG(1, "Visited URL does not match the last shown ad notification");
  }

  if (!is_supported_url) {
    BLOG(1, "Page not classified as visited URL is not supported");
    return;
  }

  if (SearchProviders::IsSearchEngine(url)) {
    BLOG(1, "Page not classified as visited URL is a search engine");
    return;
  }

  MaybeClassifyPage(url, content);
}

void AdsImpl::ExtractPurchaseIntentSignal(
    const std::string& url) {
  if (!SearchProviders::IsSearchEngine(url) &&
      SameSite(url, previous_tab_url_)) {
    return;
  }

  classification::PurchaseIntentSignalInfo purchase_intent_signal =
      purchase_intent_classifier_->ExtractIntentSignal(url);

  if (purchase_intent_signal.segments.empty() &&
      purchase_intent_signal.timestamp_in_seconds == 0) {
    return;
  }

  BLOG(1, "Extracting purchase intent signal from visited URL");

  GeneratePurchaseIntentSignalHistoryEntry(purchase_intent_signal);
}

void AdsImpl::GeneratePurchaseIntentSignalHistoryEntry(
    const classification::PurchaseIntentSignalInfo& purchase_intent_signal) {
  for (const auto& segment : purchase_intent_signal.segments) {
    PurchaseIntentSignalHistory history;
    history.timestamp_in_seconds = purchase_intent_signal.timestamp_in_seconds;
    history.weight = purchase_intent_signal.weight;
    client_->AppendToPurchaseIntentSignalHistoryForSegment(segment, history);
  }
}

void AdsImpl::MaybeClassifyPage(
    const std::string& url,
    const std::string& content) {
  std::string page_classification;

  if (page_classifier_->ShouldClassifyPages()) {
    page_classification = page_classifier_->ClassifyPage(url, content);
    if (page_classification.empty()) {
      BLOG(1, "Page not classified as not enough content");
    } else {
      const classification::CategoryList winning_categories =
          page_classifier_->GetWinningCategories();

      BLOG(1, "Classified page as " << page_classification << ". Winning "
          "page classification over time is " << winning_categories.front());
    }
  } else {
    page_classification = kUntargetedPageClassification;
  }

  LoadInfo load_info;
  load_info.tab_id = active_tab_id_;
  load_info.tab_url = active_tab_url_;
  load_info.tab_classification = page_classification;

  const Reports reports(this);
  const std::string report = reports.GenerateLoadEventReport(load_info);
  BLOG(3, "Event log: " << report);
}

classification::PurchaseIntentWinningCategoryList
AdsImpl::GetWinningPurchaseIntentCategories() {
  classification::PurchaseIntentWinningCategoryList winning_categories;

  PurchaseIntentSignalSegmentHistoryMap purchase_intent_signal_history =
      client_->GetPurchaseIntentSignalHistory();
  if (purchase_intent_signal_history.empty()) {
    return winning_categories;
  }

  winning_categories = purchase_intent_classifier_->GetWinningCategories(
      purchase_intent_signal_history, kPurchaseIntentMaxSegments);

  return winning_categories;
}

void AdsImpl::ServeAdNotificationIfReady() {
  if (!IsInitialized()) {
    FailedToServeAdNotification("Not initialized");
    return;
  }

  if (!bundle_->IsReady()) {
    FailedToServeAdNotification("Bundle not ready");
    return;
  }

  const PlatformType platform = PlatformHelper::GetInstance()->GetPlatform();
  if (platform != PlatformType::OS_ANDROID && !IsForeground()) {
    FailedToServeAdNotification("Not in foreground");
    return;
  }

  if (IsMediaPlaying()) {
    FailedToServeAdNotification("Media playing in browser");
    return;
  }

  if (ShouldNotDisturb()) {
    FailedToServeAdNotification("Should not disturb");
    return;
  }

  if (!IsAllowedToServeAdNotifications()) {
    FailedToServeAdNotification("Not allowed based on history");
    return;
  }

  classification::CategoryList categories = GetCategoriesToServeAd();
  ServeAdNotificationFromCategories(categories);
}

classification::CategoryList AdsImpl::GetCategoriesToServeAd() {
  classification::CategoryList categories =
      page_classifier_->GetWinningCategories();

  classification::PurchaseIntentWinningCategoryList purchase_intent_categories =
      GetWinningPurchaseIntentCategories();
  categories.insert(categories.end(),
      purchase_intent_categories.begin(), purchase_intent_categories.end());

  return categories;
}

void AdsImpl::ServeAdNotificationFromCategories(
    const classification::CategoryList& categories) {
  std::string catalog_id = bundle_->GetCatalogId();
  if (catalog_id.empty()) {
    FailedToServeAdNotification("No ad catalog");
    return;
  }

  if (categories.empty()) {
    BLOG(1, "No categories to serve targeted ads");
    ServeUntargetedAdNotification();
    return;
  }

  BLOG(1, "Serving ad from categories:");
  for (const auto& category : categories) {
    BLOG(1, "  " << category);
  }

  const auto callback = std::bind(&AdsImpl::OnServeAdNotificationFromCategories,
      this, _1, _2, _3);

  database::table::CreativeAdNotifications database_table(this);
  database_table.GetCreativeAdNotifications(categories, callback);
}

void AdsImpl::OnServeAdNotificationFromCategories(
    const Result result,
    const classification::CategoryList& categories,
    const CreativeAdNotificationList& ads) {
  auto eligible_ads = GetEligibleAds(ads);
  if (!eligible_ads.empty()) {
    ServeAdNotificationWithPacing(eligible_ads);
    return;
  }

  BLOG(1, "No eligible ads found in categories:");
  for (const auto& category : categories) {
    BLOG(1, "  " << category);
  }

  // TODO(https://github.com/brave/brave-browser/issues/8486): Brave Ads
  // Purchase Intent segments should not fall back to parent segments
  if (ServeAdNotificationFromParentCategories(categories)) {
    return;
  }

  ServeUntargetedAdNotification();
}

bool AdsImpl::ServeAdNotificationFromParentCategories(
    const classification::CategoryList& categories) {
  classification::CategoryList parent_categories;
  for (const auto& category : categories) {
    auto pos = category.find_last_of(kCategoryDelimiter);
    if (pos == std::string::npos) {
      return false;
    }

    std::string parent_category = category.substr(0, pos);

    if (std::find(parent_categories.begin(), parent_categories.end(),
        parent_category) != parent_categories.end()) {
      continue;
    }

    parent_categories.push_back(parent_category);
  }

  BLOG(1, "Serving ad from parent categories:");
  for (const auto& parent_category : parent_categories) {
    BLOG(1, "  " << parent_category);
  }

  const auto callback = std::bind(&AdsImpl::OnServeAdNotificationFromCategories,
      this, _1, _2, _3);

  database::table::CreativeAdNotifications database_table(this);
  database_table.GetCreativeAdNotifications(parent_categories, callback);

  return true;
}

void AdsImpl::ServeUntargetedAdNotification() {
  BLOG(1, "Serving ad notification from untargeted category");

  std::vector<std::string> categories = {
    kUntargetedPageClassification
  };

  const auto callback = std::bind(&AdsImpl::OnServeUntargetedAdNotification,
      this, _1, _2, _3);

  database::table::CreativeAdNotifications database_table(this);
  database_table.GetCreativeAdNotifications(categories, callback);
}

void AdsImpl::OnServeUntargetedAdNotification(
    const Result result,
    const classification::CategoryList& categories,
    const CreativeAdNotificationList& ads) {
  auto eligible_ads = GetEligibleAds(ads);
  if (eligible_ads.empty()) {
    FailedToServeAdNotification("No eligible ads found");
    return;
  }

  ServeAdNotificationWithPacing(eligible_ads);
}

void AdsImpl::ServeAdNotificationWithPacing(
    const CreativeAdNotificationList& ads) {
  const auto filter = EligibleAdsFilterFactory::Build(
      EligibleAdsFilter::Type::kPriority);
  DCHECK(filter);

  const CreativeAdNotificationList eligible_ads = filter->Apply(ads);
  if (eligible_ads.empty()) {
    FailedToServeAdNotification("No eligible ads found");
    return;
  }

  BLOG(1, "Found " << eligible_ads.size() << " eligible ads");

  const int rand = base::RandInt(0, eligible_ads.size() - 1);
  const CreativeAdNotificationInfo ad = eligible_ads.at(rand);

  if (ad.priority == 0) {
    FailedToServeAdNotification("Pacing ad delivery [0]");
    return;
  }

  const int rand_priority = base::RandInt(1, ad.priority);
  if (rand_priority != 1) {
    const std::string message = base::StringPrintf("Pacing ad delivery "
        "[Roll(%d):%d]", ad.priority, rand_priority);

    FailedToServeAdNotification(message);
    return;
  }

  ShowAdNotification(ad);

  SuccessfullyServedAd();
}

void AdsImpl::SuccessfullyServedAd() {
  if (PlatformHelper::GetInstance()->IsMobile()) {
    StartDeliveringAdNotificationsAfterSeconds(
        base::Time::kSecondsPerHour / ads_client_->GetAdsPerHour());
  }
}

void AdsImpl::FailedToServeAdNotification(
    const std::string& reason) {
  BLOG(1, "Ad notification not shown: " << reason);

  if (PlatformHelper::GetInstance()->IsMobile()) {
    StartDeliveringAdNotificationsAfterSeconds(
        2 * base::Time::kSecondsPerMinute);
  }
}

std::vector<std::unique_ptr<ExclusionRule>>
    AdsImpl::CreateExclusionRules() const {
  std::vector<std::unique_ptr<ExclusionRule>> exclusion_rules;

  std::unique_ptr<ExclusionRule> daily_cap_frequency_cap =
      std::make_unique<DailyCapFrequencyCap>(this);
  exclusion_rules.push_back(std::move(daily_cap_frequency_cap));

  std::unique_ptr<ExclusionRule> per_day_frequency_cap =
      std::make_unique<PerDayFrequencyCap>(this);
  exclusion_rules.push_back(std::move(per_day_frequency_cap));

  std::unique_ptr<ExclusionRule> per_hour_frequency_cap =
      std::make_unique<PerHourFrequencyCap>(this);
  exclusion_rules.push_back(std::move(per_hour_frequency_cap));

  std::unique_ptr<ExclusionRule> total_max_frequency_cap =
      std::make_unique<TotalMaxFrequencyCap>(this);
  exclusion_rules.push_back(std::move(total_max_frequency_cap));

  std::unique_ptr<ExclusionRule> conversion_frequency_cap =
      std::make_unique<ConversionFrequencyCap>(this);
  exclusion_rules.push_back(std::move(conversion_frequency_cap));

  std::unique_ptr<ExclusionRule> subdivision_targeting_frequency_cap =
      std::make_unique<SubdivisionTargetingFrequencyCap>(this);
  exclusion_rules.push_back(std::move(subdivision_targeting_frequency_cap));

  std::unique_ptr<ExclusionRule> marked_to_no_longer_recieve_frequency_cap =
      std::make_unique<MarkedToNoLongerReceiveFrequencyCap>(this);
  exclusion_rules.push_back(std::move(
      marked_to_no_longer_recieve_frequency_cap));

  std::unique_ptr<ExclusionRule> marked_as_inappropriate_frequency_cap =
      std::make_unique<MarkedAsInappropriateFrequencyCap>(this);
  exclusion_rules.push_back(std::move(marked_as_inappropriate_frequency_cap));

  return exclusion_rules;
}

CreativeAdNotificationList AdsImpl::GetEligibleAds(
    const CreativeAdNotificationList& ads) {
  CreativeAdNotificationList eligible_ads;

  const auto exclusion_rules = CreateExclusionRules();

  std::set<std::string> exclusions;

  auto unseen_ads = GetUnseenAdsAndRoundRobinIfNeeded(ads);
  for (const auto& ad : unseen_ads) {
    bool should_exclude = false;

    for (const auto& exclusion_rule : exclusion_rules) {
      if (!exclusion_rule->ShouldExclude(ad)) {
        continue;
      }

      exclusions.insert(exclusion_rule->get_last_message());

      should_exclude = true;
    }

    if (should_exclude) {
      continue;
    }

    eligible_ads.push_back(ad);
  }

  for (const auto& exclusion : exclusions) {
    BLOG(2, exclusion);
  }

  return eligible_ads;
}

CreativeAdNotificationList AdsImpl::GetUnseenAdsAndRoundRobinIfNeeded(
    const CreativeAdNotificationList& ads) const {
  if (ads.empty()) {
    return ads;
  }

  CreativeAdNotificationList ads_for_unseen_advertisers =
      GetAdsForUnseenAdvertisers(ads);
  if (ads_for_unseen_advertisers.empty()) {
    BLOG(1, "All advertisers have been shown, so round robin");

    const bool should_not_show_last_advertiser =
        client_->GetSeenAdvertisers().size() > 1 ? true : false;

    client_->ResetSeenAdvertisers(ads);

    ads_for_unseen_advertisers = GetAdsForUnseenAdvertisers(ads);

    if (should_not_show_last_advertiser) {
      const auto it = std::remove_if(ads_for_unseen_advertisers.begin(),
          ads_for_unseen_advertisers.end(),
              [&](CreativeAdNotificationInfo& info) {
        return info.advertiser_id ==
            last_shown_creative_ad_notification_.advertiser_id;
      });

      ads_for_unseen_advertisers.erase(it, ads_for_unseen_advertisers.end());
    }
  }

  CreativeAdNotificationList unseen_ads =
      GetUnseenAds(ads_for_unseen_advertisers);
  if (unseen_ads.empty()) {
    BLOG(1, "All ads have been shown, so round robin");

    const bool should_not_show_last_ad =
        client_->GetSeenAdNotifications().size() > 1 ? true : false;

    client_->ResetSeenAdNotifications(ads);

    unseen_ads = GetUnseenAds(ads);

    if (should_not_show_last_ad) {
      const auto it = std::remove_if(ads_for_unseen_advertisers.begin(),
          ads_for_unseen_advertisers.end(),
              [&](CreativeAdNotificationInfo& info) {
        return info.creative_instance_id ==
            last_shown_creative_ad_notification_.creative_instance_id;
      });

      ads_for_unseen_advertisers.erase(it, ads_for_unseen_advertisers.end());
    }
  }

  return unseen_ads;
}

CreativeAdNotificationList AdsImpl::GetUnseenAds(
    const CreativeAdNotificationList& ads) const {
  auto unseen_ads = ads;
  const auto seen_ads = client_->GetSeenAdNotifications();
  const auto seen_advertisers = client_->GetSeenAdvertisers();

  const auto it = std::remove_if(unseen_ads.begin(), unseen_ads.end(),
      [&](CreativeAdNotificationInfo& info) {
    return seen_ads.find(info.creative_instance_id) != seen_ads.end() &&
        seen_ads.find(info.advertiser_id) != seen_advertisers.end();
  });

  unseen_ads.erase(it, unseen_ads.end());

  return unseen_ads;
}

CreativeAdNotificationList AdsImpl::GetAdsForUnseenAdvertisers(
    const CreativeAdNotificationList& ads) const {
  auto unseen_ads = ads;
  const auto seen_ads = client_->GetSeenAdvertisers();

  const auto it = std::remove_if(unseen_ads.begin(), unseen_ads.end(),
      [&seen_ads](CreativeAdNotificationInfo& info) {
    return seen_ads.find(info.advertiser_id) != seen_ads.end();
  });

  unseen_ads.erase(it, unseen_ads.end());

  return unseen_ads;
}

bool AdsImpl::IsAdNotificationValid(
    const CreativeAdNotificationInfo& info) {
  if (info.title.empty() ||
      info.body.empty() ||
      info.target_url.empty()) {
    BLOG(1, "Ad notification not shown: Incomplete ad information:\n"
        << "  creativeInstanceId: " << info.creative_instance_id << "\n"
        << "  creativeSetId: " << info.creative_set_id << "\n"
        << "  campaignId: " << info.campaign_id << "\n"
        << "  title: " << info.title << "\n"
        << "  body: " << info.body << "\n"
        << "  targetUrl: " << info.target_url);

    return false;
  }

  return true;
}

bool AdsImpl::ShowAdNotification(
    const CreativeAdNotificationInfo& info) {
  if (!IsAdNotificationValid(info)) {
    return false;
  }

  auto now_in_seconds = static_cast<uint64_t>(base::Time::Now().ToDoubleT());

  client_->AppendTimestampToCreativeSetHistory(
      info.creative_set_id, now_in_seconds);
  client_->AppendTimestampToCampaignHistory(
      info.campaign_id, now_in_seconds);

  client_->UpdateSeenAdNotification(info.creative_instance_id, 1);
  client_->UpdateSeenAdvertiser(info.advertiser_id, 1);

  last_shown_creative_ad_notification_ = info;

  auto ad_notification = std::make_unique<AdNotificationInfo>();
  ad_notification->uuid = base::GenerateGUID();
  ad_notification->parent_uuid = base::GenerateGUID();
  ad_notification->creative_instance_id = info.creative_instance_id;
  ad_notification->creative_set_id = info.creative_set_id;
  ad_notification->category = info.category;
  ad_notification->title = info.title;
  ad_notification->body = info.body;
  ad_notification->target_url = info.target_url;
  ad_notification->geo_target = info.geo_targets.at(0);

  BLOG(1, "Ad notification shown:\n"
      << "  uuid: " << ad_notification->uuid << "\n"
      << "  parentUuid: " << ad_notification->parent_uuid << "\n"
      << "  creativeInstanceId: "
          << ad_notification->creative_instance_id << "\n"
      << "  creativeSetId: " << ad_notification->creative_set_id << "\n"
      << "  category: " << ad_notification->category << "\n"
      << "  title: " << ad_notification->title << "\n"
      << "  body: " << ad_notification->body << "\n"
      << "  targetUrl: " << ad_notification->target_url);

  ad_notifications_->PushBack(*ad_notification);

  if (kMaximumAdNotifications > 0 &&
      ad_notifications_->Count() > kMaximumAdNotifications) {
    ad_notifications_->PopFront(true);
  }

  return true;
}

std::vector<std::unique_ptr<PermissionRule>>
    AdsImpl::CreatePermissionRules() const {
  std::vector<std::unique_ptr<PermissionRule>> permission_rules;

  std::unique_ptr<PermissionRule> ads_per_hour_frequency_cap =
      std::make_unique<AdsPerHourFrequencyCap>(this);
  permission_rules.push_back(std::move(ads_per_hour_frequency_cap));

  std::unique_ptr<PermissionRule> minimum_wait_time_frequency_cap =
      std::make_unique<MinimumWaitTimeFrequencyCap>(this);
  permission_rules.push_back(std::move(minimum_wait_time_frequency_cap));

  std::unique_ptr<PermissionRule> ads_per_day_frequency_cap =
      std::make_unique<AdsPerDayFrequencyCap>(this);
  permission_rules.push_back(std::move(ads_per_day_frequency_cap));

  return permission_rules;
}

bool AdsImpl::IsAllowedToServeAdNotifications() {
  const auto permission_rules = CreatePermissionRules();

  bool is_allowed = true;

  for (const auto& permission_rule : permission_rules) {
    if (permission_rule->IsAllowed()) {
      continue;
    }

    BLOG(2, permission_rule->get_last_message());

    is_allowed = false;
  }

  return is_allowed;
}

void AdsImpl::StartDeliveringAdNotifications() {
  auto now_in_seconds = static_cast<uint64_t>(base::Time::Now().ToDoubleT());
  auto next_check_serve_ad_timestamp_in_seconds =
      client_->GetNextCheckServeAdNotificationTimestampInSeconds();

  uint64_t delay;
  if (now_in_seconds >= next_check_serve_ad_timestamp_in_seconds) {
    // Browser was launched after the next check to serve an ad
    delay = 1 * base::Time::kSecondsPerMinute;
  } else {
    delay = next_check_serve_ad_timestamp_in_seconds - now_in_seconds;
  }

  const base::Time time = deliver_ad_notification_timer_.Start(delay,
      base::BindOnce(&AdsImpl::DeliverAdNotification, base::Unretained(this)));

  BLOG(1, "Attempt to deliver next ad notification "
      << FriendlyDateAndTime(time));
}

void AdsImpl::StartDeliveringAdNotificationsAfterSeconds(
    const uint64_t seconds) {
  auto timestamp_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT() + seconds);
  client_->SetNextCheckServeAdNotificationTimestampInSeconds(
      timestamp_in_seconds);

  StartDeliveringAdNotifications();
}

void AdsImpl::DeliverAdNotification() {
  MaybeServeAdNotification(true);
}

void AdsImpl::MaybeServeAdNotification(
    const bool should_serve) {
  auto ok = ads_client_->ShouldShowNotifications();

  auto previous = client_->GetAvailable();

  if (ok != previous) {
    client_->SetAvailable(ok);
  }

  if (!should_serve || ok != previous) {
    const Reports reports(this);
    const std::string report = reports.GenerateSettingsEventReport();
    BLOG(3, "Event log: " << report);
  }

  if (!should_serve) {
    return;
  }

  if (!ok) {
    FailedToServeAdNotification("Notifications not allowed");
    return;
  }

  if (!ads_client_->IsNetworkConnectionAvailable()) {
    FailedToServeAdNotification("Network connection not available");
    return;
  }

  const CatalogIssuersInfo catalog_issuers =
      confirmations_->GetCatalogIssuers();
  if (!catalog_issuers.IsValid()) {
    FailedToServeAdNotification("Catalog issuers not initialized");
    return;
  }

  if (bundle_->IsOlderThanOneDay()) {
    FailedToServeAdNotification("Catalog older than one day");

    get_catalog_->Download();

    return;
  }

  ServeAdNotificationIfReady();
}

const AdNotificationInfo& AdsImpl::get_last_shown_ad_notification() const {
  return last_shown_ad_notification_;
}

void AdsImpl::set_last_shown_ad_notification(
    const AdNotificationInfo& info) {
  last_shown_ad_notification_ = info;
}

void AdsImpl::StartSustainingAdNotificationInteraction() {
  const uint64_t delay = kSustainAdNotificationInteractionAfterSeconds;

  const base::Time time = sustain_ad_notification_interaction_timer_.Start(
      delay, base::BindOnce(&AdsImpl::SustainAdNotificationInteractionIfNeeded,
          base::Unretained(this)));

  BLOG(1, "Start timer to sustain ad for "
      << last_shown_ad_notification_.target_url << " which will trigger "
          << FriendlyDateAndTime(time));
}

void AdsImpl::SustainAdNotificationInteractionIfNeeded() {
  if (!IsStillViewingAdNotification()) {
    BLOG(1, "Failed to sustain ad. The domain for the focused tab does not "
        "match " << last_shown_ad_notification_.target_url << " for the last "
            "shown ad notification");

    return;
  }

  BLOG(1, "Sustained ad for " << last_shown_ad_notification_.target_url);

  ConfirmAd(last_shown_ad_notification_, ConfirmationType::kLanded);
}

bool AdsImpl::IsStillViewingAdNotification() const {
  return SameSite(active_tab_url_, last_shown_ad_notification_.target_url);
}

void AdsImpl::AppendAdNotificationToHistory(
    const AdNotificationInfo& info,
    const ConfirmationType& confirmation_type) {
  AdHistory ad_history;
  ad_history.timestamp_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT());
  ad_history.uuid = base::GenerateGUID();
  ad_history.parent_uuid = info.parent_uuid;
  ad_history.ad_content.creative_instance_id = info.creative_instance_id;
  ad_history.ad_content.creative_set_id = info.creative_set_id;
  ad_history.ad_content.brand = info.title;
  ad_history.ad_content.brand_info = info.body;
  ad_history.ad_content.brand_display_url = GetDisplayUrl(info.target_url);
  ad_history.ad_content.brand_url = info.target_url;
  ad_history.ad_content.ad_action = confirmation_type;
  ad_history.category_content.category = info.category;

  client_->AppendAdHistoryToAdsHistory(ad_history);
}

//////////////////////////////////////////////////////////////////////////////

void AdsImpl::UpdateAdRewards(
    const bool should_reconcile) {
  if (!IsInitialized()) {
    return;
  }

  ad_rewards_->Update(wallet_, should_reconcile);
}

void AdsImpl::GetTransactionHistory(
    GetTransactionHistoryCallback callback) {
  if (!IsInitialized()) {
    return;
  }

  std::unique_ptr<TransactionsInfo> transactions =
      std::make_unique<TransactionsInfo>();

  transactions->estimated_pending_rewards =
      get_ad_rewards()->GetEstimatedPendingRewards();

  transactions->next_payment_date_in_seconds =
      get_ad_rewards()->GetNextPaymentDateInSeconds();

  transactions->ad_notifications_received_this_month =
      get_ad_rewards()->GetAdNotificationsReceivedThisMonth();

  auto to_timestamp_in_seconds =
      static_cast<uint64_t>(base::Time::Now().ToDoubleT());
  const TransactionList transaction_history =
      GetTransactionHistory(0, to_timestamp_in_seconds);
  transactions->transactions = transaction_history;

  callback(std::move(transactions));
}

TransactionList AdsImpl::GetTransactionHistory(
    const uint64_t from_timestamp_in_seconds,
    const uint64_t to_timestamp_in_seconds) {
  TransactionList transactions = confirmations_->GetTransactions();
  TransactionList filtered_transactions(transactions.size());

  auto iter = std::copy_if(transactions.begin(), transactions.end(),
      filtered_transactions.begin(), [=](TransactionInfo& transaction) {
    return transaction.timestamp_in_seconds >= from_timestamp_in_seconds &&
        transaction.timestamp_in_seconds <= to_timestamp_in_seconds;
  });

  filtered_transactions.resize(std::distance(
      filtered_transactions.begin(), iter));

  return filtered_transactions;
}

TransactionList AdsImpl::GetUnredeemedTransactions() {
  auto count = confirmations_->get_unblinded_payment_tokens()->Count();
  if (count == 0) {
    // There are no outstanding unblinded payment tokens to redeem
    return {};
  }

  // Unredeemed transactions are always at the end of the transaction history
  const TransactionList transactions = confirmations_->GetTransactions();

  const TransactionList filtered_transactions(transactions.end() - count,
      transactions.end());

  return filtered_transactions;
}

void AdsImpl::ConfirmAd(
    const AdInfo& ad,
    const ConfirmationType confirmation_type) {
  const Reports reports(this);
  const std::string report = reports.GenerateConfirmationEventReport(
      ad.creative_instance_id, confirmation_type);
  BLOG(3, "Event log: " << report);

  std::string log_message = "Confirm ad:\n";

  log_message += "  creativeInstanceId: ";
  log_message += ad.creative_instance_id;
  log_message += "\n";

  log_message += "  creativeSetId: ";
  log_message += ad.creative_set_id;
  log_message += "\n";

  if (!ad.category.empty()) {
    log_message += "  category: ";
    log_message += ad.category;
    log_message += "\n";
  }

  if (!ad.target_url.empty()) {
    log_message += "  targetUrl: ";
    log_message += ad.target_url;
    log_message += "\n";
  }

  if (!ad.geo_target.empty()) {
    log_message += "  geoTarget: ";
    log_message += ad.geo_target;
    log_message += "\n";
  }

  log_message += "  confirmationType: ";
  log_message += std::string(confirmation_type);
  log_message += "\n";

  BLOG(1, log_message);

  redeem_unblinded_token_->Redeem(ad, confirmation_type);
}

void AdsImpl::RefillUnblindedTokensIfNecessary() const {
  DCHECK(wallet_.IsValid());

  refill_unblinded_tokens_->Refill(wallet_,
      confirmations_->GetCatalogIssuers().public_key);
}

uint64_t AdsImpl::GetNextTokenRedemptionDateInSeconds() const {
  return redeem_unblinded_payment_tokens_->get_token_redemption_timestamp();
}

void AdsImpl::OnDidRedeemUnblindedToken(
    const ConfirmationInfo& confirmation) {
  BLOG(1, "Successfully redeemed unblinded token with confirmation id "
      << confirmation.id << ", creative instance id "
          << confirmation.creative_instance_id << " and "
              << std::string(confirmation.type));
}

void AdsImpl::OnFailedToRedeemUnblindedToken(
    const ConfirmationInfo& confirmation) {
  BLOG(1, "Failed to redeem unblinded token with confirmation id "
      << confirmation.id << ", creative instance id "
          <<  confirmation.creative_instance_id << " and "
              << std::string(confirmation.type));
}

void AdsImpl::OnDidRedeemUnblindedPaymentTokens() {
  BLOG(1, "Successfully redeemed unblinded payment tokens");
}

void AdsImpl::OnFailedToRedeemUnblindedPaymentTokens() {
  BLOG(1, "Failed to redeem unblinded payment tokens");
}

void AdsImpl::OnDidRetryRedeemingUnblindedPaymentTokens() {
  BLOG(1, "Retry redeeming unblinded payment tokens");
}

void AdsImpl::OnDidRefillUnblindedTokens() {
  BLOG(1, "Successfully refilled unblinded tokens");
}

void AdsImpl::OnFailedToRefillUnblindedTokens() {
  BLOG(1, "Failed to refill unblinded tokens");
}

void AdsImpl::OnDidRetryRefillingUnblindedTokens() {
  BLOG(1, "Retry refilling unblinded tokens");
}

}  // namespace ads
