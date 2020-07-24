/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ADS_IMPL_H_
#define BAT_ADS_INTERNAL_ADS_IMPL_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/values.h"
#include "bat/ads/ad_notification_info.h"
#include "bat/ads/ads_history.h"
#include "bat/ads/ads.h"
#include "bat/ads/confirmation_type.h"
#include "bat/ads/internal/ad_notifications/ad_notifications.h"
#include "bat/ads/internal/bundle/bundle.h"
#include "bat/ads/internal/bundle/creative_ad_notification_info.h"
#include "bat/ads/internal/catalog/catalog_issuers_info.h"
#include "bat/ads/internal/classification/page_classifier/page_classifier.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_classifier.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_signal_info.h"
#include "bat/ads/internal/client/client.h"
#include "bat/ads/internal/confirmations/confirmation_info.h"
#include "bat/ads/internal/confirmations/confirmations.h"
#include "bat/ads/internal/database/database_initialize.h"
#include "bat/ads/internal/database/tables/creative_ad_notifications_database_table.h"
#include "bat/ads/internal/privacy/unblinded_tokens/unblinded_tokens.h"
#include "bat/ads/internal/server/ad_rewards/ad_rewards.h"
#include "bat/ads/internal/server/get_catalog/get_catalog.h"
#include "bat/ads/internal/server/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens_delegate.h"
#include "bat/ads/internal/server/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens.h"
#include "bat/ads/internal/server/redeem_unblinded_token/redeem_unblinded_token_delegate.h"
#include "bat/ads/internal/server/redeem_unblinded_token/redeem_unblinded_token.h"
#include "bat/ads/internal/server/refill_unblinded_tokens/refill_unblinded_tokens_delegate.h"
#include "bat/ads/internal/server/refill_unblinded_tokens/refill_unblinded_tokens.h"
#include "bat/ads/internal/timer.h"
#include "bat/ads/internal/wallet_info.h"
#include "bat/ads/mojom.h"

namespace ads {

class AdConversions;
class AdNotifications;
class ExclusionRule;
class GetCatalog;
class PermissionRule;
class SubdivisionTargeting;

class AdsImpl
    : public Ads,
      public RedeemUnblindedPaymentTokensDelegate,
      public RedeemUnblindedTokenDelegate,
      public RefillUnblindedTokensDelegate {
 public:
  explicit AdsImpl(
      AdsClient* ads_client);
  ~AdsImpl() override;

  AdsClient* get_ads_client() const;
  Client* get_client() const;
  Confirmations* get_confirmations() const;
  AdRewards* get_ad_rewards() const;
  AdNotifications* get_ad_notifications() const;
  SubdivisionTargeting* get_subdivision_targeting() const;
  AdConversions* get_ad_conversions() const;
  classification::PageClassifier* get_page_classifier() const;
  privacy::UnblindedTokens* get_unblinded_tokens() const;
  privacy::UnblindedTokens* get_unblinded_payment_tokens() const;
  RedeemUnblindedToken* get_redeem_unblinded_token();
  RedeemUnblindedPaymentTokens* get_redeem_unblinded_payment_tokens();
  Bundle* get_bundle() const;

  InitializeCallback initialize_callback_;
  void Initialize(
      InitializeCallback callback) override;
  void InitializeStep2(
      const Result result);
  void InitializeStep3(
      const Result result);
  void InitializeStep4(
      const Result result);
  void InitializeStep5(
      const Result result);
  void InitializeStep6(
      const Result result);
  void InitializeStep7(
      const Result result);
  bool IsInitialized();

  void Shutdown(
      ShutdownCallback callback) override;

  bool is_foreground_;
  void OnForeground() override;
  void OnBackground() override;
  bool IsForeground() const;

  void OnIdle() override;
  void OnUnIdle() override;

  std::set<int32_t> media_playing_;
  void OnMediaPlaying(
      const int32_t tab_id) override;
  void OnMediaStopped(
      const int32_t tab_id) override;
  bool IsMediaPlaying() const;

  bool GetAdNotification(
      const std::string& uuid,
      AdNotificationInfo* info) override;
  void OnAdNotificationEvent(
      const std::string& uuid,
      const AdNotificationEventType event_type) override;

  bool ShouldNotDisturb() const;

  int32_t active_tab_id_;
  std::string active_tab_url_;
  std::string previous_tab_url_;
  void OnTabUpdated(
      const int32_t tab_id,
      const std::string& url,
      const bool is_active,
      const bool is_incognito) override;
  void OnTabClosed(
      const int32_t tab_id) override;

  void OnWalletUpdated(
      const std::string& payment_id,
      const std::string& private_key) override;

  void RemoveAllHistory(
      RemoveAllHistoryCallback callback) override;

  AdsHistory GetAdsHistory(
      const AdsHistory::FilterType filter_type,
      const AdsHistory::SortType sort_type,
      const uint64_t from_timestamp,
      const uint64_t to_timestamp) override;

  AdContent::LikeAction ToggleAdThumbUp(
      const std::string& creative_instance_id,
      const std::string& creative_set_id,
      const AdContent::LikeAction& action) override;
  AdContent::LikeAction ToggleAdThumbDown(
      const std::string& creative_instance_id,
      const std::string& creative_set_id,
      const AdContent::LikeAction& action) override;
  CategoryContent::OptAction ToggleAdOptInAction(
      const std::string& category,
      const CategoryContent::OptAction& action) override;
  CategoryContent::OptAction ToggleAdOptOutAction(
      const std::string& category,
      const CategoryContent::OptAction& action) override;
  bool ToggleSaveAd(
      const std::string& creative_instance_id,
      const std::string& creative_set_id,
      const bool saved) override;
  bool ToggleFlagAd(
      const std::string& creative_instance_id,
      const std::string& creative_set_id,
      const bool flagged) override;

  void ChangeLocale(
      const std::string& locale) override;

  void OnUserModelUpdated(
      const std::string& id) override;
  void LoadPageClassificationUserModel(
      const std::string& locale);
  void LoadPageClassificationUserModelForId(
      const std::string& id);
  void OnLoadPageClassificationUserModelForId(
      const std::string& id,
      const Result result,
      const std::string& json);
  void LoadPurchaseIntentUserModel(
      const std::string& locale);
  void LoadPurchaseIntentUserModelForId(
      const std::string& id);
  void OnLoadPurchaseIntentUserModelForId(
      const std::string& id,
      const Result result,
      const std::string& json);

  void OnAdsSubdivisionTargetingCodeHasChanged() override;

  void OnPageLoaded(
      const std::string& url,
      const std::string& content) override;

  void ExtractPurchaseIntentSignal(
      const std::string& url);
  void GeneratePurchaseIntentSignalHistoryEntry(
      const classification::PurchaseIntentSignalInfo& purchase_intent_signal);
  classification::PurchaseIntentWinningCategoryList
      GetWinningPurchaseIntentCategories();

  void MaybeClassifyPage(
      const std::string& url,
      const std::string& content);

  void MaybeServeAdNotification(
      const bool should_serve);
  void ServeAdNotificationIfReady();
  void ServeAdNotificationFromCategories(
      const classification::CategoryList& categories);
  void OnServeAdNotificationFromCategories(
      const Result result,
      const classification::CategoryList& categories,
      const CreativeAdNotificationList& ads);
  bool ServeAdNotificationFromParentCategories(
      const classification::CategoryList& categories);
  void ServeUntargetedAdNotification();
  void OnServeUntargetedAdNotification(
      const Result result,
      const classification::CategoryList& categories,
      const CreativeAdNotificationList& ads);
  classification::CategoryList GetCategoriesToServeAd();
  void ServeAdNotificationWithPacing(
      const CreativeAdNotificationList& ads);
  void SuccessfullyServedAd();
  void FailedToServeAdNotification(
      const std::string& reason);

  CreativeAdNotificationList GetEligibleAds(
      const CreativeAdNotificationList& ads);
  CreativeAdNotificationList GetUnseenAdsAndRoundRobinIfNeeded(
      const CreativeAdNotificationList& ads) const;
  CreativeAdNotificationList GetUnseenAds(
      const CreativeAdNotificationList& ads) const;
  CreativeAdNotificationList GetAdsForUnseenAdvertisers(
      const CreativeAdNotificationList& ads) const;

  bool IsAdNotificationValid(
      const CreativeAdNotificationInfo& info);
  bool ShowAdNotification(
      const CreativeAdNotificationInfo& info);
  bool IsAllowedToServeAdNotifications();

  Timer deliver_ad_notification_timer_;
  void StartDeliveringAdNotifications();
  void StartDeliveringAdNotificationsAfterSeconds(
      const uint64_t seconds);
  void DeliverAdNotification();

  #if defined(OS_ANDROID)
  void RemoveAllAdNotificationsAfterReboot();
  void RemoveAllAdNotificationsAfterUpdate();
  #endif

  const AdNotificationInfo& get_last_shown_ad_notification() const;
  void set_last_shown_ad_notification(
      const AdNotificationInfo& info);

  void ConfirmAd(
      const AdInfo& info,
      const ConfirmationType confirmation_type);

  void AppendAdNotificationToHistory(
      const AdNotificationInfo& info,
      const ConfirmationType& confirmation_type);

  // Ads rewards
  void UpdateAdRewards(
      const bool should_reconcile) override;

  // Transaction history
  void GetTransactionHistory(
      GetTransactionHistoryCallback callback) override;
  TransactionList GetTransactionHistory(
      const uint64_t from_timestamp_in_seconds,
      const uint64_t to_timestamp_in_seconds);
  TransactionList GetUnredeemedTransactions();

  // Refill tokens
  void StartRetryingToGetRefillSignedTokens(
      const uint64_t start_timer_in);
  void RefillUnblindedTokensIfNecessary() const;

  // Payout tokens
  uint64_t GetNextTokenRedemptionDateInSeconds() const;

  std::unique_ptr<Client> client_;
  std::unique_ptr<Confirmations> confirmations_;
  std::unique_ptr<Bundle> bundle_;
  std::unique_ptr<GetCatalog> get_catalog_;
  std::unique_ptr<SubdivisionTargeting> subdivision_targeting_;
  std::unique_ptr<AdConversions> ad_conversions_;
  std::unique_ptr<database::Initialize> database_;
  std::unique_ptr<classification::PageClassifier> page_classifier_;
  std::unique_ptr<classification::PurchaseIntentClassifier>
      purchase_intent_classifier_;

 private:
  bool is_initialized_;

  AdNotificationInfo last_shown_ad_notification_;
  CreativeAdNotificationInfo last_shown_creative_ad_notification_;
  Timer sustain_ad_notification_interaction_timer_;
  AdNotificationInfo last_sustained_ad_notification_;
  void StartSustainingAdNotificationInteraction();
  void SustainAdNotificationInteractionIfNeeded();
  bool IsStillViewingAdNotification() const;

  std::unique_ptr<AdNotifications> ad_notifications_;

  AdsClient* ads_client_;  // NOT OWNED

  std::vector<std::unique_ptr<PermissionRule>> CreatePermissionRules() const;

  std::vector<std::unique_ptr<ExclusionRule>> CreateExclusionRules() const;

  WalletInfo wallet_;

  std::unique_ptr<AdRewards> ad_rewards_;

  std::unique_ptr<RefillUnblindedTokens> refill_unblinded_tokens_;

  std::unique_ptr<RedeemUnblindedToken> redeem_unblinded_token_;

  std::unique_ptr<RedeemUnblindedPaymentTokens>
      redeem_unblinded_payment_tokens_;

  // RedeemUnblindedTokenDelegate implementation
  void OnDidRedeemUnblindedToken(
      const ConfirmationInfo& confirmation) override;
  void OnFailedToRedeemUnblindedToken(
      const ConfirmationInfo& confirmation) override;

  // RedeemUnblindedPaymentTokensDelegate implementation
  void OnDidRedeemUnblindedPaymentTokens() override;
  void OnFailedToRedeemUnblindedPaymentTokens() override;
  void OnDidRetryRedeemingUnblindedPaymentTokens() override;

  // RefillUnblindedTokensDelegate implementation
  void OnDidRefillUnblindedTokens() override;
  void OnFailedToRefillUnblindedTokens() override;
  void OnDidRetryRefillingUnblindedTokens() override;

  // Not copyable, not assignable
  AdsImpl(const AdsImpl&) = delete;
  AdsImpl& operator=(const AdsImpl&) = delete;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ADS_IMPL_H_
