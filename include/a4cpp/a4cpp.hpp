//
// Created by mike on 7/6/26.
//

#pragma once
#include <vector>
#include <future>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>


// Logic

enum function_status {
    IDLE,
    SUCCESS,
    FAILURE
};

using function_logic = std::function<void(nlohmann::json)>;

// Nodes

struct function {
    function_logic logic;
    function_status status = IDLE;
};

enum node_mode {
    SEQUENTIAL,
    CONCURRENT
};

struct node {
    node_mode mode;
    std::vector<function> function_vector;
};

// Executor

class executor {
protected:
    std::vector<node> nodes_;
    nlohmann::json state_;

    bool run_node_concurrent(node &n) {
        try {
            SPDLOG_INFO("Start concurrent node");

            std::vector<std::future<void> > futures;

            for (int i = 0; i < n.function_vector.size(); i++) {
                SPDLOG_DEBUG("Launching future [{}/{}]", i, n.function_vector_.size());
                function &func = n.function_vector.at(i);
                futures.push_back(std::async(std::launch::async, func.logic, state_));
            }

            for (int i = 0; i < futures.size(); i++) {
                SPDLOG_DEBUG("Collecting future [{}/{}]", i, n.function_vector_.size());
                futures[i].wait();
            }
            SPDLOG_INFO("End concurrent node");
            return true;
        } catch (std::exception &e) {
            SPDLOG_INFO("End concurrent node");
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

    bool run_node_sequential(node &n) const {
        try {
            SPDLOG_INFO("Start sequential node");

            function &func = n.function_vector.at(0);
            func.logic(state_);

            SPDLOG_INFO("End sequential node");

            return true;
        } catch (std::exception &e) {
            SPDLOG_INFO("End sequential node");
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

public:
    executor() {
        SPDLOG_INFO("Successfully initialized executor");
    }

    bool add_node(const std::vector<function_logic> logic_vector) {
        node_mode selected_mode = SEQUENTIAL;
        if (logic_vector.size() > 1) selected_mode = CONCURRENT;

        std::string selected_mode_str = selected_mode == SEQUENTIAL ? "sequential" : "concurrent";

        std::vector<function> function_vector;

        for (int i = 0; i < logic_vector.size(); i++) {
            function_vector.push_back({.logic = logic_vector.at(i),});
        }


        SPDLOG_INFO("Created {} node", selected_mode_str);
        nodes_.push_back({.mode = selected_mode, .function_vector = function_vector});
        return true;
    }

    bool start() {
        SPDLOG_INFO("Start execution sequence");

        for (int i = 0; i < nodes_.size(); i++) {
            switch (node &n = nodes_.at(i); n.mode) {
                case SEQUENTIAL: {
                    run_node_sequential(n);
                    break;
                }
                case CONCURRENT: {
                    run_node_concurrent(n);
                    break;
                }
            }
        }

        SPDLOG_INFO("End execution sequence");

        return true;
    }
};
