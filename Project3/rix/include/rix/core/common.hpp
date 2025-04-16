#pragma once

#include <random>
#include <vector>
#include "rix/util/logger.hpp"

#define RIX_HUB_PORT 8000
#define RIX_HUB_WEB_PORT 8001

using LogLevel = rix::util::LogLevel;
using Logger = rix::util::Logger;

namespace rix {
namespace core {

enum OPCODE {
    SUB_REGISTER = 80,      ///< Subscriber register operation code, sent from Node to Hub.
    PUB_REGISTER,           ///< Publisher register operation code, sent from Node to Hub.
    SRV_REGISTER,           ///< Service register operation code, sent from Node to Hub (not used in P3).
    ACT_REGISTER,           ///< Action register operation code, sent from Node to Hub (not used in P3).

    SUB_NOTIFY = 90,        ///< Used to notify a subscriber of a new publisher, sent from Hub to Node.
    PUB_NOTIFY,             ///< Used to notify a publisher of a new subscriber, sent from Hub to Node.
    SRV_NOTIFY,             ///< Service notify operation code, sent from Hub to Node (not used in P3).
    ACT_NOTIFY,             ///< Action notify operation code, sent from Hub to Node (not used in P3).

    SUB_DEREGISTER = 100,   ///< Subscriber deregister operation code, sent from Node to Hub.
    PUB_DEREGISTER,         ///< Publisher deregister operation code, sent from Node to Hub.
    SRV_DEREGISTER,         ///< Service deregister operation code, sent from Node to Hub (not used in P3).
    ACT_DEREGISTER,         ///< Action deregister operation code, sent from Node to Hub (not used in P3).

    SUB_DISCONNECT = 110,   ///< Used to tell a subscriber to remove a publisher, sent from Hub to Node.
    PUB_DISCONNECT,         ///< Used to tell a publisher to remove a subscriber, sent from Hub to Node.

    SRV_REQUEST = 140,      ///< Service request operation code, sent from Node to Hub (not used in P3).
    SRV_RESPONSE,           ///< Service response operation code, sent from Node to Hub (not used in P3).

    ACT_REQUEST = 150,      ///< Action request operation code, sent from Node to Hub (not used in P3).
    ACT_RESPONSE,           ///< Action response operation code, sent from Node to Hub (not used in P3).

    MED_TERMINATE = 160     ///< Terminate operation code, sent from Hub to Node when Hub is shutting down.
};

/**
 * @brief Enumerates the error codes.
 * 
 * The error codes are used to indicate the type of error that occurred. If an 
 * error occurs in the Hub, the error field in the Info structure will be set to
 * one of these values.
 */
enum ERROR_CODE {
    NO_ERROR = 0,
    WRONG_MSG_TYPE, 
    WRONG_REQ_TYPE,
    WRONG_RES_TYPE,
    WRONG_TOPIC,
    EMPTY_TOPIC,
    EMPTY_SRV,
    EMPTY_ACT,
    MULTICAST_EXISTS,
    SHMEM_EXISTS,
    PUB_EXISTS,
    SUB_EXISTS,
    SRV_EXISTS,
    SRV_NOT_FOUND,
    ACT_EXISTS,
    ACT_NOT_FOUND
};

/**
 * @brief Enumerates the protocols.
 * 
 * The protocols are used to indicate the type of communication protocol used by
 * the Publisher and Subscriber. The protocol field in the Info structure will 
 * be set to one of these values.
 */
enum Protocol { TCP = 0x01, UDP = 0x02, SHM = 0x04, WEB = 0x08, WEB2 = 0x10 };

/**
 * @brief Generates a random ID.
 * @return A random ID.
 */
static inline uint64_t generate_id() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;
    return dis(gen);
}

}  // namespace core
}  // namespace rix