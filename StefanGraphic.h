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
#include "Plane.h"
#include "GameObject.h"

namespace sgl
{
	class StefanGraphic
	{
	private:
		static int screen_w;
		static int screen_h;
		//a mouse offset at last frame
		static float mouse_offset_x;
		static float mouse_offset_y;
		static float mouse_last_x;
		static float mouse_last_y;
		static GLFWwindow *win;
		static CameraFPS cam;
		static Shader shader;
		static glm::mat4 projection;
		static ProjectLight flashlight;
		static DirectionalLight directional_light;
		static Plane *plane;
		static GameObject *car;

		static void moveCamera();
		static void createWindow(const char *win_name);
		static void createShader();
		static void createLights();
	public:
		static void init(const char *window_name, int win_width, int win_height);
		static void mainLoop();
		static void terminate();
	};
}
#endif // ! _STEFAN_GRAPHICS_H_