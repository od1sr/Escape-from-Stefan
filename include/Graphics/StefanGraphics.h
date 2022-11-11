#ifndef  _STEFAN_GRAPHICS_H_
#define  _STEFAN_GRAPHICS_H_

#include "CameraFPS.h"
#include "Shader.h"
#include "VAO.h"
#include "glm/glm.hpp"
#include "Model.h"
#include "ProjectLight.h"
#include "DirectionalLight.h"
#include "GameObject3D.h"
#include "IDrawable.h"
#include <vector>
#include "Player.h"
#include "Widgets.h"

namespace sgl
{
	class StefanGraphics
	{
	private:
		static const CameraFPS *cam;
		static Shader game_shader;
		static Shader menu_shader;
		static glm::mat4 projection;
		static ProjectLight flashlight;
		static DirectionalLight directional_light;
		static std::vector<const IDrawable*> objects_to_draw;
		static std::vector<const BasicWidget*> interface_objects;

		static void createShader();
		static void createLights();
		static void drawInterface(glm::mat4 projection);
	public:
		static void init(const CameraFPS *camera);
		static void drawScene();
		static void setCamera(const CameraFPS *cam);
		static void addObject(const IDrawable *object);
	};
}
#endif // ! _STEFAN_GRAPHICS_H_