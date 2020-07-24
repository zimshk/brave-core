/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/backoff_timer.h"

#include <utility>

namespace ads {

BackoffTimer::BackoffTimer() = default;

BackoffTimer::~BackoffTimer() = default;

void BackoffTimer::set_timer_for_testing(
    std::unique_ptr<base::OneShotTimer> timer) {
  timer_.set_timer_for_testing(std::move(timer));
}

base::Time BackoffTimer::Start(
    const uint64_t delay,
    base::OnceClosure user_task) {
  timer_.Stop();

  const uint64_t backoff_delay = CalculateDelay(delay);
  return timer_.Start(backoff_delay, std::move(user_task));
}

base::Time BackoffTimer::StartWithPrivacy(
    const uint64_t delay,
    base::OnceClosure user_task) {
  timer_.Stop();

  const uint64_t backoff_delay = CalculateDelay(delay);
  return timer_.StartWithPrivacy(backoff_delay, std::move(user_task));
}

bool BackoffTimer::IsRunning() const {
  return timer_.IsRunning();
}

void BackoffTimer::Stop() {
  timer_.Stop();

  backoff_count_ = 0;
}

void BackoffTimer::set_max_backoff_delay(
    const uint64_t max_delay) {
  max_backoff_delay_ = max_delay;
}

//////////////////////////////////////////////////////////////////////////////

uint64_t BackoffTimer::CalculateDelay(
    const uint64_t delay) {
  uint64_t backoff_delay = delay;
  backoff_delay <<= backoff_count_++;
  if (backoff_delay > max_backoff_delay_) {
    backoff_delay = max_backoff_delay_;
  }

  return backoff_delay;
}

}  // namespace ads
