//
// Created by mike on 7/6/26.
//
#include <iostream>
#include <vector>
#include "include/a4cpp/a4cpp.hpp"

int main() {
    executor exe;

    exe.add_node(
        {
            [](nlohmann::json state) {
                std::cout << "Hi from node!" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            },
        }
    );

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
