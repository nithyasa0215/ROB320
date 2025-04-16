#ifndef LAB8_VISITOR_HPP
#define LAB8_VISITOR_HPP

#include <iostream>

/// Forward declarations necessary to define visitors.
class Circle;
class Square;

/// Base visitor type to enforce a visit() function for every type.
class ShapeVisitor {
   public:
    virtual ~ShapeVisitor() = default;

    /// Visit() function for first fixed type.
    virtual void visit(Circle const&) const = 0;

    /// Visit() function for second fixed type.
    virtual void visit(Square const&) const = 0;
};

/// Rotate visitor to represent a rotation operation on shapes.
class Rotate : public ShapeVisitor {
   public:

    /// Visit() function, which would implement the rotation operation for Circles.
    void visit(Circle const& circle) const override {
        std::cout << "Pretend the circle got rotated." << std::endl;
    }

    /// Visit() function, which would implement the rotation operation for Squares.
    void visit(Square const& square) const override {
        std::cout << "Pretend the square got rotated." << std::endl;
    }
};

/// Draw visitor to represent a drawing operation on shapes.
class Draw : public ShapeVisitor {
   public:

    /// Visit() function, which would implement the drawing operation for Circles.
    void visit(Circle const& circle) const override {
        std::cout << "Pretend the circle got drawn." << std::endl;
    }

    /// Visit() function, which would implement the drawing operation for Squares.
    void visit(Square const& square) const override {
        std::cout << "Pretend the square got drawn." << std::endl;
    }
};

// TODO: Create a dilate visitor to add a new operation to all of the shapes.
//       Since this is a toy example, just make it output
//       "Pretend the circle got dilated." or
//       "Pretend the square got dilated."
//       depending on the accepting object type.

#endif  // LAB8_VISITOR_HPP
