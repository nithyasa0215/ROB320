#ifndef LAB8_EXTERNAL_POLYMORPHISM_HPP
#define LAB8_EXTERNAL_POLYMORPHISM_HPP

#include <iostream>
#include <memory>

/// Point in 2D space.
struct Point {
    double x, y;
};

/// Circle class separate from any inheritance hierarchy.
class Circle {
   public:
    Circle(double radius, const Point& center) : radius_(radius), center_(center) {}
    ~Circle() = default;
    double getRadius() const { return radius_; }
    Point getCenter() const { return center_; }

   private:
    double radius_;
    Point center_{};
};

/// Square class separate from any inheritance hierarchy.
class Square {
   public:
    Square(double side_length, const Point& center) : side_length_(side_length), center_(center) {}
    double getSideLength() const { return side_length_; }
    Point getCenter() const { return center_; }

   private:
    double side_length_;
    Point center_;
};

/// Function object for default behavior of a drawing operation. Calls free
/// function, relying on operator overloading.
struct DefaultDrawer {
    template <typename T>
    void operator()(T const& obj) const {
        draw(obj);
    }
};

/// External interface to the end user. Enforces required operations on the types.
class ShapeConcept {
   public:
    virtual ~ShapeConcept() = default;
    virtual void draw() const = 0;
};

/// Templated derived type which can model any type with any function object
/// implementing the required operation.
template <typename ShapeT, typename DrawerStrategy = DefaultDrawer>
class ShapeModel : public ShapeConcept {
   public:
    ShapeModel(ShapeT shape, DrawerStrategy drawer)
        : shape_(std::move(shape)), drawer_(std::move(drawer)) {}

    void draw() const override { drawer_(shape_); }

   private:
    ShapeT shape_;
    DrawerStrategy drawer_;
};

/// Function object carries actual implementation of required operations.
class OpenGLDrawStrategy {
   public:
    OpenGLDrawStrategy() = default;
    // Potential to add a constructor for data members here...

    void operator()(Circle const& circle) const {
        std::cout << "Pretend a circle got drawn with OpenGL." << std::endl;
    }

    void operator()(Square const& square) const {
        std::cout << "Pretend a square got drawn with OpenGL." << std::endl;
    }

    // Potential to add private data members here...
};

// TODO: Add a class called "Oval", no need for any members.

// TODO: Add a "VulkanDrawStrategy" which outputs
//       "Pretend a circle got drawn with Vulkan."
//       for Circles,
//       "Pretend a square got drawn with Vulkan."
//       for Squares, and
//       "Pretend an oval got drawn with Vulkan."
//       for Ovals.

#endif  // LAB8_EXTERNAL_POLYMORPHISM_HPP
