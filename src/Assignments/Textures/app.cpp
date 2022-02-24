//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION  1
#include "3rdParty/stb/stb_image.h"

void SimpleShapeApplication::init()
{
	// A utility function that reads the shader sources, compiles them and creates the program object
	// As everything in OpenGL we reference program by an integer "handle".
    xe::ColorMaterial::init();

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));
	auto program = xe::utils::create_program(
		{ {GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
		 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

	if (!program)
	{
		std::cerr << "Invalid program" << std::endl;
		exit(-1);
	}

    stbi_set_flip_vertically_on_load(true);
    GLint width;
    GLint height;
    GLint channels;
    auto textureFile = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto image = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!image) {
        std::cerr << "Could not read image from file" << std::endl;
    }

    GLuint textures_buffer;
    glGenTextures(1, &textures_buffer);
    glBindTexture(GL_TEXTURE_2D, textures_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


	/*std::vector<GLfloat> vertices = {

		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
	};*/



	std::vector<GLfloat> vertices = {
			-0.5f, 0.0f, -0.5f, 1.0f, 1.0f, 0.0f,
			-0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.0f, -0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,

			-0.5f, 0.f, -0.5f, 0.0f, 1.0f, 1.0f,
			0.f, 1.f, 0.f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.f, -0.5f, 0.0f, 1.0f, 1.0f,

			0.5f, 0.f, -0.5f, 0.0f, 0.0f, 1.0f,
			0.f, 1.f, 0.f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.f, 0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.f, 1.f, 0.f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.f, 0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, 0.f, 0.5f, 1.0f, 0.0f, 1.0f,
			0.f, 1.f, 0.f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.f, -0.5f, 1.0f, 0.0f, 1.0f,
	};

	indices_vector = {
		0,2,1,1,2,3,4,5,6,7,8,9,11,10,12,13,14,15,
	};

    // MeshesMaterials
    auto pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    pyramid->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

    pyramid->allocate_index_buffer(indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_indices(0, indices.size() * sizeof(GLfloat), indices.data());

    pyramid->add_submesh(0, 3, new xe::ColorMaterial(textures_buffer, 0));
    pyramid->add_submesh(3, 6, new xe::ColorMaterial(textures_buffer, 1));
    pyramid->add_submesh(6, 9, new xe::ColorMaterial(textures_buffer, 2));
    pyramid->add_submesh(9, 12, new xe::ColorMaterial(textures_buffer, 3));
    pyramid->add_submesh(12, 16, new xe::ColorMaterial(textures_buffer, 4));
    add_submesh(pyramid);

    float strength = 1.0;
    std::vector<GLfloat> color = { 0.9, 0.8, 0.7 };

    auto [w, h] = frame_buffer_size();
    modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(115.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    _camera->look_at(glm::vec3(-2.0f, -1.f, -5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    auto fov = glm::pi<float>() / 4.0;
    auto near = 0.1f;
    auto far = 100.0f;
    auto aspect = (float)w / h;
    _camera->perspective(fov, aspect, near, far);

	GLuint indicesBuffer = generateIndicesBuffer(indices_vector);
	GLuint verticesBuffer = generateVertexBuffer(vertices);
	GLuint uniformBuffer = generateUniformBuffer(strength, color);
	transformationBuffer = generateTransformationBuffer();
    // This setups a Vertex Array Object (VAO) that  encapsulates
        // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    bindDataToUniformBuffer(uniformBuffer, transformationBuffer);
    setDataToVertexBuffer(verticesBuffer);
    setDataToIndicesBuffer(indicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glUseProgram(program);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (_controler) {
        _controler->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);
    if (_controler) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            _controler->LMB_pressed(x, y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            _controler->LMB_released(x, y);
    }
}


unsigned int SimpleShapeApplication::generateVertexBuffer(std::vector<GLfloat> vertices) {
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return v_buffer_handle;
}

unsigned int SimpleShapeApplication::generateIndicesBuffer(std::vector<GLushort> indices) {
    GLuint indicesBuffer;
    glGenBuffers(1, &indicesBuffer);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return indicesBuffer;
}

unsigned int SimpleShapeApplication::generateUniformBuffer(float strength, std::vector<GLfloat> color) {
    GLuint uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer));
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return uniformBuffer;
}


unsigned int SimpleShapeApplication::generateTransformationBuffer() {
    GLuint transformationBuffer;
    glGenBuffers(1, &transformationBuffer);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, transformationBuffer));
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return transformationBuffer;
}

void SimpleShapeApplication::setDataToVertexBuffer(GLuint verticesBuffer) {
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    setVerticesData();
    setColorToVertices();
}

void SimpleShapeApplication::setVerticesData() {
    int arrayId = 0;
    int dataInArrayOffset = 0;
    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(arrayId);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(arrayId, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(dataInArrayOffset));
}

void SimpleShapeApplication::setColorToVertices() {
    int arrayId = 1;
    int dataInArrayOffset = 3 * sizeof(GLfloat);

    glEnableVertexAttribArray(arrayId);
    glVertexAttribPointer(arrayId, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(dataInArrayOffset));
}

void SimpleShapeApplication::setDataToIndicesBuffer(GLuint indicesBuffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}

void SimpleShapeApplication::bindDataToUniformBuffer(GLuint uniformBuffer, GLuint transformationBuffer) {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformationBuffer);
}

void SimpleShapeApplication::frame() {
    auto pvm = _camera->projection() * _camera->view() * modelMat;
    glBindBuffer(GL_UNIFORM_BUFFER, transformationBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &pvm[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //// Binding the VAO will setup all the required vertex buffers.
    //glBindVertexArray(vao_);

    //glDrawElements(GL_TRIANGLES, indices_vector.size(), GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    //glBindVertexArray(0);

    // MeshesMaterials
    for (auto m : meshes_) {
        m->draw();
    }
}
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    _camera->set_aspect((float)w / h);
}
