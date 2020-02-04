/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_SERVICE_IMPL_H_
#define BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_SERVICE_IMPL_H_

#include <memory>

#include "bat/ledger/ledger.h"
#include "brave/components/services/bat_ledger/public/interfaces/bat_ledger.mojom.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_associated_remote.h"
#include "mojo/public/cpp/bindings/unique_associated_receiver_set.h"
#include "services/service_manager/public/cpp/service_context_ref.h"

namespace bat_ledger {

class BatLedgerServiceImpl : public mojom::BatLedgerService {
 public:
  explicit BatLedgerServiceImpl(
      std::unique_ptr<service_manager::ServiceContextRef> service_ref);
  ~BatLedgerServiceImpl() override;

  // bat_ledger::mojom::BatLedgerService
  void Create(
      mojo::PendingAssociatedRemote<mojom::BatLedgerClient> client_info,
      mojo::PendingAssociatedReceiver<mojom::BatLedger> bat_ledger) override;

  void SetEnvironment(ledger::Environment environment) override;
  void SetDebug(bool isDebug) override;
  void SetReconcileTime(int32_t time) override;
  void SetShortRetries(bool short_retries) override;
  void SetTesting() override;

  void GetEnvironment(GetEnvironmentCallback callback) override;
  void GetDebug(GetDebugCallback callback) override;
  void GetReconcileTime(GetReconcileTimeCallback callback) override;
  void GetShortRetries(GetShortRetriesCallback callback) override;

 private:
  const std::unique_ptr<service_manager::ServiceContextRef> service_ref_;
  bool initialized_;
  mojo::UniqueAssociatedReceiverSet<mojom::BatLedger> receivers_;

  DISALLOW_COPY_AND_ASSIGN(BatLedgerServiceImpl);
};

}  // namespace bat_ledger

#endif  // BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_SERVICE_IMPL_H_
