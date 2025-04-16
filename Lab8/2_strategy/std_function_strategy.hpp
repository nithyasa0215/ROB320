#ifndef LAB8_STD_FUNCTION_STRATEGY_HPP
#define LAB8_STD_FUNCTION_STRATEGY_HPP

#include <functional>
#include <iostream>

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
    Circle(double radius, std::function<void(Circle const&)> drawer) :
      radius_(radius), drawer_(std::move(drawer)) {}

    void draw() const override {
        drawer_(*this);
    }

   private:
    double radius_;
    std::function<void(Circle const&)> drawer_;
};

/// Square type. Takes an implementation of draw() on construction.
class Square : public Shape {
   public:
    Square(double side_length, std::function<void(Square const&)> drawer) :
       side_length_(side_length), drawer_(std::move(drawer)) {}

    void draw() const override {
        drawer_(*this);
    }

   private:
    double side_length_;
    std::function<void(Square const&)> drawer_;
};

/// Free function (which can be represented by std::function) for providing
/// a specific implementation of the draw operation for circles.
void openGLDrawCircleStrategy(Circle const&) {
    std::cout << "Pretend this implements drawing a circle with OpenGL." << std::endl;
}

/// Free function (which can be represented by std::function) for providing
/// another specific implementation of the draw operation for circles.
void vulkanDrawCircleStrategy(Circle const&) {
    std::cout << "Pretend this implements drawing a circle with Vulkan." << std::endl;
}

// TODO: Implement a draw strategy for circles called directXDrawCircleStrategy.
//       The implementation should simply output:
//       "Pretend this implements drawing a circle with DirectX."

// TODO: Implement a draw strategy for circles called directXDrawSquareStrategy.
//       The implementation should simply output:
//       "Pretend this implements drawing a circle with DirectX."

#endif  // LAB8_STD_FUNCTION_STRATEGY_HPP
