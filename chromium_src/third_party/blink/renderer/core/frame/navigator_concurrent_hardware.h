/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_CONCURRENT_HARDWARE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_CONCURRENT_HARDWARE_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

namespace blink {

class CORE_EXPORT NavigatorConcurrentHardware : public GarbageCollectedMixin {
 public:
  explicit NavigatorConcurrentHardware(LocalFrame*);

  unsigned hardwareConcurrency() const;

  void Trace(Visitor*) override;

 private:
  WeakMember<LocalFrame> frame_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_CONCURRENT_HARDWARE_H_
