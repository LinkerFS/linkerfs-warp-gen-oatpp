# Qt
set(QT_MINIMUM_VERSION "6.4.2")
find_package(Qt6 ${QT_MINIMUM_VERSION} REQUIRED COMPONENTS Core LinguistTools)
if (WIN32)
    #Required for QNtfsPermissionCheckGuard
    set(QT_MINIMUM_VERSION "6.6.0")
endif ()
# oat++
find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(SOURCES
        App.cpp
        ErrorHandler.cpp
        common/StaticResource.cpp
        common/utils/File.cpp
        common/utils/Warp.cpp
        common/wrapper/WarpConfigWrapper.cpp
        common/wrapper/WarpFileWrapper.cpp
        service/FileService.cpp
        service/WarpService.cpp
        OptionParser.cpp
)

add_executable(linkerfs_warp_gen_oatpp ${SOURCES} ${HEADERS})
target_link_libraries(linkerfs_warp_gen_oatpp PRIVATE
        Qt6::Core
        oatpp::oatpp
        oatpp::oatpp-swagger
        liblinkerfs
)

set_target_properties(linkerfs_warp_gen_oatpp PROPERTIES OUTPUT_NAME "lwg_server" RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(linkerfs_warp_gen_oatpp
        PUBLIC ${oatpp_INCLUDE_DIRS} ${oatpp-swagger_INCLUDE_DIRS}
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        ${liblinkerfs_INCLUDE_DIRS}
)

if (${LIBLINKERFS_FEATURE_UDF})
    target_sources(linkerfs_warp_gen_oatpp
            PRIVATE
            common/utils/UDF.cpp
            service/UdfService.cpp
    )
endif ()

#i18n
set(I18N_LOCALE zh-cn)
foreach (lang ${I18N_LOCALE})
    list(APPEND TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/i18n/${lang}.ts")
endforeach ()

qt_add_lupdate(linkerfs_warp_gen_oatpp
        TS_FILES ${TS_FILES}
        OPTIONS -no-obsolete
)
qt_add_lrelease(linkerfs_warp_gen_oatpp
        TS_FILES ${TS_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
)
qt_add_resources(linkerfs_warp_gen_oatpp "i18n"
        PREFIX "/i18n"
        BASE ${CMAKE_CURRENT_BINARY_DIR}
        FILES ${QM_FILES}
)

#webui
if (NOT ${EXTERNAL_WEBUI_PATH} STREQUAL "")
    file(GLOB_RECURSE WEBUI_RESOURCE_LIST "${EXTERNAL_WEBUI_PATH}/*")
    qt_add_resources(linkerfs_warp_gen_oatpp "dist"
            PREFIX "/dist"
            BASE ${EXTERNAL_WEBUI_PATH}
            FILES ${WEBUI_RESOURCE_LIST}
    )
endif ()

#swagger
set(OATPP_SWAGGER_RES_PATH "${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")
file(GLOB_RECURSE SWAGGER_RESOURCE_LIST "${OATPP_SWAGGER_RES_PATH}/*")
qt_add_resources(linkerfs_warp_gen_oatpp "swagger"
        PREFIX "/swagger"
        BASE "${OATPP_SWAGGER_RES_PATH}"
        FILES ${SWAGGER_RESOURCE_LIST}
)