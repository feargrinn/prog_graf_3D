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

void SimpleShapeApplication::init() {

    xe::KdMaterial::init();

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

    // auto pyramid = xe::load_mesh_from_obj("Models/pyramid.obj", "Models");
    // auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/pyramid.obj", std::string(ROOT_DIR) + "/Models");
    // add_mesh(pyramid);
    auto globe = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(globe);

    // generating buffer for transformations
    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW));

    // This sets up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    // camera and PVM
    set_camera(new xe::Camera);

    M_ = glm::mat4(1.0);
    camera()->look_at(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera()->perspective(glm::radians(45.0f), float(w)/h, 0.1f, 20.0f);

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    // OGL_CALL(glUseProgram(program));

    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_ALWAYS); 

    // CameraMovement
    set_controler(new xe::CameraController(camera()));
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;

    // binding buffer for transformations
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM[0]));

    for (auto m: meshes_)
        m->draw();

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
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
