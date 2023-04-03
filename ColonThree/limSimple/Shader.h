#include <iostream>
#include <unordered_map>
#include "GL/glew.h"

#ifdef _DEBUG
#define println(msg) std::cout << msg << std::endl
#endif

#ifdef NDEBUG
#define println(msg)
#endif


class Shader {
private:
	static const std::string BASIC_VERTEX_SRC;
	static const std::string BASIC_FRAGMENT_SRC;
	unsigned int glID;
	std::unordered_map<std::string, int> uniformLocations;

	// Loading
	static unsigned int CompileShader(unsigned int shaderType, const std::string& src) {
		unsigned int id = glCreateShader(shaderType);
		const char* rawSrc = &src[0];
		glShaderSource(id, 1, &rawSrc, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			println("[LimSimple] " << src);
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* msg = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, msg);
			println("\n\n[LimSimple] " << msg);
		}
		return id;
	}
	static unsigned int CreateShader(const std::string& vShader, const std::string& fShader) {
		unsigned int shaderID = glCreateProgram();
		unsigned int vID = CompileShader(GL_VERTEX_SHADER, vShader);
		unsigned int fID = CompileShader(GL_FRAGMENT_SHADER, fShader);

		glAttachShader(shaderID, vID);
		glAttachShader(shaderID, fID);
		glLinkProgram(shaderID);
		glValidateProgram(shaderID);

		glDeleteShader(vID);
		glDeleteShader(fID);

		return shaderID;
	}
	void loadBasicShader() {		// done for the purpose of simplicity, shader code should ideally be passed from the renderer / window
		glID = CreateShader(BASIC_VERTEX_SRC, BASIC_FRAGMENT_SRC);
	}
	
	// Uniforms
	void locatedUniform(std::string uniformName) {
		auto iter = uniformLocations.begin();
		while (iter != uniformLocations.end()) {
			if (iter++->first == uniformName) { return; }
		}
		uniformLocations.insert(std::pair<std::string, int>(uniformName, glGetUniformLocation(glID, &uniformName[0])));
	}

public:
	// Constructor / Destructor
	Shader() {
		loadBasicShader();
	}
	~Shader() {
		Unbind();
		glDeleteProgram(glID);
	}

	// Uniforms
	void setUniform1i(std::string uniformName, int val) {
		locatedUniform(uniformName);
		glUniform1i(uniformLocations[uniformName], val);
	}
	void setUniform4f(std::string uniformName, std::vector<float> val) {
		locatedUniform(uniformName);
		glUniform4f(uniformLocations[uniformName], val[0], val[1], val[2], val[3]);
	}

	// Binding
	void Bind() {
		glUseProgram(glID);
	}
	void Unbind() {
		glUseProgram(0);
	}
};


/*
	SHADER CODE
*/
// VERTEX
const std::string Shader::BASIC_VERTEX_SRC =
		"#version 330 core\n"
		"layout(location=0) in vec4 position;\n"
		"layout(location=1) in vec2 texCoord;\n"
		"out vec2 v_TexCoord;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"	v_TexCoord = texCoord;\n"
		"}\n"
	;
// FRAGMENT
const std::string Shader::BASIC_FRAGMENT_SRC =
		"#version 330 core\n"
		"layout(location=0) out vec4 color;\n"
		"in vec2 v_TexCoord;\n"
		"uniform sampler2D u_Texture;\n"
		"void main()\n"
		"{\n"
		"	color = texture(u_Texture, v_TexCoord);\n"
		"}\n"
	;