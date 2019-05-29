/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/brave_sync_pref_registration.h"

#include "brave/components/brave_sync/brave_sync_pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"

namespace brave_sync {
namespace prefs {

void RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterStringPref(prefs::kSyncDeviceId, std::string());
  registry->RegisterStringPref(prefs::kSyncSeed, std::string());
  registry->RegisterStringPref(prefs::kSyncPrevSeed, std::string());
  registry->RegisterStringPref(prefs::kSyncDeviceName, std::string());
  registry->RegisterStringPref(prefs::kSyncBookmarksBaseOrder, std::string());

  registry->RegisterBooleanPref(prefs::kSyncEnabled, false);
  registry->RegisterBooleanPref(prefs::kSyncBookmarksEnabled, false);
  registry->RegisterBooleanPref(prefs::kSyncSiteSettingsEnabled, false);
  registry->RegisterBooleanPref(prefs::kSyncHistoryEnabled, false);

  registry->RegisterTimePref(prefs::kSyncLatestRecordTime, base::Time());
  registry->RegisterTimePref(prefs::kSyncLastFetchTime, base::Time());

  registry->RegisterStringPref(prefs::kSyncDeviceList, std::string());
  registry->RegisterStringPref(prefs::kSyncApiVersion, std::string("0"));
  registry->RegisterIntegerPref(prefs::kSyncMigrateBookmarksVersion, 0);
}

}  // namespace prefs
}  // namespace brave_sync
