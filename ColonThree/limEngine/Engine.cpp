#include "Engine.h"

void limEngine::Engine::InitWindow()
{
	if (glfwInit()) { std::cout << "glfw initialised successfully" << std::endl; }
	glewExperimental = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (fullscreen) 
	{
		window = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else { window = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL); }
	if (!(window == NULL)) { std::cout << "window created successfully!" << std::endl; }
	glfwMakeContextCurrent(window);  // initialises the library to the window

	if (glewInit() == GLEW_OK) { std::cout << "glew initialised successfully" << std::endl; }

	// projection matrix (will be variable)
	ProjMatrix = glm::ortho(0.0f, (float)Width, 0.0f, (float)Height,
								-1.0f, 1.0f);

	// enabling blend
	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}


void limEngine::Engine::InitBatch()
{
	// verticies
	verticies = new Vertex[numQuads * 4];

	// index buffer (10000 by standard, this does not have to be met every time)
	Q_IndexBuffer.Init(numQuads);
	Q_IndexBuffer.Bind();

	// creating vertex array object (binds layout to buffer)
	GL(glCreateVertexArrays(1, &Q_VertexArray));
	GL(glBindVertexArray(Q_VertexArray));

	// creating vertex buffer
	GL(glGenBuffers(1, &Q_VertexBuffer));
	GL(glBindBuffer(GL_ARRAY_BUFFER, Q_VertexBuffer));
	GL(glBufferData(GL_ARRAY_BUFFER, numQuads * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	// attribs
	GL(glEnableVertexArrayAttrib(Q_VertexArray, 0));	// points attrib
	GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, points)));

	GL(glEnableVertexArrayAttrib(Q_VertexArray, 1));	// color attrib
	GL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));
}


void limEngine::Engine::InitSprites() 
{
	// verticies
	S0_verticies = new Svertex[numSprites * 4];
	S1_verticies = new Svertex[numSprites * 4];
	S2_verticies = new Svertex[numSprites * 4];

	// index buffer (10000 by standard, this does not have to be met every time)
	std::cout << "SPRITE VERTEX: " << std::endl;
	S_IndexBuffer.Init(numSprites);
	S_IndexBuffer.Bind();

	// creating vertex array object (binds layout to buffer)
	GL(glCreateVertexArrays(1, &S_VertexArray));
	GL(glBindVertexArray(S_VertexArray));
	
	// creating vertex buffer
	GL(glGenBuffers(1, &S_VertexBuffer));
	GL(glBindBuffer(GL_ARRAY_BUFFER, S_VertexBuffer));
	GL(glBufferData(GL_ARRAY_BUFFER, numSprites * 4 * sizeof(Svertex), nullptr, GL_DYNAMIC_DRAW));

	// attribs
	GL(glEnableVertexArrayAttrib(S_VertexArray, 0));	// points attrib
	GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Svertex), (const void*)offsetof(Svertex, points)));

	GL(glEnableVertexArrayAttrib(S_VertexArray, 1));	//  tex ID
	GL(glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Svertex), (const void*)offsetof(Svertex, ID)));

	GL(glEnableVertexArrayAttrib(S_VertexArray, 2));	//  tex ID
	GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Svertex), (const void*)offsetof(Svertex, offset)));

	GL(glEnableVertexArrayAttrib(S_VertexArray, 3));	//  Colour
	GL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Svertex), (const void*)offsetof(Svertex, col)));
}


void limEngine::Engine::InitShaders()
{
	Q_Shader.Init("Shaders/ShapeShader.frag", "Shaders/ShapeShader.vert");

	S_Shader.Init("Shaders/TexShader.frag", "Shaders/TexShader.vert");
	int sampler[2] = { 0,1 };		// tex ids
	S_Shader.SetUniform1iv("u_texture", 2, sampler);

	overlay = { 0.0f, 0.0f, 0.0f, 0.0f };
}

void limEngine::Engine::InitTextures()
{
	AvailableTextures = 32;		// capped at 8 (for potential ios dev)

	int sampler[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,25,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };		// tex ids
	S_Shader.SetUniform1iv("u_texture", 32, sampler);
}

void limEngine::Engine::ProcessLights()
{
	for (int n = 0; n < LightAmount; n++)
	{
		LightPositions[n] = glm::vec2(LightList[n]->X, LightList[n]->Y);

		lightIntensity[n] = LightList[n]->Intensity;
		lightRadii[n] = LightList[n]->Radius;
	}
}


limEngine::Engine::Engine(int W, int H, std::string T, unsigned int special)
	:Width(W), Height(H), Title(T), numQuads(10000), CurrentQuads(0), numSprites(10000), 
	CurrentSprites0(0), CurrentSprites1(0), CurrentSprites2(0), run(1),
	currentTexID(0), fullscreen(0), Xres(Width), Yres(Height), LightAmount(0), lightID(0)
{
	if (special == LimFullscreen) { fullscreen = true; }

	InitWindow();
	InitBatch();
	InitSprites();
	InitShaders();
	InitTextures();
}


limEngine::Engine::~Engine()
{
	std::cout << "\nterminating\n" << std::endl;
	delete[] verticies;

	Q_Shader.Unload();
	glfwTerminate();
}


void limEngine::Engine::SetRes(int X, int Y)
{
	Xres = X;
	Yres = Y;
	ProjMatrix = glm::ortho(0.0f, (float)X, 0.0f, (float)Y,
		-1.0f, 1.0f);

	glm::vec2 res = glm::vec2((float)Width/X, (float)Height/Y);
	S_Shader.SetUniformVec2("u_resolution", res);
}


void limEngine::Engine::Draw(Square obj)
{
	// adds to queue
	int currentIndex = CurrentQuads * 4;
	float radius = obj.Size / 2.0f;
	int Xcenter = obj.X + radius;
	int Ycenter = obj.Y + radius;
	float a = -obj.angel * (M_PI/180.0f);

	// pre-angel calculations
	float Xmulti, Ymulti;

	// bottom left
	Xmulti = (-1 * cos(a)) - (-1 * sin(a));
	Ymulti = (-1 * sin(a)) + (-1 * cos(a));
	verticies[currentIndex].points = {Xcenter + radius*Xmulti, Ycenter + radius*Ymulti, 0.0f };	
	verticies[currentIndex].color = obj.color;

	// bottom right
	Xmulti = (1 * cos(a)) - (-1 * sin(a));
	Ymulti = (1 * sin(a)) + (-1 * cos(a));
	verticies[currentIndex+1].points = {Xcenter + radius * Xmulti, Ycenter + radius * Ymulti, 1.0f };	
	verticies[currentIndex+1].color = obj.color;

	// top right
	Xmulti = (1 * cos(a)) - (1 * sin(a));
	Ymulti = (1 * sin(a)) + (1 * cos(a));
	verticies[currentIndex + 2].points = { Xcenter + radius * Xmulti, Ycenter + radius * Ymulti, 1.0f };
	verticies[currentIndex+2].color = obj.color;
	
	// top left
	Xmulti = (-1 * cos(a)) - (1 * sin(a));
	Ymulti = (-1 * sin(a)) + (1 * cos(a));
	verticies[currentIndex+3].points = {Xcenter + radius * Xmulti, Ycenter + radius * Ymulti, 0.0f, };
	verticies[currentIndex+3].color = obj.color;

	CurrentQuads++;
}


void limEngine::Engine::Draw(Sprite* obj, int Group)
{
	// texture
	Texture* currentTexture = obj->getImage();

	bool found = false;
	int useSlot;
	Color useCol = obj->getColor();

	for (int n = 0; n < SlotMap.size(); n++) 
	{
		if (SlotMap[n]->GetID() == currentTexture->GetID()) 
		{
			found = true; 
			useSlot = n;
		}
	}
	if (!found) { SlotMap[currentSlot] = currentTexture; useSlot = currentSlot;	currentSlot++; }


	int* SpriteCount;
	Svertex* SpriteList;
	switch (Group) 
	{
	case(LimBottom):
		SpriteCount = &CurrentSprites0;
		SpriteList = S0_verticies;
		break;
	case(LimTop):
		SpriteCount = &CurrentSprites2;
		SpriteList = S2_verticies;
		break;
	default:
		SpriteCount = &CurrentSprites1;
		SpriteList = S1_verticies;
		break;
	}


	int currentIndex = *SpriteCount * 4;
	int Xcenter = obj->getPosition()[0] + (obj->getWidth()/2.0f);
	int Ycenter = obj->getPosition()[1] + (obj->getHeight()/2.0f);
	float a = -obj->getRotation() * (M_PI / 180.0f);
	
	float Xmulti, Ymulti;


	// needs to be changed
	float X = obj->getPosition()[0];
	float Y = obj->getPosition()[1];
	float newX, newY;
	float width = obj->getWidth();
	float height = obj->getHeight();

	float imgX = obj->getImageRect()[0];
	float imgY = obj->getImageRect()[1];
	float imgW = obj->getImageRect()[2];
	float imgH = obj->getImageRect()[3];
	float TexWidth = (float)currentTexture->GetWidth();
	float TexHeight = (float)currentTexture->GetHeight();

	newX = (-width / 2) * cos(a) - (-height / 2) * sin(a);
	newY = (-width / 2) * sin(a) + (-height / 2) * cos(a);
	SpriteList[currentIndex].points = { Xcenter + newX, Ycenter + newY, 0.0f };	// bottom left
	SpriteList[currentIndex].ID = useSlot;
	SpriteList[currentIndex].offset = { (imgX/TexWidth), (imgY / TexHeight) };
	SpriteList[currentIndex].col = useCol;

	newX = (width / 2) * cos(a) - (-height / 2) * sin(a);
	newY = (width / 2) * sin(a) + (-height / 2) * cos(a);
	SpriteList[currentIndex + 1].points = { Xcenter + newX, Ycenter + newY, 1.0f };	// bottom right
	SpriteList[currentIndex + 1].ID = useSlot;
	SpriteList[currentIndex + 1].offset = { ((imgX+imgW)/TexWidth), (imgY/TexHeight)};
	SpriteList[currentIndex + 1].col = useCol;

	newX = (width / 2) * cos(a) - (height / 2) * sin(a);
	newY = (width / 2) * sin(a) + (height / 2) * cos(a);
	SpriteList[currentIndex + 2].points = { Xcenter + newX, Ycenter + newY, 1.0f };	// top right
	SpriteList[currentIndex + 2].ID = useSlot;
	SpriteList[currentIndex + 2].offset = { ((imgX+imgW)/TexWidth), ((imgY+imgH)/TexHeight)};
	SpriteList[currentIndex + 2].col = useCol;

	newX = (-width / 2) * cos(a) - (height / 2) * sin(a);
	newY = (-width / 2) * sin(a) + (height / 2) * cos(a);
	SpriteList[currentIndex + 3].points = { Xcenter + newX, Ycenter + newY, 0.0f, };	// top left
	SpriteList[currentIndex + 3].ID = useSlot;
	SpriteList[currentIndex + 3].offset = { (imgX/TexWidth), ((imgY+imgH)/TexHeight)};
	SpriteList[currentIndex + 3].col = useCol;

	*SpriteCount = *SpriteCount + 1;
}

void limEngine::Engine::Draw(Text* obj, int Group)
{
	Sprite** charList = obj->getList();

	// literally just looping through, may write another shader in the future
	for (int i = 0; i < obj->getLength(); i++) {
		Draw(charList[i], Group);
	}
}

void limEngine::Engine::AddLight(Lightsource* light)
{
	LightList[LightAmount] = light;
	LightAmount++;
}

void limEngine::Engine::RemoveLight(Lightsource* light)
{
	int lookFor = light->ID;
	int Index = -1;
	for (int n = 0; n < LightAmount; n++) 
	{
		if (LightList[n]->ID == lookFor) 
		{
			Index = n;
		}
	}
	if (Index == -1) { return; }

	// order doesn't matter
	LightList[Index] = LightList[LightAmount - 1];
	LightAmount--;
}

void limEngine::Engine::SetDarkness(Color d)
{
	overlay = d;
}


void limEngine::Engine::Clear(Color color)
{
	glfwSwapBuffers(window);
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	CurrentQuads = 0;
	CurrentSprites0 = 0;
	CurrentSprites1 = 0;
	CurrentSprites2 = 0;

	SlotMap.clear();
	currentSlot = 0;
}


void limEngine::Engine::Render()
{
	SetSpriteUniforms();

	RenderSprites(S0_verticies, &CurrentSprites0);

	RenderQuads();

	RenderSprites(S1_verticies, &CurrentSprites1);
	
	RenderSprites(S2_verticies, &CurrentSprites2);
}

void limEngine::Engine::RenderQuads() 
{
	if (CurrentQuads != 0)
	{
		Q_Shader.SetUniformMat4f("u_MVP", ProjMatrix);

		Q_IndexBuffer.Bind();
		GL(glBindVertexArray(Q_VertexArray));
		Q_Shader.Bind();
		

		// submitting data
		GL(glBindBuffer(GL_ARRAY_BUFFER, Q_VertexBuffer));
		GL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * (CurrentQuads * 4), verticies));

		GL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(CurrentQuads * 6),
			GL_UNSIGNED_INT, nullptr));
	}
}

void limEngine::Engine::SetSpriteUniforms()
{
	// binding textures to slots
	for (int n = 0; n < SlotMap.size(); n++)
	{
		SlotMap[n]->Bind(n);
	}

	// submitting lightsources
	if (LightAmount > 0)
	{
		ProcessLights();

		S_Shader.SetUniformArrayVec2("lightPositions", LightPositions, LightAmount);
		S_Shader.SetUniformArrayFloat("lightIntensity", lightIntensity, LightAmount);
		S_Shader.SetUniformArrayFloat("lightRadii", lightRadii, LightAmount);
		S_Shader.SetUniform1f("nLights", LightAmount);
	}
	S_Shader.SetUniform4f("u_overlay", overlay.r, overlay.g, overlay.b, overlay.a);

	// submitting proj matrix
	S_Shader.SetUniformMat4f("u_MVP", ProjMatrix);
}

void limEngine::Engine::RenderSprites(Svertex* SpriteVerticies, int* SpriteCount)
{
	if (*SpriteCount != 0)
	{
		S_IndexBuffer.Bind();
		S_Shader.Bind();
		GL(glBindVertexArray(S_VertexArray));

		GL(glBindBuffer(GL_ARRAY_BUFFER, S_VertexBuffer));

		GL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Svertex) * (*SpriteCount * 4), SpriteVerticies));

		GL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(*SpriteCount * 6),
			GL_UNSIGNED_INT, nullptr));
	}
}


Texture* limEngine::Engine::LoadImage(const std::string p)
{
	return new Texture(p, currentTexID++);
}

Texture* limEngine::Engine::LoadImage(unsigned char* data, int w, int h) {
	return new Texture(data, w, h, currentTexID++);
}


Lightsource* limEngine::Engine::CreateLight()
{
	// adds ID for removal and identification
	Lightsource* newLight = new Lightsource;
	newLight->ID = lightID++;
	return newLight;
}

void limEngine::Engine::PollEvents()
{
	glfwPollEvents();

	// checking for closing window
	run = !GetKeyPress(LiminalKey_esc) && run;
}


bool limEngine::Engine::GetKeyPress(unsigned int Keycode)
{
	switch (Keycode) 
	{
	case(LiminalKey_esc):	return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;	break;
	case(LiminalKey_1):	return glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;	break;
	case(LiminalKey_2):	return glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;	break;
	case(LiminalKey_w):	return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;	break;
	case(LiminalKey_a):	return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;	break;
	case(LiminalKey_s):	return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;	break;
	case(LiminalKey_d):	return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;	break;
	case(LiminalKey_c):	return glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;	break;
	}
}


std::vector<int> limEngine::Engine::GetMousePos()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return {(int)xpos, (int)ypos};
}