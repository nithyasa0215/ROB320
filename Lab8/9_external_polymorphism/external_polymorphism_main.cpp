#include "external_polymorphism.hpp"

#include <vector>

int main() {
    using CircleModel = ShapeModel<Circle,OpenGLDrawStrategy>;
    using SquareModel = ShapeModel<Square,OpenGLDrawStrategy>;

    // TODO: Write "using" statements for a Square with the VulkanDrawStrategy
    //       and an Oval with the VulkanDrawStrategy.

    std::vector<std::unique_ptr<ShapeConcept>> shapes;
    shapes.emplace_back(std::make_unique<CircleModel>(
        Circle{0.25, {0, 0}}, OpenGLDrawStrategy{}));
    shapes.emplace_back(std::make_unique<SquareModel>(
        Square{0.5, {1, 1}}, OpenGLDrawStrategy{}));
    shapes.emplace_back(std::make_unique<CircleModel>(
        Circle{0.35, {1, 0}}, OpenGLDrawStrategy{}));

    // TODO: Add an instance of both new models to the vector, square first.

    // Notice how easy it is to change what this simple code does. Loose coupling!
    for (const auto& shape : shapes) {
        shape->draw();
    }
}
