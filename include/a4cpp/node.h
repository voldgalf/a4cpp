#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "../nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include <functional>
#include <vector>


namespace a4cpp::node {
    /**
     * @var logic_function
     * @brief a type alias defining a ```std::function``` with the return type ```nlohmann::json``` and single ```nlohmann::json``` parameter ```state```
     */
    using logic_function = std::function<nlohmann::json(nlohmann::json state)>;

    /**
     * @enum logic_status
     * @brief The status of logic_data
     * @property IDLE - not executed yet
     * @property SUCCESS - successful execution
     * @property FAILURE - unsuccessful execution
     *
     */
    enum logic_status {
        IDLE,
        SUCCESS,
        FAILURE,
    };

    /**
     * @struct logic_data
     * @brief Contains logic's ```status``` and ```function```
     * @property status - the status of the ```function``` property in relation to execution
     * @property function - an instance of type alias ``logic_function```
     */
    struct logic_data {
        logic_status status = IDLE;
        logic_function function;
    };

    /**
     * @var definition_vector
     * @brief a type alias defining a std::vector of ```logic_data```
     */
    using definition_vector = std::vector<logic_data>;

    /**
     * @enum definition_mode
     * @brief Determines how ```node_executor::run``` will handle the relative ```definition```
     * @property SEQUENTIAL - will run in sequence (1,2,etc)
     * @property CONCURRENT - will run in concurrency (all at once)
     */
    enum definition_mode {
        SEQUENTIAL,
        CONCURRENT,
    };

    /**
     * @struct definition
     * @brief Contains the node's ```mode``` and ```logic_vector``` \n IMPORTANT if the ```mode``` is sequential: only the first vector element passed will be called
     * @property mode - either sequential or concurrent
     * @property logic_vector - a vector of ``logic_data```
     */
    struct definition {
        definition_mode mode;
        definition_vector logic_vector;
    };
}
