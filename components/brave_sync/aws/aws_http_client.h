/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_H_

#include "aws/core/http/HttpClient.h"
#include "components/sync/engine/net/http_post_provider_factory.h"

namespace Aws {
namespace Http {
namespace Standard {
class StandardHttpResponse;
}  // namespace Standard
}  // namespace Http
}  // namespace Aws

namespace brave_sync {
namespace aws {

class AWSHttpClient : public Aws::Http::HttpClient {
 public:
  AWSHttpClient(syncer::HttpPostProviderFactory* factory);
  ~AWSHttpClient() override;

  AWS_DEPRECATED("This funciton in base class has been deprecated")
  std::shared_ptr<Aws::Http::HttpResponse> MakeRequest(
      Aws::Http::HttpRequest& request,
      Aws::Utils::RateLimits::RateLimiterInterface* readLimiter = nullptr,
      Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter =
          nullptr) const override;

  // Makes request with shared_ptr typed request and receives response
  // synchronously
  std::shared_ptr<Aws::Http::HttpResponse> MakeRequest(
      const std::shared_ptr<Aws::Http::HttpRequest>& request,
      Aws::Utils::RateLimits::RateLimiterInterface* readLimiter = nullptr,
      Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter =
          nullptr) const override;

 private:
  void MakeRequestInternal(
      Aws::Http::HttpRequest& request,
      std::shared_ptr<Aws::Http::Standard::StandardHttpResponse>& response,
      Aws::Utils::RateLimits::RateLimiterInterface* readLimiter,
      Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) const;

  syncer::HttpPostProviderFactory* http_factory_;

  DISALLOW_COPY_AND_ASSIGN(AWSHttpClient);
};

}  // namespace aws
}  // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_H_
