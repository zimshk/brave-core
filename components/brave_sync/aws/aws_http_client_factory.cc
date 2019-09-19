/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/aws/aws_http_client_factory.h"

#include "aws/core/http/standard/StandardHttpRequest.h"
#include "aws/core/http/URI.h"
#include "brave/components/brave_sync/aws/aws_http_client.h"

namespace brave_sync {
namespace aws {

using namespace Aws::Client;
using namespace Aws::Http;

static const char* HTTP_CLIENT_FACTORY_ALLOCATION_TAG = "AWSHttpClientFactory";

AWSHttpClientFactory::AWSHttpClientFactory(
    syncer::HttpPostProviderFactory* factory)
    : http_factory_(factory) {}

AWSHttpClientFactory::~AWSHttpClientFactory() = default;

std::shared_ptr<HttpClient> AWSHttpClientFactory::CreateHttpClient(
    const ClientConfiguration& clientConfiguration) const {
  return Aws::MakeShared<AWSHttpClient>(HTTP_CLIENT_FACTORY_ALLOCATION_TAG,
                                        http_factory_);
}

std::shared_ptr<HttpRequest> AWSHttpClientFactory::CreateHttpRequest(
    const Aws::String& uri,
    HttpMethod method,
    const Aws::IOStreamFactory& streamFactory) const {
  return CreateHttpRequest(URI(uri), method, streamFactory);
}

std::shared_ptr<HttpRequest> AWSHttpClientFactory::CreateHttpRequest(
    const URI& uri,
    HttpMethod method,
    const Aws::IOStreamFactory& streamFactory) const {
  auto request = Aws::MakeShared<Standard::StandardHttpRequest>(
      HTTP_CLIENT_FACTORY_ALLOCATION_TAG, uri, method);
  request->SetResponseStreamFactory(streamFactory);

  return request;
}

}  // namespace aws
}  // namespace brave_sync
