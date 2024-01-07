#include "KdMaterial.h"


namespace xe {

    GLint KdMaterial::map_Kd_location_; 

    void KdMaterial::init() {
        create_material_uniform_buffer(2*sizeof(glm::vec4));
        create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                                  {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});

        map_Kd_location_ = glGetUniformLocation(program(),"map_Kd");
        if (map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
        }
    }

    void KdMaterial::bind() const {
        int use_map_kd_;
        glUseProgram(program());
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
        if (texture_ > 0) {
            use_map_kd_ = 1;
            OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_kd_));
            OGL_CALL(glActiveTexture(GL_TEXTURE0));
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
        } else if (texture_ == 0) {
            use_map_kd_ = 0;
            OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_kd_));
        }
    }

    void KdMaterial::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
        if (texture_ > 0) {
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }
}