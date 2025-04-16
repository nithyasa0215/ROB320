#ifndef LAB8_SHAPES_HPP
#define LAB8_SHAPES_HPP

#include "visitor.hpp"

/// Point in 2D space.
struct Point {
    double x, y;
};

/// Shape base class forcing derived classes to have an accept() function.
class Shape {
   public:
    /// Accept() function taking a base visitor type.
    virtual void accept(ShapeVisitor const& visitor) = 0;
};

/// Derived class instantiating accept().
class Circle : public Shape {
   public:
    Circle(double radius, const Point& center) : radius_(radius), center_(center) {}
    ~Circle() = default;

    /// Instantiation of accept() function to allow extendable operations.
    void accept(ShapeVisitor const& visitor) override {
        visitor.visit(*this);
    }

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

/// Another derived class instantiating accept().
class Square : public Shape {
   public:
    Square(double side_length, const Point& center) : side_length_(side_length), center_(center) {}

    /// Instantiation of accept() function to allow extendable operations.
    void accept(ShapeVisitor const& visitor) override {
        visitor.visit(*this);
    }

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

#endif  // LAB8_SHAPES_HPP
