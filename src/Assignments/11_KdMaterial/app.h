//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include "glm/glm.hpp"
#include "Engine/camera.h"
#include "Engine/camera_controller.h"
#include "Engine/Mesh.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) :
    Application(width, height, title, debug), camera_(nullptr) {}

    void init() override;

    void frame() override;
    
    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(xe::Camera *camera) { camera_ = camera; }

    void scroll_callback(double xoffset, double yoffset) override;

    xe::Camera *camera() const {
        assert(camera_);
        return camera_;
    }

    void set_controler(xe::CameraController *controller) { controller_ = controller; }

    void mouse_button_callback(int button, int action, int mods) {
        Application::mouse_button_callback(button, action, mods);

        if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released(x, y);
        }
    }

    void cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controller_) {
            controller_->mouse_moved(x, y);
        }
    }

    void add_mesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }

private:
    GLuint vao_;

    glm::mat4 M_;

    GLuint u_trans_buffer_handle_;

    xe::Camera *camera_;

    xe::CameraController *controller_;

    std::vector<xe::Mesh*> meshes_; 
};