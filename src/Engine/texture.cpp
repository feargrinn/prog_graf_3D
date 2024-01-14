#include "texture.h"

#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb/stb_image.h"

namespace xe {
    
    GLuint create_texture(const std::string &name, bool is_sRGB) {

        stbi_set_flip_vertically_on_load(true);
        GLint width, height, channels;
        auto img = stbi_load(name.c_str(), &width, &height, &channels, 0);
        if (!img) {
            std::cerr<<"Could not read image from file `"<<name<<"'\n";
        } else {
            std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
        }

        // creating texture, binding it and loading image, then setting interpolation
        GLuint tex_handle;
        OGL_CALL(glGenTextures(1, &tex_handle));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, tex_handle));
        OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

        if (is_sRGB) {
            OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
        } else {
            OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
        }

        return tex_handle;
    }
}