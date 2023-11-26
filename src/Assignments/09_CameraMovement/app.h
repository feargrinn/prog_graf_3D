//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "camera_controller.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) :
    Application(width, height, title, debug), camera_(nullptr) {}

    void init() override;

    void frame() override;
    
    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    void scroll_callback(double xoffset, double yoffset) override;

    Camera *camera() const {
        assert(camera_);
        return camera_;
    }

    // CameraMovement
    void set_controler(CameraController *controller) { controller_ = controller; }

    // CameraMovement
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

private:
    GLuint vao_;

    glm::mat4 M_;

    GLuint u_trans_buffer_handle_;

    Camera *camera_;

    // CameraMovement
    CameraController *controller_;
};