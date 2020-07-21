/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define BRAVE_WORKER_NAVIGATOR_CONSTRUCTOR \
  NavigatorConcurrentHardware(nullptr),

#define BRAVE_WORKER_NAVIGATOR_TRACE \
  NavigatorConcurrentHardware::Trace(visitor);

#include "../../../../../../third_party/blink/renderer/core/workers/worker_navigator.cc"

#undef BRAVE_WORKER_NAVIGATOR_CONSTRUCTOR
#undef BRAVE_WORKER_NAVIGATOR_TRACE
