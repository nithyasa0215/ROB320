#include "type_erasure.hpp"

#include <vector>

int main() {
    // Make a Shape representing a circle.
    Circle circle{2.5, {0, 0}};
    OpenGLDrawStrategy drawer;
    Shape circle_shape1(circle, drawer);
    draw(circle_shape1);
    Shape circle_shape2(circle_shape1);
    draw(circle_shape2);

    // Make Shape representing a square.
    Square square{4, {1, -1}};
    Shape square_shape1(square, drawer);

    // TODO: Make a Shape representing a square drawn with Vulkan.

    // TODO: Make a Shape representing an oval drawn with Vulkan.

    // Make a container of shapes to demonstrate polymorphic usage.
    std::vector<Shape> shapes;
    shapes.reserve(3);
    shapes.emplace_back(circle_shape1);
    shapes.emplace_back(circle_shape2);
    shapes.emplace_back(square_shape1);

    // TODO: Add the square, then the oval to the shapes vector.

    // Notice how simple this is.
    for (const auto& shape : shapes) {
        draw(shape);
    }
}