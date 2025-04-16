#pragma once

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <signal.h>

#include <map>
#include <functional>
#include <vector>

namespace rix {
namespace util {

/**
 * @brief A class for handling signals.
 * @example shared_mutex.cpp
 * @example pipe_reader.cpp
 * @example pipe_writer.cpp
 */
class SignalHandler {
   public:
    /**
     * @brief Type alias for the handler function.
     */
    using HandlerFunction = std::function<void(int)>;

    /**
     * @brief Gets the singleton instance of the SignalHandler.
     * @return The singleton instance of the SignalHandler.
     */
    static SignalHandler& get_instance();

    /**
     * @brief Registers a handler function for a specific signal.
     * @param signal The signal to handle.
     * @param handler The handler function to register.
     */
    void register_handle(int signal, HandlerFunction handler);

    /**
     * @brief Handles the signal by calling the registered handler functions.
     * @param signal The signal to handle.
     */
    static void handle(int signal);

   private:
    /**
     * @brief Constructs a SignalHandler object.
     */
    SignalHandler();

    std::map<int, std::vector<HandlerFunction>> handlers;  ///< Map of signal handlers.
};

}  // namespace util
}  // namespace rix