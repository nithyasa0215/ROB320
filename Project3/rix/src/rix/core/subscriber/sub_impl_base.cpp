#include "rix/core/subscriber/sub_impl_base.hpp"

namespace rix {
namespace core {

SubImplBase::SubImplBase(size_t msg_size, uint64_t component_id, SubCallbackPtr callback)
    : shutdown_flag(false), msg_size(msg_size), component_id(component_id), callback(callback) {
    // TODO: Implement the constructor. The constructor should:
    //       1. Initialize the member variables of the SubImplBase class.
}

SubImplBase::~SubImplBase() { 
    // TODO: Implement the destructor. The destructor should:
    //       1. Do nothing by default, unless you need to perform cleanup for
    //          any member variables that you have created.
}

void SubImplBase::shutdown() { 
    // TODO: Implement the shutdown method. The method should:
    //       1. Set the shutdown flag to true.
}

bool SubImplBase::ok() const { 
    // TODO: Implement the is_shutdown method. The method should:
    //       1. Return true if the shutdown flag is false.
    
    return false;
}

rix::msg::component::URI SubImplBase::get_uri() const {
    // This is an optional virtual method
    // The default implementation returns an empty URI structure.
    return rix::msg::component::URI();
}


}  // namespace core
}  // namespace rix