#include <iostream>
#include "include/a4cpp.hpp"

int main() {
    executor exe; // Create an executor instance
    // You can add a node by calling executor::add_node with std::vector<std::function<void(nlohmann::json)>> as your parameter.
    // This is a sequential node due to the vector's length which is one.

    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) {
            },
        }
    );

    exe.add_node({
        {
            [](const std::shared_ptr<state> &state) {
                int result = 2 * 2;
                state->set("result", result);
            }
        }
    });

    //Similar to the previous call except by including a vector larger than one, all functions will run concurrently.
    exe.add_node(
        {
            [](const std::shared_ptr<state> &state) {
                std::cout << "What is 2 * 2? " << state->get<int>("result") << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            },
            [](const std::shared_ptr<state> &state) {
                std::cout << "2 * 2 isn't 8! It is: " << state->get<int>("result") << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            },
        }
    );

    // You can call all nodes, in order of creation by calling executor::start()
    exe.start();

    return 0;
}
