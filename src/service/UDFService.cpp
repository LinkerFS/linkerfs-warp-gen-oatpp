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

#include "UDFService.hpp"
#include <QCoreApplication>
#include "FileService.hpp"
#include "common/utils/File.hpp"
#include "common/utils/UDF.hpp"
#include "common/utils/Warp.hpp"
#include "common/wrapper/WarpFileWrapper.hpp"
#include "dto/response/CreateWarpRespDto.hpp"
#include "dto/response/ListUDFRespDto.hpp"

std::unique_ptr<udfread, decltype(&udfread_close)> UDFService::openUdf(const char *udfPath) {
    const QFile udfFile(udfPath);
    OATPP_ASSERT_HTTP(udfFile.exists(), Status::CODE_404,
                      QCoreApplication::tr("File %1 not found!").arg(udfPath).toStdString())
    auto udf = std::unique_ptr<udfread, decltype(&udfread_close)>(udfread_init(), udfread_close);

    OATPP_ASSERT_HTTP(udf.get(), Status::CODE_500, QCoreApplication::tr("udfread failed to initialize").toStdString())
    OATPP_ASSERT_HTTP(udfread_open(udf.get(), udfPath) >= 0, Status::CODE_500,
                      QCoreApplication::tr("udfread failed to open").toStdString())
    return std::move(udf);
}

oatpp::Object<ResponseDto> UDFService::listUDF(const oatpp::String &udfPath) {
    const QFile udfFile(udfPath->c_str());
    const auto udf = openUdf(udfPath->c_str());
    const auto root =
            std::unique_ptr<UDFDIR, decltype(&udfread_closedir)>(udfread_opendir(udf.get(), "/"), udfread_closedir);
    OATPP_ASSERT_HTTP(root, Status::CODE_500,
                      QCoreApplication::tr("udfread failed to open root directory").toStdString())
    auto data = ListUDFRespDto::createShared();
    auto &&rootNode = FileNodeDto::createShared();
    data->udfPath = udfPath;
    data->volumeId = udfread_get_volume_id(udf.get());
    data->fileTree = rootNode;
    rootNode->name = "/";
    rootNode->children = Utils::UDF::listDir(root.get(), data->fileTree->name);
    const oatpp::Object<ResponseDto> resp = ResponseDto::success(std::move(data));
    return resp;
}

oatpp::Object<ResponseDto> UDFService::createWarp(const oatpp::String &udfPath, const oatpp::String &savePath,
                                                  const oatpp::Vector<oatpp::Object<UdfWarpTargetDto>> &warpTargets) {
    FileService::assertFileReadable(QFileInfo(udfPath->data()));
    const QDir saveDir(savePath->data());
    std::vector<WarpFileWrapper> warpFileList;
    warpFileList.reserve(warpTargets->size());
    const auto udf = openUdf(udfPath->c_str());
    for (auto const &target: *warpTargets) {
        const auto warpFilePath=saveDir.filePath(target->warpFileName->c_str());
        FileService::assertFileCanBeCreated(QFileInfo(warpFilePath));
        const auto udfFile = std::unique_ptr<UDFFILE, decltype(&udfread_file_close)>(
                udfread_file_open(udf.get(), target->filePath->c_str()), udfread_file_close);
        OATPP_ASSERT_HTTP(
                udfFile, Status::CODE_500,
                QCoreApplication::tr("udfread failed to open file %1").arg(target->filePath->c_str()).toStdString())
        const auto fileInfo =
                std::unique_ptr<UDFFILE_INFO, decltype(&free)>(udfread_get_file_info(udfFile.get()), free);
        OATPP_ASSERT_HTTP(
                fileInfo, Status::CODE_500,
                QCoreApplication::tr("udfread failed to get file info %1").arg(target->filePath->c_str()).toStdString())
        udf_warp_target udfWarpTarget = {};
        OATPP_ASSERT_HTTP(Utils::UDF::targetValidateSizeAndFill(target, &udfWarpTarget, fileInfo.get()),
                          Status::CODE_500,
                          QCoreApplication::tr("File %1 in %2 is invalid")
                                  .arg(target->filePath->c_str(), udfPath->c_str())
                                  .toStdString());
        udf_warp_config udfWarpConfig = {
                .targets = &udfWarpTarget,
                .udf_file_path = udfPath->c_str(),
                .file_path_length = static_cast<decltype(udf_warp_config::file_path_length)>(udfPath->size()),
                .target_count = 1,
        };
        auto warpFileWrapper =
                WarpFileWrapper(target->warpFileName, udf_generate_warp_file(&udfWarpConfig));
        OATPP_ASSERT_HTTP(!warpFileWrapper.isEmpty(), Status::CODE_500,
                          QCoreApplication::tr("Failed to generate warp file %1")
                                  .arg(target->warpFileName->c_str())
                                  .toStdString())
        warpFileList.emplace_back(std::move(warpFileWrapper));
    }
    auto resp = CreateWarpRespDto::createShared();
    for (auto const &warpFileWrapper: warpFileList) {
        const auto saveFile = saveDir.filePath(warpFileWrapper.getWarpFileName().c_str());
        if (Utils::Warp::createWarpFile(saveFile, warpFileWrapper.getWarpFile()))
            resp->warpFiles->emplace_back(saveFile.toStdString());
        else
            resp->failedFiles->emplace_back(saveFile.toStdString());
    }
    return ResponseDto::success(std::move(resp));
}
