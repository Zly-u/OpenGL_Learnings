#include "ShaderProgram.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Logging.h"

ShaderProgram::ShaderProgram(const ShaderProgram& OtherShaderProgram) : ShaderProgramID(OtherShaderProgram.ShaderProgramID) {

}


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
}


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

void ShaderProgram::Render(uint32_t VAO)
{
	glUseProgram(ShaderProgramID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
