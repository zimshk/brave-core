/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_STATIC_VALUES_H_
#define BAT_ADS_INTERNAL_STATIC_VALUES_H_

#include <stdint.h>

#include "base/time/time.h"

namespace ads {

const int kIdleThresholdInSeconds = 15;

const uint64_t kMaximumPageProbabilityHistoryEntries = 5;
const int kTopWinningCategoryCountForServingAds = 3;

// Maximum entries based upon 7 days of history, 20 ads per day and 4
// confirmation types
const uint64_t kMaximumEntriesInAdsShownHistory = 7 * (20 * 4);

const uint64_t kMaximumEntriesPerSegmentInPurchaseIntentSignalHistory = 100;

const uint64_t kDebugOneHourInSeconds = 10 * base::Time::kSecondsPerMinute;

const uint64_t kSustainAdNotificationInteractionAfterSeconds = 10;

const uint64_t kDefaultCatalogPing = 2 * base::Time::kSecondsPerHour;
const uint64_t kDebugCatalogPing = 15 * base::Time::kSecondsPerMinute;

const uint64_t kDefaultFetchSubdivisionTargetingPing =
    24 * base::Time::kSecondsPerHour;
const uint64_t kDebugFetchSubdivisionTargetingPing =
    5 * base::Time::kSecondsPerMinute;

const int kDebugAdConversionFrequency = 10 * base::Time::kSecondsPerMinute;
const int kAdConversionFrequency =
    base::Time::kHoursPerDay * base::Time::kSecondsPerHour;
const int kExpiredAdConversionFrequency = 5 * base::Time::kSecondsPerMinute;

const uint16_t kPurchaseIntentMaxSegments = 3;

const int kDoNotDisturbFromHour = 21;  // 9pm
const int kDoNotDisturbToHour = 6;     // 6am

const uint64_t kRetryDownloadingCatalogAfterSeconds =
    1 * base::Time::kSecondsPerMinute;
const uint64_t kRetryFetchSubdivisionTargetingAfterSeconds =
    1 * base::Time::kSecondsPerMinute;

const int kNextPaymentDay = 5;

const int kMinimumUnblindedTokens = 20;
const int kMaximumUnblindedTokens = 50;

const uint64_t kNextTokenRedemptionAfterSeconds =
    24 * base::Time::kSecondsPerHour;
const uint64_t kDebugNextTokenRedemptionAfterSeconds =
    25 * base::Time::kSecondsPerMinute;

const uint64_t kRetryRefillUnblindedTokensAfterSeconds = 15;

const uint64_t kRetryRedeemUnblindedPaymentTokensAfterSeconds =
    1 * base::Time::kSecondsPerMinute;

const uint64_t kRetryAdRewardsAfterSeconds =
    1 * base::Time::kSecondsPerMinute;

const uint64_t kRetryFailedConfirmationsAfterSeconds =
    5 * base::Time::kSecondsPerMinute;

const char kUntargetedPageClassification[] = "untargeted";

#if defined(OS_ANDROID)
const int kMaximumAdNotifications = 3;
#else
const int kMaximumAdNotifications = 0;  // No limit
#endif

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_STATIC_VALUES_H_
