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

#ifndef LINKERFS_WARP_GEN_OATPP_WARP_HPP
#define LINKERFS_WARP_GEN_OATPP_WARP_HPP

#include "dto/common/WarpConfigDto.hpp"
#include "dto/common/WarpTargetDto.hpp"
#include "liblinkerfs/data/warp.h"
#include <qstring.h>

namespace Utils::Warp {
    /**
     * validate data size and fill data to WARP_TARGET
     * @param target target info
     * @param targetForLib object to save transformed data
     * @return true is valid otherwise false
     */
    bool targetValidateSize(const oatpp::Object<WarpTargetDto> &target, WARP_TARGET *targetForLib);
    bool canUseHardLink(const WARP_CONFIG &config);
    bool creatWarpFile(const QString &filePath, const WARP_CONFIG *config);
}// namespace Utils::Warp
#endif//LINKERFS_WARP_GEN_OATPP_WARP_HPP
