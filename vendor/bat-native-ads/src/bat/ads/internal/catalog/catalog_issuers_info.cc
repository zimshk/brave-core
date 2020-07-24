/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/catalog/catalog_issuers_info.h"

#include "base/strings/string_number_conversions.h"
#include "third_party/re2/src/re2/re2.h"
#include "bat/ads/internal/logging.h"

namespace ads {

CatalogIssuersInfo::CatalogIssuersInfo() = default;

CatalogIssuersInfo::CatalogIssuersInfo(
    const CatalogIssuersInfo& info) = default;

CatalogIssuersInfo::~CatalogIssuersInfo() = default;

bool CatalogIssuersInfo::IsValid() const {
  if (public_key.empty() || issuers.empty()) {
    return false;
  }

  return true;
}

bool CatalogIssuersInfo::PublicKeyDoesExist(
    const std::string& public_key) const {
  if (this->public_key == public_key) {
    return true;
  }

  const auto iter = std::find_if(issuers.begin(), issuers.end(),
      [&public_key] (const auto& issuer) {
    return issuer.public_key == public_key;
  });

  if (iter == issuers.end()) {
    return false;
  }

  return true;
}


double CatalogIssuersInfo::GetEstimatedRedemptionValue(
    const std::string& public_key) const {
  const auto iter = std::find_if(issuers.begin(), issuers.end(),
      [&public_key] (const auto& issuer) {
    return issuer.public_key == public_key;
  });

  if (iter == issuers.end()) {
    return 0.0;
  }

  const CatalogIssuerInfo catalog_issuer = *iter;
  std::string name = catalog_issuer.name;

  if (!re2::RE2::Replace(&name, "BAT", "")) {
    BLOG(1, "Failed to estimate redemption value due to invalid catalog "
        "issuer name");
    return 0.0;
  }

  double estimated_redemption_value;
  if (!base::StringToDouble(name, &estimated_redemption_value)) {
    BLOG(1, "Failed to estimate redemption value due to invalid catalog "
        "issuer name");
    return 0.0;
  }

  return estimated_redemption_value;
}

}  // namespace ads
