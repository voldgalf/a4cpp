# a4cpp - Automation For C++

a4cpp is a C++ dataflow library, allowing uncomplicated concurrent and sequential logic execution.


### Legend
| Not Started | Work In Progress  | Complete |
|------------------|-------------|----------|
|🟥|🟧|🟩

### TODO

| Feature             | Description                                                               | Status |
|---------------------|---------------------------------------------------------------------------|--------|
| API Refactor        | Refactor API, simplifying and optimizing logic                           | 🟧     |
| CMake find_package  | Allow CMake to make the a4cpp library importable using find_package       | 🟥     |
| Concurrent Support  | Allow a node to concurrently execute *multiple* functions                 | 🟩     |
| Error handling      | Handle errors from a node's logic appropriately and without crashing      | 🟧     |
| Logger Support      | Support for spdlog including: implementation & log exporting              | 🟧     |
| README Update       | Update the README for better branding, description, and usage             | 🟧     |
| Retry Support       | Allows a node to retry or run itself again, depending on the error        | 🟥     |
| Sequential Support  | Allow a node to sequentially execute a single function                    | 🟩     |
|Documentation | Write doxygen comments for effective and usable documentation | 🟥


## Installation

Clone the respiratory and install with CMake

```bash
git clone https://github.com/voldgalf/a4c.git
cd a4c

cmake -S . -B build
cmake --build build
sudo cmake --install build
```

## Simple Usage

```cpp

#include <iostream>
#include <a4cpp/node_executor.h>

// Did you notice a bug? Requesting a new feature? Just Complaining?
// Open an issue at: https://github.com/voldgalf/a4cpp/issues

int main() {
    a4cpp::node_executor executor; // Create an instance of node_executor

    // Add a new node by defining a lambda function that has a nlohmann::json variable as an argument and returns a nlohmann::json variable
    // Yes, the first argument is wrapped in a vector, but when the mode_flag parameter is SEQUENTIAL, only the first vector element is read
    executor.add_node(
        {
            [](nlohmann::json state) {
                std::cout << "This is a sequential node!\n";
                return state;
            }
        }, a4cpp::node::SEQUENTIAL);


    // You can create a cncurrent node by setting the mode_flag to CONCURRENT and defining multiple lambda functions within the node_vector parameter
    executor.add_node(
        {
            [](nlohmann::json state) {
                std::cout << "We run at the same time!\n";
                return state;
            },
            [](nlohmann::json state) {
                std::cout << "We do?\n";
                return state;
            }
        }, a4cpp::node::CONCURRENT);

    // This simply executes all nodes, in the order they are added
    executor.run();

    return 0;
}
```

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE.md) for details.

## Author

This respiratory is maintained by [Michael MacMullen "Voldgalf"](https://github.com/voldgalf)

---

### Powered by

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/2d/JetBrains_company_logo.svg/1280px-JetBrains_company_logo.svg.png" width="250">
