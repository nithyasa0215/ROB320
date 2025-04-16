#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <string>
#include <cstring>

namespace rix {
namespace ipc {

/**
 * @brief Get the public IP address of the machine.
 * @return The public IP address of the machine.
 * 
 * @example tcp_client.cpp
 * @example tcp_client_noblock.cpp
 * @example tcp_server.cpp
 * @example tcp_server_noblock.cpp
 * @example udp_receiver.cpp
 * @example udp_sender.cpp
 * @example web_client.cpp
 * @example web_server.cpp
 */
std::string get_public_ip();

/**
 * @brief Class for representing URIs.
 * 
 * @example tcp_client.cpp
 * @example tcp_client_noblock.cpp
 * @example tcp_server.cpp
 * @example tcp_server_noblock.cpp
 * @example udp_receiver.cpp
 * @example udp_receiver_multicast.cpp
 * @example udp_sender.cpp
 * @example udp_sender_multicast.cpp
 * @example web_client.cpp
 * @example web_server.cpp
*/
class URI {
   public:
    std::string ip;
    int port;
    bool is_ws;

    /**
     * @brief Default constructor. Initializes an empty URI.
     */
    URI();

    /**
     * @brief Constructs a URI with the given IP address and port.
     * @param ip The IP address.
     * @param port The port number.
     * @param is_ws Whether the URI is a WebSocket URI.
     */
    URI(const std::string &ip, int port, bool is_ws = false);

    /**
     * @brief Constructs a URI from a string representation.
     * @param uri The string representation of the URI.
     */
    URI(const std::string &uri);

    /**
     * @brief Less-than operator for comparing URIs.
     * @param other The other URI to compare with.
     * @return true if this URI is less than the other URI, false otherwise.
     */
    bool operator<(const URI &other) const;

    /**
     * @brief Equality operator for comparing URIs.
     * @param other The other URI to compare with.
     * @return true if the URIs are equal, false otherwise.
     */
    bool operator==(const URI &other) const;

    /**
     * @brief Inequality operator for comparing URIs.
     * @param other The other URI to compare with.
     * @return true if the URIs are not equal, false otherwise.
     */
    bool operator!=(const URI &other) const;

    /**
     * @brief Converts the URI to a string representation.
     * @return The string representation of the URI.
     */
    std::string to_string() const;

    /**
     * @brief Stream insertion operator for printing URIs.
     * @param os The output stream.
     * @param uri The URI to print.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const URI &uri) {
        os << uri.to_string();
        return os;
    }
};

}  // namespace ipc
}  // namespace rix