//
// Created by brick on 6/25/2026.
//
#pragma once
#include <nlohmann/json.hpp>


class Logic_Node {
public:
    virtual ~Logic_Node() = default;

    virtual void run(nlohmann::json &state) = 0;
};