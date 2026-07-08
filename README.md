# a4cpp - Automation For C++

a4cpp is a C++ dataflow library, allowing uncomplicated concurrent and sequential logic execution.

### Legend

| Not Started | Work In Progress | Complete |
|-------------|------------------|----------|
| 🟥          | 🟧               | 🟩       |

### TODO

| Feature            | Description                                                          | Status |
|--------------------|----------------------------------------------------------------------|--------|
| API Refactor       | Refactor API, simplifying and optimizing logic                       | 🟧     |
| CMake find_package | Allow CMake to make the a4cpp library importable using find_package  | 🟥     |
| Concurrent Support | Allow a node to concurrently execute *multiple* functions            | 🟩     |
| Error handling     | Handle errors from a node's logic appropriately and without crashing | 🟧     |
| Logger Support     | Support for spdlog including: implementation & log exporting         | 🟧     |
| README Update      | Update the README for better branding, description, and usage        | 🟧     |
| Retry Support      | Allows a node to retry or run itself again, depending on the error   | 🟥     |
| Sequential Support | Allow a node to sequentially execute a single function               | 🟩     |
| Documentation      | Write doxygen comments for effective and usable documentation        | 🟥     |

## Installation

Clone the respiratory and install with CMake

## Simple Usage

```cpp
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
```

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

### Powered by

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/2d/JetBrains_company_logo.svg/1280px-JetBrains_company_logo.svg.png" width="250">