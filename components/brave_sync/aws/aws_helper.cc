/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_sync/aws/aws_helper.h"

#include "aws/core/Aws.h"
#include "brave/components/brave_sync/aws/aws_http_client_factory.h"
#include "components/sync/engine/net/http_post_provider_factory.h"

#include "aws/s3/S3Client.h"
#include "aws/s3/model/ListObjectsRequest.h"
#include "aws/s3/model/PutObjectRequest.h"
#include "aws/s3/model/Object.h"

namespace brave_sync {
namespace aws {

AWSHelper::AWSHelper(syncer::HttpPostProviderFactory* factory) {
  Aws::SDKOptions options;
  options.httpOptions.httpClientFactory_create_fn = [factory]() {
    return Aws::MakeShared<AWSHttpClientFactory>("AWSHttpClientFactory",
                                                 factory);
  };
  options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
  Aws::InitAPI(options);
  {
    // make your SDK calls here.
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = "us-west-1";
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::ListObjectsRequest objects_request;
    objects_request.WithBucket("sync-dev-test");

    auto list_objects_outcome = s3_client.ListObjects(objects_request);

    if (list_objects_outcome.IsSuccess())
    {
      Aws::Vector<Aws::S3::Model::Object> object_list =
        list_objects_outcome.GetResult().GetContents();

      for (auto const &s3_object : object_list)
      {
        LOG(ERROR) << "* " << s3_object.GetKey() << std::endl;
      }
    }
    else
    {
      LOG(ERROR) << "ListObjects error: " <<
        list_objects_outcome.GetError().GetExceptionName() << " " <<
        list_objects_outcome.GetError().GetMessage() << std::endl;
    }

    // put
    Aws::S3::Model::PutObjectRequest object_request;
    object_request.SetBucket("sync-dev-test");
    object_request.SetKey("5566");

    // Put the object
    auto put_object_outcome = s3_client.PutObject(object_request);
    if (!put_object_outcome.IsSuccess()) {
      auto error = put_object_outcome.GetError();
      std::cout << "ERROR: " << error.GetExceptionName() << ": " 
	<< error.GetMessage() << std::endl;
    }
  }
  Aws::ShutdownAPI(options);
}

AWSHelper::~AWSHelper() {}

}  // namespace aws
}  // namespace brave_sync
