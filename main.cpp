//
// Created by mike on 7/6/26.
//
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "include/a4cpp/a4cpp.hpp"

int main() {
    executor exe;

    exe.add_node(
        {
            [](nlohmann::json state) {
                std::cout << "Hi from node!" << std::endl;
            },
        }
    );

    std::this_thread::sleep_for(std::chrono::seconds(2));

    exe.add_node(
        {
            [](nlohmann::json state) { std::cout << "Hi from node!" << std::endl; },
            [](nlohmann::json state) { std::cout << "Hi from node!" << std::endl; },
            [](nlohmann::json state) { std::cout << "Hi from node!" << std::endl; },

        }
    );

    exe.start();
    return 0;
}
