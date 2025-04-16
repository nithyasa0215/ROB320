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

class Server;

/**
 * @brief A class for creating TCP connections.
 * @example tcp_server.cpp
 * @example tcp_server_noblock.cpp
*/
class Connection {
    friend class Server;

   public:
    /**
     * @brief Default constructor. Initializes the connection with default values.
     */
    Connection();

    /**
     * @brief Destructor. Closes the connection if it is open.
     */
    ~Connection();

    /**
     * @brief Sends a message to the client.
     * @param message The message to send.
     * @param size The size of the message. This will be updated with the number of bytes sent.
     * @return 0 on success, 1 if the operation would block, -1 on failure.
     */
    int send(const uint8_t * const message, size_t& size);

    /**
     * @brief Receives a message from the client.
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
     * @brief Checks if the connection is operating in non-blocking mode.
     * @return true if the connection is non-blocking, false otherwise.
     */
    bool is_nonblocking() const;

    /**
     * @brief Sets the non-blocking mode of the connection.
     * @param nonblocking Whether the connection should operate in non-blocking mode.
     * @return 0 on success, -1 on failure.
     */
    int set_nonblocking(bool nonblocking);

    /**
     * @brief Checks if the connection is writable.
     * @return true if the connection is writable, false otherwise.
     */
    bool is_writable() const;

    /**
     * @brief Checks if the connection is readable.
     * @return true if the connection is readable, false otherwise.
     */
    bool is_readable() const;

    /**
     * @brief Waits for the connection to become writable.
     * @param timeout The timeout in milliseconds.
     * @return true if the connection is writable, false otherwise.
     */
    bool wait_for_writable(int timeout) const;

    /**
     * @brief Waits for the connection to become readable.
     * @param timeout The timeout in milliseconds.
     * @return true if the connection is readable, false otherwise.
     */
    bool wait_for_readable(int timeout) const;

    /**
     * @brief Gets the URI of the connected client.
     * @return The URI of the client.
     */
    URI get_client_uri() const;

   private:
    /**
     * @brief Constructs a connection with the given file descriptor, client address, and non-blocking mode.
     * @param fd The file descriptor of the connection.
     * @param client The address of the client.
     * @param nonblocking Whether the connection should operate in non-blocking mode.
     */
    Connection(int fd, sockaddr_in client, bool nonblocking);

    int fd;
    bool opened;
    bool nonblocking;
    URI client_uri;
    sockaddr_in client;
};

using ConnectionPtr = std::shared_ptr<Connection>;

/**
 * @brief A class for creating TCP servers.
 * @example tcp_server.cpp
 * @example tcp_server_noblock.cpp
*/
class Server {
   public:
    /**
     * @brief Default constructor. Initializes the server with default values.
     */
    Server();

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    Server(const Server& other) = delete;

    /**
     * @brief Constructs a server and opens a connection to the specified URI.
     * @param uri The URI to bind to.
     * @param nonblocking Whether the server should operate in non-blocking mode.
     */
    Server(const URI& uri, bool nonblocking = 1);

    /**
     * @brief Destructor. Closes the server if it is open.
     */
    ~Server();

    /**
     * @brief Opens a connection to the specified URI.
     * @param uri The URI to bind to.
     * @param nonblocking Whether the server should operate in non-blocking mode.
     * @return 0 on success, -1 on failure.
     */
    int open(const URI& uri, bool nonblocking = 1);

    /**
     * @brief Binds the server to the specified URI.
     * @return 0 on success, -1 on failure.
     */
    int bind();

    /**
     * @brief Starts listening for incoming connections.
     * @param connections The maximum number of connections to accept.
     * @return 0 on success, -1 on failure.
     */
    int listen(size_t connections);

    /**
     * @brief Accepts an incoming connection.
     * @param connection A shared pointer to store the accepted connection.
     * @return 0 on success, -1 on failure.
     */
    int accept(std::shared_ptr<Connection>& connection);

    /**
     * @brief Closes the server.
     */
    void close();

    /**
     * @brief Checks if the server is open.
     * @return true if the server is open, false otherwise.
     */
    bool is_open() const;

    /**
     * @brief Checks if the server is bound to a URI.
     * @return true if the server is bound, false otherwise.
     */
    bool is_bound() const;

    /**
     * @brief Checks if the server is listening for connections.
     * @return true if the server is listening, false otherwise.
     */
    bool is_listening() const;

    /**
     * @brief Checks if the server is operating in non-blocking mode.
     * @return true if the server is non-blocking, false otherwise.
     */
    bool is_nonblocking() const;

    /**
     * @brief Sets the non-blocking mode of the server.
     * @param nonblocking Whether the server should operate in non-blocking mode.
     * @return 0 on success, -1 on failure.
     */
    int set_nonblocking(bool nonblocking);

    /**
     * @brief Gets the URI of the server.
     * @return The URI of the server.
     */
    URI get_uri() const;

   private:
    int fd;
    size_t max_connections;
    size_t connections;
    bool opened;
    bool bound;
    bool listening;
    bool nonblocking;
    URI uri;

    sockaddr_in server;
};

using ServerPtr = std::shared_ptr<Server>;

}  // namespace ipc
}  // namespace rix