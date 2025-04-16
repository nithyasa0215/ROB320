#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <string>

#include "rix/util/timing.hpp"
#include "stdlib.h"

namespace rix {
namespace util {

/**
 * @brief Log levels for the Logger.
 */
enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };

/**
 * @brief A class for logging messages with different log levels.
 */
class Logger {
   public:
    /**
     * @brief The base log level. Messages with a lower level will not be logged.
     */
    static LogLevel BaseLevel;

    /**
     * @brief Destructor for the Logger class.
     */
    ~Logger() {
        if (is_file_logging) {
            log_file.close();
        }
    }

    /**
     * @brief Initializes the logger.
     * @param name The name of the logger.
     * @param log_to_file Whether to log messages to a file.
     */
    static void init(const std::string &name, bool log_to_file = false) {
        instance.name = name;
        instance.is_file_logging = log_to_file;
        if (!instance.is_file_logging) {
            instance.is_init = true;
            return;
        }

        system(std::string("mkdir -p ~/.rix/" + name).c_str());
        std::string homeDir = getenv("HOME");
        instance.log_file.open(homeDir + "/.rix/" + name + "/" + rix::util::now().to_string() + ".log");
        if (!instance.log_file.is_open()) {
            throw std::runtime_error("Failed to open log file.");
        }
        instance.is_init = true;
    }

    /**
     * @brief Logs a formatted message with the specified log level.
     * @tparam Level The log level.
     * @tparam Args The types of the arguments.
     * @param format The format string.
     * @param args The arguments to format.
     */
    template <LogLevel Level, typename... Args>
    static void log(const std::string &format, Args... args) {
        if (Level < BaseLevel) {
            return;
        }
        int size = snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args...);
        std::string final_str = std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
        instance.log(Level, final_str);
    }

    /**
     * @brief Logs a message with the specified log level.
     * @tparam Level The log level.
     * @param msg The message to log.
     */
    template <LogLevel Level>
    static void log(const std::string &msg) {
        if (Level < BaseLevel) {
            return;
        }
        instance.log(Level, msg);
    }

    /**
     * @brief Sets whether to log messages to a file.
     * @param status True to log to a file, false otherwise.
     */
    void set_file_logging(bool status) { is_file_logging = status; }

   private:
    static Logger instance;  ///< The singleton instance of the Logger.
    const std::string default_color = "\033[0m";  ///< The default color for log messages.

    std::ofstream log_file;  ///< The file stream for logging to a file.
    std::mutex mutex;  ///< The mutex for thread-safe logging.
    std::string name;  ///< The name of the logger.
    rix::util::DateTime start_time;  ///< The start time of the logger.
    bool is_file_logging;  ///< Whether to log messages to a file.
    bool is_init;  ///< Whether the logger is initialized.

    /**
     * @brief Constructs a Logger object.
     */
    Logger();

    /**
     * @brief Logs a message with the specified log level.
     * @param level The log level.
     * @param msg The message to log.
     */
    void log(LogLevel level, const std::string &msg);

    /**
     * @brief Gets the string representation of a log level.
     * @param level The log level.
     * @return The string representation of the log level.
     */
    static std::string get_level_string(LogLevel level);

    /**
     * @brief Gets the color representation of a log level.
     * @param level The log level.
     * @return The color representation of the log level.
     */
    static std::string get_level_color(LogLevel level);
};

}  // namespace util
}  // namespace rix