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

#include "AppComponent.hpp"
#include "OptionParser.hpp"
#include "SwaggerComponent.hpp"
#include "controller/FileController.hpp"
#include "controller/StaticController.hpp"
#include "controller/WarpController.hpp"
#include <QTranslator>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/network/Server.hpp>

void run(const std::shared_ptr<oatpp::String> &address, const std::shared_ptr<uint16_t> &port) {

    AppComponent components;
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(FileController::createShared())->getEndpoints());
    docEndpoints.append(router->addController(WarpController::createShared())->getEndpoints());
    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
    router->addController(StaticController::createShared());
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {*address, *port, oatpp::network::Address::IP_4});

    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGD("Server", QCoreApplication::tr("Running on port %1...")
                                 .arg(connectionProvider->getProperty("port").toString()->c_str())
                                 .toLocal8Bit())
    server.run();
}

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    QTranslator translator;
    QCommandLineParser &parser = (new OptionParser(&app))->parser;
    parser.process(app);
    //todo load config from cli
    auto address = std::make_shared<oatpp::String>(parser.value(OptionParser::address).toStdString());
    auto port = std::make_shared<uint16_t>(parser.value(OptionParser::port).toUInt());
    QString locale = parser.value(OptionParser::i18n);
    const static bool debug=parser.isSet(OptionParser::debug);

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