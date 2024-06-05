# Qt
set(QT_MINIMUM_VERSION "6.6.0")
find_package(Qt6 ${QT_MINIMUM_VERSION} REQUIRED COMPONENTS Core)

# oat++
find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(SOURCES
        App.cpp
        ErrorHandler.cpp
        common/utils/File.cpp
        service/FileService.cpp
        service/WarpService.cpp
        common/utils/Warp.cpp
)

add_executable(linkerfs_warp_gen_oatpp ${SOURCES} ${HEADERS})
target_link_libraries(linkerfs_warp_gen_oatpp PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        oatpp::oatpp
        oatpp::oatpp-swagger
        liblinkerfs
)

set_target_properties(linkerfs_warp_gen_oatpp PROPERTIES OUTPUT_NAME "lwg_server" RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(linkerfs_warp_gen_oatpp
        PUBLIC ${oatpp_INCLUDE_DIRS} ${oatpp-swagger_INCLUDE_DIRS}
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)