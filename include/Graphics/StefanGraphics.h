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

namespace sgl
{
	class StefanGraphics
	{
	private:
		static const CameraFPS *cam;
		static Shader shader;
		static glm::mat4 projection;
		static ProjectLight flashlight;
		static DirectionalLight directional_light;
		static std::vector<const sgl::IDrawable*> objects_to_draw;

		static void createShader();
		static void createLights();
	public:
		static void init(const CameraFPS *camera);
		static void drawScene();
		static void setCamera(const CameraFPS *cam);
		static void addObject(const IDrawable *object);
	};
}
#endif // ! _STEFAN_GRAPHICS_H_