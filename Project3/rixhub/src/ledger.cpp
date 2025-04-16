#include "ledger.hpp"

namespace rix {
namespace core {

Ledger::Ledger() {}

bool Ledger::add_topic(const std::string &topic, const rix::msg::component::MessageInfo &info) {
    // TODO: Implement the add_topic method. This method should:
    //       1. Check if the topic already exists in the ledger. If it does,
    //          return false.
    //       2. Add the topic to the ledger and return true. Ensure that the
    //          topic is initialized with the provided message info.

    return false;
}

bool Ledger::topic_exists(const std::string &topic) { 
    // TODO: Implement the topic_exists method. This method should:
    //       1. Return true if the topic exists in the ledger.

    return false;
}

bool Ledger::remove_topic(const std::string &topic) {
    // TODO: Implement the remove_topic method. This method should:
    //       1. If the topic does not exist, return false.
    //       2. Otherwise, remove the topic from the ledger.

    return false;
}

bool Ledger::get_topic_info(const std::string &topic, rix::msg::component::MessageInfo &info) {
    // TODO: Implement the get_topic_info method. This method should:
    //       1. If the topic does not exist, return false.
    //       2. Otherwise, copy the message info of the topic to the provided 
    //          reference and return true.

    return false;
}

std::vector<std::pair<ComponentInfo, ID>> Ledger::get_components(std::string topic, Type type, uint8_t protocol) {
    // TODO: Implement the get_components method. This method should: 
    //       1. If the topic does not exist, return an empty vector.
    //       2. Otherwise, iterate through the publishers or subscribers of the
    //          topic according to the component type. If the component protocol
    //          matches the provided protocol, add the component to the result
    //          vector.
    //       3. Return the result vector.

    return {};
}

bool Ledger::add_component(Type type, const ComponentInfo &component, const ID &id) {
    // TODO: Implement the add_component method. This method should:
    //       1. If the component already exists in the ledger, return false.
    //       2. Add the component to the ledger.
    //          a. If the component is a publisher, add the publisher to the 
    //             topic.
    //          b. If the component is a subscriber, add the subscriber to the 
    //             topic.

    return false;
}

bool Ledger::component_exists(uint64_t id) {
    // TODO: Implement the component_exists method. This method should:
    //       1. return true if the component exists in the ledger.
    
    return false;
}

bool Ledger::remove_component(Type type, uint64_t id) {
    // TODO: Implement the remove_component method. This method should:
    //       1. If the component does not exist, return false.
    //       2. Remove the component from the topic.
    //       3. Remove the component from components.
    //       4. If the topic is empty, remove the topic from the ledger and
    //          return true.

    return false;
}

bool Ledger::get_component(uint64_t id, ComponentInfo &component_info, ID &component_id) {
    // TODO: Implement the get_component method. This method should:
    //       1. If the component does not exist, return false.
    //       2. Copy the component info and ID to the provided references and
    //          return true.

    return false;
}

}
}