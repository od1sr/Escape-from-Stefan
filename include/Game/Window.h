#ifndef _WINDOWS_CLASS_
#define _WINDOWS_CLASS_

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace sgl
{
	typedef void (*CursorMovementCallback) (float delta_x, float delta_y, void* user_pointer);
	
	class Window
	{
	public:
		static const int FULL_SCREEN = -1;
	private:
		//a mouse offset at last frame
		static float mouse_last_x;
		static float mouse_last_y;
		static GLFWwindow *win;
		static CursorMovementCallback cursor_callback;
		static void *user_pointer_for_cursor_callback;
		static int screen_w;
		static int screen_h;
		static bool is_paused;
		static bool pressed_esc_recently;
		static bool opened_settings;

		static void framebufferSizeCallback(GLFWwindow *win, int w, int h);
		static void mouseMovementCallback(GLFWwindow *win, double xpos, double ypos);
	
	public:
		static void init(const char *window_name, int height = FULL_SCREEN, int width = FULL_SCREEN);
		static void setCursorMovementCallback(CursorMovementCallback callback, void* user_pointer = NULL);
		inline static bool keyIsPressed(int key) { return glfwGetKey(win, key); }
		static void handleEvents();
		inline static bool windowShouldBeClosed() { return glfwWindowShouldClose(win); };
		inline static bool isPaused() { return is_paused; };
		static void terminate();
		inline static int getWindowWidth() { return screen_w; }
		inline static int getWindowHeight() { return screen_h; }
		inline static void swapBuffers() { glfwSwapBuffers(win); }
	};
}
#endif // !_WINDOWS_CLASS_
