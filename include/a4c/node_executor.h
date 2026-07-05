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

  template<typename TInput, typename TOutput>
  bool map_async(std::function<TOutput(TInput)> base_node, std::vector<TInput> input_vector) {
    try
      {
        
        std::vector<std::future<TOutput> > logic_futures;


        for (int i = 0; i < input_vector.size(); i++) {
            SPDLOG_INFO("CONCURRENT\tCalling logic [{}/{}]", i, input_vector.size());
            logic_futures.push_back(std::async(std::launch::async, base_node, input_vector.at(i)));
        }

        for (int i = 0; i < logic_futures.size(); i++) {
            SPDLOG_INFO("CONCURRENT\tRetrieving future [{}/{}]", i, logic_futures.size());

            nlohmann::json retrieved_state = logic_futures.at(i).get();

            state_.merge_patch(retrieved_state);
        }

        return true;
      }
    catch (std::exception &e)
      {
        SPDLOG_ERROR(e.what());
        return false;
      }
  }

  bool add_node(std::vector<node::logic_function> node_vector,node::definition_mode mode_flag);

  bool run();

  nlohmann::json get_state() const;
};
}