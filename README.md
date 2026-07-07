# a4cpp - Automation For C++

a4cpp is a C++ dataflow library, allowing uncomplicated concurrent and sequential logic execution.

## TODO

### Legend

| Not Started | Work In Progress  | Complete |
|------------------|-------------|----------|
|🟥|🟧|🟩

### TODO Table

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

## Dependencies

The following dependencies are as of 7/7/2026

- [nlohmann/json](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)

## Installation

```bash
git clone https://github.com/voldgalf/a4c.git
cd a4c

cmake -S . -B build
cmake --build build
sudo cmake --install build
```

## Support

```bash
*****************************************************************
Did you notice a bug? Requesting a new feature? Or just Complaining?
Support Available: michael.macmullen@tutamail.com
In the subject line put "a4cpp - <query_here>" please.
*****************************************************************
```

## Simple Usage

```cpp
#include <iostream>
#include <vector>
#include <a4cpp/a4cpp.hpp>

// *****************************************************************
// Did you notice a bug? Requesting a new feature? Or just Complaining?
// Support Available: michael.macmullen@tutamail.com
// In the subject line put "a4cpp - <query_here>" please.
// *****************************************************************

int main() {
    executor exe; // Create an instance of executor

    // You can create a new node by calling executor::add_node
    // The first argument is a vector of lambda functions ...
    // whose only parameter is a nlohman::json variable

    // NOTICE: there is only ONE function passed to add_node ...
    // this node will run as sequential
    exe.add_node(
        {
            [](nlohmann::json state) {
                std::cout << "Hi from node!" << std::endl;
            },
        }
    );


    // NOTICE: there is only MORE THEN ONE function passed to add_node ...
    // this node will run as sequential
    exe.add_node(
        {
            [](nlohmann::json state) { std::cout << "We run in parallel!" << std::endl; },
            [](nlohmann::json state) { std::cout << "Wait" << std::endl; },
            [](nlohmann::json state) { std::cout << "We do?" << std::endl; },

        }
    );


    // You can execute your created nodes by calling executor::start
    exe.start();

    return 0;
}
```

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE.md) for details.

---

### Powered by

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/2d/JetBrains_company_logo.svg/1280px-JetBrains_company_logo.svg.png" width="250">
