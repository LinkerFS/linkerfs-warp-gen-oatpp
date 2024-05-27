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

#ifndef LINKERFS_WARP_GEN_ERRORHANDLER_HPP
#define LINKERFS_WARP_GEN_ERRORHANDLER_HPP

#include "dto/ResponseDto.hpp"

#include "oatpp/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "oatpp/web/server/handler/ErrorHandler.hpp"

class ErrorHandler : public oatpp::web::server::handler::ErrorHandler {
private:
    typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
    typedef oatpp::web::protocol::http::Status Status;
    typedef oatpp::web::protocol::http::outgoing::ResponseFactory ResponseFactory;

private:
    std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;

public:
    ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper);
    std::shared_ptr<OutgoingResponse>
    handleError(const Status &status, const oatpp::String &message, const Headers &headers) override;
};


#endif//LINKERFS_WARP_GEN_ERRORHANDLER_HPP
