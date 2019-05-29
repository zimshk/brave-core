/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREFS_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREFS_H_

#include <string>
#include <memory>

#include "base/macros.h"

class PrefService;
class Profile;

namespace base {
class Time;
}

namespace brave_sync {

class Settings;
class SyncDevices;

namespace prefs {

class Prefs {
 public:
  explicit Prefs(PrefService* pref_service);

  std::string GetSeed() const;
  void SetSeed(const std::string& seed);
  std::string GetPrevSeed() const;
  void SetPrevSeed(const std::string& seed);
  std::string GetThisDeviceId() const;
  void SetThisDeviceId(const std::string& device_id);
  std::string GetThisDeviceName() const;
  void SetThisDeviceName(const std::string& device_name);
  std::string GetBookmarksBaseOrder();
  void SetBookmarksBaseOrder(const std::string& order);

  bool GetSyncEnabled() const;
  void SetSyncEnabled(const bool sync_this_device);
  bool GetSyncBookmarksEnabled() const;
  void SetSyncBookmarksEnabled(const bool sync_bookmarks_enabled);
  bool GetSyncSiteSettingsEnabled() const;
  void SetSyncSiteSettingsEnabled(const bool sync_site_settings);
  bool GetSyncHistoryEnabled() const;
  void SetSyncHistoryEnabled(const bool sync_history_enabled);

  void SetLatestRecordTime(const base::Time &time);
  base::Time GetLatestRecordTime();
  void SetLastFetchTime(const base::Time &time);
  base::Time GetLastFetchTime();

  std::unique_ptr<SyncDevices> GetSyncDevices();
  void SetSyncDevices(const SyncDevices& sync_devices);

  std::string GetApiVersion();
  void SetApiVersion(const std::string& api_version);

  std::unique_ptr<Settings> GetBraveSyncSettings() const;

  int GetMigratedBookmarksVersion();
  void SetMigratedBookmarksVersion(const int);

  void Clear();

 private:
  // May be null.
  PrefService* pref_service_;

  DISALLOW_COPY_AND_ASSIGN(Prefs);
};

}  // namespace prefs
}  // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREFS_H_
