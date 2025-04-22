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

#ifndef LINKERFS_WARP_GEN_OATPP_CREATEUDFWARPREQDTO_HPP
#define LINKERFS_WARP_GEN_OATPP_CREATEUDFWARPREQDTO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include "dto/common/WarpTargetDto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class CreateUdfWarpReqDto : public oatpp::DTO {
    DTO_INIT(CreateUdfWarpReqDto, DTO)

    DTO_FIELD(String, udfPath);
    DTO_FIELD_INFO(udfPath) { info->description = "absolute path of UDF file to remap data"; }

    DTO_FIELD(String, savePath);
    DTO_FIELD_INFO(savePath) { info->description = "absolute path of directory to save warp configuration files"; }

    DTO_FIELD(Vector<Object<WarpTargetDto>>, warpTargets);
    DTO_FIELD_INFO(warpTargets) { info->description = "warp targets in UDF file"; }
};

#include OATPP_CODEGEN_END(DTO)

#endif //LINKERFS_WARP_GEN_OATPP_CREATEUDFWARPREQDTO_HPP
