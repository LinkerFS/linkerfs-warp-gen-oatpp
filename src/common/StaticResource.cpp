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

#include "StaticResource.hpp"

#include <QDirIterator>
#include <QMimeDatabase>

#include "utils/File.hpp"

const static auto emptyResource = MimeResource("", "");
const static QMimeDatabase mimeDatabase;

std::shared_ptr<StaticResource> StaticResource::LoadResource(const char *resourceDir) {
    return std::make_shared<StaticResource>(resourceDir);
}

const MimeResource &StaticResource::getResource(const oatpp::String &resourcePath) const {
    auto it = cache.find(resourcePath->c_str());
    if (it != cache.end())
        return *it;
    return emptyResource;
}

void StaticResource::updateResource(const QString &key, const MimeResource &value) {
    cache.insert(key, value);
}

StaticResource::StaticResource(const char *resourceDir) {
    QDir resource(QString(":/%1").arg(resourceDir));
    if (!resource.exists() || resource.isEmpty()) {
        resource.setPath(QString("./%1").arg(resourceDir));
    }
    cacheResource(resource);
}

void StaticResource::cacheResource(const QDir &resourceDir) {
    QDirIterator it(resourceDir.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (QFile file(it.filePath()); file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            MimeResource mimeResource(
                    data.toStdString(),
                    mimeDatabase.mimeTypeForFileNameAndData(it.fileName(), data).name().toStdString());
            cache.insert(resourceDir.relativeFilePath(it.filePath()), mimeResource);
        }
    }
}
