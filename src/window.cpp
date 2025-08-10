#include <glm/glm.hpp>
#include <cstring>
#include <stdexcept>

#include "window.hpp"
static std::unordered_map<GLFWwindow*, mGLu::Window*> __glfwWindowToWindowMap{};

static void glfw_error_callback(int error, const char* description)
{
	std::fprintf(stderr, "GLFW error: %s\n", description);
}
static void GLAPIENTRY gl_error_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
{
	if(type == GL_DEBUG_TYPE_ERROR)
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
}
void mGLu::Window::set_mGLu_glfw_callbacks()
{
	glfwSetScrollCallback(window, general_glfw_scroll_callback);
	glfwSetCursorPosCallback(window, general_glfw_cursor_pos_callback);
	glfwSetKeyCallback(window, general_glfw_key_callback);
	glfwSetMouseButtonCallback(window, general_glfw_mouse_button_callback);
	glfwSetWindowSizeCallback(window, general_glfw_window_size_callback);
}
void mGLu::Window::general_glfw_scroll_callback(GLFWwindow* window, double scrollX, double scrollY)
{
	//Window::__glfwWindowToWindowMap[window]->mouseScroll += glm::vec2((float)scrollX, (float)scrollY);
	if(__glfwWindowToWindowMap.contains(window))
		__glfwWindowToWindowMap[window]->scroll_callback(scrollX, scrollY);
}
void mGLu::Window::general_glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(__glfwWindowToWindowMap.contains(window))
		__glfwWindowToWindowMap[window]->mouse_button_callback(button, action, mods);
}
void mGLu::Window::general_glfw_cursor_pos_callback(GLFWwindow* window, double posX, double posY)
{
	if(__glfwWindowToWindowMap.contains(window))
		__glfwWindowToWindowMap[window]->cursor_pos_callback(posX, posY);
}
void mGLu::Window::general_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(__glfwWindowToWindowMap.contains(window))
		__glfwWindowToWindowMap[window]->key_callback(key, scancode, action, mods);
}
void mGLu::Window::general_glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	if(__glfwWindowToWindowMap.contains(window))
		__glfwWindowToWindowMap[window]->window_size_callback(width, height);
}
void mGLu::Window::window_size_callback(int width, int height)
{
	size = {width, height};
	sizeRatio = (float)width / height;
}
struct GLFW_init_handler
{
	static bool initDone;
	GLFW_init_handler()
	{
		if(initDone) return;
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			throw std::runtime_error("GLFW Initialization failed!");
		initDone = true;
	}
	~GLFW_init_handler()
	{
		if(initDone)
			glfwTerminate();
	}
};
bool GLFW_init_handler::initDone = false;

mGLu::Window::Window(unsigned int _width, unsigned int _height, const char* title, bool fullscreen, unsigned int maj, unsigned int min, bool debug) : 
    sizeRatio((float)_width/_height),
    size({(float)_width, (float)_height}),
	GLmaj(maj),
	GLmin(min)
{
	static GLFW_init_handler __glfwInitGlobal;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 0);
	
	window = glfwCreateWindow(_width, _height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW window creation failed!");
	}
	
	__glfwWindowToWindowMap[window] = this;
	
	glfwMakeContextCurrent(window);

	glewInit();

    if(debug)
    {
        glEnable(GL_DEBUG_OUTPUT);
	    glDebugMessageCallback(gl_error_callback, nullptr);
    }

	glfwSwapInterval(1);
}
mGLu::Window::~Window()
{
	__glfwWindowToWindowMap.erase(window);
}
void mGLu::Window::__PreStart()
{
	set_mGLu_glfw_callbacks();
	startTime = std::chrono::high_resolution_clock::now();
	lastFrameTime = std::chrono::high_resolution_clock::now();
}
void mGLu::Window::__PreUpdate()
{
	glfwSwapBuffers(window);
	
	glfwPollEvents();

	currFrameTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<float>(currFrameTime - lastFrameTime).count();
	mainLoopTime = std::chrono::duration<float>(currFrameTime - startTime).count();
	lastFrameTime = currFrameTime;

    mouseInput.Clear1FrameStates();
	keyInput.ClearUnpressedKeys();
}
void mGLu::Window::StartMainLoop()
{
	__PreStart();
	Start();
	while (!glfwWindowShouldClose(window) && !_shouldClose)
	{
		__PreUpdate();
		Update();
	}
}
void mGLu::Window::UseCamera(mGLu::Camera &camera)
{
	// TODO
}