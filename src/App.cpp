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

#include "AppComponent.hpp"
#include "SwaggerComponent.hpp"
#include "controller/FileController.hpp"
#include "controller/WebuiController.hpp"
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/network/Server.hpp>

void run(const std::shared_ptr<oatpp::String> &address, const std::shared_ptr<uint16_t> &port) {

    AppComponent components;
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(FileController::createShared())->getEndpoints());
    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
    router->addController(WebuiController::createShared());
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {*address, *port, oatpp::network::Address::IP_4});

    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGD("Server", "Running on port %s...", connectionProvider->getProperty("port").toString()->c_str());
    server.run();
}

int main(int argc, char *argv[]) {

    //todo load config from cli
    auto address = std::make_shared<oatpp::String>("0.0.0.0");
    auto port = std::make_shared<uint16_t>(8080);
    oatpp::base::Environment::init();
    run(address, port);
    oatpp::base::Environment::destroy();
    return 0;
}