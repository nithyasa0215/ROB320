#include "rix/rdf/tree.hpp"

#include <gtest/gtest.h>

#include "nlohmann/json.hpp"
#include <Eigen/Geometry>

using namespace rix::rdf;
using namespace nlohmann;

#define TOL 1e-5

const char* ROBOT = R"(
{
  "name": "simple_bot",
  "links": [
    {
      "name": "base",
      "visual": {
        "geometry": {
          "type": "box",
          "size": [1.0, 0.5, 0.5]
        },
        "origin": {
          "xyz": [0, 1.0, 0],
          "rpy": [0, 0, 0]
        }
      }
    },
    {
      "name": "upper_arm",
      "visual": {
        "geometry": {
          "type": "box",
          "size": [1.0, 0.5, 0.5]
        },
        "origin": {
          "xyz": [1.0, 0, 0],
          "rpy": [0, 0, 0]
        }
      }
    },
    {
      "name": "forearm",
      "visual": {
        "geometry": {
          "type": "box",
          "size": [1.0, 0.5, 0.5]
        },
        "origin": {
          "xyz": [1.0, 0, 0],
          "rpy": [0, 0, 0]
        }
      }
    }
  ],
  "joints": [
    {
      "name": "base_to_upper_arm",
      "type": "revolute",
      "parent": "base",
      "child": "upper_arm",
      "origin": {
        "xyz": [1.0, 0, 0.0],
        "rpy": [0, 0, 0]
      },
      "axis": [0, 1, 0],
      "limits": {
        "lower": -3.14159,
        "upper": 3.14159
      }
    },
    {
      "name": "upper_arm_to_forearm",
      "type": "revolute",
      "parent": "upper_arm",
      "child": "forearm",
      "origin": {
        "xyz": [1.0, 0, 0],
        "rpy": [0, 0, 0]
      },
      "axis": [0, 0, 1],
      "limits": {
        "lower": -3.14159,
        "upper": 3.14159
      }
    }
  ]
}
)";

using TF = rix::msg::geometry::TF;
using JS = rix::msg::sensor::JS;

void print_tf(const TF &tf) {
    std::stringstream ss;
    for (const auto &t : tf.transforms) {
        ss << t.header.frame_id << " -> " << t.child_frame_id << "\n";
        ss << "  Translation: [x: " << t.transform.translation.x << ", y: " << t.transform.translation.y << ", z: " << t.transform.translation.z << "]\n";
        ss << "  Rotation:    [w: " << t.transform.rotation.w << ", x: " << t.transform.rotation.x << ", y: " << t.transform.rotation.y << ", z: " << t.transform.rotation.z << "]\n";
    }
    std::cout << ss.str() << std::endl;
}

void print_js(const JS &js) {
    std::stringstream ss;
    for (const auto &j : js.joint_states) {
        ss << j.name << "\n";
        ss << "  Position: " << j.position << "\n";
        ss << "  Velocity: " << j.velocity << "\n";
        ss << "  Effort:   " << j.effort << "\n";
    }
    std::cout << ss.str() << std::endl;
}

TEST(TreeTest, TestTFDefault) {
    Tree tree(Json::parse(ROBOT));

    TF tf = tree.TF();
    EXPECT_EQ(tf.transforms.size(), 3);

    print_tf(tf);

    for (const auto &t : tf.transforms) {
        if (t.header.frame_id == "world") {
            EXPECT_EQ(t.child_frame_id, "base");
            EXPECT_NEAR(t.transform.translation.x, 0, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.header.frame_id == "base") {
            EXPECT_EQ(t.child_frame_id, "upper_arm");
            EXPECT_NEAR(t.transform.translation.x, 2, TOL);
            EXPECT_NEAR(t.transform.translation.y, 0, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.header.frame_id == "upper_arm") {
            EXPECT_EQ(t.child_frame_id, "forearm");
            EXPECT_NEAR(t.transform.translation.x, 2, TOL);
            EXPECT_NEAR(t.transform.translation.y, 0, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else {
            FAIL();
        }
    }
}

TEST(TreeTest, TestJSDefault) {
    Tree tree(Json::parse(ROBOT));

    JS js = tree.JS();
    EXPECT_EQ(js.joint_states.size(), 2);

    print_js(js);

    for (const auto &j : js.joint_states) {
        if (j.name == "base_to_upper_arm") {
            EXPECT_NEAR(j.position, 0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else if (j.name == "upper_arm_to_forearm") {
            EXPECT_NEAR(j.position, 0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else {
            FAIL();
        }
    }
}

TEST(TreeTest, TestSetJointState) {
    Tree tree(Json::parse(ROBOT));

    JS js;
    js.joint_states.resize(2);
    js.joint_states[0].name = "base_to_upper_arm";
    js.joint_states[0].position = 1.0;
    js.joint_states[1].name = "upper_arm_to_forearm";
    js.joint_states[1].position = 2.0;

    tree.set_state(js);

    JS js2 = tree.JS();
    EXPECT_EQ(js2.joint_states.size(), 2);

    print_js(js2);

    for (const auto &j : js2.joint_states) {
        if (j.name == "base_to_upper_arm") {
            EXPECT_NEAR(j.position, 1.0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else if (j.name == "upper_arm_to_forearm") {
            EXPECT_NEAR(j.position, 2.0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else {
            FAIL();
        }
    }
}

TEST(TreeTest, TestTFWithSetJointState) {
    Tree tree(Json::parse(ROBOT));

    JS js;
    js.joint_states.resize(2);
    js.joint_states[0].name = "base_to_upper_arm";
    js.joint_states[0].position = 1.0;
    js.joint_states[1].name = "upper_arm_to_forearm";
    js.joint_states[1].position = 2.0;

    tree.set_state(js);

    JS js2 = tree.JS();
    EXPECT_EQ(js2.joint_states.size(), 2);

    print_js(js2);

    for (const auto &j : js2.joint_states) {
        if (j.name == "base_to_upper_arm") {
            EXPECT_NEAR(j.position, 1.0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else if (j.name == "upper_arm_to_forearm") {
            EXPECT_NEAR(j.position, 2.0, TOL);
            EXPECT_NEAR(j.velocity, 0, TOL);
            EXPECT_NEAR(j.effort, 0, TOL);
        } else {
            FAIL();
        }
    }

    TF tf = tree.TF();
    EXPECT_EQ(tf.transforms.size(), 3);

    print_tf(tf);

    for (const auto &t : tf.transforms) {
        if (t.header.frame_id == "world") {
            EXPECT_EQ(t.child_frame_id, "base");
            EXPECT_NEAR(t.transform.translation.x, 0, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.header.frame_id == "base") {
            EXPECT_EQ(t.child_frame_id, "upper_arm");
            EXPECT_NEAR(t.transform.translation.x, 1.5403, TOL);
            EXPECT_NEAR(t.transform.translation.y, 0, TOL);
            EXPECT_NEAR(t.transform.translation.z, -0.841471, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 0.877583, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0.479426, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.header.frame_id == "upper_arm") {
            EXPECT_EQ(t.child_frame_id, "forearm");
            EXPECT_NEAR(t.transform.translation.x, 0.583853, TOL);
            EXPECT_NEAR(t.transform.translation.y, 0.909297, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 0.540302, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0.0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0.841471, TOL);
        } else {
            FAIL();
        }
    }
}