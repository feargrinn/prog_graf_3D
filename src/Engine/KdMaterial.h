#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "AbstractMaterial.h"


namespace xe {

    class KdMaterial : public AbstractMaterial<KdMaterial> {
    public:
        KdMaterial(const glm::vec4 &Kd) : Kd_(Kd) {}
        // static void init() {
        //     create_material_uniform_buffer(2*sizeof(glm::vec4));
        //     create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
        //                     {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
        // }
        static void init();
        void bind() const override;
        void unbind() const override;

    private:
        // color of material
        const glm::vec4 Kd_;
        int use_vertex_colors_ = false;
    };

 }

