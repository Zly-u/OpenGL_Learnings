#include "Rendering/ShaderProgram.hpp"

#include <sstream>

int64_t
ShaderProgramBase::LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType) const
{
	std::ifstream File(ShaderFile.data());

	if (File.fail())
	{
		Log::println("Error opening shader file: {}", ShaderFile);
		return -1;
	}

	std::stringstream Buffer_ShaderShader;
	Buffer_ShaderShader << File.rdbuf();
	std::string ShaderContent = Buffer_ShaderShader.str();

	const char* VertexShaderData      = ShaderContent.c_str();
	const GLint VertexShaderData_Size = static_cast<GLint>(ShaderContent.size());

	const GLuint NewShaderID = glCreateShader(ShaderType);
	glShaderSource(NewShaderID, 1, &VertexShaderData, &VertexShaderData_Size);
	glCompileShader(NewShaderID);

	int Success;
	glGetShaderiv(NewShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
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
