#include "app.h"
#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/KdMaterial.h"
#include "stb/stb_image.h"
#include "Engine/texture.h"
#include "Engine/BlinnPhongMaterial.h"

void SimpleShapeApplication::init() {

    xe::KdMaterial::init();
    xe::BlinnPhongMaterial::init();

    // A vector containing the x,y,z vertex coordinates and rgb color values for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, -0.5f,  0.0f,  0.1910f,  0.5000f, // 0
            -0.5f,  0.5f,  0.0f,  0.5000f,  0.1910f, // 1
             0.5f,  0.5f,  0.0f,  0.8090f,  0.5000f, // 2
             0.5f, -0.5f,  0.0f,  0.5000f,  0.8090f, // 3

             0.0f,  0.0f,  1.0f,  0.0000f,  1.0000f, // 4
             0.0f,  0.0f,  1.0f,  0.0000f,  0.0000f, // 5
             0.0f,  0.0f,  1.0f,  1.0000f,  0.0000f, // 6
             0.0f,  0.0f,  1.0f,  1.0000f,  1.0000f  // 7
            };

    // index vector
    std::vector<GLubyte> indices = {
        0,1,2,
        0,2,3,

        3,2,7,
        1,0,5,
        0,3,4,
        2,1,6
        };

    auto square = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(square);

    // generating buffer for transformations
    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 48 * sizeof(float), nullptr, GL_STATIC_DRAW));

    // gen buffer for lights
    OGL_CALL(glGenBuffers(2, &u_light_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(int) + xe::MAX_POINT_LIGHTS * sizeof(xe::PointLight), nullptr, GL_STATIC_DRAW));

    // This sets up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    // camera and PVM
    set_camera(new xe::Camera);

    M_ = glm::mat4(1.0);
    camera()->look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera()->perspective(glm::radians(45.0f), float(w)/h, 0.1f, 20.0f);

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    OGL_CALL(glDisable(GL_CULL_FACE));
    OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    OGL_CALL(glEnable(GL_DEPTH_TEST));

    // CameraMovement
    set_controler(new xe::CameraController(camera()));

    xe::PointLight white_light(glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 0.1);
    add_light(white_light);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;
    glm::mat4 VM = camera()->view() * M_;

    auto R = glm::mat3(VM);
    auto VM_Normal = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    glm::vec3 ambient = glm::vec3(0.25, 0.25, 0.25);
    int n_lights = lights_.size();

    // binding buffer for transformations
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), &VM[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 32 * sizeof(float), 4 * sizeof(float), &VM_Normal[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 36 * sizeof(float), 4 * sizeof(float), &VM_Normal[1]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 40 * sizeof(float), 4 * sizeof(float), &VM_Normal[2]));

    // loading light buffer
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &ambient));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(int), &n_lights));
    for (int i = 0; i < n_lights; i++) {
        xe::PointLight transformed_light = xe::transform(lights_[i], VM);
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(int) + i * sizeof(xe::PointLight), sizeof(xe::PointLight), &transformed_light));
    }
    
    for (auto m: meshes_)
        m->draw();

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, 0));
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    OGL_CALL(glViewport(0, 0, w, h));
    camera()->set_aspect(float(w)/h);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);   
    camera()->zoom(yoffset / 20.0f);
}
