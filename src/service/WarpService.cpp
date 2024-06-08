/*
 * linkerfs_warp_gen_oatpp: warp configuration file generator backend for project linkerfs
 * Copyright (C) 2024  kaedeair <kaedeair@outlook.com>
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
#include "common/utils/File.hpp"
#include "common/utils/Warp.hpp"
#include "dto/response/CreateWarpRespDto.hpp"

oatpp::Object<ResponseDto>
WarpService::createWarp(const oatpp::String &savePath, oatpp::Vector<oatpp::Object<WarpConfigDto>> &warpConfigs) {
    QDir dir(savePath->data());
    OATPP_ASSERT_HTTP(dir.exists(), Status::CODE_500, QString("%1 does not exist").arg(dir.path()).toStdString())
    OATPP_ASSERT_HTTP(Utils::File::checkDirWritePermission(dir.path()), Status::CODE_500,
                      QString("%1 is not writable").arg(dir.path()).toStdString())
    auto resp = CreateWarpRespDto::createShared();
    auto warpConfigsForLib = std::vector<WARP_CONFIG>();
    warpConfigsForLib.reserve(warpConfigs->size());
    auto warpTargetsForLibPtr = std::vector<std::shared_ptr<WARP_TARGET>>(warpConfigs->size());
    warpTargetsForLibPtr.reserve(warpConfigs->size());
    //validate
    for (auto const &config: *warpConfigs) {
        auto configForLib = WARP_CONFIG();
        char *fileName = config->fileName->data();
        OATPP_ASSERT_HTTP(!dir.exists(fileName), Status::CODE_500,
                          QString("File %1 already exists").arg(dir.filePath(fileName)).toStdString())
        OATPP_ASSERT_HTTP(!config->warpTargets->empty(), Status::CODE_500,
                          QString("Config %1 has no target").arg(fileName).toStdString())
        OATPP_ASSERT_HTTP(config->warpTargets->size() <= 0xffff, Status::CODE_500,
                          QString("Number of target in config %1 is out of range").arg(fileName).toStdString())
        configForLib.warp_count = config->warpTargets->size();
        std::shared_ptr<WARP_TARGET> targetsForLib(new WARP_TARGET[config->warpTargets->size()],
                                                   std::default_delete<WARP_TARGET[]>());
        configForLib.warp_targets = targetsForLib.get();
        for (uint32_t i = 0; i < config->warpTargets->size(); ++i) {
            auto target = config->warpTargets[i];
            OATPP_ASSERT_HTTP(Utils::Warp::targetValidateSize(target, targetsForLib.get() + i), Status::CODE_500,
                              QString("Target %1 in config %2 is invalid").arg(QString::number(i + 1), fileName).toStdString())
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
                OATPP_LOGW("WarpService", QString("Hardlink %1 create failed for %2. fallback now...")
                                                  .arg(warpFilePath, QString::fromLocal8Bit(errorCode.message().data()))
                                                  .toLocal8Bit()
                                                  .data())
            } else {
                resp->hardlinkFiles->emplace_back(warpFilePath.toStdString());
                continue;
            }
        }
        ret = Utils::Warp::creatWarpFile(warpFilePath, &configForLib);
        if (ret) {
            resp->warpFiles->emplace_back(warpFilePath.toStdString());
        } else {
            resp->failedFiles->emplace_back(warpFilePath.toStdString());
        }
    }
    return ResponseDto::success(std::move(resp));
}
