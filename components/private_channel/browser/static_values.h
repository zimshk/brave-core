  /* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_PRIVATE_CHANNEL_BROWSER_STATIC_VALUES_H_
#define BRAVE_COMPONENTS_PRIVATE_CHANNEL_BROWSER_STATIC_VALUES_H_

#include <stdint.h>
#include <string>

const int kMaxPrivateChannelServerResponseSizeBytes = 1024 * 1024;

#define PRIVATE_CHANNEL_STAGING_SERVER     \
"https://repsys.rewards.brave.software"
#define PRIVATE_CHANNEL_PRODUCTION_SERVER   \
"https://repsys.rewards.brave.com"
#define PRIVATE_CHANNEL_DEVELOPMENT_SERVER   \
"https://repsys.rewards.brave.software"
#define PRIVATE_CHANNEL_LOCAL_SERVER   \
"http://0.0.0.0:80"

#define PRIVATE_CHANNEL_API_VERSION "/v1"
#define PRIVATE_CHANNEL_META_ENDPOINT "/meta"
#define PRIVATE_CHANNEL_FIRST_ROUND_ENDPOINT "/attestation/start"
#define PRIVATE_CHANNEL_SECOND_ROUND_ENDPOINT "/attestation/result"

static const uint8_t PRIVATE_CHANNEL_SERVER_PK[] = {
  250, 180, 202, 58, 178, 175, 23, 9, 183, 97, 62, 167, 202,
  142, 210, 41, 122, 118, 82, 204, 98, 71, 134, 215, 67, 15,
  128, 124, 217, 120, 172, 93};

#endif  // BRAVE_COMPONENTS_PRIVATE_CHANNEL_BROWSER_STATIC_VALUES_H_
