#pragma once

#include <functional>
#include <memory>

#include "rix/core/service/service_callback_base.hpp"

namespace rix {
namespace core {

template <typename TRequest, typename TResponse>
class ServiceCallback : public ServiceCallbackBase {
    ASSERT_RIXMSG_TYPE(TRequest);
    ASSERT_RIXMSG_TYPE(TResponse);

   public:
    void call(const uint8_t* request, size_t request_len, uint8_t* response, size_t response_len) override {
        if (request_len != sizeof(TRequest) || response_len != sizeof(TResponse)) {
            Logger::log<LogLevel::ERROR>("Invalid buffer size in ServiceCallback");
            return;
        }
        const TRequest* req = TRequest::decode(request, request_len);
        if (req) {
            TResponse* resp = reinterpret_cast<TResponse*>(response);
            _cb(*req, *resp);
        }
    }

    ServiceCallback(TSrvCallback<TRequest, TResponse> cb) : _cb(cb) {}

   private:
    TSrvCallback<TRequest, TResponse> _cb;
};

}  // namespace core
}  // namespace rix
