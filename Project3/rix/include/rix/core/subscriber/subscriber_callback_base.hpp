#pragma once

#include <functional>
#include <memory>

namespace rix {
namespace core {

/**
 * @brief Type alias for a subscriber callback function.
 * @tparam TMsg The type of the message.
 */
template <typename TMsg>
using TSubCallback = std::function<void(const TMsg&)>;

/**
 * @class SubscriberCallbackBase
 * @brief Base class for subscriber callbacks.
 */
class SubscriberCallbackBase {
   public:
    /**
     * @brief Virtual destructor for the SubscriberCallbackBase class.
     */
    virtual ~SubscriberCallbackBase() = default;

    /**
     * @brief Calls the subscriber callback with the given message.
     * @param msg The message to pass to the callback.
     * @param len The length of the message.
     */
    virtual void call(const uint8_t* msg, size_t len) = 0;
};

/**
 * @brief Type alias for a shared pointer to a SubscriberCallbackBase.
 */
using SubCallbackPtr = std::shared_ptr<SubscriberCallbackBase>;

}  // namespace core
}  // namespace rix