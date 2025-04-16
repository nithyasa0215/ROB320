#include <fstream>
#include <sstream>
#include "rix/util/log.hpp"
#include "rix/msg/serializer.hpp"
#include "rix/msg/sensor/JS.hpp"

using Log = rix::util::Log;
using JS = rix::msg::sensor::JS;
using Buffer = std::vector<uint8_t>;

void log_joint_state(const JS &msg) {
    std::stringstream ss;
    ss << "JS (t= " << msg.stamp.sec << "." << msg.stamp.nsec << ")" << std::endl;
    for (const auto &joint_state : msg.joint_states) {
        ss << "  " << joint_state.name << ": " << joint_state.position << std::endl;
    }
    Log::info << ss.str();
}

void deserialize_js_from_file(const std::string &file_path, std::vector<JS> &js_msgs) {
    Buffer buffer;
    std::ifstream file(file_path, std::ios::binary);
    file.seekg(0, std::ios::end);
    buffer.resize(file.tellg());
    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    file.close();

    size_t offset = 0;
    while (offset < buffer.size()) {
        JS js;
        rix::msg::Serializer<JS>::deserialize(js, buffer, offset);
        js_msgs.push_back(js);
        log_joint_state(js);
    }
}

bool js_is_zero(const JS &js) {
    for (const auto &joint_state : js.joint_states) {
        if (joint_state.position != 0.0) {
            return false;
        }
    }
    return true;
}

bool js_is_equal(const JS &js1, const JS &js2) {
    if (js1.joint_states.size() != js2.joint_states.size()) {
        return false;
    }
    for (size_t i = 0; i < js1.joint_states.size(); i++) {
        if (js1.joint_states[i].name != js2.joint_states[i].name) {
            return false;
        }
        if (js1.joint_states[i].position != js2.joint_states[i].position) {
            return false;
        }
    }
    return true;
}

size_t find_first_nonzero_js(const std::vector<JS> &js_msgs) {
    for (size_t i = 0; i < js_msgs.size(); i++) {
        if (!js_is_zero(js_msgs[i])) {
            return i;
        }
    }
    return js_msgs.size();
}

size_t find_first_nonzero_repeated_js(const std::vector<JS> &js_msgs) {
    for (size_t i = 0; i < js_msgs.size(); i++) {
        if (!js_is_zero(js_msgs[i]) && js_is_equal(js_msgs[i], js_msgs[i + 1])) {
            return i;
        }
    }
    return js_msgs.size();
}


bool compare_js_from_offset(const std::vector<JS> &sol_js_msgs, const std::vector<JS> &test_js_msgs, size_t sol_start, size_t sol_end, size_t test_start, size_t test_end) {
    if (sol_end - sol_start != test_end - test_start) {
        return false;
    }
    for (size_t i = 0; i < sol_end - sol_start; i++) {
        if (!js_is_equal(sol_js_msgs[sol_start + i], test_js_msgs[test_start + i])) {
            return false;
        }
    }
    return true;
}


/**
 * This program is used to compare the output from the js_recorder program. 
 * test/sol_js_msgs.bin contains the output from the following test:
 * 
 * Terminal 1:
 * ./bin/rixhub
 * 
 * Terminal 2 (test/bin/test_js_msgs.bin will be created. This is the file that will
 * be compared with sol_js_msgs.bin):
 * ./build/js_recorder test/test_js_msgs.bin
 * 
 * Terminal 3:
 * ./build/joint_publisher base_to_upper_arm --rate 1
 * 
 * Terminal 4:
 * ./build/jsp robots/simple_but.json base_to_upper_arm --rate 1
 * 
 * Once joint_publisher publishes 10 messages, you can stop the terminals. 
 * 
 * To test your output, run this program. If the output is correct, it will 
 * return 0. Otherwise, it will return 1.
 * 
 * The joint states from the binary files will be deserialized and printed to 
 * the console. This program compares two vectors from the first non-zero
 * JS message to the first repeated non-zero JS message. If the vectors are
 * equal, the program will return 0. Otherwise, it will return 1.
 * 
 * In simplified terms,
 * 
 * 00000[ABCDEFGH]H
 * 00000000[ABCDEFGH]HHH
 * 
 * are equal.
 * 
 * 000[ABCDEFGHIJ]J
 * [ABCDEFGH]
 * 
 * are not equal.
 */

int main() {

    std::vector<JS> sol_js_msgs;
    deserialize_js_from_file("test/bin/sol_js_msgs.bin", sol_js_msgs);

    std::vector<JS> test_js_msgs;
    deserialize_js_from_file("test/test_js_msgs.bin", test_js_msgs);

    size_t sol_start = find_first_nonzero_js(sol_js_msgs);
    size_t test_start = find_first_nonzero_js(test_js_msgs);
    size_t sol_end = find_first_nonzero_repeated_js(sol_js_msgs);
    size_t test_end = find_first_nonzero_repeated_js(test_js_msgs);

    bool status = compare_js_from_offset(sol_js_msgs, test_js_msgs, sol_start, sol_end, test_start, test_end);
    if (!status) {
        Log::error << "JS messages do not match" << std::endl;
        return 1;
    }
    return 0;
}