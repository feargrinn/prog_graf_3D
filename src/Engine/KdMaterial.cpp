#include "KdMaterial.h"


namespace xe {

    void KdMaterial::init() {
                create_material_uniform_buffer(2*sizeof(glm::vec4));
                create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                                {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
    }

    void KdMaterial::bind() const {
        
        glUseProgram(program());
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
        // printf("%f %f %f %f \n",Kd_[0],Kd_[1],Kd_[2],Kd_[3]);
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
    }

    void KdMaterial::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
    }
}