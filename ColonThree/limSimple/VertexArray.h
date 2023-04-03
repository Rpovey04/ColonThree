#include "Shader.h"

class VertexArray {	// actually vertex attribute array
private:
	unsigned int glID;
	int currentVertexAttributes;

public:
	// Constructor / Destructor
	VertexArray() : currentVertexAttributes(0) {
		glGenVertexArrays(1, &glID);
	}
	~VertexArray() {
		Unbind();
		glDeleteVertexArrays(1, &glID);
	}

	// adding attributes
	void addAttribute(int numAttribs, unsigned int type, int stepSize, const void* offset) {
		glEnableVertexAttribArray(currentVertexAttributes);
		glVertexAttribPointer(currentVertexAttributes, numAttribs, type, GL_FALSE, stepSize, offset);
		currentVertexAttributes += 1;
	}

	// Binding
	void Bind() {
		glBindVertexArray(glID);
	}
	void Unbind() {
		glBindVertexArray(0);
	}


};