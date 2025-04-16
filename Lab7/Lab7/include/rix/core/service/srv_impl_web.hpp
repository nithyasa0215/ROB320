#pragma once

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>

#include "rix/core/service/service_callback_base.hpp"
#include "rix/core/service/srv_impl_base.hpp"
#include "rix/core/common.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/MessageInfo.hpp"
#include "rix/msg/component/URI.hpp"
#include "rix/ipc/web_server.hpp"

namespace rix {
namespace core {

class SrvImplWEB : public SrvImplBase {
    std::shared_ptr<ipc::WebServer> server;

   public:
    SrvImplWEB(size_t request_size, size_t response_size, uint64_t component_id, SrvCallbackPtr callback);
    ~SrvImplWEB();
    
    virtual rix::msg::component::URI get_uri() const override;
    virtual uint8_t get_protocol() override;
    virtual void run_once() override;

};

}  // namespace core
}  // namespace rix