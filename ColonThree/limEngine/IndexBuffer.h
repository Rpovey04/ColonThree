#pragma once
#include "Vertex.h"

class IndexBuffer
{
private:
	int Quads;
	
	unsigned int ID;

	unsigned int* Indicies;
	void GenIndicies(int num) 
	{
		auto GenQuad = [](int n, unsigned int* list)
		{
			int index = n * 6;
			int point = n * 4;

			list[index] = point + 3;	list[index + 1] = point + 2;	list[index + 2] = point;	// first triangel
			list[index + 3] = point + 2;	list[index + 4] = point + 1;	list[index + 5] = point;	// second triangel
		};

		Indicies = new unsigned int[num * 6];
		for (int k = 0; k < num; k++)
		{
			GenQuad(k, Indicies);
		}
		
		for (int n = 0; n < 5; n++) 
		{
			int index = n * 6;
			std::cout << Indicies[index] << Indicies[index + 1] << Indicies[index + 2] << Indicies[index + 3] << Indicies[index + 4] << Indicies[index + 5] << std::endl;
		}
	}

public:
	IndexBuffer() {}
	~IndexBuffer() { delete[] Indicies; }

	void Init(int Q) 
	{
		Quads = Q;
		GenIndicies(Q);	//data

		GL(glGenBuffers(1, &ID));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
		GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Quads * 6 * sizeof(unsigned int), Indicies, GL_STATIC_DRAW));
	}

	void Bind() 
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	}

	void Unbind() 
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
};

