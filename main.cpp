//
// Created by mike on 7/6/26.
//
#include <iostream>
#include <vector>
#include <chrono>
#include "include/a4cpp.hpp"

int main() {
    executor exe;


    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) {
                std::cout << "Opening database" << std::endl;
                state->set("port", 8080);
                state->set("name", std::string("myapp"));
                state->set("callback", std::function<void()>([] { std::cout << "hi\n"; }));
            }
        }
    );

    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) {
                std::cout << "Filtering database" << std::endl;
                std::cout << state->get<std::string>("name") << "\n";
            },
        }
    );

    exe.add_node(
        {
            [](std::shared_ptr<state> state) { std::cout << "Hi from node!" << std::endl; },
            [](std::shared_ptr<state> state) { std::cout << "Hi from node!" << std::endl; },
            [](std::shared_ptr<state> state) { std::cout << "Hi from node!" << std::endl; },

        }
    );

    exe.start();
    return 0;
}
