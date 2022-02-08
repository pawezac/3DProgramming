#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "camera.h"
#include "cameraControler.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;


    void set_camera(Camera* camera) { _camera = camera; }

    Camera* camera() { return _camera; }

    ~SimpleShapeApplication() {
        if (_camera) {
            delete _camera;
        }
    }

    void set_controler(CameraControler* controler) { _controler = controler; }


private:
    GLuint vao_;
    std::vector<GLushort> indices_vector;
    glm::mat4 modelMat;
    Camera* _camera;
    CameraControler* _controler;

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

    void cursor_position_callback(double x, double y);

    void mouse_button_callback(int button, int action, int mods);
};