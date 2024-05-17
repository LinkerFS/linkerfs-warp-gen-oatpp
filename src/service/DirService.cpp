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

#include "DirService.hpp"
#include <QDir>

oatpp::Object<ResponseDto<DirRespDto>> DirService::listDir(const oatpp::String &dirPath) {

    QDir dir(dirPath->c_str());
    OATPP_ASSERT_HTTP(dir.exists(), Status::CODE_404, "Dir not found")
    auto dirRespDto = ResponseDto<DirRespDto>::createShared();
    dirRespDto->code=Status::CODE_200.code;
    dirRespDto->message="Success";
    dirRespDto->data=DirRespDto::createShared();
    dirRespDto->data->files={};
    dirRespDto->data->dirPath = dirPath;
    for (auto const &file: dir.entryList()) {
        dirRespDto->data->files->emplace_back(file.toStdString());
    }
    return dirRespDto;
}
