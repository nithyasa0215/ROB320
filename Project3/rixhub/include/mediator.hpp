#pragma once

#include <signal.h>

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "rix/util/signal_handler.hpp"
#include "rix/util/logger.hpp"
#include "rix/util/timing.hpp"
#include "rix/core/common.hpp"
#include "rix/msg/component/Info.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/common.hpp"
#include "rix/ipc/server.hpp"
#include "ledger.hpp"

using SignalHandler = rix::util::SignalHandler;
using ID = rix::msg::component::ID;
using ComponentInfo = rix::msg::component::ComponentInfo;
using Info = rix::msg::component::Info;

namespace rix {
namespace core {

/**
 * @class Mediator
 * @brief Manages communication between publishers and subscribers.
 */
class Mediator {
   public:
    /**
     * @brief Destroys the Mediator object.
     */
    ~Mediator();

    /**
     * @brief Gets the singleton instance of the Mediator.
     * @return The singleton instance of the Mediator.
     */
    static Mediator &get_instance();

    /**
     * @brief Initializes the Mediator.
     */
    void init();

    /**
     * @brief Shuts down the Mediator.
     */
    void shutdown();

    /**
     * @brief Checks if the Mediator is running.
     * @return True if the Mediator is running, false otherwise.
     */
    bool ok();

    /**
     * @brief Starts the Mediator's main loop.
     */
    void spin(bool block = true);

   private:
    /**
     * @brief Constructs a new Mediator object.
     */
    Mediator();

    /**
     * @brief Handles SIGINT signal.
     * @param signal The signal number.
     */
    static void sigint_handler(int signal);

    /**
     * @brief Runs the Mediator's main loop.
     */
    void run();

    /**
     * @brief Handles incoming messages.
     * @param in_msg The incoming message.
     */
    void handle_msg(const Info &in_msg);

    /**
     * @brief Registers a publisher.
     * @param component_info The component information of the publisher.
     * @param id The ID of the publisher.
     */
    void register_publisher(const ComponentInfo &component_info, const ID &id);

    /**
     * @brief Registers a subscriber.
     * @param component_info The component information of the subscriber.
     * @param id The ID of the subscriber.
     */
    void register_subscriber(const ComponentInfo &component_info, const ID &id);

    /**
     * @brief Deregisters a publisher.
     * @param component_info The component information of the publisher.
     */
    void deregister_publisher(const ComponentInfo &component_info);

    /**
     * @brief Deregisters a subscriber.
     * @param component_info The component information of the subscriber.
     */
    void deregister_subscriber(const ComponentInfo &component_info);

    /**
     * @brief Sends a message to a node.
     * @param node_id The ID of the node.
     * @param msg The message to send.
     */
    void send(uint64_t node_id, const Info &msg);

    Ledger ledger;  ///< The ledger managing topics and components.

    ipc::URI uri;  ///< The URI of the IPC server.
    ipc::Server server;  ///< The IPC server.

    std::vector<std::shared_ptr<ipc::Connection>> conns;  ///< The list of connections.
    std::unordered_map<uint64_t, std::shared_ptr<ipc::Connection>> nodes;  ///< The map of node IDs to connections.

    std::thread spin_thread;  ///< The thread for spinning.
    std::thread::id spin_thread_id;  ///< The ID of the spin thread.

    bool initialized;  ///< Indicates if the Mediator is initialized.
    bool spinning;  ///< Indicates if the Mediator is spinning.
    bool shutdown_flag;  ///< Indicates if the Mediator is shutting down.
};

}  // namespace core
}  // namespace rix