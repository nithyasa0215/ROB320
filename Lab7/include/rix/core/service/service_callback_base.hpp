#pragma once

#include <functional>
#include <memory>

namespace rix {
namespace core {

template <typename TRequest, typename TResponse>
using TSrvCallback = std::function<void(const TRequest&, TResponse&)>;

class ServiceCallbackBase {
   public:
    virtual void call(const uint8_t* request, size_t request_len, uint8_t* response, size_t response_len) = 0;
};

using SrvCallbackPtr = std::shared_ptr<ServiceCallbackBase>;

}  // namespace core
}  // namespace rix