#include <memory>
#include <vector>

#include "shapes.hpp"
#include "visitor.hpp"

int main() {
    // Make a vector of some shapes of different types.
    Point point {0, 0};
    auto circle1 = std::make_unique<Circle>(0.1, point);
    auto circle2 = std::make_unique<Circle>(0.2, point);
    auto square1 = std::make_unique<Square>(0.5, point);
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(circle1));
    shapes.push_back(std::move(circle2));
    shapes.push_back(std::move(square1));

    // Use the draw visitor on all the shapes.
    for (const auto &shape : shapes) {
        shape->accept(Draw{});
    }

    // Use the rotate visitor on all the shapes.
    for (const auto &shape : shapes) {
        shape->accept(Rotate{});
    }

    // TODO: Use the dilate visitor on all the shapes.
}