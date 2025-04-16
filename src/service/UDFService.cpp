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

#include "common/utils/UDF.hpp"
#include "udfread/udfread.h"
#include "UDFService.hpp"
#include <QCoreApplication>
#include <QFile>

oatpp::Object<ResponseDto> UDFService::listUDF(const oatpp::String &udfPath) {
    oatpp::Object<ResponseDto> resp;
    QFile udfFile(udfPath->c_str());
    OATPP_ASSERT_HTTP(udfFile.exists(), Status::CODE_404,
                      QCoreApplication::tr("File %1 not found!").arg(udfPath->c_str()).toStdString())
    udfread *udf=udfread_init();
    if (!udf)
        return ResponseDto::fail(Status::CODE_500,QCoreApplication::tr("udfread failed to initialize").toStdString());
    if(udfread_open(udf,udfPath->c_str())<0)
    {
        udfread_close(udf);
        return ResponseDto::fail(Status::CODE_500,QCoreApplication::tr("udfread failed to open").toStdString());
    }
    UDFDIR *root= udfread_opendir(udf,"/");
    if(!root)
    {
        udfread_closedir(root);
        udfread_close(udf);
        return ResponseDto::fail(Status::CODE_500,QCoreApplication::tr("udfread failed to open root directory").toStdString());
    }
    auto data=ListUDFRespDto::createShared();
    auto rootNode=FileNodeDto::createShared();

    data->udfPath=udfPath;
    data->volumeId= udfread_get_volume_id(udf);
    data->fileTree=rootNode;
    rootNode->name="/";
    rootNode->children=Utils::UDF::listDir(root,data->fileTree->name);
    resp=ResponseDto::success(std::move(data));
    udfread_closedir(root);
    udfread_close(udf);
    return resp;
}