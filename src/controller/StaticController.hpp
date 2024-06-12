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

#ifndef LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP
#define LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP

#include <QDir>
#include <QFile>
#include <QMimeDatabase>
#include <fstream>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <sstream>

#include OATPP_CODEGEN_BEGIN(ApiController)

static const std::unordered_map<std::string, std::string> contentMap = {
        {"js", "text/javascript"},
        {"mjs", "text/javascript"},
        {"html", "text/html"},
        {"htm", "text/html"},
        {"css", "text/css"},
        {"svg", "image/svg+xml"}};

class StaticController : public oatpp::web::server::api::ApiController {
    using oatpp::web::server::api::ApiController::ApiController;

public:
    static std::shared_ptr<StaticController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
        return std::make_shared<StaticController>(objectMapper);
    }

    static QByteArray loadFile(const QString &filePath) {
        QByteArray data;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            file.close();
        }
        return data;
    }

    ENDPOINT("GET", "/webui/*", webui, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        oatpp::String filePath = request->getPathTail();
        QDir dir("./dist");
        const char *fileName;
        if (!filePath->empty() && dir.exists(filePath->data())) {
            fileName = filePath->data();
        } else {
            fileName = "index.html";
        }
        QByteArray fileData = loadFile(dir.filePath(fileName));
        QMimeType type = mimeDatabase.mimeTypeForFileNameAndData(fileName, fileData);
        auto respData = oatpp::String(fileData.data(), fileData.size());
        auto resp = ResponseFactory::createResponse(Status::CODE_200, respData);
        resp->putHeader(Header::CONTENT_TYPE, type.name().toStdString());
        return resp;
    }

    ENDPOINT("GET", "/doc", swagger, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = ResponseFactory::createResponse(Status::CODE_301, nullptr);
        resp->putHeader("Location", "/swagger/ui");
        return resp;
    }

    ENDPOINT("GET", "/", root, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = ResponseFactory::createResponse(Status::CODE_301, nullptr);
        resp->putHeader("Location", "/webui");
        return resp;
    }

private:
    QMimeDatabase mimeDatabase;
};

#include OATPP_CODEGEN_END(ApiController)

#endif//LINKERFS_WARP_GEN_OATPP_STATICCONTROLLER_HPP
