/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/syncer_helper.h"
#include "../../../../src/components/sync_bookmarks/bookmark_model_associator.cc"    // NOLINT

#include "components/bookmarks/browser/bookmark_node.h"

namespace {

bool PlaceSyncNode(
      const BookmarkNode* parent, int index, syncer::WriteTransaction* trans,
      syncer::WriteNode* dst,
      sync_bookmarks::BookmarkModelAssociator* associator) {
  syncer::ReadNode sync_parent(trans);
  if (!associator->InitSyncNodeFromChromeId(parent->id(), &sync_parent)) {
    LOG(WARNING) << "Parent lookup failed";
    return false;
  }

  bool success = false;
  if (index == 0) {
    // Insert into first position.
    success = dst->SetPosition(sync_parent, nullptr);

    if (success) {
      DCHECK_EQ(dst->GetParentId(), sync_parent.GetId());
      DCHECK_EQ(dst->GetId(), sync_parent.GetFirstChildId());
      DCHECK_EQ(dst->GetPredecessorId(), syncer::kInvalidId);
    }
  } else {
    // Find the bookmark model predecessor, and insert after it.
    const BookmarkNode* prev = parent->GetChild(index - 1);
    syncer::ReadNode sync_prev(trans);
    if (!associator->InitSyncNodeFromChromeId(prev->id(), &sync_prev)) {
      LOG(WARNING) << "Predecessor lookup failed";
      return false;
    }
    success = dst->SetPosition(sync_parent, &sync_prev);
    if (success) {
      DCHECK_EQ(dst->GetParentId(), sync_parent.GetId());
      DCHECK_EQ(dst->GetPredecessorId(), sync_prev.GetId());
      DCHECK_EQ(dst->GetId(), sync_prev.GetSuccessorId());
    }
  }
  return success;
}

}

namespace sync_bookmarks {

syncer::SyncError BookmarkModelAssociator::ReorderChildrenAndUpdateSyncEntities(
    const bookmarks::BookmarkNode* parent_node,
    const int index_begin,
    const int index_end,
    syncer::WriteTransaction* trans) {
  // Mostly taken from BookmarkChangeProcessor::BookmarkNodeMoved

  brave_sync::ReorderChildern(bookmark_model_, parent_node,
    index_begin, index_end);

  for (int i = 0; i < parent_node->child_count(); ++i) {
    const bookmarks::BookmarkNode* node = parent_node->GetChild(i);
    syncer::WriteNode sync_node(trans);
    if (!this->InitSyncNodeFromChromeId(node->id(), &sync_node)) {
      syncer::SyncError error(FROM_HERE,
                              syncer::SyncError::DATATYPE_ERROR,
                              "Failed to init sync node from chrome node",
                              syncer::BOOKMARKS);
      unrecoverable_error_handler_->OnUnrecoverableError(error);
      return error;
    }
    if (!PlaceSyncNode(parent_node, i, trans, &sync_node,
                       this)) {
      syncer::SyncError error(FROM_HERE,
                              syncer::SyncError::DATATYPE_ERROR,
                              "Failed to place sync node",
                              syncer::BOOKMARKS);
      unrecoverable_error_handler_->OnUnrecoverableError(error);
      return error;
    }
  }

  return syncer::SyncError();
}

}   // namespace sync_bookmarks
