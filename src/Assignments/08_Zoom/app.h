//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "glad/gl.h"

#include "Application/application.h"

#include "glm/glm.hpp"

#include "camera.h"

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

    glm::mat4 M_;

    GLuint u_trans_buffer_handle_;

    Camera *camera_;
};