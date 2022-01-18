//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }
    std::vector<GLfloat> vertices = {
       
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  
         0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f   
    };

    std::vector<GLushort> indices = {
            0,1,2,3,4,5,6,5,4
    };

    // Vertex buffer
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Index Buffer
    GLuint indices_buffer_handle;
    glGenBuffers(1, &indices_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Vertex Array
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    // VBO Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    // VBO Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(12));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_handle);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
