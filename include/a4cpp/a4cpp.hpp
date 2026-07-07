//
// Created by mike on 7/6/26.
//

#pragma once
#include <vector>
#include <future>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>


// Logic

enum status {
    IDLE,
    SUCCESS,
    FAILURE
};

using logic = std::function<void(nlohmann::json)>;

// Nodes

struct function {
    std::function<void(nlohmann::json)> logic_;
    status status_ = IDLE;
};

enum mode {
    SEQUENTIAL,
    CONCURRENT
};

struct node {
    mode mode_;
    std::vector<function> function_vector_;
};

// Executor

class executor {
protected:
    std::vector<node> nodes_;
    nlohmann::json state_;

    bool run_node_concurrent(node &n) {
        SPDLOG_INFO("Start concurrent node");

        std::vector<std::future<void> > futures;

        for (int i = 0; i < n.function_vector_.size(); i++) {
            SPDLOG_DEBUG("Launching future [{}/{}]", i, n.function_vector_.size());
            function &func = n.function_vector_.at(i);
            futures.push_back(std::async(std::launch::async, func.logic_, state_));
        }

        for (int i = 0; i < futures.size(); i++) {
            SPDLOG_DEBUG("Collecting future [{}/{}]", i, n.function_vector_.size());
            futures[i].wait();
        }

        SPDLOG_INFO("End concurrent node");

        return true;
    }

    bool run_node_sequential(node &n) const {
        SPDLOG_INFO("Start sequential node");

        function &func = n.function_vector_.at(0);
        func.logic_(state_);

        SPDLOG_INFO("End sequential node");

        return true;
    }

public:
    executor() {
        SPDLOG_INFO("Successfully initialized executor");
    }

    bool add_node(const std::vector<logic> logic_vector) {
        mode selected_mode = SEQUENTIAL;
        if (logic_vector.size() > 1) selected_mode = CONCURRENT;

        std::string selected_mode_str = selected_mode == SEQUENTIAL ? "sequential" : "concurrent";

        std::vector<function> function_vector;

        for (int i = 0; i < logic_vector.size(); i++) {
            function_vector.push_back({.logic_ = logic_vector.at(i),});
        }


        SPDLOG_INFO("Created {} node", selected_mode_str);
        nodes_.push_back({.mode_ = selected_mode, .function_vector_ = function_vector});
        return true;
    }

    bool start() {
        SPDLOG_INFO("Start execution sequence");

        for (int i = 0; i < nodes_.size(); i++) {
            switch (node &n = nodes_.at(i); n.mode_) {
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
