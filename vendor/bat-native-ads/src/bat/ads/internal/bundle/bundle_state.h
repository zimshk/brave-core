/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_BUNDLE_BUNDLE_STATE_H_
#define BAT_ADS_INTERNAL_BUNDLE_BUNDLE_STATE_H_

#include <stdint.h>

#include <string>

#include "base/time/time.h"
#include "bat/ads/internal/ad_conversions/ad_conversion_info.h"
#include "bat/ads/internal/bundle/creative_ad_notification_info.h"

namespace ads {

struct BundleState {
  BundleState();
  BundleState(
      const BundleState& state);
  ~BundleState();

  std::string catalog_id;
  uint64_t catalog_version = 0;
  uint64_t catalog_ping = 0;
  base::Time catalog_last_updated;
  CreativeAdNotificationList creative_ad_notifications;
  AdConversionList ad_conversions;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_BUNDLE_BUNDLE_STATE_H_
