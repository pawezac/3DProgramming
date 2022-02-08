//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "camera.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void set_camera(Camera* camera) { _camera = camera; }

    Camera* camera() const { return _camera; }

private:
    GLuint vao_;
    std::vector<GLushort> indices_vector;
    glm::mat4 modelMat;
    Camera* _camera;

    GLuint transformationBuffer;

    static void setColorToVertices();

    static void setVerticesData();

    static unsigned int generateIndicesBuffer(std::vector<GLushort> vertices);

    static unsigned int generateVertexBuffer(std::vector<GLfloat> vertices);

    static void setDataToVertexBuffer(GLuint buffer);

    static void setDataToIndicesBuffer(GLuint buffer);

    static unsigned int generateUniformBuffer(float strength, std::vector<GLfloat> color);

    static void bindDataToUniformBuffer(GLuint uniformBuffer, GLuint transformationBuffer);

    static unsigned int generateTransformationBuffer();

    void framebuffer_resize_callback(int w, int h) override;

    void scroll_callback(double xoffset, double yoffset) override;
};