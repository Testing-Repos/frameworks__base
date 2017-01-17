/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APKASSETS_H_
#define APKASSETS_H_

#include <memory>
#include <string>

#include "android-base/macros.h"
#include "ziparchive/zip_archive.h"

#include "androidfw/Asset.h"
#include "androidfw/LoadedArsc.h"

namespace android {

// Holds an APK.
class ApkAssets {
 public:
  static std::unique_ptr<ApkAssets> Load(const std::string& path);

  std::unique_ptr<Asset> Open(const std::string& path,
                              Asset::AccessMode mode = Asset::AccessMode::ACCESS_RANDOM) const;

  inline const std::string& GetPath() const { return path_; }

  inline const LoadedArsc* GetLoadedArsc() const { return loaded_arsc_.get(); }

 private:
  DISALLOW_COPY_AND_ASSIGN(ApkAssets);

  ApkAssets() = default;

  struct ZipArchivePtrCloser {
    void operator()(::ZipArchiveHandle handle) { ::CloseArchive(handle); }
  };

  using ZipArchivePtr =
      std::unique_ptr<typename std::remove_pointer<::ZipArchiveHandle>::type, ZipArchivePtrCloser>;
  ZipArchivePtr zip_handle_;
  std::string path_;
  std::unique_ptr<Asset> resources_asset_;
  std::unique_ptr<LoadedArsc> loaded_arsc_;
};

}  // namespace android

#endif /* APKASSETS_H_ */
