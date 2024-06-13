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

#include "OptionParser.hpp"

QCommandLineOption OptionParser::address = {"address", "Server listen address, default 0.0.0.0", "address", "0.0.0.0"};
QCommandLineOption OptionParser::port = {"port", "Server listen port, default 8080", "port", "8080"};
QCommandLineOption OptionParser::i18n = {"lang", "Locale code ID string, default en-US", "locale", ""};

OptionParser::OptionParser(QObject *parent) : QObject(parent) {
    parser.addOption(address);
    parser.addOption(port);
    parser.addOption(i18n);
    parser.addHelpOption();
}
