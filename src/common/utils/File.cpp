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

#ifdef _WIN32
#include <windows.h>
#endif

#include "File.hpp"

namespace Utils::File {

    oatpp::Object<ListDirRespDto> listDir(fs::directory_entry &&dir) {
        auto respData = ListDirRespDto::createShared();
        respData->dirPath = dir.path().string();
        auto iterator = fs::directory_iterator(std::move(dir));
        for (const auto &item: iterator) {
            if (item.is_regular_file()) {
                auto fileInfo = FileInfoDto::createShared();
                fileInfo->name = item.path().filename().string();
                fileInfo->size = item.file_size();
                respData->fileList->emplace_back(fileInfo);
            } else if (item.is_directory()) {
                auto dirInfo = DirInfoDto::createShared();
                dirInfo->name = item.path().filename().string();
                dirInfo->isEmpty = checkDirEmpty(std::move(dir));
                respData->dirList->emplace_back(dirInfo);
            }
        }
        return respData;
    }

    oatpp::Object<ListDirRespDto> listDrivers() {
        auto respData = ListDirRespDto::createShared();
        respData->dirPath = "";
#ifdef _WIN32
        WCHAR driveName[] = L"A:\\";
        DWORD drives = GetLogicalDrives();
        while (drives) {
            if (drives & 1) {
                UINT type = GetDriveTypeW(driveName);
                //exclude DRIVE_UNKNOWN DRIVE_NO_ROOT_DIR
                if (type > 1 && type < 7) {
                    auto dirInfo = DirInfoDto::createShared();
                    auto wstring = std::wstring(driveName);
                    auto name = std::string(wstring.begin(), wstring.end());
                    dirInfo->isEmpty = checkDirEmpty(fs::directory_entry(name));
                    name[name.size() - 1] = '/';
                    dirInfo->name = std::move(name);
                    respData->dirList->emplace_back(dirInfo);
                }
            }
            ++driveName[0];
            drives >>= 1;
        }
#else
        auto dirInfo = oatpp::Object<DirInfo>::createShared();
        dirInfo->name = "/";
        dirInfo->isEmpty = false;
        respData->dirList->push_back(dirInfo);
#endif
        return respData;
    }

    bool checkDirEmpty(fs::directory_entry &&dir) {
        bool isEmpty = true;
        auto iterator = fs::directory_iterator(dir);
        while (isEmpty && iterator != end(iterator))
            isEmpty = !(iterator->is_directory() || iterator->is_regular_file());
        return isEmpty;
    }
}// namespace Utils::File
