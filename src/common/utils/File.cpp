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

#include "File.hpp"

namespace Utils::File {
    oatpp::Object<DirRespDto> listDir(QDir &&dir, QDir::Filter &&filter) {
        auto respData = DirRespDto::createShared();
        respData->dirPath = dir.path().toStdString();
        auto filterFlag =
                QDir::Filter::NoDotAndDotDot | QDir::Filter::Dirs |
                QDir::Filter::Files | QDir::Filter::NoSymLinks & filter;
        auto items = dir.entryInfoList(filterFlag);
        for (const auto &item: items) {
            if (item.isFile()) {
                auto fileInfo = oatpp::Object<FileInfo>::createShared();
                fileInfo->name = item.fileName().toStdString();
                fileInfo->size = item.size();
                respData->fileList->emplace_back(std::move(fileInfo));
            } else if (item.isDir()) {
                auto dirInfo = DirInfo::createShared();
                dirInfo->name = std::move(item.fileName().toStdString());
                dirInfo->isEmpty = QDir(item.filePath()).isEmpty(filterFlag);
                respData->dirList->emplace_back(dirInfo);
            }
        }
        return respData;
    }

    oatpp::Object<DirRespDto> listDrivers() {
        auto respData = DirRespDto::createShared();
        auto drivers = QDir::drives();
        respData->dirPath = "";
        for (const auto &driver: drivers) {
            auto dirInfo = oatpp::Object<DirInfo>::createShared();
            dirInfo->name = driver.path().toStdString();
            dirInfo->isEmpty = QDir(driver.path()).isEmpty();
            respData->dirList->push_back(dirInfo);
        }
        return respData;
    }

}// namespace Utils::File
