#include "command.hpp"

int main() {
    // Draw and remove a circle with OpenGL.
    Circle opengl_circle(0.25, std::make_unique<OpenGLCircleCommand>());
    opengl_circle.draw();
    opengl_circle.remove();

    // TODO: Draw and remove a circle with Vulkan.
}