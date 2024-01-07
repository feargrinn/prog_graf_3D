#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "AbstractMaterial.h"


namespace xe {

    class KdMaterial : public AbstractMaterial<KdMaterial> {
    public:
        KdMaterial(const glm::vec4 &Kd) : Kd_(Kd), texture_(0) {}
        KdMaterial(const glm::vec4 &Kd,  int use_vertex_colors, GLuint &texture) : Kd_(Kd), use_vertex_colors_(use_vertex_colors), texture_(texture) {}
        static void init();
        void bind() const override;
        void unbind() const override;

    private:
        // color of material
        const glm::vec4 Kd_;
        int use_vertex_colors_ = false;
        static GLint map_Kd_location_;
        GLuint texture_;
    };
 }
