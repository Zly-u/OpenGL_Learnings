#include "ShaderProgram.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GLFW/glfw3.h>

#include "glm/trigonometric.hpp"

#include "Logging.h"


ShaderProgram::ShaderProgram(const std::string_view& VertexShader, const std::string_view& FragmentShader) {
	const int64_t VertexShaderID = LoadShaderFromFile(VertexShader, GL_VERTEX_SHADER);
	if (VertexShaderID == -1) {
		return;
	}

	const int64_t FragmentShaderID = LoadShaderFromFile(FragmentShader, GL_FRAGMENT_SHADER);
	if (FragmentShaderID == -1) {
		return;
	}

	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, VertexShaderID);
	glAttachShader(ShaderProgramID, FragmentShaderID);
	glLinkProgram(ShaderProgramID);

	int Success;
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &Success);
	if(!Success) {
		char infoLog[512];
		glGetProgramInfoLog(ShaderProgramID, 512, nullptr, infoLog);

		Log::println("ERROR::SHADERPROGRAM::LINKING_FAILED: {}", infoLog);

		return;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	FindUniforms();
	PreparePolygon();
}


ShaderProgram::~ShaderProgram() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void ShaderProgram::FindUniforms() {
	Uniform_VertexColorLocation = glGetUniformLocation(ShaderProgramID, "PassedColor");
}

// ---------------------------------------------------------------------------------------------------------------------

int64_t ShaderProgram::LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType) {
	std::ifstream infile(ShaderFile.data());

	if(infile.fail()) {
		Log::println("Error opening shader file: {}", ShaderFile);
		return -1;
	}

	std::stringstream Buffer_ShaderShader;
	Buffer_ShaderShader << infile.rdbuf();
	std::string ShaderContent = Buffer_ShaderShader.str();

	const char* VertexShaderData = ShaderContent.c_str();
	const GLint VertexShaderData_Size = static_cast<GLint>(ShaderContent.size());

	const GLuint NewShaderID = glCreateShader(ShaderType);
	glShaderSource(NewShaderID, 1, &VertexShaderData, &VertexShaderData_Size);
	glCompileShader(NewShaderID);

	int Success;
	glGetShaderiv(NewShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success) {
		char infoLog[512];
		glGetShaderInfoLog(NewShaderID, 512, nullptr, infoLog);

		Log::println(
			"ERROR::SHADER::{}::COMPILATION_FAILED: {}",
			ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT",
			infoLog
		);

		return -1;
	}

	return NewShaderID;
}


void ShaderProgram::PreparePolygon() {
	float Vertices[] =
	// {
	// 	1.0f,  1.0f, 0.0f,  // top right
	// 	1.0f, -1.0f, 0.0f,  // bottom right
	//    -1.0f, -1.0f, 0.0f,  // bottom left
	//    -1.0f,  1.0f, 0.0f   // top left
 //   };
	{
		0.5f,  0.5f, 0.0f,  // top right
		1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.0f,  // bottom right
		1.0f, 1.0f, 0.0f,

	   -0.5f, -0.5f, 0.0f,  // bottom left
		0.0f, 1.0f, 0.0f,

	   -0.5f,  0.5f, 0.0f,  // top left
		0.0f, 1.0f, 1.0f
	};

	unsigned int Indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// Vertex Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void ShaderProgram::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(ShaderProgramID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
