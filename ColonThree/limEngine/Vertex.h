#pragma once
#include "Header.h"

#define white {1.0f, 1.0f, 1.0f, 1.0f}
#define black {0.0f, 0.0f, 0.0f, 1.0f}
#define lightgrey {0.25f, 0.25f, 0.25f, 1.0f}
#define grey	{0.5f, 0.5f, 0.5f, 1.0f}
#define darkgrey {0.75f, 0.75f, 0.75f, 1.0f}


struct Color 
{
	float r, g, b, a;
};

struct Points
{
	float x, y, z;
};

struct Coord 
{
	float x, y;
};

// used structures
struct Vertex
{
	Points points;	// 3 component
	Color color;	// 4 component
};


struct Svertex 
{
	Points points;
	float ID;		// texture ID
	Coord offset;	// texture position
	Color col;
};


struct Lightsource 
{
	int ID;		// not for use in shader
	float X, Y;
	float Intensity;
	float Radius;
};

