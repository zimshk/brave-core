/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_LINUX_H_
#define BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_LINUX_H_

#include "bat/ads/internal/platform/platform_helper.h"

#include <string>

namespace ads {

class PlatformHelperLinux : public PlatformHelper {
 public:
  PlatformHelperLinux(const PlatformHelperLinux&) = delete;
  PlatformHelperLinux& operator=(const PlatformHelperLinux&) = delete;

  static PlatformHelperLinux* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<PlatformHelperLinux>;

  PlatformHelperLinux();
  ~PlatformHelperLinux() override;

  // PlatformHelper impl
  bool IsMobile() const override;
  std::string GetPlatformName() const override;
  PlatformType GetPlatform() const override;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_LINUX_H_
