//
// Created by mike on 7/6/26.
//

#pragma once
#include <vector>
#include <future>
#include <any>
#include <shared_mutex>
#include <spdlog/spdlog.h>

class state {
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, std::any> data_;

public:
    template<typename T>
    void set(const std::string &key, T value) {
        std::unique_lock lock(mutex_);

        data_[key] = std::move(value);
    }

    template<typename T>
    T &get(const std::string &key) {
        std::unique_lock lock(mutex_);

        return std::any_cast<T &>(data_.at(key));
    }

    bool contains(const std::string &key) const {
        std::unique_lock lock(mutex_);

        return data_.contains(key);
    }
};

// Logic

enum function_status {
    idle,
    success,
    failure
};

using function_logic = std::function<void(std::shared_ptr<state>)>;

// Nodes

struct function {
    function_logic logic;
    function_status status = idle;
};

enum node_mode {
    sequential,
    concurrent
};

struct node {
    node_mode mode;
    std::vector<function> function_vector;
};

// Executor

class executor {
protected:
    std::vector<node> nodes_;
    std::shared_ptr<state> state_;

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
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

    bool run_node_sequential(node &n) const {
        try {
            SPDLOG_INFO("Start sequential node");

            function &func = n.function_vector.at(0);
            func.logic(state_);
            return true;
        } catch (std::exception &e) {
            SPDLOG_INFO("End sequential node");
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

public:
    executor() {
        state_ = std::make_shared<state>();
        SPDLOG_INFO("Successfully initialized executor");
    }

    bool add_node(const std::vector<function_logic> &logic_vector) {
        try {
            node_mode selected_mode = sequential;
            if (logic_vector.size() > 1) selected_mode = concurrent;

            std::string selected_mode_str = selected_mode == sequential ? "sequential" : "concurrent";

            std::vector<function> function_vector;

            for (const auto &func_logic: logic_vector) {
                function_vector.push_back({.logic = func_logic,});
            }


            SPDLOG_INFO("Created {} node", selected_mode_str);
            nodes_.push_back({.mode = selected_mode, .function_vector = function_vector});
            return true;
        } catch (std::exception &e) {
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

    bool start() {
        SPDLOG_INFO("Start execution sequence");
        try {
            for (auto &n: nodes_) {
                switch (n.mode) {
                    case sequential: {
                        run_node_sequential(n);
                        break;
                    }
                    case concurrent: {
                        run_node_concurrent(n);
                        break;
                    }
                }
            }

            SPDLOG_INFO("End execution sequence");

            return true;
        } catch (std::exception &e) {
            SPDLOG_ERROR(e.what());
            return false;
        }
    }
};
