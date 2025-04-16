#include <vector>

#include "variant_visitor.hpp"

int main() {
    // Make a vector of some shapes of different types.
    Point point {0, 0};
    Circle circle1(0.1, point);
    Circle circle2(0.2, point);
    Square square1(0.5, point);
    std::vector<Shape> shapes;
    shapes.emplace_back(circle1);
    shapes.emplace_back(circle2);
    shapes.emplace_back(square1);

    // Use the draw function object on all the shapes.
    for (const auto &shape : shapes) {
        std::visit(Draw{}, shape);
    }

    // Use the rotate function object on all the shapes.
    for (const auto &shape : shapes) {
        std::visit(Rotate{}, shape);
    }

    // TODO: Use the dilate function object on all the shapes.
}
