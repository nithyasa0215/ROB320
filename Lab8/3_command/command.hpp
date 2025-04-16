#ifndef LAB8_COMMAND_HPP
#define LAB8_COMMAND_HPP

#include <memory>
#include <iostream>

/// Interface for isolating a reversible draw operation. The names execute() and
/// undo() signal the classic Command design pattern.
template <typename T>
class DrawCommand {
   public:
    virtual ~DrawCommand() = default;
    virtual void execute(T const&) const = 0;
    virtual void undo(T const&) const = 0;
};

/// Interface for a shape forcing a draw() and remove() operation.
class Shape {
   public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual void remove() const = 0;
};

/// Circle type. Takes an implementation of the draw() and remove() operations.
class Circle : public Shape {
   public:
    Circle(double radius, std::unique_ptr<DrawCommand<Circle>> drawer) :
        radius_(radius), drawer_(std::move(drawer)) {}

    /// The draw operation simply executes the command.
    void draw() const override {
        drawer_->execute(*this);
    }

    /// The remove operation simply undoes the command.
    void remove() const override {
        drawer_->undo(*this);
    }

   private:
    double radius_;
    std::unique_ptr<DrawCommand<Circle>> drawer_;
};

/// Object implementing a drawing and undrawing operation on a circle.
class OpenGLCircleCommand : public DrawCommand<Circle> {
   public:
    void execute(Circle const& circle) const override {
        std::cout << "Pretend this implements drawing a circle with OpenGL." << std::endl;
    }

    void undo(Circle const& circle) const override {
        std::cout << "Pretend this implements removing a circle with OpenGL." << std::endl;
    }
};

// TODO: Implement a command called VulkanCircleCommand. It should output
//       "Pretend this implements drawing a circle with Vulkan."
//       during the execute operation. It should output
//       "Pretend this implements removing a circle with Vulkan."
//       during the undo operation.

#endif //LAB8_COMMAND_HPP
