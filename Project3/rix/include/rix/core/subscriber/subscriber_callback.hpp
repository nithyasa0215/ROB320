#pragma once

#include <functional>
#include <memory>

#include "rix/core/common.hpp"
#include "rix/msg/common.hpp"
#include "rix/core/subscriber/subscriber_callback_base.hpp"

namespace rix {
namespace core {

/**
 * @class SubscriberCallback
 * @brief Template class for subscriber callbacks.
 * @tparam TMsg The type of the message.
 */
template <typename TMsg>
class SubscriberCallback : public SubscriberCallbackBase {
    ASSERT_RIXMSG_TYPE(TMsg);

   public:
    /**
     * @brief Constructs a new SubscriberCallback object.
     * @param cb The callback function to invoke when a message is received.
     */
    SubscriberCallback(TSubCallback<TMsg> cb) : _cb(cb) {}

    /**
     * @brief Calls the subscriber callback with the given message.
     * @param msg The message to pass to the callback.
     * @param len The length of the message.
     */
    void call(const uint8_t* msg, size_t len) override {
        // TODO: Implement the call method. This method should:
        //       1. Check if the length of the message buffer is equal to the
        //          size of TMsg. If not, return.
        //       2. Decode the input message. If successful, invoke the callback
        //          with the decoded message.
    }

   private:
    TSubCallback<TMsg> _cb;  ///< The callback function to invoke when a message is received.
};

}  // namespace core
}  // namespace rix