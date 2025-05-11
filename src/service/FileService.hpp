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

#ifndef LINKERFS_WARP_GEN_OATPP_FILESERVICE_HPP
#define LINKERFS_WARP_GEN_OATPP_FILESERVICE_HPP

#include <QFileInfo>

#include "AbstractService.hpp"
#include "dto/response/ListDirRespDto.hpp"

class FileService : public AbstractService {

public:
    static oatpp::Object<ResponseDto> listDir(const oatpp::String &dirPath);
    static void assertFileCanBeCreated(const QFileInfo &fileInfo);
    static void assertFileReadable(const QFileInfo &fileInfo);
    static void assertFileWritable(const QFileInfo &fileInfo);
    static bool tryCreateHardLink(const std::string &srcPath, const std::string &dstPath);
};

#endif  //LINKERFS_WARP_GEN_OATPP_FILESERVICE_HPP
