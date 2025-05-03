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

#ifndef LINKERFS_WARP_GEN_OATPP_WARPCONFIGWRAPPER_HPP_HPP
#define LINKERFS_WARP_GEN_OATPP_WARPCONFIGWRAPPER_HPP_HPP

#include <string>
#include <vector>
#include "liblinkerfs/data/warp.h"

class WarpConfigWrapper {
    typedef decltype(WARP_CONFIG::warp_count) WarpTargetSizeType;

public:
    WarpConfigWrapper(const WarpTargetSizeType &targetCount, std::string &&fileName);
    WARP_TARGET &operator[](const std::size_t &index) {
        return warpTargets.at(index);
    }
    [[nodiscard]] const WARP_CONFIG &getConfig() const {
        return warpConfig;
    }
    [[nodiscard]] const std::string &getWarpFileName() const {
        return warpFileName;
    }

private:
    std::string warpFileName;
    WARP_CONFIG warpConfig = {};
    std::vector<WARP_TARGET> warpTargets;
};

#endif //LINKERFS_WARP_GEN_OATPP_WARPCONFIGWRAPPER_HPP_HPP
