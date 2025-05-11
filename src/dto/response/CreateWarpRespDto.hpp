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

#ifndef LINKERFS_WARP_GEN_OATPP_CREATEWARPRESPDTO_HPP
#define LINKERFS_WARP_GEN_OATPP_CREATEWARPRESPDTO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class CreateWarpRespDto : public oatpp::DTO {
    DTO_INIT(CreateWarpRespDto, DTO)

    DTO_FIELD(Vector<String>, warpFiles) = {};

    DTO_FIELD_INFO(warpFiles) {
        info->description = "Warp files created";
    }

    DTO_FIELD(Vector<String>, hardlinkFiles) = {};

    DTO_FIELD_INFO(hardlinkFiles) {
        info->description = "Hardlink files created";
    }

    DTO_FIELD(Vector<String>, failedFiles) = {};

    DTO_FIELD_INFO(failedFiles) {
        info->description = "Files failed to create";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif  //LINKERFS_WARP_GEN_OATPP_CREATEWARPRESPDTO_HPP
