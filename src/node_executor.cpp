//
// Created by mike on 7/3/26.
//
#include "../include/a4c/node_executor.h"
namespace a4c {
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
        std::cerr << e.what() << std::endl;
        return false;
      }
}

bool node_executor::run() {
  try
    {
      for (node::definition &node: nodes_) {
          switch (node.mode) {
            case node::CONCURRENT: {
              std::vector<std::future<nlohmann::json> > logic_future_vector;

              // Iterates through every node_logic item, calling them through async and adding their future to logic_future_vector
              for (const node::logic& logic: node.logic_vector) {
                  logic_future_vector.push_back(std::async(std::launch::async, logic.function, state_));
              }
                  // Iterates through every node_logic item, calling them through async and adding their future to logic_future_vector
                  for (int i = 0; i < node.logic_vector.size(); i++) {
                      logic_future_vector.push_back(std::async(std::launch::async, node.logic_vector.at (i).function, state_));
                  }

              // Checks every future from logic_future_vector, updating the member state_ with the result of each node
              for (int i = 0; i< node.logic_vector.size(); ++i) {

                  if (logic_future_vector.at(i).valid())
                    {
                      node.logic_vector.at(i).status = node::SUCCESS;
                      nlohmann::json state_modified = logic_future_vector.at(i).get();
                      state_.merge_patch(state_modified);
                    } else
                      {
                        node.logic_vector.at(i).status = node::FAILURE;
                      }
              }

                  }
              break;
            }
            case node::SEQUENTIAL: {
              node::logic &logic = node.logic_vector.at(0);

              state_.merge_patch(logic.function(state_));
                  node::logic_data &logic_data = node.logic_vector.at(0);

                  state_.merge_patch(logic_data.function(state_));

                  logic_data.status = node::SUCCESS;
            }
          }
      }

      return true;
    }
  catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
      return false;
    }
}

nlohmann::json node_executor::get_state() const {
  return state_;
}
}