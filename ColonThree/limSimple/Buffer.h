#include "IndexBuffer.h"

class Buffer {
private:
	unsigned int glID;

public:
	// Constructor / Destructor
	Buffer() {
		glGenBuffers(1, &glID);
	}
	~Buffer() {
		Unbind();
		glDeleteBuffers(1, &glID);
	}

	// Submit Data
	void submitData(int size, const void* data) {
		Bind();		// leaves buffer bound, no way to re-bind previously bound buffer
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// Binding
	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, glID);
	}
	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
