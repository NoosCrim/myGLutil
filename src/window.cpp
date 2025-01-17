#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstring>
#include <stdexcept>

#include "shader.hpp"
#include "camera.hpp"

#include "window.hpp"

static const char* __DefaultWindowShaderPrefix = R"DENOM(
#version XX0
struct _mGLuGlobal{
		mat4 projection;
		mat4 view;
		float time;
		float deltaTime;
		ivec2 viewportSize;
	};
layout(binding = 16, std430) buffer sharedShaderVars {
	_mGLuGlobal mGLuGlobal;
};

)DENOM";
static std::size_t __DefaultWindowShaderPrefixLength = sizeof(__DefaultWindowShaderPrefixLength)-1;
std::unordered_map<GLFWwindow*, mGLu::Window*> mGLu::Window::__glfwWindowToWindowMap{};

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

glm::vec2 mGLu::Window::ScaleWindowPos(glm::vec2 pos, MousePosScaling scaling)
{
	switch(scaling)
	{
		case WINDOW_CENTER_0_1_CURSOR_POS:
			return pos;
		case WINDOW_ORIGIN_0_1_CURSOR_POS:
			return 0.5f * (pos * glm::vec2(1, -1) + glm::vec2(1,1));
        case WINDOW_ORIGIN_PIXEL_CURSOR_POS:
			return ScaleWindowPos(pos, WINDOW_ORIGIN_0_1_CURSOR_POS) * glm::vec2(size);
		case WINDOW_CENTER_PIXEL_CURSOR_POS:
			return 0.5f * pos * glm::vec2(size) * glm::vec2(1, -1);
        case WINDOW_O_1_WIDTH_CURSOR_POS:
			return ScaleWindowPos(pos, WINDOW_ORIGIN_0_1_CURSOR_POS) * (float)size.x;
        case WINDOW_O_1_HEIGHT_CURSOR_POS:
			return ScaleWindowPos(pos, WINDOW_ORIGIN_0_1_CURSOR_POS) * (float)size.y;
        case WINDOW_O_1_MIN_CURSOR_POS:
			return ScaleWindowPos(pos, WINDOW_ORIGIN_0_1_CURSOR_POS) * (float)std::min(size.x, size.y);
        case WINDOW_O_1_MAX_CURSOR_POS:
			return ScaleWindowPos(pos, WINDOW_ORIGIN_0_1_CURSOR_POS) * (float)std::max(size.x, size.y);
	}
	return pos;
}

mGLu::Window::Window(unsigned int _width, unsigned int _height, const char* title, bool fullscreen, unsigned int maj, unsigned int min, bool debug) : 
	size({(float)_width, (float)_height}),
	sizeRatio((float)_width/_height),
	GLmaj(maj),
	GLmin(min),
	shaderPrefix(__DefaultWindowShaderPrefix)
{
	static GLFW_init_handler __glfwInitGlobal;
	shaderPrefix[10] = '0' + maj;
	shaderPrefix[11] = '0' + min;
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

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, nullptr);

	glfwSwapInterval(1);

	sharedShaderVars.data.viewportWidth = _width;
	sharedShaderVars.data.viewportHeight = _height;
	sharedShaderVars.Init();
	UpdateSharedShaderVars();
}
mGLu::Window::~Window()
{
	__glfwWindowToWindowMap.erase(window);
}
void mGLu::Window::__PreStart()
{
	sharedShaderVars.Init();
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
}
void mGLu::Window::StartMainLoop()
{
	__PreStart();
	Start();
	while (!glfwWindowShouldClose(window) && !m_shouldClose)
	{
		__PreUpdate();
		Update();
	}
}
void mGLu::Window::UpdateSharedShaderVars()
{
	sharedShaderVars.data.deltaTime = DeltaTime();
	sharedShaderVars.data.time = GetTime();
	sharedShaderVars.UpdateData();
}
void mGLu::Window::UseCamera(mGLu::Camera &camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, camera.fbo);
    glViewport(camera.xOffset, camera.yOffset, camera.width, camera.height);
	glScissor(camera.xOffset, camera.yOffset, camera.width, camera.height);
    sharedShaderVars.data.projection = camera.projection;
    sharedShaderVars.data.view = camera.view;
	sharedShaderVars.data.viewportWidth = camera.GetSize().x;
	sharedShaderVars.data.viewportHeight = camera.GetSize().y;
}


void mGLu::Window::_GlobalShaderVars::Init()
{
	glCreateBuffers(1, &ubo);
	glNamedBufferStorage(ubo, sizeof(_GlobalShaderVarsData), &data, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboBindingPoint, ubo);
}
void mGLu::Window::_GlobalShaderVars::UpdateData()
{
	glNamedBufferSubData(ubo, 0, sizeof(_GlobalShaderVarsData), &data);
}
const char* mGLu::Window::GetShaderPrefix(std::size_t *shaderPrefixLength) const
{
	if(shaderPrefixLength)
		*shaderPrefixLength = __DefaultWindowShaderPrefixLength;
	return shaderPrefix.data();
}