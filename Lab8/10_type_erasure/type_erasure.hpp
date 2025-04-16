#ifndef LAB8_TYPE_ERASURE_HPP
#define LAB8_TYPE_ERASURE_HPP

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

/// Namespace to hide implementation of Type Erasure. Never call into a detail::
/// namespace as an outside user!
namespace detail {

/// Same concept class from External Polymorphism, now with clone().
class ShapeConcept {
   public:
    virtual ~ShapeConcept() = default;
    virtual void draw() const = 0;
    virtual std::unique_ptr<ShapeConcept> clone() const = 0;
};

/// Same model class template from External Polymorphism, now with clone().
template <typename ShapeT, typename DrawStrategy>
class OwningShapeModel : public ShapeConcept {
   public:
    OwningShapeModel(ShapeT shape, DrawStrategy drawer)
        : shape_{std::move(shape)}, drawer_{std::move(drawer)} {}

    void draw() const override { drawer_(shape_); }

    std::unique_ptr<ShapeConcept> clone() const override {
        return std::make_unique<OwningShapeModel>(*this);
    }

   private:
    ShapeT shape_;
    DrawStrategy drawer_;
};

}  // namespace detail

/// Type erased shape, hiding the annoying pointer usage from the user.
class Shape {
   public:
    /// Constructor instantiates pimpl_ via ShapeModel, all other functions
    /// interact with the ShapeConcept interface.
    template <typename ShapeT, typename DrawStrategy>
    Shape(ShapeT shape, DrawStrategy drawer) {
        using Model = detail::OwningShapeModel<ShapeT,DrawStrategy>;
        pimpl_ = std::make_unique<Model>(std::move(shape),
                                         std::move(drawer));
    }

    Shape(Shape const& other)
       : pimpl_(other.pimpl_->clone()) {}

    Shape& operator=(Shape const& other) {
        Shape copy(other);
        pimpl_.swap(copy.pimpl_);
        return *this;
    }

    ~Shape() = default;
    Shape(Shape&&) = default;
    Shape& operator=(Shape&&) = default;

   private:
    std::unique_ptr<detail::ShapeConcept> pimpl_;

    /// Private friends are callable publicly, but can access private members.
    friend void draw(Shape const& shape) {
        shape.pimpl_->draw();
    }
};

/// Same function object as External Polymorphism for actual implementation.
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

// TODO: Add a class called "Oval", no need for any members. Hint: Same as in
//       External Polymorphism.

// TODO: Add a "VulkanDrawStrategy" which outputs
//       "Pretend a circle got drawn with Vulkan."
//       for Circles,
//       "Pretend a square got drawn with Vulkan."
//       for Squares, and
//       "Pretend an oval got drawn with Vulkan."
//       for Ovals. Hint: Same as in External Polymorphism.

#endif  // LAB8_TYPE_ERASURE_HPP
