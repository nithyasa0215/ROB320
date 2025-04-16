#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "rix/core/common.hpp"
#include "rix/core/publisher.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/common.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/Info.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/ipc/client.hpp"
#include "rix/util/logger.hpp"
#include "rix/util/signal_handler.hpp"
#include "rix/util/timing.hpp"

namespace rix {
namespace core {

/**
 * @class Node
 * @brief The Node class represents a node in the RIX system.
 */
class Node {
   public:
    /**
     * @brief Get the singleton instance of the Node.
     * @return Reference to the Node instance.
     */
    static Node &get_instance();

    /**
     * @brief Deleted copy constructor.
     */
    Node(const Node &) = delete;

    /**
     * @brief Deleted assignment operator.
     */
    Node &operator=(const Node &) = delete;

    /**
     * @brief Destructor. Calls the shutdown method.
     */
    ~Node();

    /**
     * @brief Initialize the Node with the given name, hub IP, and hub port.
     * @param name The name of the node.
     * @param hub_ip The IP address of the hub.
     * @param hub_port The port number of the hub.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(const std::string &name, const std::string &hub_ip, uint16_t hub_port);

    /**
     * @brief Start the node's spinning process.
     * @param block If true, block the calling thread until spinning stops.
     */
    void spin(bool block = true);

    /**
     * @brief Execute one iteration of the node's spinning process.
     */
    void spin_once();

    /**
     * @brief Shutdown the node.
     */
    void shutdown();

    /**
     * @brief Check if the node is running.
     * @return True if the node is running, false otherwise.
     */
    bool ok();

    /**
     * @brief Advertise a topic for publishing messages.
     * @tparam TMsg The message type.
     * @tparam TImpl The implementation type (default is PubImplTCP).
     * @param topic The topic name.
     * @return Shared pointer to the Publisher.
     */
    template <typename TMsg, typename TImpl = PubImplTCP>
    std::shared_ptr<Publisher> advertise(const std::string &topic) {
        ASSERT_RIXMSG_TYPE(TMsg);
        static_assert(std::is_base_of<PubImplBase, TImpl>::value, "TImpl must derive from PubImplBase");

        if (!initialized) {
            Logger::log<LogLevel::ERROR>("Node: not initialized");
            return nullptr;
        }

        // TODO: Implement the advertise method. This method should:
        //       1. Create a shared pointer to a TImpl object with the size of
        //          TMsg and a generated component ID and store it as a PubImplBase
        //          shared pointer. This upcasting is done so that the Publisher
        //          class is agnostic to the implementation type.
        //       2. Create a shared pointer to a Publisher object with the topic,
        //          node ID, message info, and the PubImplBase shared pointer.
        //       3. Register the publisher with the Hub. If registration fails,
        //          return a nullptr.
        //       4. Start the publisher thread.
        //       5. Insert the publisher shared pointer into the publishers 
        //          container.
        //       6. Return the publisher shared pointer.

        return nullptr;
    }

    /**
     * @brief Subscribe to a topic for receiving messages.
     * @tparam TMsg The message type.
     * @tparam TImpl The implementation type (default is SubImplTCP).
     * @param topic The topic name.
     * @param callback The callback function to handle received messages.
     * @return Shared pointer to the Subscriber.
     */
    template <typename TMsg, typename TImpl = SubImplTCP>
    std::shared_ptr<Subscriber> subscribe(const std::string &topic, TSubCallback<TMsg> callback) {
        ASSERT_RIXMSG_TYPE(TMsg);
        static_assert(std::is_base_of<SubImplBase, TImpl>::value, "TImpl must derive from SubImplBase");

        if (!initialized) {
            Logger::log<LogLevel::ERROR>("Node: not initialized");
            return nullptr;
        }

        // TODO: Implement the subscribe method. This method should:
        //       1. Create a SubscriberCallback shared pointer, passing the
        //          callback to its constructor. Store it as a 
        //          SubscriberCallbackBase shared pointer. This upcasting is 
        //          done so that the SubImplBase class is agnostic to the 
        //          message type.
        //       2. Create a TImpl shared pointer and store it as a SubImplBase
        //          shared pointer. Pass the SubscriberCallbackBase shared 
        //          pointer to the constructor.
        //       3. Create a Subscriber shared pointer, passing the implementation
        //          base shared pointer to the constructor.
        //       4. Register the subscriber with the Hub. If registration fails,
        //          return a nullptr.
        //       5. Start the subscriber thread.
        //       6. Insert the subscriber into the subscribers container.
        //       7. Return the subscriber shared pointer.

        return nullptr;
    }

   private:
    /**
     * @brief Constructor. Initializes member variables.
     */
    Node();

    /**
     * @brief Signal handler for handling system signals.
     * @param signal The signal number.
     */
    static void signal_handler(int signal);

    /**
     * @brief Run the node's spinning process.
     */
    void run();

    /**
     * @brief Execute one iteration of the node's spinning process.
     */
    void run_once();

    /**
     * @brief Register a publisher with the hub.
     * @param pub_ptr Shared pointer to the Publisher.
     * @return True if registration is successful, false otherwise.
     */
    bool register_publisher(std::shared_ptr<Publisher> pub_ptr);

    /**
     * @brief Register a subscriber with the hub.
     * @param sub_ptr Shared pointer to the Subscriber.
     * @return True if registration is successful, false otherwise.
     */
    bool register_subscriber(std::shared_ptr<Subscriber> sub_ptr);

    /**
     * @brief Deregister a subscriber from the hub.
     * @param sub_ptr Shared pointer to the Subscriber.
     * @return True if deregistration is successful, false otherwise.
     */
    bool deregister_subscriber(std::shared_ptr<Subscriber> sub_ptr);

    /**
     * @brief Deregister a publisher from the hub.
     * @param pub_ptr Shared pointer to the Publisher.
     * @return True if deregistration is successful, false otherwise.
     */
    bool deregister_publisher(std::shared_ptr<Publisher> pub_ptr);

    /**
     * @brief Handle publisher notification messages from the hub.
     * @param notify The notification message.
     */
    void handle_pub_notify(const rix::msg::component::Info *notify);

    /**
     * @brief Handle subscriber notification messages from the hub.
     * @param notify The notification message.
     */
    void handle_sub_notify(const rix::msg::component::Info *notify);

    /**
     * @brief Handle publisher disconnection messages from the hub.
     * @param disconnect The disconnection message.
     */
    void handle_pub_disconnect(const rix::msg::component::Info *disconnect);

    /**
     * @brief Handle subscriber disconnection messages from the hub.
     * @param disconnect The disconnection message.
     */
    void handle_sub_disconnect(const rix::msg::component::Info *disconnect);

    std::string name; ///< The name of the node.
    ipc::URI hub_uri; ///< The URI of the hub.
    uint64_t machine_id; ///< The machine ID.
    uint64_t node_id; ///< The node ID.

    bool initialized; ///< Flag indicating if the node is initialized.
    bool spinning; ///< Flag indicating if the node is spinning.
    bool shutdown_flag; ///< Flag indicating if the node is shutting down.

    std::thread spin_thread; ///< The thread for spinning the node.
    std::thread::id spin_thread_id; ///< The ID of the spin thread.

    std::map<std::string, std::set<std::shared_ptr<Publisher>>> publishers; ///< Container for publishers. Key is the topic. Value is a set of publishers.
    std::map<std::string, std::set<std::shared_ptr<Subscriber>>> subscribers; ///< Container for subscribers. Key is the topic. Value is a set of subscribers.

    std::mutex client_mutex; ///< Mutex for thread-safe client operations.
    std::shared_ptr<ipc::Client> client; ///< Client for communicating with the hub.
};

}  // namespace core
}  // namespace rix