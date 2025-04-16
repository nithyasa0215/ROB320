#pragma once

#include <atomic>
#include <map>
#include <queue>
#include <string>
#include <type_traits>

#include "rix/core/common.hpp"
#include "rix/core/publisher/pub_impl_base.hpp"
#include "rix/msg/common.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/MessageInfo.hpp"
#include "rix/msg/component/URI.hpp"

namespace rix {
namespace core {

class Node;

/**
 * @class Publisher
 * @brief Manages the publishing of messages to subscribers.
 */
class Publisher {
    friend class Node;

   public:
    /**
     * @brief Destroys the Publisher object.
     */
    ~Publisher();

    /**
     * @brief Gets the topic of the Publisher.
     * @return The topic of the Publisher.
     */
    std::string get_topic() const;

    /**
     * @brief Gets the number of subscribers.
     * @return The number of subscribers.
     */
    size_t get_num_subscribers() const;

    /**
     * @brief Shuts down the Publisher.
     */
    void shutdown();

    /**
     * @brief Checks if the Publisher is running.
     * @return True if the Publisher is running, false otherwise.
     */
    bool ok() const;

    /**
     * @brief Publishes a message.
     * @tparam TMsg The type of the message.
     * @param msg The message to publish.
     */
    template <typename TMsg>
    void publish(const TMsg &msg) {
        ASSERT_RIXMSG_TYPE(TMsg);
        if (!ok()) {
            Logger::log<LogLevel::ERROR>("Publisher: publish called on a shutdown Publisher");
            return;
        }

        // TODO: Implement the publish method. This method should:
        //       1. Check if TMsg's info matches the message info of the 
        //          publisher. Check the message definitions in include/rix/msg/
        //          for how to access the message info.
        //       2. Lock the mutex and call the transport_message method of the
        //          implementation on the encoded message.
    }

   private:
    rix::msg::component::ComponentInfo component_info;  ///< The component information of the Publisher.
    std::map<uint64_t, rix::msg::component::ID> accept_sub_container;  ///< Container for accepting subscriber IDs.
    std::queue<rix::msg::component::ID> remove_sub_container;  ///< Container for removing subscriber IDs.
    mutable std::mutex mtx;  ///< Mutex for thread safety (mutable for const methods).
    std::thread::id thread_id; ///< ID of the thread running the Publisher.
    std::thread thread;  ///< Thread for running the Publisher.
    std::atomic<bool> request_shutdown_flag;
    PubImplPtr impl;  ///< Implementation of the Publisher.

    /**
     * @brief Constructs a new Publisher object.
     * @param topic The topic of the Publisher.
     * @param node_id The ID of the node.
     * @param msg_info The message information.
     * @param impl The implementation of the Publisher.
     */
    Publisher(const std::string &topic, uint64_t node_id, const rix::msg::component::MessageInfo &msg_info, PubImplPtr impl);

    /**
     * @brief Adds a subscriber.
     * @param id The ID of the subscriber.
     */
    void add_subscriber(const rix::msg::component::ID &id);

    /**
     * @brief Removes a subscriber.
     * @param id The ID of the subscriber.
     */
    void remove_subscriber(const rix::msg::component::ID &id);

    /**
     * @brief Gets the ID of the Publisher.
     * @return The ID of the Publisher.
     */
    rix::msg::component::ID get_id() const;

    /**
     * @brief Starts the Publisher.
     */
    void start();

    /**
     * @brief Runs the Publisher.
     */
    void run();

    /**
     * @brief Runs the Publisher once.
     */
    void run_once();
};

}  // namespace core
}  // namespace rix