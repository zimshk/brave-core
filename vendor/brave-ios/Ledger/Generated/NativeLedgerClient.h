/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>
#import "bat/ledger/ledger_client.h"

@protocol NativeLedgerClientBridge;

class NativeLedgerClient : public ledger::LedgerClient {
public:
  NativeLedgerClient(id<NativeLedgerClientBridge> bridge);
  ~NativeLedgerClient() override;

private:
  __unsafe_unretained id<NativeLedgerClientBridge> bridge_;

  void FetchFavIcon(const std::string & url, const std::string & favicon_key, ledger::FetchIconCallback callback) override;
  void KillTimer(const uint32_t timer_id) override;
  void LoadLedgerState(ledger::OnLoadCallback callback) override;
  void LoadNicewareList(ledger::GetNicewareListCallback callback) override;
  void LoadPublisherState(ledger::OnLoadCallback callback) override;
  void LoadState(const std::string & name, ledger::OnLoadCallback callback) override;
  void LoadURL(const std::string & url, const std::vector<std::string> & headers, const std::string & content, const std::string & contentType, const ledger::UrlMethod method, ledger::LoadURLCallback callback) override;
  void Log(const char * file, const int line, const int verbose_level, const std::string & message) override;
  void OnPanelPublisherInfo(ledger::Result result, ledger::PublisherInfoPtr publisher_info, uint64_t windowId) override;
  void OnReconcileComplete(ledger::Result result, ledger::ContributionInfoPtr contribution) override;
  void ResetState(const std::string & name, ledger::ResultCallback callback) override;
  void PublisherListNormalized(ledger::PublisherInfoList list) override;
  void SaveState(const std::string & name, const std::string & value, ledger::ResultCallback callback) override;
  void SetTimer(uint64_t time_offset, uint32_t * timer_id) override;
  std::string URIEncode(const std::string & value) override;
  void OnContributeUnverifiedPublishers(ledger::Result result, const std::string& publisher_key, const std::string& publisher_name) override;
  void SetBooleanState(const std::string& name, bool value) override;
  bool GetBooleanState(const std::string& name) const override;
  void SetIntegerState(const std::string& name, int value) override;
  int GetIntegerState(const std::string& name) const override;
  void SetDoubleState(const std::string& name, double value) override;
  double GetDoubleState(const std::string& name) const override;
  void SetStringState(const std::string& name, const std::string& value) override;
  std::string GetStringState(const std::string& name) const override;
  void SetInt64State(const std::string& name, int64_t value) override;
  int64_t GetInt64State(const std::string& name) const override;
  void SetUint64State(const std::string& name, uint64_t value) override;
  uint64_t GetUint64State(const std::string& name) const override;
  void ClearState(const std::string& name) override;
  std::map<std::string, ledger::ExternalWalletPtr> GetExternalWallets() override;
  void SaveExternalWallet(const std::string& wallet_type, ledger::ExternalWalletPtr wallet) override;
  void ShowNotification(const std::string& type, const std::vector<std::string>& args,  ledger::ResultCallback callback) override;
  void SetTransferFee(const std::string& wallet_type, ledger::TransferFeePtr transfer_fee) override;
  ledger::TransferFeeList GetTransferFees(const std::string& wallet_type) override;
  void RemoveTransferFee(const std::string& wallet_type, const std::string& id) override;
  bool GetBooleanOption(const std::string& name) const override;
  int GetIntegerOption(const std::string& name) const override;
  double GetDoubleOption(const std::string& name) const override;
  std::string GetStringOption(const std::string& name) const override;
  int64_t GetInt64Option(const std::string& name) const override;
  uint64_t GetUint64Option(const std::string& name) const override;
  ledger::ClientInfoPtr GetClientInfo() override;
  void UnblindedTokensReady() override;
  void ReconcileStampReset() override;
  void RunDBTransaction(ledger::DBTransactionPtr transaction, ledger::RunDBTransactionCallback callback) override;
  void GetCreateScript(ledger::GetCreateScriptCallback callback) override;
  void PendingContributionSaved(const ledger::Result result) override;
  void ClearAllNotifications() override;
};
