#ifndef LAB8_STRATEGY_HPP
#define LAB8_STRATEGY_HPP

#include <memory>
#include <iostream>

/// Interface for an object representing a draw operation on an arbitrary type.
template <typename T>
class DrawStrategy {
   public:
    virtual ~DrawStrategy() = default;
    virtual void draw(T const&) const = 0;
};

/// Interface for a shape forcing a draw() operation.
class Shape {
   public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

/// Circle type. Takes an implementation of draw() on construction.
class Circle : public Shape {
   public:

    /// Constructor stores pointer to drawing object.
    Circle(double radius, std::unique_ptr<DrawStrategy<Circle>> drawer) :
        radius_(radius), drawer_(std::move(drawer)) {}

    /// Draw operation simply calls implementation of drawing object.
    void draw() const override {
        drawer_->draw(*this);
    }

   private:
    double radius_;
    std::unique_ptr<DrawStrategy<Circle>> drawer_;
};

/// Square type. Takes an implementation of draw() on construction.
class Square : public Shape {
   public:
    Square(double side_length, std::unique_ptr<DrawStrategy<Square>> drawer) :
        side_length_(side_length), drawer_(std::move(drawer)) {}

    /// Draw operation simply calls implementation of drawing object. Note that
    /// this can't be defined in the base class since we rely on the dynamic
    /// type of the "this" pointer.
    void draw() const override {
        drawer_->draw(*this);
    }

   private:
    double side_length_;
    std::unique_ptr<DrawStrategy<Square>> drawer_;
};

/// Instance of drawing object for circles providing a specific implementation.
class OpenGLCircleStrategy : public DrawStrategy<Circle> {
   public:
    void draw(Circle const& circle) const override {
        std::cout << "Pretend this implements drawing a circle with OpenGL." << std::endl;
    }
};

/// Another instance of a drawing object for circles providing a specific implementation.
class VulkanCircleStrategy : public DrawStrategy<Circle> {
   public:
    void draw(Circle const& circle) const override {
        std::cout << "Pretend this implements drawing a circle with Vulkan." << std::endl;
    }
};

// TODO: Implement a draw strategy for circles called DirectXCircleStrategy.
//       The implementation should simply output:
//       "Pretend this implements drawing a circle with DirectX."

// TODO: Implement a draw strategy for squares called DirectXSquareStrategy.
//       The implementation should simply output:
//       "Pretend this implements drawing a square with DirectX."

#endif  // LAB8_STRATEGY_HPP
