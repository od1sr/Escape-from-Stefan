#ifndef  _STEFAN_GRAPHICS_H_
#define  _STEFAN_GRAPHICS_H_

#include "glad/glad.h"
#include "CameraFPS.h"
#include "Shader.h"
#include "VAO.h"
#include "glm/glm.hpp"
#include "Model.h"
#include "ProjectLight.h"
#include "DirectionalLight.h"
#include "GLFW/glfw3.h"
#include "GameObject3D.h"
#include "IDrawable.h"
#include <vector>
#include "Player.h"

namespace sgl
{
	typedef void (*CursorMovementCallback) (float delta_x, float delta_y, void *user_pointer);

	class StefanGraphics
	{
	private:
		static int screen_w;
		static int screen_h;
		//a mouse offset at last frame
		static float mouse_last_x;
		static float mouse_last_y;
		static GLFWwindow *win;
		static const CameraFPS *cam;
		static CursorMovementCallback cursor_callback;
		static void *user_pointer_for_cursor_callback;
		static Shader shader;
		static glm::mat4 projection;
		static ProjectLight flashlight;
		static DirectionalLight directional_light;
		static std::vector<const sgl::IDrawable*> objects_to_draw;

		static void moveCamera();
		static void createWindow(const char *win_name);
		static void createShader();
		static void createLights();
		static void framebufferSizeCallback(GLFWwindow *win, int w, int h);
		static void mouseMovementCallback(GLFWwindow *win, double xpos, double ypos);
	public:
		static void init(const char *window_name, const CameraFPS *camera);
		static bool drawScene(); // returns false if window is closed
		static void terminate();
		static void setCamera(const CameraFPS *cam);
		static void setCursorMovementCallback(CursorMovementCallback callback, void *user_pointer = NULL);
		static void addObject(const IDrawable *object);
		inline static bool keyIsPressed(int key) 
		{ return glfwGetKey(win, key); }
	};
}
#endif // ! _STEFAN_GRAPHICS_H_