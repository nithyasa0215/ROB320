#include "rix/core/publisher/pub_impl_base.hpp"

namespace rix {
namespace core {

PubImplBase::PubImplBase(size_t msg_size, uint64_t component_id)
    : shutdown_flag(false), msg_size(msg_size), component_id(component_id) {
    // TODO: Implement the constructor. The constructor should:
    //       1. Initialize the member variables of the PubImplBase class.
}

PubImplBase::~PubImplBase() { 
    // TODO: Implement the destructor. The destructor should:
    //       1. Do nothing by default, unless you need to perform cleanup for
    //          any member variables that you have created.
}

void PubImplBase::shutdown() { 
    // TODO: Implement the shutdown method. The method should:
    //       1. Set the shutdown flag to true.
}

bool PubImplBase::ok() const {
    // TODO: Implement the ok method. The method should:
    //       1. Return true if the shutdown flag is false.
    
    return false;
}

}  // namespace core
}  // namespace rix