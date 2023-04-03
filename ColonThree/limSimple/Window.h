#include "src/Texture.h"
#include "GLFW/glfw3.h"

class Window {
private:
	GLFWwindow* window;
	VertexArray* vArr;	// pointers so instantiation can be done in order
	Buffer* buff;
	IndexBuffer* iBuff;
	Shader* shader;
	Texture* currentTexture;
	bool firstLoad;

	struct pointVertex {
		float x, y, im1, im2;
	};
	pointVertex* quadData;
	bool badInit;

	const std::string title;
	int width, height;
	bool close;

	// Loading
	void initContext() {
		if (!glfwInit()) {
			badInit = 1;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, &title[0], NULL, NULL);
		if (!window) {
			badInit = 1;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {
			badInit = 1;
		}
		println("[LimSimple] openGL VERSION : " << glGetString(GL_VERSION));
	}
	void initRendering() {
		quadData = new pointVertex[4];
		quadData[0] = { -1.0f, -1.0f, 0.0f, 0.0f };		// bottom left(0)
		quadData[1] = { 1.0f, -1.0f, 1.0f, 0.0f };		// Bottom Right(1)	
		quadData[2] = { 1.0f, 1.0f, 1.0f, 1.0f };			// Top Right(2)
		quadData[3] = { -1.0f, 1.0f, 0.0f, 1.0f };			// Top Left(3)

		vArr = new VertexArray();
		vArr->Bind();

		buff = new Buffer();
		buff->Bind();
		buff->submitData(sizeof(pointVertex) * 4, quadData);

		iBuff = new IndexBuffer(1);
		iBuff->Bind();

		vArr->addAttribute(2, GL_FLOAT, sizeof(pointVertex), (const void*)offsetof(pointVertex, pointVertex::x));
		vArr->addAttribute(2, GL_FLOAT, sizeof(pointVertex), (const void*)offsetof(pointVertex, pointVertex::im1));

		shader = new Shader();
		shader->Bind();
		shader->setUniform4f("u_color", { 0.0f, 0.0f, 0.0f, 1.0f });

		currentTexture = new Texture(shader);		// nothing loaded yet
	}

	// Display (not done in a loop)
	void updateDisplay() {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glfwSwapBuffers(window);
	}
public:
	// Constructor / Destructor
	Window(const std::string t, int w, int h) : badInit(0), close(0), title(t), width(w), height(h), firstLoad(1) {
		initContext();
		initRendering();
		updateDisplay();
	}
	~Window() {
		delete vArr;	// pointers so instantiation can be done in order
		delete buff;
		delete iBuff;
		delete currentTexture; 
		delete shader;
		glfwTerminate();
	}

	// User input
	void displayImage(std::string& path) {
		if (!firstLoad) {
			currentTexture->reset();
			firstLoad = 0;
		}
		currentTexture->loadFromPath(path);
		currentTexture->Bind();
		updateDisplay();
	}
	void displayImage(unsigned char* data, int w, int h, int BPP) {
		if (!firstLoad) {
			currentTexture->reset();
			firstLoad = 0;
		}
		currentTexture->loadFromSource(data, w, h, BPP);
		currentTexture->Bind();
		updateDisplay();
	}

	// Polling
	void pollOnce() {
		glfwPollEvents();
	}
	void closeWindow() {
		close = 1;
	}	

	// Getters
	bool shouldClose() {
		return close || (glfwWindowShouldClose(window));
	}
	bool goodInit() {
		return !badInit;
	}

};
