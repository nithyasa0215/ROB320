#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <queue>
#include <type_traits>

#include "rix/core/common.hpp"
#include "rix/core/subscriber/subscriber_callback_base.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"

namespace rix {
namespace core {

/**
 * @class SubImplBase
 * @brief Base class for subscriber implementations.
 */
class SubImplBase {
   public:
    /**
     * @brief Constructs a new SubImplBase object.
     * @param msg_size The size of the messages.
     * @param component_id The ID of the component.
     * @param callback The callback for handling messages.
     */
    SubImplBase(size_t msg_size, uint64_t component_id, SubCallbackPtr callback);

    /**
     * @brief Destroys the SubImplBase object.
     */
    virtual ~SubImplBase();

    /**
     * @brief Shuts down the subscriber.
     */
    void shutdown();

    /**
     * @brief Checks if the subscriber is running.
     * @return True if the subscriber is running, false otherwise.
     */
    bool ok() const;

    /**
     * @brief Gets the URI of the subscriber.
     * @return The URI of the subscriber.
     */
    virtual rix::msg::component::URI get_uri() const;

    /**
     * @brief Connects clients to the subscriber.
     * @param subs The container of subscriber IDs.
     */
    virtual void connect_clients(std::queue<rix::msg::component::ID> &subs) = 0;

    /**
     * @brief Removes clients from the subscriber.
     * @param subs The container of subscriber IDs.
     */
    virtual void remove_clients(std::queue<rix::msg::component::ID> &subs) = 0;

    /**
     * @brief Handles incoming messages.
     */
    virtual void handle_message() = 0;

    /**
     * @brief Gets the protocol used by the subscriber.
     * @return The protocol used by the subscriber.
     */
    virtual uint8_t get_protocol() = 0;

    /**
     * @brief Gets the number of publishers.
     * @return The number of publishers.
     */
    virtual size_t get_num_publishers() = 0;

   protected:
    SubCallbackPtr callback;  ///< The callback for handling messages.
    std::atomic<bool> shutdown_flag;  ///< Indicates if the subscriber is shutting down.

   public:
    const uint64_t component_id;  ///< The ID of the component.
    const size_t msg_size;  ///< The size of the messages.
};

using SubImplPtr = std::shared_ptr<SubImplBase>;

}  // namespace core
}  // namespace rix