#include "rix/rdf/tree.hpp"

namespace rix {
namespace rdf {

Eigen::Affine3d matrix_from_json_origin(const Json &origin) {
    Eigen::Affine3d origin_obj = Eigen::Affine3d::Identity();
    if (!origin.is_object()) {
        return origin_obj;
    }
    bool has_xyz = origin.count("xyz") > 0;
    bool has_rpy = origin.count("rpy") > 0;
    if (!has_xyz || !has_rpy) {
        return origin_obj;
    }

    Json xyz = origin["xyz"];
    if (!xyz.is_array()) {
        return origin_obj;
    }
    Json rpy = origin["rpy"];
    if (!rpy.is_array()) {
        return origin_obj;
    }
    if (xyz.size() != 3 || rpy.size() != 3) {
        throw std::runtime_error("Invalid size for origin array");
    }

    Eigen::Vector3d xyz_v(xyz[0].template get<double>(), xyz[1].template get<double>(), xyz[2].template get<double>());
    Eigen::Vector3d rpy_v(rpy[0].template get<double>(), rpy[1].template get<double>(), rpy[2].template get<double>());
    origin_obj = Eigen::Translation3d(xyz_v) * Eigen::AngleAxisd(rpy_v[0], Eigen::Vector3d::UnitX()) *
                 Eigen::AngleAxisd(rpy_v[1], Eigen::Vector3d::UnitY()) *
                 Eigen::AngleAxisd(rpy_v[2], Eigen::Vector3d::UnitZ());
    return origin_obj;
}

Tree::Tree(const Json &json) {
    // Check for required fields of JRDF
    bool has_links = json.count("links") > 0;
    bool has_joints = json.count("joints") > 0;

    if (!has_links || !has_joints) {
        throw std::runtime_error("Invalid robot model JSON. Missing required fields.");
    }

    Json joints = json["joints"];
    Json links = json["links"];
    if (!joints.is_array() || !links.is_array()) {
        throw std::runtime_error("Invalid robot model JSON. Links and joints must be arrays.");
    }

    std::unordered_map<std::string, Eigen::Affine3d> link_origin_map;
    for (auto link : links) {
        bool has_name = link.count("name") > 0;
        if (!has_name) {
            throw std::runtime_error("Invalid link JSON. Link is missing required fields");
        }
        if (!link["name"].is_string()) {
            throw std::runtime_error("Invalid link JSON. Link name must be a string");
        }
        std::string link_name = link["name"].template get<std::string>();

        bool has_visual = link.count("visual") > 0;

        Eigen::Affine3d visual_origin_obj = Eigen::Affine3d::Identity();
        if (has_visual) {
            Json visual = link["visual"];
            if (!visual.is_object()) {
                throw std::runtime_error("Invalid visual JSON. Link visual must be an object");
            }
            bool has_origin = visual.count("origin") > 0;
            if (has_origin) {
                visual_origin_obj = matrix_from_json_origin(visual["origin"]);
            }
        }

        link_origin_map[link_name] = visual_origin_obj;
    }

    // Parse JSON joints list into Joint objects
    std::unordered_map<std::string, std::vector<std::string>> child_map;
    std::unordered_map<std::string, std::string> parent_map;
    for (auto &joint : joints) {
        // Check for required fields of joints
        bool has_name = joint.count("name") > 0;
        bool has_type = joint.count("type") > 0;
        bool has_parent = joint.count("parent") > 0;
        bool has_child = joint.count("child") > 0;

        if (!has_name || !has_type || !has_parent || !has_child) {
            throw std::runtime_error("Invalid joint JSON. Joint is missing required fields.");
        }
        if (!joint["name"].is_string() || !joint["type"].is_string() || !joint["parent"].is_string() ||
            !joint["child"].is_string()) {
            throw std::runtime_error("Invalid joint JSON. Joint name, type, parent, and child must be strings.");
        }
        std::string joint_name = joint["name"].template get<std::string>();
        std::string parent = joint["parent"].template get<std::string>();
        std::string child = joint["child"].template get<std::string>();
        std::string type_str = joint["type"].template get<std::string>();

        // Check for optional origin field
        Eigen::Affine3d origin_obj = Eigen::Affine3d::Identity();
        bool has_origin = joint.count("origin") > 0;
        if (has_origin) {
            origin_obj = matrix_from_json_origin(joint["origin"]);
        }

        // Parse joint type
        JointType type;
        if (type_str == "revolute") {
            type = JointType::REVOLUTE;
        } else if (type_str == "prismatic") {
            type = JointType::PRISMATIC;
        } else if (type_str == "fixed") {
            type = JointType::FIXED;
        } else if (type_str == "continuous") {
            type = JointType::CONTINUOUS;
        } else {
            throw std::runtime_error("Invalid joint type");
        }

        // Parse axis
        Eigen::Vector3d a(0, 0, 0);
        if (type != JointType::FIXED) {
            bool has_axis = joint.count("axis") > 0;
            Json axis = joint["axis"];
            if (axis.size() != 3) {
                throw std::runtime_error("Invalid size for axis array");
            }
            a[0] = axis[0].template get<double>();
            a[1] = axis[1].template get<double>();
            a[2] = axis[2].template get<double>();
        }

        // Check for joint limits
        double lower = std::numeric_limits<double>::lowest();
        double upper = std::numeric_limits<double>::max();
        bool has_limits = joint.count("limits") > 0;
        if (has_limits) {
            Json limits = joint["limits"];
            bool hasUpper = limits.find("upper") != limits.end();
            bool hasLower = limits.find("lower") != limits.end();
            if (!hasUpper || !hasLower) {
                throw std::runtime_error("Invalid limits JSON");
            }
            lower = limits["lower"].template get<double>();
            upper = limits["upper"].template get<double>();
        } 
        this->joints.emplace(joint_name, Joint(a, origin_obj, type, parent, child, upper, lower));
        parent_map[child] = joint_name;
        child_map[parent].push_back(joint_name);
    }

    for (const auto &origin : link_origin_map) {
        const std::string &link_name = origin.first;
        const std::vector<std::string> &children = child_map[link_name];
        const std::string &parent = parent_map[link_name];
        this->links.emplace(link_name, Link(origin.second, parent, children));
    }

    for (const auto &pair : this->links) {
        if (pair.second.is_root()) {
            this->root = pair.first;
            break;
        }
    }
}

size_t Tree::get_num_joints() const {
    return joints.size();
}

size_t Tree::get_num_links() const {
    return links.size();
}

std::vector<std::string> Tree::get_joint_names() const {
    std::vector<std::string> names;
    for (const auto &pair : joints) {
        names.push_back(pair.first);
    }
    return names;
}

std::vector<std::string> Tree::get_link_names() const {
    std::vector<std::string> names;
    for (const auto &pair : links) {
        names.push_back(pair.first);
    }
    return names;
}

bool Tree::has_joint(const std::string &name) const {
    return joints.find(name) != joints.end();
}

bool Tree::has_link(const std::string &name) const {
    return links.find(name) != links.end();
}

Joint &Tree::get_joint(const std::string &name) {
    return joints.at(name);
}

Link &Tree::get_link(const std::string &name) {
    return links.at(name);
}

const Joint &Tree::get_joint(const std::string &name) const {
    return joints.at(name);
}

const Link &Tree::get_link(const std::string &name) const {
    return links.at(name);
}

std::string Tree::get_root() const {
    return root;
}

std::vector<std::string> Tree::get_end_effectors() const {
    std::vector<std::string> end_effectors;
    for (const auto &pair : links) {
        if (pair.second.is_end_effector()) {
            end_effectors.push_back(pair.first);
        }
    }
    return end_effectors;
}

rix::msg::geometry::TF Tree::TF() {
    /**
     * TODO: Generate a TF message from the tree.
     */

    return rix::msg::geometry::TF();
}

rix::msg::sensor::JS Tree::JS() {
    /**
     * TODO: Generate a JS message from the tree.
     */

    return rix::msg::sensor::JS();
}

void Tree::set_state(const rix::msg::sensor::JS &js) {
    /**
     * TODO: Set the state of the joints within the tree from a JS message.
     */
}

}  // namespace rdf
}  // namespace rix