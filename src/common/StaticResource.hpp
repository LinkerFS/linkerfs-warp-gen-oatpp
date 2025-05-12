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

#ifndef LINKERFS_WARP_GEN_OATPP_STATICRESOURCE_HPP_HPP
#define LINKERFS_WARP_GEN_OATPP_STATICRESOURCE_HPP_HPP

#include <QDir>
#include <oatpp/core/Types.hpp>

class MimeResource {
public:
    MimeResource(std::string &&resourceData, std::string &&mime)
        : resourceData(std::move(resourceData)), mime(std::move(mime)) {}

    std::string resourceData;
    std::string mime;
};

class StaticResource {
public:
    explicit StaticResource(const char *resourceDir);
    static std::shared_ptr<StaticResource> LoadResource(const char *resourceDir);
    [[nodiscard]] const MimeResource &getResource(const oatpp::String &resourcePath) const;
    void updateResource(const QString &key, const MimeResource &value);

private:
    void cacheResource(const QDir &resourceDir);
    QHash<QString, MimeResource> cache;
};

#endif  //LINKERFS_WARP_GEN_OATPP_STATICRESOURCE_HPP_HPP
