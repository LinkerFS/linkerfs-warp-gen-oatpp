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

#include <QTranslator>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/network/Server.hpp>

#include "AppComponent.hpp"
#include "OptionParser.hpp"
#include "controller/FileController.hpp"
#include "controller/StaticController.hpp"
#include "controller/SwaggerController.hpp"
#include "controller/WarpController.hpp"
#include "liblinkerfs/log/log_config.h"

void run(const oatpp::String &address, const uint16_t &port) {

    AppComponent components;
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(FileController::createShared())->getEndpoints());
    docEndpoints.append(router->addController(WarpController::createShared())->getEndpoints());
    docEndpoints.append(router->addController(StaticController::createShared())->getEndpoints());
    router->addController(SwaggerController::createShared(docEndpoints));
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    const auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {address, port, oatpp::network::Address::IP_4});

    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGD("Server", "%s",
               QCoreApplication::tr("Running on port %1...")
                       .arg(connectionProvider->getProperty("port").toString()->c_str())
                       .toLocal8Bit()
                       .data())
    server.run();
}

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    QTranslator translator;
    QCommandLineParser &parser = (new OptionParser(&app))->parser;
    parser.process(app);
    const oatpp::String address = parser.value(OptionParser::address).toStdString();
    const uint16_t port = parser.value(OptionParser::port).toUInt();
    const QString locale = parser.value(OptionParser::i18n);
    const static bool debug = parser.isSet(OptionParser::debug);
    liblinkerfs_set_log_handler([](const char *file, const int line, const char *func, const char *msg) {
        if (debug) {
            OATPP_LOGD("Liblinkerfs", "%s:%d %s: %s", file, line, func, msg);
        } else {
            OATPP_LOGD("Liblinkerfs", "%s", msg);
        }
    });
    oatpp::base::Environment::init();
    if (!locale.isEmpty()) {
        if (translator.load(QString(":/i18n/%1.qm").arg(locale.toLower()))) {
            QCoreApplication::installTranslator(&translator);
        } else {
            OATPP_LOGW("Server", "Translation %s load failed", locale.toLocal8Bit().data())
        }
    }
    run(address, port);
    oatpp::base::Environment::destroy();
    return 0;
}
