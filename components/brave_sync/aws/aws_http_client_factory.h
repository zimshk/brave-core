/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_FACTORY_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_FACTORY_H_

#include "aws/core/http/HttpClientFactory.h"
#include "components/sync/engine/net/http_post_provider_factory.h"

// namespace Aws {
// namespace Http {
// class URI;
// }  // namespace Http
// }  // namespace Aws

namespace brave_sync {
namespace aws {

class AWSHttpClientFactory : public Aws::Http::HttpClientFactory {
 public:
  AWSHttpClientFactory(syncer::HttpPostProviderFactory* factory);
  ~AWSHttpClientFactory() override;

  std::shared_ptr<Aws::Http::HttpClient> CreateHttpClient(
      const Aws::Client::ClientConfiguration& clientConfiguration)
      const override;
  std::shared_ptr<Aws::Http::HttpRequest> CreateHttpRequest(
      const Aws::String& uri,
      Aws::Http::HttpMethod method,
      const Aws::IOStreamFactory& streamFactory) const override;
  std::shared_ptr<Aws::Http::HttpRequest> CreateHttpRequest(
      const Aws::Http::URI& uri,
      Aws::Http::HttpMethod method,
      const Aws::IOStreamFactory& streamFactory) const override;

 private:
  syncer::HttpPostProviderFactory* http_factory_;

  DISALLOW_COPY_AND_ASSIGN(AWSHttpClientFactory);
};

}  // namespace aws
}  // namespace brave_sync

#endif  // BRAVE_COMPONENTS_BRAVE_SYNC_AWS_AWS_HTTP_CLIENT_FACTORY_H_
