/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/transactions_info.h"

#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/values.h"

namespace ads {

TransactionsInfo::TransactionsInfo() = default;

TransactionsInfo::TransactionsInfo(
    const TransactionsInfo& info) = default;

TransactionsInfo::~TransactionsInfo() = default;

std::string TransactionsInfo::ToJson() const {
  base::Value dictionary(base::Value::Type::DICTIONARY);

  // Estimated pending rewards
  dictionary.SetKey("estimated_pending_rewards",
      base::Value(estimated_pending_rewards));

  // Next payment date in seconds
  dictionary.SetKey("next_payment_date_in_seconds",
      base::Value(std::to_string(next_payment_date_in_seconds)));

  // Ad notifications received this month
  dictionary.SetKey("ad_notifications_received_this_month",
      base::Value(std::to_string(ad_notifications_received_this_month)));

  // Transactions
  auto transactions_list = GetTransactionsAsList();
  dictionary.SetKey("transactions",
      base::Value(std::move(transactions_list)));

  // Write to JSON
  std::string json;
  base::JSONWriter::Write(dictionary, &json);

  return json;
}

bool TransactionsInfo::FromJson(
    const std::string& json) {
  base::Optional<base::Value> value = base::JSONReader::Read(json);
  if (!value || !value->is_dict()) {
    return false;
  }

  base::DictionaryValue* dictionary = nullptr;
  if (!value->GetAsDictionary(&dictionary)) {
    return false;
  }

  estimated_pending_rewards =
      GetEstimatedPendingRewardsFromJson(dictionary);

  next_payment_date_in_seconds =
      GetNextPaymentDateInSecondsFromJson(dictionary);

  ad_notifications_received_this_month =
      GetAdNotificationsReceivedThisMonthFromJson(dictionary);

  transactions = GetTransactionsFromJson(dictionary);

  return true;
}

///////////////////////////////////////////////////////////////////////////////

double TransactionsInfo::GetEstimatedPendingRewardsFromJson(
    base::DictionaryValue* dictionary) const {
  DCHECK(dictionary);

  const auto value = dictionary->FindDoubleKey("estimated_pending_rewards");
  if (!value) {
    return 0.0;
  }

  return *value;
}

uint64_t TransactionsInfo::GetNextPaymentDateInSecondsFromJson(
    base::DictionaryValue* dictionary) const {
  DCHECK(dictionary);

  const auto* value = dictionary->FindStringKey("next_payment_date_in_seconds");
  if (!value) {
    return 0;
  }

  return std::stoull(*value);
}

uint64_t TransactionsInfo::GetAdNotificationsReceivedThisMonthFromJson(
    base::DictionaryValue* dictionary) const {
  DCHECK(dictionary);

  const auto* value =
      dictionary->FindStringKey("ad_notifications_received_this_month");
  if (!value) {
    return 0;
  }

  return std::stoull(*value);
}

base::Value TransactionsInfo::GetTransactionsAsList() const {
  base::Value list(base::Value::Type::LIST);

  for (const auto& transaction : transactions) {
    base::Value transaction_dictionary(base::Value::Type::DICTIONARY);

    transaction_dictionary.SetKey("timestamp_in_seconds",
        base::Value(std::to_string(transaction.timestamp_in_seconds)));

    transaction_dictionary.SetKey("estimated_redemption_value",
        base::Value(transaction.estimated_redemption_value));

    transaction_dictionary.SetKey("confirmation_type",
        base::Value(transaction.confirmation_type));

    list.Append(std::move(transaction_dictionary));
  }

  return list;
}

TransactionList TransactionsInfo::GetTransactionsFromJson(
    base::DictionaryValue* dictionary) const {
  DCHECK(dictionary);

  TransactionList transactions;

  auto* transactions_value = dictionary->FindListKey("transactions");
  if (!transactions_value) {
    return transactions;
  }

  for (auto& transaction_value : transactions_value->GetList()) {
    base::DictionaryValue* transaction_dictionary = nullptr;
    if (!transaction_value.GetAsDictionary(&transaction_dictionary)) {
      continue;
    }

    TransactionInfo info;

    // Timestamp
    const auto* timestamp_in_seconds_value =
        transaction_dictionary->FindStringKey("timestamp_in_seconds");
    if (!timestamp_in_seconds_value) {
      continue;
    }
    info.timestamp_in_seconds = std::stoull(*timestamp_in_seconds_value);

    // Estimated redemption value
    const auto estimated_redemption_value_value =
        transaction_dictionary->FindDoubleKey("estimated_redemption_value");
    if (!estimated_redemption_value_value) {
      continue;
    }
    info.estimated_redemption_value = *estimated_redemption_value_value;

    // Confirmation type
    const auto* confirmation_type_value =
        transaction_dictionary->FindStringKey("confirmation_type");
    if (!confirmation_type_value) {
      continue;
    }
    info.confirmation_type = *confirmation_type_value;

    transactions.push_back(info);
  }

  return transactions;
}

}  // namespace ads
