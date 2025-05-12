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

#ifndef LINKERFS_WARP_GEN_OATPP_SWAGGERCONTROLLER_HPP_HPP
#define LINKERFS_WARP_GEN_OATPP_SWAGGERCONTROLLER_HPP_HPP

#include "common/StaticResource.hpp"

class SwaggerController final : public oatpp::web::server::api::ApiController {

public:
    explicit SwaggerController(const std::shared_ptr<ObjectMapper> &objectMapper,
                               const oatpp::Object<oatpp::swagger::oas3::Document> &document)
        : ApiController(objectMapper), document(document) {}

    static std::shared_ptr<SwaggerController>
    createShared(const oatpp::web::server::api::Endpoints &endpointsList,
                 const OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo)) {
        const auto generatorConfig = std::make_shared<oatpp::swagger::Generator::Config>();
        oatpp::swagger::Generator generator(generatorConfig);
        const auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
        serializerConfig->includeNullFields = false;
        const auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
        deserializerConfig->allowUnknownFields = false;
        const auto objectMapper =
                oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
        auto document = generator.generateDocument(documentInfo, endpointsList);
        return std::make_shared<SwaggerController>(objectMapper, document);
    }

#include OATPP_CODEGEN_BEGIN(ApiController)

    ENDPOINT("GET", "/api-docs/oas-3.0.0.json", docApi) {
        return createDtoResponse(Status::CODE_200, document);
    }

    ENDPOINT("GET", "/swagger/{filename}", swaggerUiResource, PATH(String, filename)) {
        const MimeResource* resource;
        if (filename == "ui") {
            resource = &swaggerResource.getResource("index.html");
        } else {
            resource = &swaggerResource.getResource(filename->c_str());
        }
        if (resource->resourceData.empty())
            return createResponse(Status::CODE_404, "");
        auto resp = createResponse(Status::CODE_200, resource->resourceData);
        resp->putHeader(Header::CONTENT_TYPE, resource->mime);
        return resp;
    }

#include OATPP_CODEGEN_END(ApiController)

private:
    oatpp::Object<oatpp::swagger::oas3::Document> document;
    StaticResource swaggerResource = StaticResource("swagger");
};

#endif  //LINKERFS_WARP_GEN_OATPP_SWAGGERCONTROLLER_HPP_HPP
