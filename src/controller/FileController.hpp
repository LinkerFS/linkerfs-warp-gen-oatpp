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

#ifndef LINKERFS_WARP_GEN_OATPP_FILECONTROLLER_HPP
#define LINKERFS_WARP_GEN_OATPP_FILECONTROLLER_HPP

#include "dto/common/DocExampleDtos.hpp"
#include "dto/request/ListDirReqDto.hpp"
#include "dto/response/ListDirRespDto.hpp"
#include "liblinkerfs/config.h"
#include "service/FileService.hpp"
#include <QCoreApplication>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#if LIBLINKERFS_ENABLE_UDF
#include "dto/request/ListUDFReqDto.hpp"
#include "service/UdfService.hpp"
#endif

#include OATPP_CODEGEN_BEGIN(ApiController)

class FileController : public oatpp::web::server::api::ApiController {
    using oatpp::web::server::api::ApiController::ApiController;

public:
    static std::shared_ptr<FileController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
        return std::make_shared<FileController>(objectMapper);
    }

    ENDPOINT_INFO(listDir) {
        info->summary = "List dir";
        info->addConsumes<Object<ListDirReqDto>>("application/json");
        info->addResponse<Object<RespWithDataExample<ListDirRespDto>>>(Status::CODE_200, "application/json");
        info->addResponse<Object<RespNoDataExample>>(Status::CODE_404, "application/json");
        info->addResponse<Object<RespNoDataExample>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("POST", "api/file/listDir", listDir,
             BODY_DTO(Object<ListDirReqDto>, dirReqDto)) {
        OATPP_ASSERT_HTTP(dirReqDto->dirPath, Status::CODE_400,
                          QCoreApplication::tr("Field dirPath can not be Empty").toStdString())
        return createDtoResponse(Status::CODE_200, FileService::listDir(dirReqDto->dirPath));
    }

#if LIBLINKERFS_ENABLE_UDF

    ENDPOINT_INFO(listUDF) {
        info->summary = "Open UDF file and list content";
        info->addConsumes<Object<ListUDFReqDto>>("application/json");
        info->addResponse<Object<RespWithDataExample<ListUDFReqDto>>>(Status::CODE_200, "application/json");
        info->addResponse<Object<RespNoDataExample>>(Status::CODE_404, "application/json");
        info->addResponse<Object<RespNoDataExample>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("POST", "api/file/listUDF", listUDF,
             BODY_DTO(Object<ListUDFReqDto>, dirReqDto)) {
        OATPP_ASSERT_HTTP(dirReqDto->udfPath, Status::CODE_400,
                          QCoreApplication::tr("Field dirPath can not be Empty").toStdString())
        return createDtoResponse(Status::CODE_200, UdfService::listUDF(dirReqDto->udfPath));
    }
#endif
};

#include OATPP_CODEGEN_END(ApiController)

#endif//LINKERFS_WARP_GEN_OATPP_FILECONTROLLER_HPP
