/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_NAMES_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_NAMES_H_

namespace brave_sync {
namespace prefs {

// String of device id. Supposed to be an integer
extern const char kSyncDeviceId[];
// String of 32 comma separated bytes
// like "145,58,125,111,85,164,236,38,204,67,40,31,182,114,14,152,242,..."
extern const char kSyncSeed[];
// For storing previous seed after reset. It won't be cleared by Clear()
extern const char kSyncPrevSeed[];
// String of current device namefor sync
extern const char kSyncDeviceName[];
// The initial bookmarks order, in a format of
// "<1(desktop)|2(mobile)>.<device_id>.">
extern const char kSyncBookmarksBaseOrder[];
// Boolean, whether sync is enabled for the current device
// If true, then sync is enabled and running
// If false, then sync is not enabled or not running (disabled after enabling,
// but seed and device id are configured)
extern const char kSyncEnabled[];
extern const char kSyncBookmarksEnabled[];
extern const char kSyncSiteSettingsEnabled[];
extern const char kSyncHistoryEnabled[];
// The latest time of synced record, field 'syncTimestamp'
extern const char kSyncLatestRecordTime[];
// The time of latest fetch records operation
extern const char kSyncLastFetchTime[];
// the list of all known sync devices
// TODO(bridiver) - this should be a dictionary - not raw json
extern const char kSyncDeviceList[];
// the sync api version from the server
extern const char kSyncApiVersion[];
// The version of bookmarks state: 0,1,... .
// Current to migrate to is 1.
extern const char kSyncMigrateBookmarksVersion[];

}  // namespace prefs
}  // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_NAMES_H_
