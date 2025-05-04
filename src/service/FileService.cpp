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

#include "FileService.hpp"
#include <QCoreApplication>
#include "common/utils/File.hpp"

oatpp::Object<ResponseDto> FileService::listDir(const oatpp::String &dirPath) {
    oatpp::Object<ResponseDto> resp;
    QDir dir(dirPath->c_str());
    if (dirPath == "") resp = ResponseDto::success(Utils::File::listDrivers());
    else {
        OATPP_ASSERT_HTTP(dir.exists(), Status::CODE_404,
                          QCoreApplication::tr("Dir %1 not found!").arg(dirPath->c_str()).toStdString())
        resp = ResponseDto::success(Utils::File::listDir(std::move(dir)));
    }
    return resp;
}

void FileService::assertFileCanBeCreated(const QFileInfo &fileInfo) {
    OATPP_ASSERT_HTTP(!fileInfo.exists(), Status::CODE_500,
                      QCoreApplication::tr("%1 already exists").arg(fileInfo.absoluteFilePath()).toStdString())
    OATPP_ASSERT_HTTP(Utils::File::checkFileWritePermission(QFileInfo(fileInfo.absolutePath())), Status::CODE_500,
                      QCoreApplication::tr("%1 is not writable").arg(fileInfo.absoluteFilePath()).toStdString())
}

void FileService::assertFileReadable(const QFileInfo &fileInfo) {
    OATPP_ASSERT_HTTP(fileInfo.exists(), Status::CODE_500,
                      QCoreApplication::tr("%1 does not exist").arg(fileInfo.absoluteFilePath()).toStdString())
    OATPP_ASSERT_HTTP(Utils::File::checkFileReadPermission(fileInfo), Status::CODE_500,
                      QCoreApplication::tr("%1 is not readable").arg(fileInfo.absoluteFilePath()).toStdString())
}

void FileService::assertFileWritable(const QFileInfo &fileInfo) {
    OATPP_ASSERT_HTTP(fileInfo.exists(), Status::CODE_500,
                      QCoreApplication::tr("%1 does not exist").arg(fileInfo.absoluteFilePath()).toStdString())
    OATPP_ASSERT_HTTP(Utils::File::checkFileWritePermission(fileInfo), Status::CODE_500,
                      QCoreApplication::tr("%1 is not writable").arg(fileInfo.absoluteFilePath()).toStdString())
}
