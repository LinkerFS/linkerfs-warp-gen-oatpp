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

#ifndef LINKERFS_WARP_GEN_OATPP_DOCDTOMACRO_HPP
#define LINKERFS_WARP_GEN_OATPP_DOCDTOMACRO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class EmptyDTO : public oatpp::DTO {};

#include OATPP_CODEGEN_END(DTO)

#define GEN_SUCCESS_RESP_DTO(DTO_NAME, DATA_OBJECT)                                                                    \
    class DTO_NAME : public oatpp::DTO {                                                                               \
        using dataType = std::conditional<std::is_base_of_v<oatpp::DTO, DATA_OBJECT>, oatpp::Object<DATA_OBJECT>,      \
                                          DATA_OBJECT>::type;                                                          \
        DTO_INIT(DTO_NAME, DTO);                                                                                       \
        DTO_FIELD(Int32, code) = 200;                                                                                  \
        DTO_FIELD_INFO(code) {                                                                                         \
            info->description = "Response state code";                                                                 \
        }                                                                                                              \
        DTO_FIELD(dataType, data);                                                                                     \
        DTO_FIELD_INFO(data) {                                                                                         \
            info->description = "Response data";                                                                       \
        }                                                                                                              \
        DTO_FIELD(String, msg);                                                                                        \
        DTO_FIELD_INFO(msg) {                                                                                          \
            info->description = "Response message";                                                                    \
        }                                                                                                              \
    };

#define GEN_ERROR_RESP_DTO(DTO_NAME, RESP_CODE)                                                                        \
    class DTO_NAME : public oatpp::DTO {                                                                               \
        DTO_INIT(DTO_NAME, DTO);                                                                                       \
        DTO_FIELD(Int32, code) = RESP_CODE;                                                                            \
        DTO_FIELD_INFO(code) {                                                                                         \
            info->description = "Response state code";                                                                 \
        }                                                                                                              \
        DTO_FIELD(String, msg);                                                                                        \
        DTO_FIELD_INFO(msg) {                                                                                          \
            info->description = "Error message";                                                                       \
        }                                                                                                              \
    };

#endif  //LINKERFS_WARP_GEN_OATPP_DOCDTOMACRO_HPP