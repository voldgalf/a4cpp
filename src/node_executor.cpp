//
// Created by mike on 7/3/26.
//
#include "../include/a4cpp/node_executor.h"

namespace a4cpp {
bool node_executor::add_node(std::vector<node::logic_function> node_vector,const node::definition_mode mode_flag)
{
  try
    {
      std::vector<node::logic_data> logic_vector;
      for (int i = 0; i < node_vector.size(); ++i)
        {
          logic_vector.push_back({.function = node_vector.at(i)});
        }

      nodes_.push_back({.mode = mode_flag, .logic_vector = logic_vector});

      return true;
    } catch (std::exception &e)
      {
        SPDLOG_ERROR(e.what());
        return false;
      }
}

bool node_executor::run() {
  try
    {
      for (node::definition &node: nodes_) {
          switch (node.mode) {
            case node::CONCURRENT: {
              try
                {

                  SPDLOG_INFO("Concurrent node start");

                  std::vector<std::future<nlohmann::json> > logic_future_vector;

                  // Iterates through every node_logic item, calling them through async and adding their future to logic_future_vector
                  for (int i = 0; i < node.logic_vector.size(); i++) {
                      SPDLOG_DEBUG("Calling logic [{}/{}]", i, node.logic_vector.size());
                      logic_future_vector.push_back(std::async(std::launch::async, node.logic_vector.at (i).function, state_));
                  }

                  // Checks every future from logic_future_vector, updating the member state_ with the result of each node
                  for (int i = 0; i< node.logic_vector.size(); ++i) {
                      SPDLOG_DEBUG("Retrieving future [{}/{}]", i, node.logic_vector.size());
                      node.logic_vector.at(i).status = node::SUCCESS;
                      nlohmann::json state_modified = logic_future_vector.at(i).get();
                      state_.merge_patch(state_modified);
                  }

                  SPDLOG_INFO("Concurrent node complete");

                } catch (std::exception &e)
                  {
                    SPDLOG_ERROR(e.what());
                  }
              break;
            }
            case node::SEQUENTIAL: {
              try
                {

                  SPDLOG_INFO("Sequential node start");

                  SPDLOG_DEBUG("Calling logic");
                  node::logic_data &logic_data = node.logic_vector.at(0);

                  state_.merge_patch(logic_data.function(state_));

                  SPDLOG_INFO("Sequential node complete");

                  logic_data.status = node::SUCCESS;
                } catch (std::exception &e)
                  {
                    SPDLOG_ERROR(e.what());
                  }
            }
          }
      }

      return true;
    }
  catch (std::exception &e)
    {
      SPDLOG_ERROR(e.what());
      return false;
    }
}

nlohmann::json node_executor::get_state() const {
  return state_;
}
}