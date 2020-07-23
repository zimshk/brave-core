/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/speedreader/speedreader_whitelist.h"

#include <utility>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/task/post_task.h"
#include "brave/components/speedreader/rust/ffi/speedreader.h"
#include "brave/components/speedreader/speedreader_switches.h"
#include "url/gurl.h"

namespace speedreader {

namespace {

constexpr base::FilePath::CharType kDatFileVersion[] = FILE_PATH_LITERAL("1");

constexpr base::FilePath::CharType kDatFileName[] =
    FILE_PATH_LITERAL("speedreader-updater.dat");

constexpr char kComponentName[] = "Brave SpeedReader Updater";
constexpr char kComponentId[] = "jicbkmdloagakknpihibphagfckhjdih";
constexpr char kComponentPublicKey[] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3j/+grwCsrYVA99oDHa+E9z5edPIV"
    "3J+lzld3X7K8wfJXbSauGf2DSxW0UEh+MqkkcIK/66Kkd4veuWqnUCAGXUzrHVy/N6kksDkrS"
    "cOlpKT9zfyIvLc/4nmiyPCSc5c7UrDVUwZnIUBBpEHiwkpiM4pujeJkZSl5783RWIDRN92GDB"
    "dHMdD97JH3bPp3SCTmfAAHzzYUAHUSrOAfodD8qWkfWT19VigseIqwK6dH30uFgaZIOwU9uJV"
    "2Ts/TDEddNv8eV7XbwQdL1HUEoFj+RXDq1CuQJjvQdc7YRmy0WGV0GIXu0lAFOQ6D/Z/rjtOe"
    "//2uc4zIkviMcUlrvHaJwIDAQAB";

}  // namespace

SpeedreaderWhitelist::SpeedreaderWhitelist(Delegate* delegate)
    : brave_component_updater::BraveComponent(delegate),
      speedreader_(new speedreader::SpeedReader) {
  const auto* cmd_line = base::CommandLine::ForCurrentProcess();
  if (!cmd_line->HasSwitch(speedreader::kSpeedreaderWhitelistPath)) {
    // Register component
    Register(kComponentName, kComponentId, kComponentPublicKey);
  } else {
    const base::FilePath whitelist_path(
        cmd_line->GetSwitchValuePath(speedreader::kSpeedreaderWhitelistPath));
    VLOG(2) << "Speedreader whitelist from " << whitelist_path;

    OnWhitelistFileReady(whitelist_path, false /* no error */);

    whitelist_path_watcher_ = std::make_unique<base::FilePathWatcher>();
    if (!whitelist_path_watcher_->Watch(
            whitelist_path, false /*recursive*/,
            base::Bind(&SpeedreaderWhitelist::OnWhitelistFileReady,
                       weak_factory_.GetWeakPtr()))) {
      LOG(ERROR) << "SpeedReader could not watch filesystem for changes"
                 << " at path " << whitelist_path.LossyDisplayName();
    }
  }
}

SpeedreaderWhitelist::~SpeedreaderWhitelist() = default;

void SpeedreaderWhitelist::OnWhitelistFileReady(const base::FilePath& path,
                                                bool error) {
  if (error) {
    LOG(ERROR) << "SpeedReader got an error watching for file changes."
               << " Stopping watching.";
    whitelist_path_watcher_.reset();
    return;
  }
  base::PostTaskAndReplyWithResult(
      FROM_HERE, {base::ThreadPool(), base::MayBlock()},
      base::BindOnce(
          &brave_component_updater::LoadDATFileData<speedreader::SpeedReader>,
          path),
      base::BindOnce(&SpeedreaderWhitelist::OnGetDATFileData,
                     weak_factory_.GetWeakPtr()));
}

void SpeedreaderWhitelist::OnComponentReady(const std::string& component_id,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  base::PostTaskAndReplyWithResult(
      FROM_HERE, {base::ThreadPool(), base::MayBlock()},
      base::BindOnce(
          &brave_component_updater::LoadDATFileData<speedreader::SpeedReader>,
          install_dir.Append(kDatFileVersion).Append(kDatFileName)),
      base::BindOnce(&SpeedreaderWhitelist::OnGetDATFileData,
                     weak_factory_.GetWeakPtr()));
}

bool SpeedreaderWhitelist::IsWhitelisted(const GURL& url) {
  return speedreader_->IsReadableURL(url.spec());
}

std::unique_ptr<Rewriter> SpeedreaderWhitelist::MakeRewriter(const GURL& url) {
  return speedreader_->MakeRewriter(url.spec());
}

void SpeedreaderWhitelist::OnGetDATFileData(GetDATFileDataResult result) {
  VLOG(2) << "Speedreader loaded from DAT file";
  speedreader_ = std::move(result.first);
}

}  // namespace speedreader
