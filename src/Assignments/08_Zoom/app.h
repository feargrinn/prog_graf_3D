//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "glad/gl.h"

#include "Application/application.h"

#include "glm/glm.hpp"


class SimpleShapeApplication : public xe::Application {
public:
     SimpleShapeApplication(int width, int height, std::string title, bool debug) :
     Application(width, height, title, debug), camera_(nullptr) {}

    void init() override;

    void frame() override;
    
    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    Camera *camera() const {
        assert(camera_);
        return camera_;
    }

private:
    GLuint vao_;

    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 P_;
    glm::mat4 V_;
    glm::mat4 M_;

    GLuint u_trans_buffer_handle_;

    Camera *camera_;
};