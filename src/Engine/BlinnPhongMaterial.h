#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "AbstractMaterial.h"
#include "ObjectReader/sMesh.h"
#include "texture.h"
#include "utils.h"
#include "mesh_loader.h"


namespace xe {

    class BlinnPhongMaterial : public AbstractMaterial<BlinnPhongMaterial> {
    public:
        BlinnPhongMaterial(const glm::vec4 &Kd) : Kd_(Kd), texture_(0) {}
        BlinnPhongMaterial(const glm::vec4 &Kd, int use_vertex_colors) : Kd_(Kd), use_vertex_colors_(use_vertex_colors), texture_(0) {}
        BlinnPhongMaterial(const glm::vec4 &Kd,  int use_vertex_colors, GLuint &texture) : Kd_(Kd), use_vertex_colors_(use_vertex_colors), texture_(texture) {}
        static void init();
        void bind() const override;
        void unbind() const override;
        static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);
        void set_texture(GLuint &texture);

    private:
        // color of material
        const glm::vec4 Kd_;
        int use_vertex_colors_ = false;
        static GLint map_Kd_location_;
        GLuint texture_;
        static glm::vec4 Ka_;
        static glm::vec4 Ks_;
        static float Ns_;
    };
 }
