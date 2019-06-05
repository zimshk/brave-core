/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/syncer_helper.h"

#include "base/strings/string_number_conversions.h"
#include "brave/components/brave_sync/bookmark_order_util.h"
#include "brave/components/brave_sync/tools.h"
#include "components/bookmarks/browser/bookmark_model.h"

namespace brave_sync {
namespace {

void ClearOrder(const bookmarks::BookmarkNode* leaf) {
  bookmarks::BookmarkNode* node = const_cast<bookmarks::BookmarkNode*>(leaf);
  while (!node->is_permanent_node()) {
    node->DeleteMetaInfo("order");
    node = node->parent();
  }
}

void SetOrder(bookmarks::BookmarkModel* model,
              const bookmarks::BookmarkNode* node,
              const std::string& parent_order) {
  DCHECK(!parent_order.empty());
  int index = node->parent()->GetIndexOf(node);
  std::string order = parent_order + "." + base::NumberToString(index + 1);
  model->SetNodeMetaInfo(node, "order", order);
}

void ReCalculateOrder(bookmarks::BookmarkModel* model,
                      const bookmarks::BookmarkNode* node) {
  DCHECK(!node->is_permanent_node());
  std::string parent_order;
  const bookmarks::BookmarkNode* parent = node->parent();
  parent->GetMetaInfo("order", &parent_order);
  if (parent_order.empty()) {
    ReCalculateOrder(model, node->parent());
    parent->GetMetaInfo("order", &parent_order);
  }
  SetOrder(model, node, parent_order);
}

// Used only in test
uint64_t GetIndexByOrder(const std::string& record_order) {
  uint64_t index = 0;
  size_t last_dot_index = record_order.rfind(".");
  DCHECK(last_dot_index != std::string::npos);
  std::string last_digit = record_order.substr(last_dot_index + 1);
  bool result = base::StringToUint64(last_digit, &index);
  --index;
  DCHECK_GE(index, 0u);
  DCHECK(result);
  return index;
}

bool IsOrderConsistent(const bookmarks::BookmarkNode* node) {
  DCHECK(node);
  DCHECK(node->parent());

  std::string node_order;
  node->GetMetaInfo("order", &node_order);

  if (node_order.empty()) {
    return false;
  }

  const auto* parent = node->parent();
  if (parent->child_count() == 1) {
    return true;
  }

  const auto node_index = parent->GetIndexOf(node);

  std::string prev_node_order;
  if (node_index > 0) {
    parent->GetChild(node_index - 1)->GetMetaInfo("order", &prev_node_order);
  }

  std::string next_node_order;
  if (node_index < parent->child_count() - 1) {
    parent->GetChild(node_index + 1)->GetMetaInfo("order", &next_node_order);
  }

  bool is_consistent = (prev_node_order.empty() ||
      CompareOrder(prev_node_order, node_order)) &&
      (next_node_order.empty() ||
      CompareOrder(node_order, next_node_order));

  return is_consistent;
}

uint64_t GetIndexByCompareOrderStartFrom(
    const bookmarks::BookmarkNode* parent,
    const bookmarks::BookmarkNode* src,
    int index) {

  std::string src_order;
  src->GetMetaInfo("order", &src_order);

  DCHECK(!src_order.empty());
  DCHECK(index >= 0);

  while (index < parent->child_count()) {
    const bookmarks::BookmarkNode* node = parent->GetChild(index);
    std::string node_order;
    node->GetMetaInfo("order", &node_order);

    if (!node_order.empty() &&
        brave_sync::CompareOrder(src_order, node_order)) {
      return index;
    }

    ++index;
  }

  return index;
}

// |node| is near the end in parent
void RepositionRespectOrder(
    bookmarks::BookmarkModel* bookmark_model,
    const bookmarks::BookmarkNode* node) {
  const bookmarks::BookmarkNode* parent = node->parent();
  int index = GetIndexByCompareOrderStartFrom(parent, node, 0);
  bookmark_model->Move(node, parent, index);
}

}   // namespace

void AddBraveMetaInfo(
    const bookmarks::BookmarkNode* node,
    bookmarks::BookmarkModel* model,
    bool has_new_parent) {
  if (has_new_parent) {
    ClearOrder(node);
    ReCalculateOrder(model, node);
  } else if (!IsOrderConsistent(node)) {
    std::string parent_order;
    node->parent()->GetMetaInfo("order", &parent_order);
    SetOrder(model, node, parent_order);
  }

  std::string object_id;
  node->GetMetaInfo("object_id", &object_id);
  // newly created node
  if (object_id.empty()) {
    object_id = tools::GenerateObjectId();
  }
  model->SetNodeMetaInfo(node, "object_id", object_id);

  std::string parent_object_id;
  node->parent()->GetMetaInfo("object_id", &parent_object_id);
  model->SetNodeMetaInfo(node, "parent_object_id", parent_object_id);

  std::string sync_timestamp;
  node->GetMetaInfo("sync_timestamp", &sync_timestamp);
  if (sync_timestamp.empty()) {
    sync_timestamp = std::to_string(base::Time::Now().ToJsTime());
    model->SetNodeMetaInfo(node, "sync_timestamp", sync_timestamp);
  }
  DCHECK(!sync_timestamp.empty());
}

// Used only in test
uint64_t GetIndex(const bookmarks::BookmarkNode* parent,
                  const bookmarks::BookmarkNode* src) {
  DCHECK(parent);
  DCHECK(src);
  int index = 0;
  std::string src_order;
  src->GetMetaInfo("order", &src_order);
  DCHECK(!src_order.empty());
  index = GetIndexByOrder(src_order);
  if (index < parent->child_count()) {
    const bookmarks::BookmarkNode* node = parent->GetChild(index);
    if (node) {
      std::string node_order;
      node->GetMetaInfo("order", &node_order);

      DCHECK(!node_order.empty());
      if (CompareOrder(node_order, src_order))
        return index + 1;
    }
  }
  return index;
}

uint64_t GetIndexByCompareOrder(
    const bookmarks::BookmarkNode* parent,
    const bookmarks::BookmarkNode* src) {
  return GetIndexByCompareOrderStartFrom(parent, src, 0);
}

void ReorderChildern(bookmarks::BookmarkModel* bookmark_model,
                     const bookmarks::BookmarkNode* parent,
                     const int index_begin,
                     const int index_end) {

  DCHECK(index_end < parent->child_count());

  // assume [index_begin, index_end] - unsorted
  //        (index_end, child_count()) - sorted
  // as we arrived
  // from BookmarkModelAssociator::ReorderChildrenAndUpdateSyncEntities

  int num_nodes = index_end - index_begin + 1;

  while (num_nodes > 0) {
    const bookmarks::BookmarkNode* node_to_move = parent->GetChild(index_begin);
    int new_position = GetIndexByCompareOrderStartFrom(parent, node_to_move,
        index_begin + num_nodes);
    bookmark_model->Move(node_to_move, parent, new_position);
    --num_nodes;
  }
}

void RepositionOnApplyChangesFromSyncModel(
    bookmarks::BookmarkModel* bookmark_model,
    const std::multimap<int, const bookmarks::BookmarkNode*>& to_reposition) {
  for (auto it = to_reposition.begin(); it != to_reposition.end(); ++it) {
    brave_sync::RepositionRespectOrder(bookmark_model, it->second);
  }
}

}   // namespace brave_sync
