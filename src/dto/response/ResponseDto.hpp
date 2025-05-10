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

#ifndef LINKERFS_WARP_GEN_OATPP_RESPONSEDTO_HPP
#define LINKERFS_WARP_GEN_OATPP_RESPONSEDTO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class ResponseDto : public oatpp::DTO {
    DTO_INIT(ResponseDto, DTO)

    DTO_FIELD(Int32, code);
    DTO_FIELD_INFO(code) {
        info->description = "Response state code";
    }

    DTO_FIELD(Any, data);
    DTO_FIELD_INFO(data) {
        info->description = "Response data";
    }

    DTO_FIELD(String, msg);
    DTO_FIELD_INFO(msg) {
        info->description = "Response message";
    }

private:
    typedef oatpp::web::protocol::http::Status Status;

public:
    template<typename T,std::enable_if_t<std::is_base_of_v<oatpp::DTO,T>>>
    static Object<ResponseDto> success(Object<T> &&data) {
        auto dto = wrapper(Status::CODE_200.code, std::forward<Object<T>>(data), String("Success"));
        return dto;
    }

    template<typename T>
    static Object<ResponseDto> success(T &&data) {
        auto dto = wrapper(Status::CODE_200.code, std::forward<T>(data), String("Success"));
        return dto;
    }

    static Object<ResponseDto> success() {
        auto dto = wrapper(Status::CODE_200.code, nullptr, String("Success"));
        return dto;
    }

    static Object<ResponseDto> fail(const Status &status, oatpp::String &&msg = "") {
        auto response = wrapper(status.code, nullptr, std::move(msg));
        return response;
    }

private:
    static Object<ResponseDto> wrapper(const int &code, Any &&data, String &&msg) {
        auto dto = ResponseDto::createShared();
        dto->code = code;
        dto->data = std::move(data);
        dto->msg = std::move(msg);
        return dto;
    }
};

#endif//LINKERFS_WARP_GEN_OATPP_RESPONSEDTO_HPP
