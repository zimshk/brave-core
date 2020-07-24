/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_H_  // NOLINT
#define BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_H_  // NOLINT

#include <stdint.h>

#include <string>
#include <vector>

#include "bat/ads/internal/classification/purchase_intent_classifier/funnel_keyword_info.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_signal_history.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/purchase_intent_signal_info.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/segment_keyword_info.h"
#include "bat/ads/internal/classification/purchase_intent_classifier/site_info.h"
#include "bat/ads/internal/search_engine/search_providers.h"

namespace ads {
namespace classification {

using PurchaseIntentWinningCategoryList = std::vector<std::string>;
using PurchaseIntentSegmentList = std::vector<std::string>;

class PurchaseIntentClassifier {
 public:
  PurchaseIntentClassifier();

  ~PurchaseIntentClassifier();

  bool IsInitialized();

  bool Initialize(
      const std::string& json);

  PurchaseIntentSignalInfo ExtractIntentSignal(
      const std::string& url);

  PurchaseIntentWinningCategoryList GetWinningCategories(
      const PurchaseIntentSignalSegmentHistoryMap& history,
      const uint16_t max_segments);

 private:
  bool FromJson(
      const std::string& json);

  uint16_t GetIntentScoreForHistory(
      const PurchaseIntentSignalSegmentHistoryList& segment_history);

  SiteInfo GetSite(
      const std::string& url);

  PurchaseIntentSegmentList GetSegments(
      const std::string& search_query);

  uint16_t GetFunnelWeight(
      const std::string& search_query);

  std::vector<std::string> TransformIntoSetOfWords(
      const std::string& search_query);

  bool IsSubset(
      const std::vector<std::string>& set_a,
      const std::vector<std::string>& set_b);

  bool is_initialized_;
  uint16_t version_;
  uint16_t signal_level_;
  uint16_t classification_threshold_;
  uint64_t signal_decay_time_window_in_seconds_;
  std::vector<SiteInfo> sites_;  // sites2segments
  std::vector<SegmentKeywordInfo> segment_keywords_;  // keywords2segments
  std::vector<FunnelKeywordInfo> funnel_keywords_;  // keywords2funnelstages
};

}  // namespace classification
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_H_  // NOLINT
