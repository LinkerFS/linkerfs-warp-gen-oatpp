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

#include "Warp.hpp"

#include "File.hpp"
#include "liblinkerfs/generator.h"

namespace Utils::Warp {
    bool targetValidateSizeAndFill(const oatpp::Object<WarpTargetDto> &target, WARP_TARGET *warpTarget) {
        bool ok;
        const QFileInfo file(target->filePath->data());
        if (!file.exists() && file.isFile())
            return false;
        const qint64 dataOffset = QString(target->dataOffset->data()).toLongLong(&ok);
        if (!ok || dataOffset < 0)
            return false;
        const qint64 dataSize = QString(target->dataSize->data()).toLongLong(&ok);
        if (!ok || dataSize <= 0)
            return false;
        warpTarget->file_path = target->filePath->data();
        warpTarget->offset_in_file = dataOffset;
        warpTarget->size_to_read = dataSize;
        warpTarget->path_length = static_cast<int32_t>(file.absoluteFilePath().length());
        return dataSize + dataOffset <= file.size();
    }

    bool canUseHardLink(const WARP_CONFIG &config) {
        if (config.warp_count == 1) {
            QFileInfo file(config.warp_targets->file_path);
            return config.warp_targets->offset_in_file == 0 && file.size() == config.warp_targets->size_to_read;
        }
        return false;
    }

    bool createWarpFile(const QString &filePath, const WARP_CONFIG *config) {
        WARP_FILE warpFile = generate_warp_file(config);
        const bool ret = createWarpFile(filePath, warpFile);
        release_warp_file(&warpFile);
        return ret;
    }

    inline bool createWarpFile(const QString &filePath, const WARP_FILE &warpFile) {
        const QByteArray data = QByteArray::fromRawData(reinterpret_cast<char *>(warpFile.data), warpFile.length);
        return Utils::File::writeFile(filePath, data);
    }

}  //namespace Utils::Warp
