#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "json.hpp"
#include "Eigen/Geometry"

using json = nlohmann::json;

enum JointType {
    REVOLUTE,
    PRISMATIC,
    FIXED
};

struct Joint {
    std::string parent;
    std::string child;
    Eigen::Vector3d axis;
    Eigen::Affine3d origin;
    JointType type;
};

struct Link {
    Eigen::Affine3d origin;
    std::string parent;
    std::vector<std::string> children;
};

struct KinematicTree {
    KinematicTree(std::string file_path);

    std::string root;
    std::unordered_map<std::string, Joint> joints;
    std::unordered_map<std::string, Link> links;
};

KinematicTree::KinematicTree(std::string file_path) {
    /**
     * TODO: Parse the JSON file at file_path and populate the KinematicTree struct.
     * Use the nlohmann::json library to parse the JSON file and Eigen for the
     * Affine3d and Vector3d types. Assume that the JSON file has all of the 
     * required fields and types. If an origin is not provided, set it to the
     * identity matrix.
     * 
     * The joints map should have the joint name as the key and the Joint struct
     * as the value. The links map should have the link name as the key and the
     * Link struct as the value. 
     * 
     * Link::parent should contain the name of the link's parent joint.
     * Link::children should contain the names of the child joints. 
     * Joint::parent should contain the name of the joint's parent link.
     * Joint::child should contain the name of the joint's child link.
     * 
     * As an example, to access the parent Link structure of a joint, you would
     * use:
     * KinematicTree tree("robot.json");
     * Link parent_link = tree.links[tree.joints["joint_name"].parent];
     */
}