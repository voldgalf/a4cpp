//
// Created by mike on 7/5/26.
//

#pragma once

#include <functional>
#include <vector>

namespace a4c {
namespace node {

using function_alias = std::function<nlohmann::json(nlohmann::json state)>;

enum execution_mode {
  SEQUENTIAL,
  CONCURRENT,
};

enum logic_status
{
  IDLE,
  SUCCESS,
  FAILURE,
};

struct logic
{
  logic_status status;
  function_alias function;
};

struct definition {
  execution_mode mode;
  std::vector<logic> logic_vector;
};
}
}