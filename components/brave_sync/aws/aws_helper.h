/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HELPER_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HELPER_H_

namespace syncer {
class HttpPostProviderFactory;
}  // namespace syncer

namespace brave_sync {
namespace aws {

class AWSHelper {
 public:
  AWSHelper(syncer::HttpPostProviderFactory* factory);
  ~AWSHelper();
};

} // namespace aws
} // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HELPER_H_
