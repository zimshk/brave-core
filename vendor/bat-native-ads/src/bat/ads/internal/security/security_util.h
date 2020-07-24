/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_SECURITY_SECURITY_UTIL_H_
#define BAT_ADS_INTERNAL_SECURITY_SECURITY_UTIL_H_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>

namespace ads {

struct ConfirmationInfo;

namespace security {

std::vector<uint8_t> GenerateSecretKeyFromSeed(
    const std::string& seed_base64);

std::string Sign(
    const std::map<std::string, std::string>& headers,
    const std::string& key_id,
    const std::string& secret_key);

std::vector<uint8_t> Sha256Hash(
    const std::string& value);

bool Verify(
    const ConfirmationInfo& confirmation,
    const std::string& build_channel);

}  // namespace security
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_SECURITY_SECURITY_UTIL_H_
