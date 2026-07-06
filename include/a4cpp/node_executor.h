#pragma once
#include <functional>
#include <vector>
#include <future>

#include "./node.h"

#include <iostream>

namespace a4cpp {


/**
 * @class node_executor
 * @brief Handles node execution - ADD MORE
 */
class node_executor {
protected:
  std::vector<node::definition> nodes_;
  nlohmann::json state_;

public:
  node_executor()
  {
    spdlog::set_pattern("[%H:%M:%S:%e] [%^%l%$] [%s:%# %!] %v");
    SPDLOG_INFO("node_executor created successfully.");
  }

  /**
   *
   *
   * @brief Creates a new ```node::definition``` based on the ```node_vector``` and ```mode_flag``` parameters
   * @param node_vector
   * @param mode_flag
   * @return true
   * @return false
   */
  bool add_node(std::vector<node::logic_function> node_vector,node::definition_mode mode_flag);

  /**
   *
   * @brief
   * iterates through the ```node_``` member, calling sequential and concurrent nodes
   * @return bool
   */
  bool run();

  nlohmann::json get_state() const;
};
}