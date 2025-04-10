if (BUILD_SHARED_LIBS)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".dll.a")
else ()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")
endif ()

# find liblinkerfs
find_library(LIB_linkerfs NAMES linkerfs PATHS ${CMAKE_CURRENT_SOURCE_DIR}/binary/liblinkerfs/lib NO_CACHE)
if (LIB_linkerfs)
    message(STATUS "Found Library linkerfs: ${LIB_linkerfs}")
    add_library(liblinkerfs UNKNOWN IMPORTED GLOBAL)
    set_target_properties(liblinkerfs PROPERTIES IMPORTED_LOCATION ${LIB_linkerfs})
    target_include_directories(liblinkerfs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/binary/liblinkerfs/include)
else ()
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/liblinkerfs/CMakeLists.txt)
        add_subdirectory(liblinkerfs)
    else ()
        message(FATAL_ERROR "Not Found liblinkerfs, please run `git submodule update --recursive --init`")
    endif ()
endif ()