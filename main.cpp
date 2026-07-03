#include <iostream>
#include <memory>
#include <WorkFlowManager.h>
#include <Logic_Node.h>
#include <chrono>
#include <thread>

class Node_HelloWorld : public Logic_Node {
protected:
    std::string variable_template;
    std::string output_variable;

public:
    Node_HelloWorld() = default;

    explicit Node_HelloWorld(const nlohmann::json &properties);

    void run(nlohmann::json &state) override {
        std::cout << "Hello World!\n";
    };
};

class Node_ParallelHelloWorld : public Logic_Node {
protected:
    std::string variable_template;
    std::string output_variable;

public:
    Node_ParallelHelloWorld() = default;

    explicit Node_ParallelHelloWorld(const nlohmann::json &properties);

    void run(nlohmann::json &state) override {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Hello World! - Parallel\n";


    };
};

/*
void AINode::run(nlohmann::json &state) {
    try {
        if (model.empty() || prompt.empty()) {
            throw std::runtime_error("Missing model name or prompt");
        }

        nlohmann::json llm_request_body;
        llm_request_body["model"] = model;
        llm_request_body["input"] = inja::render(prompt, state);

        http::Request llm_request{"http://10.0.0.212:1234/v1/responses"};
        http::Response llm_response = llm_request.send("POST", llm_request_body.dump(),
                                                       {{"Content-Type", "application/json"}},
                                                       std::chrono::milliseconds(7000));

        nlohmann::json llm_response_body = nlohmann::json::parse(
            std::string{llm_response.body.begin(), llm_response.body.end()});
        std::string llm_response_output = llm_response_body.value("/output/0/content/0/text"_json_pointer,
                                                                  "");
        if (llm_response_output.empty()) {
            throw std::runtime_error("Invalid LLM Response");
        }

        state["ai_node"]["response"] = llm_response_output;
    } catch (std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}
*/

int main() {
    auto manager = WorkflowManager();

    std::vector<std::unique_ptr<Logic_Node>> sync_nodes;

    for (int i = 0; i < 4; i++) {
        sync_nodes.emplace_back(std::make_unique<Node_HelloWorld>());
    }
    manager.add_logic_nodes_sync(std::move(sync_nodes));

    std::vector<std::unique_ptr<Logic_Node>> async_nodes;

    for (int i = 0; i < 4; i++) {
        async_nodes.emplace_back(std::make_unique<Node_ParallelHelloWorld>());
    }

    manager.add_logic_nodes_async(std::move(async_nodes));

    manager.run();
}
