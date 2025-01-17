#pragma once
#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <cstdio>
#include "shader.hpp"
#include "mouse_input.hpp"
#include "key_input.hpp"
#include "basic_shapes.hpp"

class GLFWwindow;
namespace mGLu
{
	enum MousePosScaling : unsigned char
    {
        WINDOW_ORIGIN_PIXEL_CURSOR_POS,
        WINDOW_CENTER_PIXEL_CURSOR_POS,
        WINDOW_ORIGIN_0_1_CURSOR_POS,
        WINDOW_CENTER_0_1_CURSOR_POS,
        WINDOW_O_1_WIDTH_CURSOR_POS,
        WINDOW_O_1_HEIGHT_CURSOR_POS,
        WINDOW_O_1_MIN_CURSOR_POS,
        WINDOW_O_1_MAX_CURSOR_POS

    };
	class Camera;
	class Window
	{
	private:
		float sizeRatio;
		glm::ivec2 size;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime, currFrameTime, startTime;
		float deltaTime, mainLoopTime;
		bool m_shouldClose = false;
		GLFWwindow* window = nullptr;
		static std::unordered_map<GLFWwindow*, Window*> __glfwWindowToWindowMap;
		void set_mGLu_glfw_callbacks();
		static void general_glfw_scroll_callback(GLFWwindow* window, double scrollX, double scrollY);
		static void general_glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void general_glfw_cursor_pos_callback(GLFWwindow* window, double posX, double posY);
		static void general_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void general_glfw_window_size_callback(GLFWwindow* window, int width, int height);
		std::string shaderPrefix;
		struct _GlobalShaderVars
		{
			friend class Window;
		private:
			GLuint ubo;
			static constexpr unsigned int ssboBindingPoint = 16;
			struct _GlobalShaderVarsData
			{
				friend class Window;
			private:
				glm::mat4 projection{1};
				glm::mat4 view{1};
				float time = 0.0f;
				float deltaTime = 0.0f;
				int viewportWidth, viewportHeight;
			} data;
			void Init();
			void UpdateData();
		};
		


	public:
		glm::vec2 ScaleWindowPos(glm::vec2 center_01_window_pos, MousePosScaling scaling);
		_GlobalShaderVars sharedShaderVars;
		const unsigned int GLmaj, GLmin;
		Window(unsigned int width, unsigned int height, const char* title, bool fullscreen, unsigned int maj, unsigned int min, bool debug = true);
		Window(const Window&) = delete;
		virtual ~Window();
		void StartMainLoop();

		void UpdateSharedShaderVars();
		void UseCamera(Camera &camera);

		virtual const char* GetShaderPrefix(std::size_t *outPrefixLength) const;
		GLFWwindow* GetWindow() const { return window; }
		inline float DeltaTime() const { return deltaTime; }
		inline float GetTime() const { return mainLoopTime; }
		float GetAspectRatio() const { return sizeRatio; }
		glm::ivec2 GetSize() const { return size; }
	protected:
		void Close() { m_shouldClose = true; }
		virtual void __PreStart();
		virtual void Start(){};
		virtual void __PreUpdate();
		virtual void Update(){};
		virtual void scroll_callback(double scrollX, double scrollY){};
		virtual void mouse_button_callback(int button, int action, int mods){};
		virtual void cursor_pos_callback(double posX, double posY){};
		virtual void key_callback(int key, int scancode, int action, int mods){};
		virtual void window_size_callback(int width, int height);
	};
	
	class DefaultWindow : public Window
	{
	public:
		BasicShapeHandlers basicShapeHandlers;
		MouseCallbackHandler mouseInput;
		KeyCallbackHandler keyInput;
		DefaultWindow(unsigned int _width, unsigned int _height, const char* _title, bool _fullscreen, unsigned int _maj, unsigned int _min, bool _debug = true):
			Window(_width, _height, _title, _fullscreen, _maj, _min, _debug),
			basicShapeHandlers(this)
		{

		}
	protected:
		virtual void scroll_callback(double scrollX, double scrollY)
		{
			mouseInput.UpdateMouseScroll(glm::vec2(scrollX, scrollY));
		};
		virtual void mouse_button_callback(int button, int action, int mods)
		{
			mouseInput.HandleMouseInput(button, action);
		};
		virtual void cursor_pos_callback(double posX, double posY)
		{
			mouseInput.UpdateMousePos(glm::vec2(posX/GetSize().x-0.5f, -posY/GetSize().y+0.5f) * 2.f);
		};
		virtual void key_callback(int key, int scancode, int action, int mods)
		{
			keyInput.HandleKeyInput(key, action);
		};
		virtual void __PreUpdate()
		{
			mouseInput.Clear1FrameStates();
			keyInput.ClearUnpressedKeys();
			Window::__PreUpdate();
		}
	};
}
