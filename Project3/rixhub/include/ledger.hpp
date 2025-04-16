#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "rix/core/common.hpp"
#include "rix/msg/component/Info.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/msg/common.hpp"

using ID = rix::msg::component::ID;
using ComponentInfo = rix::msg::component::ComponentInfo;
using URI = rix::msg::component::URI;

namespace rix {
namespace core {

/**
 * @class Ledger
 * @brief Manages topics and components for publishers and subscribers.
 */
class Ledger {
  public:
    /**
     * @enum Type
     * @brief Specifies the type of component.
     */
    enum Type {
        PUBLISHER,  ///< Publisher type
        SUBSCRIBER  ///< Subscriber type
    };

    /**
     * @brief Constructs a new Ledger object.
     */
    Ledger();

    /**
     * @brief Adds a topic to the ledger.
     * @param topic The name of the topic.
     * @param info The message information associated with the topic.
     * @return True if the topic was added successfully, false otherwise.
     */
    bool add_topic(const std::string &topic, const rix::msg::component::MessageInfo &info);

    /**
     * @brief Checks if a topic exists in the ledger.
     * @param topic The name of the topic.
     * @return True if the topic exists, false otherwise.
     */
    bool topic_exists(const std::string &topic);

    /**
     * @brief Removes a topic from the ledger.
     * @param topic The name of the topic.
     * @return True if the topic was removed successfully, false otherwise.
     */
    bool remove_topic(const std::string &topic);

    /**
     * @brief Retrieves information about a topic.
     * @param topic The name of the topic.
     * @param info The message information associated with the topic.
     * @return True if the topic information was retrieved successfully, false otherwise.
     */
    bool get_topic_info(const std::string &topic, rix::msg::component::MessageInfo &info);

    /**
     * @brief Retrieves components associated with a topic.
     * @param topic The name of the topic.
     * @param type The type of components to retrieve (PUBLISHER or SUBSCRIBER).
     * @param protocol The protocol used by the components.
     * @return A vector of pairs containing component information and IDs.
     */
    std::vector<std::pair<ComponentInfo, ID>> get_components(std::string topic, Type type, uint8_t protocol);

    /**
     * @brief Adds a component to the ledger.
     * @param type The type of the component (PUBLISHER or SUBSCRIBER).
     * @param component The component information.
     * @param id The ID of the component.
     * @return True if the component was added successfully, false otherwise.
     */
    bool add_component(Type type, const ComponentInfo &component, const ID &id);

    /**
     * @brief Removes a component from the ledger.
     * @param type The type of the component (PUBLISHER or SUBSCRIBER).
     * @param id The ID of the component.
     * @return True if the component was removed successfully, false otherwise.
     */
    bool remove_component(Type type, uint64_t id);

    /**
     * @brief Checks if a component exists in the ledger.
     * @param id The ID of the component.
     * @return True if the component exists, false otherwise.
     */
    bool component_exists(uint64_t id);

    /**
     * @brief Retrieves information about a component.
     * @param id The ID of the component.
     * @param component_info The component information.
     * @param component_id The component ID.
     * @return True if the component information was retrieved successfully, false otherwise.
     */
    bool get_component(uint64_t id, ComponentInfo &component_info, ID &component_id);

  private:
    /**
     * @struct Topic
     * @brief Represents a topic with its associated information and components.
     */
    struct Topic {
        rix::msg::component::MessageInfo info;  ///< Message information associated with the topic.
        std::set<uint64_t> pubs;  ///< Set of publisher component IDs.
        std::set<uint64_t> subs;  ///< Set of subscriber component IDs.
    };

    std::map<uint64_t, std::pair<ComponentInfo, ID>> components;  ///< Map of component IDs to component information and IDs.
    std::map<std::string, Topic> topics;  ///< Map of topic names to topic information.
};

}
}