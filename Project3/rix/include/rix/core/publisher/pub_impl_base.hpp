#pragma once

#include <atomic>
#include <map>
#include <queue>
#include <type_traits>

#include "rix/core/common.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"

namespace rix {
namespace core {

/**
 * @class PubImplBase
 * @brief Base class for publisher implementations.
 */
class PubImplBase {
   public:
    /**
     * @brief Constructs a new PubImplBase object.
     * @param msg_size The size of the messages.
     * @param component_id The ID of the component.
     */
    PubImplBase(size_t msg_size, uint64_t component_id);

    /**
     * @brief Destroys the PubImplBase object.
     */
    virtual ~PubImplBase();

    /**
     * @brief Shuts down the publisher.
     */
    void shutdown();

    /**
     * @brief Checks if the publisher is running.
     * @return True if the publisher is running, false otherwise.
     */
    bool ok() const;

    /**
     * @brief Gets the URI of the publisher.
     * @return The URI of the publisher.
     */
    virtual rix::msg::component::URI get_uri() const = 0;

    /**
     * @brief Transports a message.
     * @param msg The message to transport.
     * @param len The length of the message.
     */
    virtual void transport_message(const uint8_t *msg, size_t len) = 0;

    /**
     * @brief Accepts connections from subscribers.
     * @param subs The container of subscriber IDs.
     */
    virtual void accept_connections(std::map<uint64_t, rix::msg::component::ID> &subs) = 0;

    /**
     * @brief Removes connections from subscribers.
     * @param subs The container of subscriber IDs.
     */
    virtual void remove_connections(std::queue<rix::msg::component::ID> &subs) = 0;

    /**
     * @brief Gets the protocol used by the publisher.
     * @return The protocol used by the publisher.
     */
    virtual uint8_t get_protocol() = 0;

    /**
     * @brief Gets the number of subscribers.
     * @return The number of subscribers.
     */
    virtual size_t get_num_subscribers() = 0;

   protected:
    std::atomic<bool> shutdown_flag;  ///< Indicates if the publisher is shutting down.

   public:
    const uint64_t component_id;  ///< The ID of the component.
    const size_t msg_size;  ///< The size of the messages.
};

using PubImplPtr = std::shared_ptr<PubImplBase>;

}  // namespace core
}  // namespace rix