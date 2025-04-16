#include <fstream>
#include <sstream>

#include "rix/msg/geometry/TF.hpp"
#include "rix/msg/serializer.hpp"
#include "rix/util/log.hpp"

using Log = rix::util::Log;
using TF = rix::msg::geometry::TF;
using Buffer = std::vector<uint8_t>;

void log_tf(const TF &msg) {
    std::stringstream ss;
    ss << "TF:\n";
    for (const auto &t : msg.transforms) {
        ss << t.header.frame_id << " -> " << t.child_frame_id << "\n";
        ss << "Translation: [x: " << t.transform.translation.x << ", y: " << t.transform.translation.y
           << ", z: " << t.transform.translation.z << "]\n";
        ss << "Rotation: [w: " << t.transform.rotation.w << ", x: " << t.transform.rotation.x
           << ", y: " << t.transform.rotation.y << ", z: " << t.transform.rotation.z << "]\n";
    }
    Log::info << ss.str() << std::endl;
}

void deserialize_tf_from_file(const std::string &file_path, std::vector<TF> &tf_msgs) {
    Buffer buffer;
    std::ifstream file(file_path, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    Log::info << "File size: " << file_size << std::endl;
    buffer.resize(file_size);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    file.close();

    size_t offset = 0;
    while (offset < buffer.size()) {
        TF tf;
        rix::msg::Serializer<TF>::deserialize(tf, buffer, offset);
        tf_msgs.push_back(tf);
        log_tf(tf);
    }
}

bool is_approx(double a, double b, double epsilon = 0.0001) { return std::abs(a - b) < epsilon; }

bool transform_is_zero(const rix::msg::geometry::Transform &transform) {
    return is_approx(transform.translation.x, 0) && is_approx(transform.translation.y, 0) &&
           is_approx(transform.translation.z, 0) && is_approx(transform.rotation.w, 0) &&
           is_approx(transform.rotation.x, 0) && is_approx(transform.rotation.y, 0) &&
           is_approx(transform.rotation.z, 0);
}

bool transform_is_equal(const rix::msg::geometry::Transform &transform1,
                        const rix::msg::geometry::Transform &transform2) {
    return is_approx(transform1.translation.x, transform2.translation.x) &&
           is_approx(transform1.translation.y, transform2.translation.y) &&
           is_approx(transform1.translation.z, transform2.translation.z) &&
           is_approx(transform1.rotation.w, transform2.rotation.w) &&
           is_approx(transform1.rotation.x, transform2.rotation.x) &&
           is_approx(transform1.rotation.y, transform2.rotation.y) &&
           is_approx(transform1.rotation.z, transform2.rotation.z);
}

bool tf_is_zero(const TF &tf) {
    for (const auto &transforms : tf.transforms) {
        if (!transform_is_zero(transforms.transform)) {
            return false;
        }
    }
    return true;
}

bool tf_is_equal(const TF &tf1, const TF &tf2) {
    if (tf1.transforms.size() != tf2.transforms.size()) {
        return false;
    }

    size_t n = tf1.transforms.size();
    for (size_t i = 0; i < tf1.transforms.size(); i++) {
        // Search for the same frame_id and child_frame_id
        size_t j = 0;
        for (; j < tf2.transforms.size(); j++) {
            if (tf1.transforms[i].header.frame_id == tf2.transforms[j].header.frame_id &&
                tf2.transforms[i].child_frame_id == tf2.transforms[j].child_frame_id) {
                // If the transforms are not equal, return false
                if (!transform_is_equal(tf1.transforms[i].transform, tf2.transforms[j].transform)) {
                    return false;
                } else {
                    break;
                }
            }
        }
        // If the frame_id and child_frame_id are not found, return false
        if (j == n) {
            return false;
        }
    }
    return true;
}

size_t find_first_unrepeated_tf(const std::vector<TF> &tf_msgs) {
    for (size_t i = 0; i < tf_msgs.size(); i++) {
        if (!tf_is_equal(tf_msgs[i], tf_msgs[i + 1])) {
            return i;
        }
    }
    return tf_msgs.size();
}

size_t find_last_unrepeated_tf(const std::vector<TF> &tf_msgs) {
    for (size_t i = tf_msgs.size() - 1; i > 0; i--) {
        if (!tf_is_equal(tf_msgs[i], tf_msgs[i - 1])) {
            return i;
        }
    }
    return 0;
}

bool compare_tf_from_offset(const std::vector<TF> &sol_tf_msgs, const std::vector<TF> &test_tf_msgs, size_t sol_start,
                            size_t sol_end, size_t test_start, size_t test_end) {
    if (sol_end - sol_start != test_end - test_start) {
        return false;
    }
    for (size_t i = 0; i < sol_end - sol_start; i++) {
        if (!tf_is_equal(sol_tf_msgs[sol_start + i], test_tf_msgs[test_start + i])) {
            return false;
        }
    }
    return true;
}

/**
 * This program is used to compare the output from the tf_recorder program.
 * test/bin/sol_tf_msgs.bin contains the output from the following test:
 *
 * Terminal 1:
 * ./bin/rixhub
 *
 * Terminal 2 (test/test_tf_msgs.bin will be created. This is the file that will
 * be compared with sol_tf_msgs.bin):
 * ./build/tf_recorder test/test_tf_msgs.bin
 *
 * Terminal 3:
 * ./build/rsp robots/simple_bot.json
 *
 * Terminal 4:
 * ./build/jsp robots/simple_bot.json base_to_upper_arm --rate 1
 *
 * Terminal 5:
 * ./build/joint_publisher base_to_upper_arm --rate 1
 *
 * Once joint_publisher publishes 10 messages, you can stop the terminals.
 *
 * To test your output, run this program. If the output is correct, it will
 * return 0. Otherwise, it will return 1.
 *
 * The joint states from the binary files will be deserialized and printed to
 * the console. This program compares two vectors from the first non-zero
 * TF message to the first repeated non-zero TF message. If the vectors are
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
    std::vector<TF> sol_tf_msgs;
    deserialize_tf_from_file("test/bin/sol_tf_msgs.bin", sol_tf_msgs);

    std::vector<TF> test_tf_msgs;
    deserialize_tf_from_file("test/test_tf_msgs.bin", test_tf_msgs);

    size_t sol_start = find_first_unrepeated_tf(sol_tf_msgs);
    size_t test_start = find_first_unrepeated_tf(test_tf_msgs);
    size_t sol_end = find_last_unrepeated_tf(sol_tf_msgs);
    size_t test_end = find_last_unrepeated_tf(test_tf_msgs);

    bool status = compare_tf_from_offset(sol_tf_msgs, test_tf_msgs, sol_start, sol_end, test_start, test_end);
    if (!status) {
        Log::error << "TF messages do not match" << std::endl;
        return 1;
    }
    return 0;
}