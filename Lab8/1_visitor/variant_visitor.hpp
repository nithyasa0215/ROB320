#ifndef LAB8_VARIANT_VISITOR_HPP
#define LAB8_VARIANT_VISITOR_HPP

#include <iostream>
#include <variant>

/// Point in 2D space.
struct Point {
    double x, y;
};

/// Circle type. Note that it is free from any inheritance hierarchy.
class Circle {
   public:
    Circle(double radius, const Point& center) : radius_(radius), center_(center) {}
    ~Circle() = default;
    double getRadius() const {
        return radius_;
    }
    Point getCenter() const {
        return center_;
    }
   private:
    double radius_;
    Point center_{};
};

/// Square type. Note that it is free from any inheritance hierarchy.
class Square {
   public:
    Square(double side_length, const Point& center) : side_length_(side_length), center_(center) {}
    double getSideLength() const {
        return side_length_;
    }
    Point getCenter() const {
        return center_;
    }
   private:
    double side_length_;
    Point center_;
};

/// Variant type representing either a Circle or a Square.
using Shape = std::variant<Circle, Square>;

/// Draw function object to be used on the Shape with std::visit(). Note that
/// std::visit() on a Shape will only compile if there is an operator() for each
/// type in the std::variant.
struct Draw {
    void operator()(Circle const& circle) const {
        std::cout << "Pretend the circle got drawn." << std::endl;
    }
    void operator()(Square const& square) const {
        std::cout << "Pretend the square got drawn." << std::endl;
    }
};

/// Rotate function object to be used on the Shape with std::visit(). Same note
/// as above.
struct Rotate {
    void operator()(Circle const& circle) const {
        std::cout << "Pretend the circle got rotated." << std::endl;
    }
    void operator()(Square const& square) const {
        std::cout << "Pretend the square got rotated." << std::endl;
    }
};

// TODO: Create a dilate struct to add a new operation to all of the shapes.
//       Since this is a toy example, just make it output
//       "Pretend the circle got dilated." or
//       "Pretend the square got dilated."
//       depending on the accepting object type.

#endif  // LAB8_VARIANT_VISITOR_HPP
