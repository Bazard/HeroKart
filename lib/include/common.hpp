#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace glimac {

static const float PI = 3.14159265358979323846;

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

}
