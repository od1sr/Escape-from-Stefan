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

namespace sgl
{
	class StefanGraphics
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
		static std::vector<const sgl::IDrawable*> objects_to_draw;

		static void moveCamera();
		static void createWindow(const char *win_name);
		static void createShader();
		static void createLights();
		static void framebufferSizeCallback(GLFWwindow *win, int w, int h);
		static void mouseMovementCallback(GLFWwindow *win, double xpos, double ypos);
	public:
		static void init(const char *window_name);
		static void mainLoop();
		static void terminate();
		static void addObject(const IDrawable *object);
	};
}
#endif // ! _STEFAN_GRAPHICS_H_