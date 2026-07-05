#pragma once

#include <functional>
#include <vector>


namespace a4c::node {

using logic_function = std::function<nlohmann::json(nlohmann::json state)>;

enum logic_status
{
  IDLE,
  SUCCESS,
  FAILURE,
};

struct logic_data
{
  logic_status status = IDLE;
  logic_function function;
};

using definition_vector = std::vector<logic_data>;

enum definition_mode {
  SEQUENTIAL,
  CONCURRENT,
};

struct definition {
  definition_mode mode;
  definition_vector logic_vector;
};
}
