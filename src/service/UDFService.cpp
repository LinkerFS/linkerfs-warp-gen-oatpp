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
#include <QDir>
#include "FileService.hpp"
#include "common/utils/UDF.hpp"
#include "dto/response/ListUDFRespDto.hpp"
#include "liblinkerfs/generator.h"

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