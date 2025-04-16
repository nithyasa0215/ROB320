#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <queue>
#include <type_traits>

#include "rix/core/publisher/pub_impl_base.hpp"
#include "rix/core/common.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/ipc/server.hpp"

namespace rix {
namespace core {

/**
 * @class PubImplTCP
 * @brief TCP implementation of the PubImplBase class.
 */
class PubImplTCP : public PubImplBase {
    std::shared_ptr<ipc::Server> server; ///< The TCP server.
    std::map<uint64_t, std::shared_ptr<rix::ipc::Connection>> conns; ///< The TCP connections.

   public:
    /**
     * @brief Constructs a new PubImplTCP object.
     * @param msg_size The size of the messages.
     * @param component_id The ID of the component.
     */
    PubImplTCP(size_t msg_size, uint64_t component_id);

    /**
     * @brief Destroys the PubImplTCP object.
     */
    ~PubImplTCP();

    /**
     * @brief Gets the URI of the publisher.
     * @return The URI of the publisher.
     */
    virtual rix::msg::component::URI get_uri() const override;

    /**
     * @brief Transports a message.
     * @param msg The message to transport.
     * @param len The length of the message.
     */
    virtual void transport_message(const uint8_t *msg, size_t len) override;

    /**
     * @brief Accepts connections from subscribers.
     * @param subs The container of subscriber IDs.
     */
    virtual void accept_connections(std::map<uint64_t, rix::msg::component::ID> &subs) override;

    /**
     * @brief Removes connections from subscribers.
     * @param subs The container of subscriber IDs.
     */
    virtual void remove_connections(std::queue<rix::msg::component::ID> &subs) override;

    /**
     * @brief Gets the protocol used by the publisher.
     * @return The protocol used by the publisher.
     */
    virtual uint8_t get_protocol() override;

    /**
     * @brief Gets the number of subscribers.
     * @return The number of subscribers.
     */
    virtual size_t get_num_subscribers() override;
};

}  // namespace core
}  // namespace rix