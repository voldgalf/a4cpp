cmake_minimum_required(VERSION 3.28)
project(a4cpp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(a4cpp ${SOURCES})

target_include_directories(a4cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/*)

find_package(spdlog REQUIRED)
target_link_libraries(a4cpp PRIVATE spdlog::spdlog)