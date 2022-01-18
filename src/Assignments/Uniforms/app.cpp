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


    // Uniform

    // Create uniform buffer (UBO)
    GLuint uniform_buffer_handle;
    glGenBuffers(1, &uniform_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);

    // Assign data to UBO
    float strength = 0.5;
    float light[3] = { 0.7, 0.2, 0.3 };

    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), &light);

    // Bind UBO to the binding point at index
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer_handle);

    // Get index of uniform block (from fragment shader).
    // If successful, assign it to the point of binding (the same as UBO is binded to)
    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    //
    // Clean Buffers
    //
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


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
