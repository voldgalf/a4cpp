cmake_minimum_required(VERSION 3.28)
project(a4cpp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(a4cpp main.cpp
        include/a4cpp/a4cpp.hpp)

find_package(spdlog REQUIRED)
target_link_libraries(a4cpp PRIVATE spdlog::spdlog)