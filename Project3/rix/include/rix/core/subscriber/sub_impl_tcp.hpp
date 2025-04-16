#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <type_traits>

#include "rix/core/common.hpp"
#include "rix/core/subscriber/sub_impl_base.hpp"
#include "rix/core/subscriber/subscriber_callback_base.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/ipc/client.hpp"

namespace rix {
namespace core {

/**
 * @class SubImplTCP
 * @brief TCP implementation of the SubImplBase class.
 */
class SubImplTCP : public SubImplBase {
    std::map<uint64_t, std::shared_ptr<rix::ipc::Client>> clients; ///< The TCP clients ordered by component ID.

   public:
    /**
     * @brief Constructs a new SubImplTCP object.
     * @param msg_size The size of the messages.
     * @param component_id The ID of the component.
     * @param callback The callback for handling messages.
     */
    SubImplTCP(size_t msg_size, uint64_t component_id, SubCallbackPtr callback);

    /**
     * @brief Destroys the SubImplTCP object.
     */
    ~SubImplTCP();

    /**
     * @brief Connects clients to the subscriber.
     * @param pubs The container of publisher IDs.
     */
    virtual void connect_clients(std::queue<rix::msg::component::ID> &pubs) override;

    /**
     * @brief Removes clients from the subscriber.
     * @param pubs The container of publisher IDs.
     */
    virtual void remove_clients(std::queue<rix::msg::component::ID> &pubs) override;

    /**
     * @brief Handles incoming messages.
     */
    virtual void handle_message() override;

    /**
     * @brief Gets the protocol used by the subscriber.
     * @return The protocol used by the subscriber.
     */
    virtual uint8_t get_protocol() override;

    /**
     * @brief Gets the number of publishers.
     * @return The number of publishers.
     */
    virtual size_t get_num_publishers() override;
};

}  // namespace core
}  // namespace rix