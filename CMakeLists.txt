cmake_minimum_required(VERSION 3.28)
project(a4cpp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(a4cpp ${SOURCES})

target_include_directories(a4cpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

find_package(spdlog REQUIRED)
target_link_libraries(a4cpp PRIVATE spdlog::spdlog)

option(A4CPP_INSTALL "Generate install target and CMake package config for a4cpp" ON)

if (A4CPP_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(A4CPP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/a4cpp")

    install(TARGETS a4cpp
            EXPORT a4cppTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )

    # Exported target file: a4cppTargets.cmake
    install(EXPORT a4cppTargets
            FILE a4cppTargets.cmake
            NAMESPACE a4cpp::
            DESTINATION ${A4CPP_INSTALL_CMAKEDIR}
    )
endif ()