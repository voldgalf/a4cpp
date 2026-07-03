//
// Created by brick on 6/25/2026.
//
#pragma once
#include <nlohmann/json.hpp>


class Node {
public:
    bool is_executed;
    bool is_executed = false;

    virtual void run(nlohmann::json &state) = 0;
};
