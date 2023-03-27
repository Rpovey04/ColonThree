#pragma once
#include "IndexBuffer.h"

class Texture
{
private:
	unsigned int RendererID;
	std::string FilePath;
	unsigned char* LocalBuffer;
	int EngineID;

	int Width, Height, BPP;	// bits per pixel
	int currentBoundSlot;
	int loadedFromFile;

public:
	Texture(const std::string& path, int id);
	Texture(unsigned char* data, int w, int h, int id);
	~Texture();

	void Bind(unsigned int slot = 0);		// approimately 32 slots
	void Unbind();

	int GetWidth() { return Width; }
	int GetHeight() { return Height; }
	int GetID() { return EngineID; }

	bool boundTo(int slot) { return slot == currentBoundSlot; }

	unsigned char* getData() { return LocalBuffer; }
	void ReloadImg() {
		GL(glBindTexture(GL_TEXTURE_2D, RendererID));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// GL NEAREST IS A GODSEND!!!!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));

		GL(glBindTexture(GL_TEXTURE_2D, 0));
	}
};
