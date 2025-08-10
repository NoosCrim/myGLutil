#pragma once
#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "mouse_input.hpp"
#include "key_input.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace mGLu
{
	class Camera;
	class Window
	{
	private:
		float sizeRatio;
		glm::ivec2 size;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime, currFrameTime, startTime;
		float deltaTime, mainLoopTime;
		bool _shouldClose = false;
		GLFWwindow* window = nullptr;
		void set_mGLu_glfw_callbacks();
		static void general_glfw_scroll_callback(GLFWwindow* window, double scrollX, double scrollY);
		static void general_glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void general_glfw_cursor_pos_callback(GLFWwindow* window, double posX, double posY);
		static void general_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void general_glfw_window_size_callback(GLFWwindow* window, int width, int height);

	public:
		const unsigned int GLmaj, GLmin;
        MouseCallbackHandler mouseInput;
		KeyCallbackHandler keyInput;
		Window(unsigned int width, unsigned int height, const char* title, bool fullscreen, unsigned int maj, unsigned int min, bool debug = true);
		Window(const Window&) = delete;
		virtual ~Window();
		void StartMainLoop();

		void UseCamera(Camera &camera);

		GLFWwindow* GetWindow() const { return window; }
		inline float DeltaTime() const { return deltaTime; }
		inline float GetTime() const { return mainLoopTime; }
		float GetAspectRatio() const { return sizeRatio; }
		glm::ivec2 GetSize() const { return size; }

        void Close() { _shouldClose = true; }
        void __PreStart();
        void __PreUpdate();
	protected:
		virtual void Start(){};
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
		DefaultWindow(unsigned int _width, unsigned int _height, const char* _title, bool _fullscreen, unsigned int _maj, unsigned int _min, bool _debug = true):
			Window(_width, _height, _title, _fullscreen, _maj, _min, _debug)
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
	};
}