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

#ifndef LINKERFS_WARP_GEN_OATPP_WEBUICONTROLLER_HPP
#define LINKERFS_WARP_GEN_OATPP_WEBUICONTROLLER_HPP

#include <fstream>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <sstream>

#include OATPP_CODEGEN_BEGIN(ApiController)

static const oatpp::UnorderedMap<oatpp::String, oatpp::String> contentMap = {
        {"js", "text/javascript"},
        {"mjs", "text/javascript"},
        {"html", "text/html"},
        {"htm", "text/html"},
        {"css", "text/css"},
        {"svg", "image/svg+xml"}};

class WebuiController : public oatpp::web::server::api::ApiController {
    using oatpp::web::server::api::ApiController::ApiController;

public:
    static std::shared_ptr<WebuiController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
        return std::make_shared<WebuiController>(objectMapper);
    }

    static oatpp::String loadFile(const oatpp::String &filepath, std::stringstream &buf) {
        oatpp::String contentType = "text/html";
        oatpp::String root("./dist/");
        oatpp::String resPath = root + filepath;
        std::ifstream ifs(resPath->c_str(), std::ios::in);
        if (ifs) {
            buf << ifs.rdbuf();
            auto pos = filepath->find_last_of(".") + 1;
            oatpp::String suffix = filepath->substr(pos, filepath->size() - pos);
            auto ret = contentMap->find(suffix);
            contentType = ret == contentMap->end() ? "" : ret->second;

        } else {
            ifs.open("./dist/index.html", std::ios::in);
            buf << ifs.rdbuf();
        }
        ifs.close();
        return contentType;
    }

    ENDPOINT("GET", "/webui/*", index, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto filePath = request->getPathTail();
        OATPP_ASSERT_HTTP(filePath, Status::CODE_400, "Empty filename")
        std::stringstream buffer;
        oatpp::String type = loadFile(filePath, buffer);
        auto res = createResponse(Status::CODE_200, buffer.str());
        res->putHeader(Header::CONTENT_TYPE, type);
        return res;
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif//LINKERFS_WARP_GEN_OATPP_WEBUICONTROLLER_HPP
