#include "Buffer.h"
#include "limSimple/stb_image/stb_image.h"

class Texture {
private:
	unsigned int glID;		// taken to also be slot
	Shader* activeShader;
	bool holdImgData;
	unsigned char* imgData;
	int width, height, BPP;

	void loadTexture(unsigned char* src) {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, src);
	}
	void unload() {
		if (holdImgData) { stbi_image_free(imgData); }
		glDeleteTextures(1, &glID);
		Unbind();
	}
public:
	// Constructor / Destructor
	Texture(Shader* s, bool hold = 0) : activeShader(s), holdImgData(hold), imgData(nullptr), width(0), height(0), BPP(0) {
		glGenTextures(1, &glID);
	}
	~Texture() {
		unload();
	}
	void reset() {		// call manually before loading a new texture to the same instance to ensure proper deletion
		width = 0; height = 0; BPP = 0;
		unload();
	}

	// Loading images
	void loadFromSource(unsigned char* src, int w, int h, int bpp) {
		width = w; height = h; BPP = bpp;
		loadTexture(src);
	}
	void loadFromPath(std::string& path) {
		unsigned char* tempImgBuffer;
		stbi_set_flip_vertically_on_load(1);
		tempImgBuffer = stbi_load(&path[0], &width, &height, &BPP, 4);
		loadTexture(tempImgBuffer);

		if (!holdImgData) { stbi_image_free(tempImgBuffer); }
		else { imgData = tempImgBuffer; }
	}

	// Binding
	void Bind() {
		activeShader->setUniform1i("u_Texture", glID);		// shader is geared to deal with one texture. Not useful for batch rendering
		glActiveTexture(GL_TEXTURE0 + glID);
		glBindTexture(GL_TEXTURE_2D, glID);
	}
	void Unbind() {
		activeShader->setUniform1i("u_Texture", 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Getters
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int getBPP() {
		return BPP;
	}
	unsigned char* getImgData() {
		return imgData;
	}
};