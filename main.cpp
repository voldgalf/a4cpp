#include <iostream>
#include <vector>
#include <chrono>
#include "include/a4cpp.hpp"

int main() {
    executor exe; // Create an executor instance


    // You can add a node by calling executor::add_node with std::vector<std::function<void(nlohmann::json)>> as your parameter.
    // This is a sequential node due to the vector's length which is one.
    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) {
                std::cout << "Hi from node!" << std::endl;
            },
        }
    );


    //Similar to the previous call except by including a vector larger than one, all functions will run concurrently.
    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) { std::cout << "Hi from node!" << std::endl; },
            [](const std::shared_ptr<state> &state) { std::cout << "Hi from node!" << std::endl; },
            [](const std::shared_ptr<state> &state) { std::cout << "Hi from node!" << std::endl; },

        }
    );

    // You can call all nodes, in order of creation by calling executor::start()
    exe.start();

    return 0;
}
