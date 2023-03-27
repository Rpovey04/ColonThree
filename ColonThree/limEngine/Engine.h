#pragma once
#include "Shader.h"

namespace limEngine
{
	class Engine
	{
	private:
		// settings
		int Width;
		int Height;
		std::string Title;
		bool run;

		// window variables
		GLFWwindow* window;
		glm::mat4 ProjMatrix;
		bool fullscreen;
		int Xres, Yres;

		// handling quads
		int numQuads;
		Vertex* verticies;
		int CurrentQuads;

		// ID	(quads)
		unsigned int Q_VertexBuffer;
		unsigned int Q_VertexArray;
		IndexBuffer Q_IndexBuffer;
		Shader Q_Shader;
		//	(sprites)
		unsigned int S_VertexBuffer;
		unsigned int S_VertexArray;
		IndexBuffer S_IndexBuffer;
		Shader S_Shader;
		int numSprites;

		Svertex* S0_verticies;
		Svertex* S1_verticies;
		Svertex* S2_verticies;
		int CurrentSprites0;
		int CurrentSprites1;
		int CurrentSprites2;


		// textures
		int AvailableTextures;
		int currentTexID;
		std::unordered_map<int, Texture*> SlotMap;
		int currentSlot;


		// lighting
		Lightsource* LightList[50];
		glm::vec2 LightPositions[50];
		float lightIntensity[50];
		float lightRadii[50];
		Color overlay;
		int lightID;

		int LightAmount;

		// initialisation functions
		void InitWindow();
		void InitBatch();
		void InitSprites();
		void InitShaders();
		void InitTextures();

		void ProcessLights();

	public:
		Engine(int W, int H, std::string T, unsigned int special = 0);
		~Engine();

		void SetRes(int X, int Y);

		void Draw(Sprite* obj, int Group = LimMid);

		void AddLight(Lightsource* light);
		void RemoveLight(Lightsource* light);
		void SetDarkness(Color d);

		void Clear(Color color);
		void Render();
		void RenderQuads();
		void SetSpriteUniforms();
		void RenderSprites(Svertex* SpriteVerticies, int* SpriteCount);

		bool isOpen() { return run; }
		void Close() { run = 0; }

		// creator functions
		Texture* LoadImage(const std::string p);
		Texture* LoadImage(unsigned char* data, int w, int h );
		Lightsource* CreateLight();

		// POLLING	(tempory, build in for this build but can be abstracted)
		void PollEvents();
		bool GetKeyPress(unsigned int KeyCode);
		std::vector<int> GetMousePos();
		bool GetMouseClickedLeft() { return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS; }

	};

	class Clock
	{
	private:
		// pausing
		int FrameRate;
		// framerate
		int TimesCalled;
		int ST;

	public:
		Clock() {}
		void Init(int F)
		{
			ST = time(NULL);
			FrameRate = F;
		}

		void Tick()
		{
			TimesCalled++;
			std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 / FrameRate)));
		}

		void Report()
		{
			if (time(NULL) == ST + 1)
			{
				std::cout << "FPS: " << TimesCalled << std::endl;
				TimesCalled = 0;
				ST = time(NULL);
			}
		}
	};
}
