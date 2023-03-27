#include "Texture.h"

Texture::Texture(const std::string& path, int id) : FilePath(path), LocalBuffer(nullptr),
									  Width(0), Height(0), BPP(0), RendererID(0), EngineID(id),
									  currentBoundSlot(-1), loadedFromFile(1)
{
	std::cout << "Loading Texture..." << std::endl;
	// loading image
	stbi_set_flip_vertically_on_load(1);	// GL loads from bottom left
	LocalBuffer = stbi_load(path.c_str(), &Width, &Height, &BPP, 4);	

	if (LocalBuffer) { std::cout << "Image loaded successfully!" << std::endl; }
	else { std::cout << "Error loading image" << std::endl; }
	
	// saves width, height and BPP. 4 channels for RGBA

	// Creating and binding texture
	GL(glGenTextures(1, &RendererID));
	GL(glBindTexture(GL_TEXTURE_2D, RendererID));

	std::cout << "TEX ID: " << RendererID << std::endl;

	// settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// GL NEAREST IS A GODSEND!!!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// giving data
	GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));
	
	// unbinding and clearning data from local buffer
	GL(glBindTexture(GL_TEXTURE_2D, 0));
	// stbi_image_free(LocalBuffer);
}

Texture::Texture(unsigned char* data, int w, int h, int id) : FilePath("na"), LocalBuffer(nullptr),
Width(w), Height(h), BPP(4), RendererID(0), EngineID(id),
currentBoundSlot(-1), loadedFromFile(0)
{
	LocalBuffer = data;
	// if (LocalBuffer) { std::cout << "Image loaded successfully!" << std::endl; }
	// else { std::cout << "Error loading image" << std::endl; }

	// Creating and binding texture
	GL(glGenTextures(1, &RendererID));
	GL(glBindTexture(GL_TEXTURE_2D, RendererID));


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// GL NEAREST IS A GODSEND!!!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// giving data
	GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));

	// unbinding and clearning data from local buffer
	GL(glBindTexture(GL_TEXTURE_2D, 0));
	// stbi_image_free(LocalBuffer);
}

Texture::~Texture()
{
	GL(glDeleteTextures(1, &RendererID));
	if (loadedFromFile) { stbi_image_free(LocalBuffer); }
}

void Texture::Bind(unsigned int slot)
{
	GL(glActiveTexture(GL_TEXTURE0 + slot));
	GL(glBindTexture(GL_TEXTURE_2D, RendererID));

	currentBoundSlot = slot;
}

void Texture::Unbind()
{
	GL(glBindTexture(GL_TEXTURE_2D, 0));
}
