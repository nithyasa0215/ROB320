#pragma once

#include <atomic>
#include <memory>
#include <type_traits>

#include "rix/core/service_client/srv_client_impl_base.hpp"
#include "rix/core/common.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/ipc/client.hpp"

namespace rix {
namespace core {

class SrvClientImplTCP : public SrvClientImplBase {
   public:
    SrvClientImplTCP(size_t request_msg_size, size_t response_msg_size, uint64_t component_id,
                     rix::msg::component::ID service_id);
    ~SrvClientImplTCP();

    virtual bool handle_message(const uint8_t *request, uint8_t *response) override;
    virtual uint8_t get_protocol() override;
};

}  // namespace core
}  // namespace rix