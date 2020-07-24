/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_
#define BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_

#include <string>

#include "base/memory/singleton.h"

namespace ads {

enum class PlatformType {
  UNKNOWN,
  OS_ANDROID,
  OS_IOS,
  OS_LINUX,
  OS_MACOS,
  OS_WIN
};

class PlatformHelper {
 public:
  PlatformHelper(const PlatformHelper&) = delete;
  PlatformHelper& operator=(const PlatformHelper&) = delete;

  static PlatformHelper* GetInstance();

  void set_for_testing(
      PlatformHelper* platform_helper);

  virtual bool IsMobile() const;
  virtual std::string GetPlatformName() const;
  virtual PlatformType GetPlatform() const;

 protected:
  friend struct base::DefaultSingletonTraits<PlatformHelper>;

  PlatformHelper();
  virtual ~PlatformHelper();

  static PlatformHelper* GetInstanceImpl();
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_
