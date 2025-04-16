#pragma once

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>

#include "rix/core/subscriber/sub_impl_base.hpp"
#include "rix/core/subscriber/subscriber_callback_base.hpp"
#include "rix/core/common.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/MessageInfo.hpp"
#include "rix/msg/component/URI.hpp"

namespace rix {
namespace core {

class Node;

/**
 * @class Subscriber
 * @brief Manages the subscription to messages from publishers.
 */
class Subscriber {
    friend class Node;

   public:
    /**
     * @brief Destroys the Subscriber object.
     */
    ~Subscriber();

    /**
     * @brief Gets the topic of the Subscriber.
     * @return The topic of the Subscriber.
     */
    std::string get_topic() const;

    /**
     * @brief Gets the number of publishers.
     * @return The number of publishers.
     */
    int get_num_publishers() const;

    /**
     * @brief Shuts down the Subscriber.
     */
    void shutdown();

    /**
     * @brief Checks if the Subscriber is running.
     * @return True if the Subscriber is running, false otherwise.
     */
    bool ok() const;

   private:
    rix::msg::component::ComponentInfo component_info;  ///< The component information of the Subscriber.
    std::queue<rix::msg::component::ID> connect_pub_container;  ///< Container for connecting publisher IDs.
    std::queue<rix::msg::component::ID> remove_pub_container;  ///< Container for removing publisher IDs.
    mutable std::mutex mtx;  ///< Mutex for thread safety (mutable for const methods).
    std::thread::id thread_id;  ///< ID of the thread running the Subscriber.
    std::thread thread;  ///< Thread for running the Subscriber.
    std::atomic<bool> request_shutdown_flag;
    SubImplPtr impl;  ///< Implementation of the Subscriber.

    /**
     * @brief Constructs a new Subscriber object.
     * @param topic The topic of the Subscriber.
     * @param node_id The ID of the node.
     * @param msg_info The message information.
     * @param impl The implementation of the Subscriber.
     */
    Subscriber(const std::string &topic, uint64_t node_id, const rix::msg::component::MessageInfo &msg_info, SubImplPtr impl);

    /**
     * @brief Adds a publisher.
     * @param id The ID of the publisher.
     */
    void add_publisher(const rix::msg::component::ID &id);

    /**
     * @brief Removes a publisher.
     * @param id The ID of the publisher.
     */
    void remove_publisher(const rix::msg::component::ID &id);

    /**
     * @brief Starts the Subscriber.
     */
    void start();

    /**
     * @brief Runs the Subscriber.
     */
    void run();

    /**
     * @brief Runs the Subscriber once.
     */
    void run_once();

    /**
     * @brief Gets the ID of the Subscriber.
     * @return The ID of the Subscriber.
     */
    rix::msg::component::ID get_id() const;
};

}  // namespace core
}  // namespace rix