/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_REGISTRATION_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_REGISTRATION_H_

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace brave_sync {
namespace prefs {

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

}  // namespace prefs
}  // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_PREF_REGISTRATION_H_
