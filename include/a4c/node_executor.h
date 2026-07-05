#pragma once

#include <functional>
#include <vector>
#include <future>

#include "../nlohmann/json.hpp"
#include "./node.h"

#include <iostream>

namespace a4c {

class node_executor {
protected:
  std::vector<node::definition> nodes_;
  nlohmann::json state_;

public:
  node_executor()
  {

    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [%s:%# %!] %v");
    SPDLOG_INFO("node_executor created successfully.");
  }


  bool add_node(std::vector<node::logic_function> node_vector,node::definition_mode mode_flag);

  bool run();

  nlohmann::json get_state() const;
};
}