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

#ifndef LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP
#define LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP

#include <QMimeDatabase>
#include <fstream>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "common/StaticResource.hpp"
#include "dto/common/DocDtoMacro.hpp"
#include "liblinkerfs/common.h"

#include OATPP_CODEGEN_BEGIN(DTO)
GEN_SUCCESS_RESP_DTO(FeatureRespDoc, UInt64)
#include OATPP_CODEGEN_END(DTO)

class StaticController : public oatpp::web::server::api::ApiController {
    using ApiController::ApiController;

public:
    static std::shared_ptr<StaticController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                          objectMapper)) {
        return std::make_shared<StaticController>(objectMapper);
    }

#include OATPP_CODEGEN_BEGIN(ApiController)

    ENDPOINT_INFO(webui) {
        info->hide = true;
    }

    ENDPOINT("GET", "/webui/*", webui, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        const oatpp::String filePath = request->getPathTail();
        const MimeResource *resource = &webuiResource.getResource(filePath->c_str());
        if (filePath->empty() || resource->resourceData.empty()) {
            resource = &webuiResource.getResource("index.html");
        }
        auto resp = ResponseFactory::createResponse(Status::CODE_200, resource->resourceData);
        resp->putHeader(Header::CONTENT_TYPE, resource->mime);
        return resp;
    }

    ENDPOINT_INFO(swagger) {
        info->hide = true;
    }

    ENDPOINT("GET", "/doc", swagger, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = ResponseFactory::createResponse(Status::CODE_301, nullptr);
        resp->putHeader("Location", "/swagger/ui");
        return resp;
    }

    ENDPOINT_INFO(getFeature) {
        info->summary = "Create warp file for UDF file";
        info->addResponse<FeatureRespDoc::Wrapper>(Status::CODE_200, "application/json");
    }

    ENDPOINT("GET", "api/feature", getFeature, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        const auto resp = ResponseDto::success(oatpp::UInt64(feature()));
        return createDtoResponse(Status::CODE_200, resp);
    }

    ENDPOINT_INFO(root) {
        info->hide = true;
    }

    ENDPOINT("GET", "/", root, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = ResponseFactory::createResponse(Status::CODE_301, nullptr);
        resp->putHeader("Location", "/webui");
        return resp;
    }

private:
    StaticResource webuiResource = StaticResource("dist");
};

#include OATPP_CODEGEN_END(ApiController)

#endif  //LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP
