#include "app.h"
#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

void SimpleShapeApplication::init() {
    
    /*
     * A utility function that reads the shaders' source files, compiles them and creates the program object,
     * as everything in OpenGL we reference the program by an integer "handle".
     * The input to this function is a map that associates a shader type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER) with
     * its source file. The source files are located in the PROJECT_DIR/shaders directory, where  PROJECT_DIR is the
     * current assignment directory (e.g., src/Assignments/Triangle).
     */
    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });

    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates and rgb color values for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // 0
            -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // 1
            0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // 2
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // 3

            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 4
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 5
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 6

            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 7
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 8
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 9

            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // 10
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // 11
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 12

            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 13
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 14
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 15
            };

    // index vector
    std::vector<GLubyte> indices = {
        0,1,2,
        0,2,3,
        4,5,6,
        7,8,9,
        10,11,12,
        13,14,15
        };

    // mesh for pyramid
    auto pyramid = new xe::Mesh(6 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                             indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);

    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0);
    pyramid->add_attribute(xe::AttributeType::COLOR_0, 3, GL_FLOAT, 3 * sizeof(GLfloat));

    pyramid->load_indices(0, indices.size() * sizeof(GLbyte), indices.data());

    pyramid->add_primitive(0, 18);
    add_mesh(pyramid);

    /*
     * All the calls to the OpenGL API are "encapsulated" in the OGL_CALL macro for debugging purposes as explained in
     * Assignments/DEBUGGING.md. The macro is defined in src/Application/utils.h. If the call to the OpenGL API returns an
     * error code, the macro will print the name of the function that failed, the file and line number where the error
     * occurred.
     */

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
    // I suggest not using white or black for better debugging.
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    OGL_CALL(glUseProgram(program));

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
