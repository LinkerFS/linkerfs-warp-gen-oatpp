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

#include "UDF.hpp"
#include <QCoreApplication>

oatpp::Vector<oatpp::Object<FileNodeDto>> Utils::UDF::listDir(UDFDIR *udfDir,oatpp::String &dirName){
    udfread_dirent dirent{};
    const size_t dirLength=dirName->size();
    const auto dirNodes=oatpp::Vector<oatpp::Object<FileNodeDto>>::createShared();
    const auto fileNodes=oatpp::Vector<oatpp::Object<FileNodeDto>>::createShared();
    while (udfread_readdir(udfDir, &dirent)) {
        if (!strcmp(dirent.d_name, ".") || !strcmp(dirent.d_name, "..")) continue;
        auto fileNode=FileNodeDto::createShared();
        fileNode->name = dirent.d_name;
        if (dirent.d_type == UDF_DT_DIR) {
            auto child=std::unique_ptr<UDFDIR,decltype(&udfread_closedir)>(udfread_opendir_at(udfDir, dirent.d_name),udfread_closedir);
            if(!child)
            {
                dirNodes->emplace_back(std::move(fileNode));
                OATPP_LOGD("UDF", "%s",QCoreApplication::tr("error opening directory %1%2").arg(dirName->c_str(), dirent.d_name).toLocal8Bit().data())
                continue;
            }
            dirName->append(dirent.d_name).append("/");
            fileNode->children=listDir(child.get(),dirName);
            dirNodes->emplace_back(std::move(fileNode));
            dirName->resize(dirLength);
        } else {
            const auto udfFile=std::unique_ptr<UDFFILE,decltype(&udfread_file_close)>(udfread_file_openat(udfDir, dirent.d_name),udfread_file_close);
            if (!udfFile) {
                fileNode->size="";
                fileNodes->emplace_back(std::move(fileNode));
                OATPP_LOGD("UDF", "%s",QCoreApplication::tr("error opening file %1%2").arg(dirName->c_str(), dirent.d_name).toLocal8Bit().data())
                continue;
            }
            fileNode->size = std::to_string(udfread_file_size(udfFile.get()));
            fileNodes->emplace_back(std::move(fileNode));
        }
    }
    auto ret=oatpp::Vector<oatpp::Object<FileNodeDto>>::createShared();
    ret->reserve(dirNodes->size()+fileNodes->size());
    ret->insert(ret->end(),std::make_move_iterator(dirNodes->begin()),std::make_move_iterator(dirNodes->end()));
    ret->insert(ret->end(),std::make_move_iterator(fileNodes->begin()),std::make_move_iterator(fileNodes->end()));
    return ret;
}

