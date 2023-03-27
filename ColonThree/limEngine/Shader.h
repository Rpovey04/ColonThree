#pragma once
#include "Sprite.h"

class Shader
{
private:
	unsigned int RendererID;
	
	std::unordered_map<std::string, int> UniformLocs;

	std::string FragSource;
	std::string VertSource;

	unsigned int GetUniformLoc(const std::string name);
	unsigned int CompileShader(const std::string source, unsigned int type);
	unsigned int CreateShader();

	

public:
	Shader() {}
	void Init(const std::string FragFileName, const std::string VertFileName);
	
	~Shader();
	void Unload();


	void Bind();
	void Unbind();

	void SetUniform1f(const std::string name, float value);
	void SetUniform1i(const std::string name, int value);
	void SetUniform1iv(const std::string name, int amount, int* value);
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string name, const glm::mat4& Matrix);
	void SetUniformVec2(const std::string name, const glm::vec2& Vector);
	void SetUniformArrayVec2(const std::string name, const glm::vec2* Vector, int amount);
	void SetUniformArrayVec4(const std::string name, const glm::vec4* Vector, int amount);
	void SetUniformArrayFloat(const std::string name, float* f, int amount);
};

