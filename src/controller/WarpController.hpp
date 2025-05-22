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

#ifndef LINKERFS_WARP_GEN_OATPP_WARPCONTROLLER_HPP
#define LINKERFS_WARP_GEN_OATPP_WARPCONTROLLER_HPP

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "dto/common/DocExampleDtos.hpp"
#include "dto/request/CreateWarpReqDto.hpp"
#include "dto/response/CreateWarpRespDto.hpp"
#include "liblinkerfs/config.h"
#include "service/WarpService.hpp"

#if LIBLINKERFS_ENABLE_UDF
#include "dto/request/CreateUdfWarpReqDto.hpp"
#endif

#include OATPP_CODEGEN_BEGIN(DTO)
    GEN_SUCCESS_RESP_DTO(CreateWarpRespExample, CreateWarpRespDto);
#include OATPP_CODEGEN_END(DTO)


#include OATPP_CODEGEN_BEGIN(ApiController)

class WarpController : public oatpp::web::server::api::ApiController {
    using ApiController::ApiController;

public:
    static std::shared_ptr<WarpController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
        return std::make_shared<WarpController>(objectMapper);
    }

    ENDPOINT_INFO(createWarp) {
        info->summary = "Create warp file";
        info->addConsumes<CreateWarpReqDto::Wrapper>("application/json");
        info->addResponse<CreateWarpRespExample::Wrapper>(Status::CODE_200, "application/json");
        info->addResponse<ErrorResponseExapmle400::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<ErrorResponseExapmle500::Wrapper>(Status::CODE_500, "application/json");
    }

    ENDPOINT("POST", "api/warp/create", createWarp, BODY_DTO(Object<CreateWarpReqDto>, createWarpReqDto)) {
        OATPP_ASSERT_HTTP(createWarpReqDto->savePath, Status::CODE_400,
                          QCoreApplication::tr("Field 'savePath' cannot be null").toStdString())
        OATPP_ASSERT_HTTP(!createWarpReqDto->savePath->empty(), Status::CODE_400,
                          QCoreApplication::tr("Field 'savePath' cannot be empty").toStdString())
        OATPP_ASSERT_HTTP(createWarpReqDto->warpConfigs, Status::CODE_400,
                          QCoreApplication::tr("Field 'warpConfig' cannot be null").toStdString())
        return createDtoResponse(Status::CODE_200,
                                 WarpService::createWarp(createWarpReqDto->savePath, createWarpReqDto->warpConfigs));
    }

#if LIBLINKERFS_ENABLE_UDF

    ENDPOINT_INFO(createUdfWarp) {
        info->summary = "Create warp file for UDF file";
        info->addConsumes<CreateUdfWarpReqDto::Wrapper>("application/json");
        info->addResponse<CreateWarpRespExample::Wrapper>(Status::CODE_200, "application/json");
        info->addResponse<ErrorResponseExapmle400::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<ErrorResponseExapmle500::Wrapper>(Status::CODE_500, "application/json");
    }

    ENDPOINT("POST", "api/warp/udf/create", createUdfWarp, BODY_DTO(Object<CreateUdfWarpReqDto>, createUdfWarpReqDto)) {
        OATPP_ASSERT_HTTP(createUdfWarpReqDto->savePath, Status::CODE_400,
                          QCoreApplication::tr("Field 'savePath' cannot be null").toStdString())
        OATPP_ASSERT_HTTP(!createUdfWarpReqDto->savePath->empty(), Status::CODE_400,
                          QCoreApplication::tr("Field 'savePath' cannot be empty").toStdString())
        OATPP_ASSERT_HTTP(createUdfWarpReqDto->udfPath, Status::CODE_400,
                          QCoreApplication::tr("Field 'udfPath' cannot be null").toStdString())
        OATPP_ASSERT_HTTP(!createUdfWarpReqDto->udfPath->empty(), Status::CODE_400,
                          QCoreApplication::tr("Field 'udfPath' cannot be empty").toStdString())
        OATPP_ASSERT_HTTP(createUdfWarpReqDto->warpTargets, Status::CODE_400,
                          QCoreApplication::tr("Field 'warpTargets' cannot be null").toStdString())
        return createDtoResponse(Status::CODE_200,
                                 UdfService::createWarp(createUdfWarpReqDto->udfPath, createUdfWarpReqDto->savePath,
                                                        createUdfWarpReqDto->warpTargets));
    }

#endif
};

#include OATPP_CODEGEN_END(ApiController)

#endif  //LINKERFS_WARP_GEN_OATPP_WARPCONTROLLER_HPP
