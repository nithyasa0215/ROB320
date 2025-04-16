#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#include <iostream>
#include <string>
#include <memory>

#include "rix/ipc/uri.hpp"

namespace rix {
namespace ipc {

/**
 * @brief A class for creating TCP clients.
 * @example tcp_client.cpp
 * @example tcp_client_noblock.cpp
*/
class Client {
   public:

    /**
     * @brief Default constructor. Initializes the client with default values.
     */
    Client();

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    Client(const Client& other) = delete;

    /**
     * @brief Constructs a client and opens a connection to the specified URI.
     * @param uri The URI to connect to.
     * @param nonblocking Whether the client should operate in non-blocking mode.
     */
    Client(const URI& uri, bool nonblocking = true);

    /**
     * @brief Destructor. Closes the connection if it is open.
     */
    ~Client();

    /**
     * @brief Opens a connection to the specified URI.
     * @param uri The URI to connect to.
     * @param nonblocking Whether the client should operate in non-blocking mode.
     * @return 0 on success, -1 on failure.
     */
    int open(const URI& uri, bool nonblocking = true);

    /**
     * @brief Connects to the server.
     * @return 0 on success, -1 on failure.
     */
    int connect();

    /**
     * @brief Sends a message to the server.
     * @param message The message to send.
     * @param size The size of the message. This will be updated with the number of bytes sent.
     * @return 0 on success, 1 if the operation would block, -1 on failure.
     */
    int send(const uint8_t * const message, size_t& size);

    /**
     * @brief Receives a message from the server.
     * @param message The buffer to store the received message.
     * @param size The size of the buffer. This will be updated with the number of bytes received.
     * @return 0 on success, -1 on failure.
     */
    int recv(uint8_t * message, size_t& size);

    /**
     * @brief Closes the connection.
     */
    void close();

    /**
     * @brief Checks if the connection is open.
     * @return true if the connection is open, false otherwise.
     */
    bool is_open() const;

    /**
     * @brief Checks if the client is operating in non-blocking mode.
     * @return true if the client is non-blocking, false otherwise.
     */
    bool is_nonblocking() const;

    /**
     * @brief Sets the non-blocking mode of the client.
     * @param nonblocking Whether the client should operate in non-blocking mode.
     * @return 0 on success, -1 on failure.
     */
    int set_nonblocking(bool nonblocking);

    /**
     * @brief Checks if the client is writable.
     * @return true if the client is writable, false otherwise.
     */
    bool is_writable() const;

    /**
     * @brief Checks if the client is readable.
     * @return true if the client is readable, false otherwise.
     */
    bool is_readable() const;

    /**
     * @brief Waits for the client to become writable.
     * @param timeout The timeout in milliseconds.
     * @return true if the client is writable, false otherwise.
     */
    bool wait_for_writable(int timeout) const;

    /**
     * @brief Waits for the client to become readable.
     * @param timeout The timeout in milliseconds.
     * @return true if the client is readable, false otherwise.
     */
    bool wait_for_readable(int timeout) const;

    /**
     * @brief Gets the URI of the connected server.
     * @return The URI of the server.
     */
    URI get_server_uri() const;
    
   private:
    int fd;
    bool connected;
    bool opened;
    bool nonblocking;
    URI server_uri;
    sockaddr_in server;
};

using ClientPtr = std::shared_ptr<Client>;

}  // namespace ipc
}  // namespace rix