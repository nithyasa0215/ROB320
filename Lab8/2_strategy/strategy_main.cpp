#include <memory>

#include "strategy.hpp"

int main() {
    // Draw a circle using the OpenGL implementation.
    auto opengl_drawer = std::make_unique<OpenGLCircleStrategy>();
    Circle opengl_circle(0.25, std::move(opengl_drawer));
    opengl_circle.draw();

    // Draw a circle using the Vulkan implementation.
    auto vulkan_drawer = std::make_unique<VulkanCircleStrategy>();
    Circle vulkan_circle(0.25, std::move(vulkan_drawer));
    vulkan_circle.draw();

    // TODO: Draw a square using the DirectX implementation.

    // TODO: Draw a square using the DirectX implementation.
}