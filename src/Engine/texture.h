#pragma once

#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include "glm/glm.hpp"

namespace xe {
    GLuint create_texture(const std::string &name, bool is_sRGB = true);
}