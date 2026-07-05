//
// Created by mike on 7/3/26.
//

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
  node_executor() = default;


  template<typename TInput, typename TOutput>
  bool map_async(std::function<TOutput(TInput)> base_node, std::vector<TInput> input_vector) {
    try
      {
        std::vector<std::future<TOutput> > logic_futures;


        for (TInput input_item: input_vector) {
            nlohmann::json state_copy = state_;

            logic_futures.push_back(std::async(std::launch::async, base_node, input_item));
        }

        for (std::future<TOutput> &future: logic_futures) {
            state_.merge_patch(future.get());
        }

        return true;
      }
    catch (std::exception &e)
      {
        std::cerr << e.what() << std::endl;
        return false;
      }
  }


  bool add_node(std::vector<node::function_alias> node_vector,node::execution_mode mode_flag);

  bool run();

  nlohmann::json get_state() const;
};
}