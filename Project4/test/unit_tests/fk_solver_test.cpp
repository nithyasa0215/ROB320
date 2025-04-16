#include "rix/rdf/tree.hpp"
#include "rix/rdf/fk_solver.hpp"

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
using FKSolver = rix::rdf::FKSolver;

void print_tf(const TF &tf) {
    std::stringstream ss;
    for (const auto &t : tf.transforms) {
        ss << t.header.frame_id << " -> " << t.child_frame_id << "\n";
        ss << "  Translation: [x: " << t.transform.translation.x << ", y: " << t.transform.translation.y << ", z: " << t.transform.translation.z << "]\n";
        ss << "  Rotation:    [w: " << t.transform.rotation.w << ", x: " << t.transform.rotation.x << ", y: " << t.transform.rotation.y << ", z: " << t.transform.rotation.z << "]\n";
    }
    std::cout << ss.str() << std::endl;
}

TEST(Tree, SolveDefault) {
    Tree tree(Json::parse(ROBOT));
    FKSolver solver(tree);

    Eigen::Affine3d t = solver.solve("base");
    std::cout << "Link: base" << std::endl;
    std::cout << t.matrix() << std::endl;
    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;

    t = solver.solve("upper_arm");
    std::cout << "\nLink: upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("forearm");
    std::cout << "\nLink: forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 4,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
                         
    EXPECT_TRUE(t.isApprox(expected, TOL));
}

TEST(Tree, SolveReferenceDefault) {
    Tree tree(Json::parse(ROBOT));
    FKSolver solver(tree);

    Eigen::Affine3d t = solver.solve("base", "base");
    std::cout << "base -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("base", "upper_arm");
    std::cout << "\nupper_arm -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -2,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("base", "forearm");
    std::cout << "\nforearm -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -4,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("upper_arm", "base");
    std::cout << "\nbase -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("upper_arm", "upper_arm");
    std::cout << "\nupper_arm -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("upper_arm", "forearm");
    std::cout << "\nforearm -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -2,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("forearm", "base");
    std::cout << "\nbase -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 4,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("forearm", "upper_arm");
    std::cout << "\nupper_arm -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = solver.solve("forearm", "forearm");
    std::cout << "\nforearm -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));
}

TEST(Tree, SolveFromTF) {
    TF tf;
    tf.transforms.resize(3);
    tf.transforms[0].header.frame_id = "world";
    tf.transforms[0].child_frame_id = "base";
    tf.transforms[0].transform.translation.x = 0;
    tf.transforms[0].transform.translation.y = 1;
    tf.transforms[0].transform.translation.z = 0;
    tf.transforms[0].transform.rotation.w = 1;
    tf.transforms[0].transform.rotation.x = 0;
    tf.transforms[0].transform.rotation.y = 0;
    tf.transforms[0].transform.rotation.z = 0;
    tf.transforms[1].header.frame_id = "base";
    tf.transforms[1].child_frame_id = "upper_arm";
    tf.transforms[1].transform.translation.x = 2;
    tf.transforms[1].transform.translation.y = 0;
    tf.transforms[1].transform.translation.z = 0;
    tf.transforms[1].transform.rotation.w = 1;
    tf.transforms[1].transform.rotation.x = 0;
    tf.transforms[1].transform.rotation.y = 0;
    tf.transforms[1].transform.rotation.z = 0;
    tf.transforms[2].header.frame_id = "upper_arm";
    tf.transforms[2].child_frame_id = "forearm";
    tf.transforms[2].transform.translation.x = 2;
    tf.transforms[2].transform.translation.y = 0;
    tf.transforms[2].transform.translation.z = 0;
    tf.transforms[2].transform.rotation.w = 1;
    tf.transforms[2].transform.rotation.x = 0;
    tf.transforms[2].transform.rotation.y = 0;
    tf.transforms[2].transform.rotation.z = 0;

    Eigen::Affine3d t = FKSolver::solve(tf, "base");
    std::cout << "Link: base" << std::endl;
    std::cout << t.matrix() << std::endl;
    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;

    t = FKSolver::solve(tf, "upper_arm");
    std::cout << "\nLink: upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "forearm");
    std::cout << "\nLink: forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 4,
                         0, 1, 0, 1,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
                         
    EXPECT_TRUE(t.isApprox(expected, TOL));
}

TEST(Tree, SolveRefereneceFromTF) {
    TF tf;
    tf.transforms.resize(3);
    tf.transforms[0].header.frame_id = "world";
    tf.transforms[0].child_frame_id = "base";
    tf.transforms[0].transform.translation.x = 0;
    tf.transforms[0].transform.translation.y = 1;
    tf.transforms[0].transform.translation.z = 0;
    tf.transforms[0].transform.rotation.w = 1;
    tf.transforms[0].transform.rotation.x = 0;
    tf.transforms[0].transform.rotation.y = 0;
    tf.transforms[0].transform.rotation.z = 0;
    tf.transforms[1].header.frame_id = "base";
    tf.transforms[1].child_frame_id = "upper_arm";
    tf.transforms[1].transform.translation.x = 2;
    tf.transforms[1].transform.translation.y = 0;
    tf.transforms[1].transform.translation.z = 0;
    tf.transforms[1].transform.rotation.w = 1;
    tf.transforms[1].transform.rotation.x = 0;
    tf.transforms[1].transform.rotation.y = 0;
    tf.transforms[1].transform.rotation.z = 0;
    tf.transforms[2].header.frame_id = "upper_arm";
    tf.transforms[2].child_frame_id = "forearm";
    tf.transforms[2].transform.translation.x = 2;
    tf.transforms[2].transform.translation.y = 0;
    tf.transforms[2].transform.translation.z = 0;
    tf.transforms[2].transform.rotation.w = 1;
    tf.transforms[2].transform.rotation.x = 0;
    tf.transforms[2].transform.rotation.y = 0;
    tf.transforms[2].transform.rotation.z = 0;
    
    Eigen::Affine3d t = FKSolver::solve(tf, "base", "base");
    std::cout << "base -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    Eigen::Affine3d expected;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "base", "upper_arm");
    std::cout << "\nupper_arm -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -2,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "base", "forearm");
    std::cout << "\nforearm -> base" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -4,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "upper_arm", "base");
    std::cout << "\nbase -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "upper_arm", "upper_arm");
    std::cout << "\nupper_arm -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "upper_arm", "forearm");
    std::cout << "\nforearm -> upper_arm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, -2,
                         0, 1, 0,  0,
                         0, 0, 1,  0,
                         0, 0, 0,  1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "forearm", "base");
    std::cout << "\nbase -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 4,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "forearm", "upper_arm");
    std::cout << "\nupper_arm -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 2,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));

    t = FKSolver::solve(tf, "forearm", "forearm");
    std::cout << "\nforearm -> forearm" << std::endl;
    std::cout << t.matrix() << std::endl;
    expected.matrix() << 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1;
    EXPECT_TRUE(t.isApprox(expected, TOL));
}

TEST(Tree, GlobalTFDefault) {
    Tree tree(Json::parse(ROBOT));
    FKSolver solver(tree);

    TF global_tf = solver.global_tf();
    EXPECT_EQ(global_tf.transforms.size(), 3);
    
    print_tf(global_tf);

    for (const auto &t : global_tf.transforms) {
        if (t.child_frame_id == "base") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 0, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.child_frame_id == "upper_arm") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 2, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.child_frame_id == "forearm") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 4, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        }
    }
}

TEST(Tree, GlobalTFFromTF) {
    TF tf;
    tf.transforms.resize(3);
    tf.transforms[0].header.frame_id = "world";
    tf.transforms[0].child_frame_id = "base";
    tf.transforms[0].transform.translation.x = 0;
    tf.transforms[0].transform.translation.y = 1;
    tf.transforms[0].transform.translation.z = 0;
    tf.transforms[0].transform.rotation.w = 1;
    tf.transforms[0].transform.rotation.x = 0;
    tf.transforms[0].transform.rotation.y = 0;
    tf.transforms[0].transform.rotation.z = 0;
    tf.transforms[1].header.frame_id = "base";
    tf.transforms[1].child_frame_id = "upper_arm";
    tf.transforms[1].transform.translation.x = 2;
    tf.transforms[1].transform.translation.y = 0;
    tf.transforms[1].transform.translation.z = 0;
    tf.transforms[1].transform.rotation.w = 1;
    tf.transforms[1].transform.rotation.x = 0;
    tf.transforms[1].transform.rotation.y = 0;
    tf.transforms[1].transform.rotation.z = 0;
    tf.transforms[2].header.frame_id = "upper_arm";
    tf.transforms[2].child_frame_id = "forearm";
    tf.transforms[2].transform.translation.x = 2;
    tf.transforms[2].transform.translation.y = 0;
    tf.transforms[2].transform.translation.z = 0;
    tf.transforms[2].transform.rotation.w = 1;
    tf.transforms[2].transform.rotation.x = 0;
    tf.transforms[2].transform.rotation.y = 0;
    tf.transforms[2].transform.rotation.z = 0;

    TF global_tf = FKSolver::global_tf(tf);
    EXPECT_EQ(global_tf.transforms.size(), 3);

    print_tf(global_tf);

    for (const auto &t : global_tf.transforms) {
        if (t.child_frame_id == "base") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 0, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.child_frame_id == "upper_arm") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 2, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        } else if (t.child_frame_id == "forearm") {
            EXPECT_EQ(t.header.frame_id, "world");
            EXPECT_NEAR(t.transform.translation.x, 4, TOL);
            EXPECT_NEAR(t.transform.translation.y, 1, TOL);
            EXPECT_NEAR(t.transform.translation.z, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.w, 1, TOL);
            EXPECT_NEAR(t.transform.rotation.x, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.y, 0, TOL);
            EXPECT_NEAR(t.transform.rotation.z, 0, TOL);
        }
    }
}