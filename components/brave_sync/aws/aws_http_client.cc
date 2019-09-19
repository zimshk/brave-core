/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/aws/aws_http_client.h"

#include "aws/core/http/HttpRequest.h"
#include "aws/core/http/standard/StandardHttpResponse.h"
#include "aws/core/utils/logging/LogMacros.h"
#include "components/sync/engine/net/http_bridge.h"
#include "services/network/public/cpp/header_util.h"

namespace brave_sync {
namespace aws {

using namespace Aws::Http;
using namespace Aws::Http::Standard;

static const char* AWS_HTTP_CLIENT_TAG = "AWSHttpClient";

AWSHttpClient::AWSHttpClient(syncer::HttpPostProviderFactory* factory)
    : http_factory_(factory) {}

AWSHttpClient::~AWSHttpClient() = default;

std::shared_ptr<HttpResponse> AWSHttpClient::MakeRequest(
    HttpRequest& request,
    Aws::Utils::RateLimits::RateLimiterInterface* readLimiter,
    Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) const {
  auto response =
      Aws::MakeShared<StandardHttpResponse>(AWS_HTTP_CLIENT_TAG, request);
  MakeRequestInternal(request, response, readLimiter, writeLimiter);
  return response;
}

std::shared_ptr<HttpResponse> AWSHttpClient::MakeRequest(
    const std::shared_ptr<HttpRequest>& request,
    Aws::Utils::RateLimits::RateLimiterInterface* readLimiter,
    Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) const {
  auto response =
      Aws::MakeShared<StandardHttpResponse>(AWS_HTTP_CLIENT_TAG, request);
  MakeRequestInternal(*request, response, readLimiter, writeLimiter);
  return response;
}

void AWSHttpClient::MakeRequestInternal(
    HttpRequest& request,
    std::shared_ptr<StandardHttpResponse>& response,
    Aws::Utils::RateLimits::RateLimiterInterface* readLimiter,
    Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) const {
  DCHECK(http_factory_);
  syncer::HttpPostProviderInterface* http_client = http_factory_->Create();
  URI uri = request.GetUri();
  Aws::String url = uri.GetURIString();

  AWS_LOGSTREAM_TRACE(AWS_HTTP_CLIENT_TAG, "Making request to " << url);
  http_client->SetURL(url.c_str(), 443);

  Aws::StringStream headerStream;
  HeaderValueCollection requestHeaders = request.GetHeaders();

  std::string headers;
  AWS_LOGSTREAM_TRACE(AWS_HTTP_CLIENT_TAG, "Including headers:");
  for (auto& requestHeader : requestHeaders) {
    headerStream.str("");
    headerStream << requestHeader.first << ": " << requestHeader.second;
    Aws::String headerString = headerStream.str();
    AWS_LOGSTREAM_TRACE(AWS_HTTP_CLIENT_TAG, headerString);
    if (!network::IsRequestHeaderSafe(requestHeader.first.c_str(),
                                      requestHeader.second.c_str()))
      continue;
    // net::HttpRequestHeaders::ToString()
    headers.append(headerString.c_str());
    headers.append("\r\n");
  }

  headers.append("\r\n");

  http_client->SetExtraRequestHeaders(headers.c_str());

  std::string method;
  switch (request.GetMethod()) {
    case HttpMethod::HTTP_GET:
      method = "GET";
      break;
    case HttpMethod::HTTP_POST:
      method = "POST";
      break;
    case HttpMethod::HTTP_PUT:
      method = "PUT";
      break;
    case HttpMethod::HTTP_HEAD:
      method = "HEAD";
      break;
    case HttpMethod::HTTP_PATCH:
      method = "PATCH";
      break;
    case HttpMethod::HTTP_DELETE:
      method = "DELETE";
      break;
    default:
      assert(0);
      method = "GET";
      break;
  }
  http_client->SetMethod(method.c_str());

  const std::shared_ptr<Aws::IOStream>& content_body = request.GetContentBody();
  Aws::String content_type = request.GetContentType();
  if (content_body) {
    content_body->seekg(0, content_body->end);
    int length = content_body->tellg();
    content_body->seekg(0, content_body->beg);
    char buf[length];
    content_body->read(buf, length);
    if (!content_body->gcount())
      http_client->SetPostPayload(content_type.c_str(), length, buf);
  }
  int net_error_code = 0;
  int http_status_code = 0;
  if (!http_client->MakeSynchronousPost(&net_error_code, &http_status_code)) {
    LOG(ERROR) << "HTTP Status: " << http_status_code;
    LOG(ERROR) << "Net Error: " << net_error_code;
    response->SetResponseCode(HttpResponseCode::REQUEST_NOT_MADE);
  } else {
    LOG(ERROR) << "HTTP Status: " << http_status_code;
    response->SetResponseCode(static_cast<HttpResponseCode>(http_status_code));
  }

  response->GetResponseBody().write(http_client->GetResponseContent(),
                                    http_client->GetResponseContentLength());

  http_factory_->Destroy(http_client);
}

}  // namespace aws
}  // namespace brave_sync
