#pragma once
#include "Texture.h"

class Sprite
{
private:
	// dimensions between verticies
	int width, height;
	// coordinates of offset
	Texture* image;
	
	int Xpos, Ypos;
	float rotation;

	float imgX, imgY, imgW, imgH;
	Color col;
	
public:
	Sprite()
		:Xpos(0), Ypos(0), rotation(0), imgX(0), imgY(0), col({0, 0, 0, 0})
	{};
	~Sprite() {}

	void LoadImage(Texture*& base) { image = base; imgW = base->GetWidth(); imgH = base->GetHeight(); }
	void setDimensions(int w, int h) { width = w; height = h; }
	void setPosition(int X, int Y) { Xpos = X; Ypos = Y; }
	void setRotation(double a) { rotation = a; }
	void setImgRect(float x, float y, float w, float h) { imgX = x; imgY = y; imgW = w; imgH = h; }
	void setCol(Color c) { col = c; }


	std::vector<int> getPosition() { return { Xpos, Ypos }; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	float getRotation() { return rotation; }
	Texture* getImage() { return image; }
	Color getColor() { return col; }

	std::vector<float> getImageRect() { return { imgX, imgY, imgW, imgH }; }
};
