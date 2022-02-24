//
// Created by Piotr Białas on 02/11/2021.
//

#include "glm/gtx/string_cast.hpp"

#include "Material.h"

namespace xe {

    GLuint ColorMaterial::color_uniform_buffer_ = 0u;
    GLuint ColorMaterial::shader_ = 0u;
    GLint  ColorMaterial::uniform_map_Kd_location_ = 0;

    void ColorMaterial::bind() {
        //Textures
        bool use_map_Kd = 0;
        if(texture_)
        {
            use_map_Kd = 1;
            glActiveTexture(GL_TEXTURE0 + texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
            glUniform1i(uniform_map_Kd_location_, texture_unit_);
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_uniform_buffer_);
        glUseProgram(program());
        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &color_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(bool), &use_map_Kd); //Textures

    }

    void ColorMaterial::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void ColorMaterial::init() {


        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/color_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/color_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;

        glGenBuffers(1, &color_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::vec4), nullptr, GL_STATIC_DRAW); // 2 hours...
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            // spdlog::warn("Cannot get uniform {} location", "map_Kd");
            std::cerr << "Cannot get uniform" << std::endl;
        }
    }
}