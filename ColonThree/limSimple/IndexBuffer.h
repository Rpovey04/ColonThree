#include "VertexArray.h"

class IndexBuffer {
private:
	unsigned int glID;
	unsigned int* indexArr;
	int maxQuads;

	void initIndecies() {		// engine can only draw quads (2d images)
		// Using 0, 1, 2, 2, 3, 0 (anti-clockwise winding)
		indexArr = new unsigned int[maxQuads * 6];
		for (int i = 0; i < maxQuads; i++) {
			indexArr[i * 6] = i * 6 + 0;
			indexArr[i * 6 + 1] = i * 6 + 1;
			indexArr[i * 6 + 2] = i * 6 + 2;
			indexArr[i * 6 + 3] = i * 6 + 2;
			indexArr[i * 6 + 4] = i * 6 + 3;
			indexArr[i * 6 + 5] = i * 6 + 0;
		}
	}
public:
	// Constructor / Destructor
	IndexBuffer(int n = 1) : maxQuads(n) {
		initIndecies();		// only one image is to be drawn at a time
		glGenBuffers(1, &glID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6*maxQuads, indexArr, GL_STATIC_DRAW);
		Unbind();
	}
	~IndexBuffer() {
		Unbind();
		glDeleteBuffers(1, &glID);
		delete[] indexArr;
	}

	// Binding
	void Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glID);
	}
	void Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};