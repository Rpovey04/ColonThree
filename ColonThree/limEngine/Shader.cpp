#include "Shader.h"


std::string ReadFile(std::string Address)
{
	std::ifstream file(Address);
	std::string currentLine, result;

	while (std::getline(file, currentLine))
	{
		result += (currentLine + "\n");
	}

	file.close();
	return result;
}



/*
	CLASS FUNCTIONS
*/

unsigned int Shader::GetUniformLoc(const std::string name)
{
	// checking if the uniform has already been retrieved
	if (UniformLocs.find(name) != UniformLocs.end()) { return UniformLocs[name]; }

	// getting the location if it has not already been done
	std::cout << "Locating uniform: (" << name << ")" << std::endl;
	int location = glGetUniformLocation(RendererID, name.c_str());
	if (location == -1) { std::cout << "Uniform not in shader" << std::endl; }
	std::cout << "Found with location: " << location << std::endl;
	UniformLocs[name] = location;

	return location;
}

void Shader::Init(const std::string FragFileName, const std::string VertFileName)
{
	// retrieving source code
	VertSource = ReadFile(VertFileName);
	FragSource = ReadFile(FragFileName);
	RendererID = CreateShader();
}

Shader::~Shader()
{

}

void Shader::Unload()
{
	GL(glDeleteProgram(RendererID));
}

unsigned int Shader::CompileShader(const std::string source, unsigned int type)
{
	// creates the initial ID of the shader
	unsigned int ID = glCreateShader(type);

	// allows the string to be used as a char array
	const char* src = source.c_str();
	GL(glShaderSource(ID, 1, &src, nullptr));
	GL(glCompileShader(ID));

	int result;
	GL(glGetShaderiv(ID, GL_COMPILE_STATUS, &result));
	// error handling
	if (!result)
	{
		std::cout << "Error compiling shader: " << source << std::endl;
		int length;
		GL(glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length));
		char* message = new char[length];
		GL(glGetShaderInfoLog(ID, length, &length, message));
		std::cout << message << std::endl;
		delete message;

		return 0;
	}


	return ID;
}

unsigned int Shader::CreateShader() 
{
	unsigned int ShaderId;
	ShaderId = glCreateProgram();
	// compiling the vertex and fragment shaders
	unsigned int Vshader = CompileShader(VertSource, GL_VERTEX_SHADER);
	unsigned int Fshader = CompileShader(FragSource, GL_FRAGMENT_SHADER);
	// attaching the two shaders together
	GL(glAttachShader(ShaderId, Vshader));
	GL(glAttachShader(ShaderId, Fshader));
	// finalising the program
	GL(glLinkProgram(ShaderId));
	GL(glValidateProgram(ShaderId));
	// deleting the shaders after linking
	GL(glDeleteShader(Vshader));
	GL(glDeleteShader(Fshader));
	
	return ShaderId;
}

void Shader::Bind()
{
 	GL(glUseProgram(RendererID));
}

void Shader::Unbind()
{
	GL(glUseProgram(0));
}


void Shader::SetUniform1f(const std::string name, float value)
{
	Bind();
	GL(glUniform1f(GetUniformLoc(name), value));
	Unbind();
}

void Shader::SetUniform1i(const std::string name, int value)
{
	GL(glUniform1i(GetUniformLoc(name), value));
}

void Shader::SetUniform1iv(const std::string name, int amount, int* value)
{
	Bind();
	GL(glUniform1iv(GetUniformLoc(name), amount, value));
	Unbind();
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
	Bind();
	GL(glUniform4f(GetUniformLoc(name), v0, v1, v2, v3));
	Unbind();
}

void Shader::SetUniformMat4f(const std::string name, const glm::mat4& Matrix)
{
	Bind();
	GL(glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &Matrix[0][0]));
	Unbind();
}

void Shader::SetUniformVec2(const std::string name, const glm::vec2& Vector)
{
	Bind();
	GL(glUniform2fv(GetUniformLoc(name), 1, &Vector[0]));
	Unbind();
}

void Shader::SetUniformArrayVec2(const std::string name, const glm::vec2* Vector, int amount)
{
	Bind();
	GL(glUniform2fv(GetUniformLoc(name), amount, &Vector[0][0]));
	Unbind();
}

void Shader::SetUniformArrayVec4(const std::string name, const glm::vec4* Vector, int amount)
{
	Bind();
	GL(glUniform4fv(GetUniformLoc(name), amount, &Vector[0][0]));
	Unbind();
}

void Shader::SetUniformArrayFloat(const std::string name, float* f, int amount)
{
	Bind();
	GL(glUniform1fv(GetUniformLoc(name), amount, f));
	Unbind();
}


