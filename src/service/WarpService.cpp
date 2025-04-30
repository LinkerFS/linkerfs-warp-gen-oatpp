/*
 * linkerfs_warp_gen_oatpp: warp configuration file generator backend for project linkerfs
 * Copyright (C) 2024-2025  kaedeair <kaedeair@outlook.com>
 *
 * This file is part of linkerfs_warp_gen_oatpp.
 *
 * linkerfs_warp_gen_oatpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * linkerfs_warp_gen_oatpp is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with linkerfs_warp_gen_oatpp. If not, see <https://www.gnu.org/licenses/>.
 */

#include "WarpService.hpp"
#include <QCoreApplication>
#include "FileService.hpp"
#include "common/utils/File.hpp"
#include "common/utils/Warp.hpp"
#include "dto/response/CreateWarpRespDto.hpp"
#include "linkerfs/filesystem/data/header_info.h"

oatpp::Object<ResponseDto> WarpService::createWarp(const oatpp::String &savePath,
                                                   const oatpp::Vector<oatpp::Object<WarpConfigDto>> &warpConfigs) {
    const QDir dir(savePath->data());
    OATPP_ASSERT_HTTP(QFileInfo(savePath->data()).isDir(), Status::CODE_500,
                      QCoreApplication::tr("%1 is not a directory").arg(savePath->data()).toStdString())
    auto resp = CreateWarpRespDto::createShared();
    auto warpConfigsForLib = std::vector<WARP_CONFIG>();
    warpConfigsForLib.reserve(warpConfigs->size());
    auto warpTargetsForLibPtr = std::vector<std::unique_ptr<WARP_TARGET[]>>(warpConfigs->size());
    warpTargetsForLibPtr.reserve(warpConfigs->size());
    //validate
    for (auto const &config: *warpConfigs) {
        auto configForLib = WARP_CONFIG();
        char *fileName = config->fileName->data();
        FileService::assertFileCanBeCreated(QFileInfo(dir.filePath(fileName)));
        OATPP_ASSERT_HTTP(!config->warpTargets->empty(), Status::CODE_500,
                          QCoreApplication::tr("Config %1 has no target").arg(fileName).toStdString())
        OATPP_ASSERT_HTTP(
                checkWarpTargetNumWithinRange(config->warpTargets->size()), Status::CODE_500,
                QCoreApplication::tr("Number of target in config %1 is out of range").arg(fileName).toStdString())
        configForLib.warp_count = config->warpTargets->size();
        std::unique_ptr<WARP_TARGET[]> targetsForLib(new WARP_TARGET[config->warpTargets->size()]);
        configForLib.warp_targets = targetsForLib.get();
        for (uint32_t i = 0; i < config->warpTargets->size(); ++i) {
            auto target = config->warpTargets[i];
            OATPP_ASSERT_HTTP(Utils::Warp::targetValidateSize(target, targetsForLib.get() + i), Status::CODE_500,
                              QCoreApplication::tr("Target %1 in config %2 is invalid")
                                      .arg(QString::number(i + 1), fileName)
                                      .toStdString())
        }
        warpConfigsForLib.emplace_back(configForLib);
        warpTargetsForLibPtr.emplace_back(std::move(targetsForLib));
    }
    for (decltype(warpConfigs->size()) i = 0; i < warpConfigs->size(); ++i) {
        bool ret;
        oatpp::Object<WarpConfigDto> config = warpConfigs->at(i);
        WARP_CONFIG configForLib = warpConfigsForLib.at(i);
        QString warpFilePath = dir.filePath(config->fileName->c_str());
        if (Utils::Warp::canUseHardLink(warpConfigsForLib.at(i))) {
            std::error_code errorCode =
                    Utils::File::makeHardLink(configForLib.warp_targets->file_path, warpFilePath.toStdString());
            if (errorCode) {
                OATPP_LOGW("WarpService", "%s",
                           QCoreApplication::tr("Hardlink %1 create failed for %2. fallback now...")
                                   .arg(warpFilePath, QString::fromLocal8Bit(errorCode.message().data()))
                                   .toLocal8Bit()
                                   .data())
            } else {
                resp->hardlinkFiles->emplace_back(warpFilePath.toStdString());
                continue;
            }
        }
        ret = Utils::Warp::createWarpFile(warpFilePath, &configForLib);
        if (ret) {
            resp->warpFiles->emplace_back(warpFilePath.toStdString());
        } else {
            resp->failedFiles->emplace_back(warpFilePath.toStdString());
        }
    }
    return ResponseDto::success(std::move(resp));
}

bool WarpService::checkWarpTargetNumWithinRange(const size_t& size) {
    constexpr size_t maxSize = std::numeric_limits<decltype(LINKERFS_HEADER::num_parts)>::max();
    return size <= maxSize;
}
