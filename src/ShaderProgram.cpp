#include "ShaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Logging.h"
#include "Random.h"


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
	auto Rand = [] {
		// return Random::Normal() - 0.5f;
		return 0.f;
	};
	float Vertices[] =
	{
		-0.5f,  0.5f, // top left
		0.0f, 1.0f, 1.0f,	// Color
		0.0f, 0.0f,			// UV

		0.5f,  0.5f,  // top right
		1.0f, 0.0f, 0.0f,	// Color
		1.0f, 0.0f,			// UV

		0.5f, -0.5f,  // bottom right
		0.0f, 1.0f, 0.0f,	// Color
		1.0f, 1.0f,			// UV

	   -0.5f, -0.5f,  // bottom left
		0.0f, 0.0f, 1.0f,	// Color
		0.0f, 1.0f,			// UV
	};

	unsigned int Indices[] = {
		0, 1, 2,   // first triangle
		0, 2, 3    // second triangle
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Vertex UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void ShaderProgram::FindUniforms() {
	// TODO: Generalized Uniforms access.
	Uniform_VertexColorLocation = glGetUniformLocation(ShaderProgramID, "PassedColor");
	Uniform_Transform = glGetUniformLocation(ShaderProgramID, "PassedTransform");
}

void ShaderProgram::Use()
{
	glUseProgram(ShaderProgramID);
}

void ShaderProgram::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// glUseProgram(ShaderProgramID);

	// TODO: Set the uniforms from some template function.
	glUniformMatrix4fv(Uniform_Transform, 1, GL_FALSE, glm::value_ptr(Transform));

	// glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void ShaderProgram::SetTranform(const glm::mat4& NewTransform)
{
	Transform = NewTransform;
}
