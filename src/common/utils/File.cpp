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

#include "File.hpp"

#include <filesystem>

namespace Utils::File {
    oatpp::Object<ListDirRespDto> listDir(QDir &&dir, QDir::Filter &&filter) {
        auto respData = ListDirRespDto::createShared();
        respData->dirPath = dir.path().toStdString();
        auto filterFlag = QDir::Filter::NoDotAndDotDot | QDir::Filter::Dirs | QDir::Filter::Files
                          | QDir::Filter::NoSymLinks & filter;
        auto items = dir.entryInfoList(filterFlag);
        for (const auto &item : items) {
            if (item.isFile()) {
                auto fileInfo = FileInfoDto::createShared();
                fileInfo->name = item.fileName().toStdString();
                fileInfo->size = std::to_string(item.size());
                respData->fileList->emplace_back(std::move(fileInfo));
            } else if (item.isDir()) {
                auto dirInfo = DirInfoDto::createShared();
                dirInfo->name = std::move(item.fileName().toStdString());
                dirInfo->isEmpty = QDir(item.filePath()).isEmpty(filterFlag);
                respData->dirList->emplace_back(dirInfo);
            }
        }
        return respData;
    }

    oatpp::Object<ListDirRespDto> listDrivers() {
        auto respData = ListDirRespDto::createShared();
        auto drivers = QDir::drives();
        respData->dirPath = "";
        for (const auto &driver : drivers) {
            auto dirInfo = DirInfoDto::createShared();
            dirInfo->name = driver.path().toStdString();
            dirInfo->isEmpty = QDir(driver.path()).isEmpty();
            respData->dirList->push_back(dirInfo);
        }
        return respData;
    }

    bool checkFileWritePermission(const QFileInfo &fileInfo) {
#ifdef __WIN32
        QNtfsPermissionCheckGuard permissionGuard;
#endif
        return fileInfo.isWritable();
    }

    bool checkFileReadPermission(const QFileInfo &fileInfo) {
#ifdef __WIN32
        QNtfsPermissionCheckGuard permissionGuard;
#endif
        return fileInfo.isReadable();
    }

    std::error_code makeHardLink(const std::string &srcPath, const std::string &dstPath) {
        std::error_code errorCode;
        std::filesystem::create_hard_link(srcPath, dstPath, errorCode);
        return errorCode;
    }

    bool writeFile(const QString &filePath, const QByteArray &data) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::NewOnly))
            return false;
        if (file.write(data) != data.size()) {
            file.remove();
            return false;
        }
        return true;
    }
}  //namespace Utils::File
