#include "StefanGraphics.h"
#include "StefanPhysics.h"
#include "BoundedPlane.h"
#include "Texture.h"


int main()
{
	sgl::BoundedPlane *plane;
	sgl::GameObject3D *car;
	sgl::StefanPhysics::init();
	sgl::StefanGraphics::init("Minesraft");
	sgl::BoundedPlaneSettings settings;
	sgl::Texture diffuse_texture, specular_texture;
	settings.y = -5.f;
	settings.z = -20.f;
	settings.width = settings.length = 40.f;
	settings.roll = settings.yaw = 0.f;
	settings.shininess = 64;
	settings.pitch = glm::radians(-90.f);
	loadTexture(&diffuse_texture, "assets\\textures\\box.png", sgl::TextureType::DIFFUSE);
	settings.diffuse_texture = diffuse_texture;
	loadTexture(&specular_texture, "assets\\textures\\lightmap.png", sgl::TextureType::SPECULAR);
	settings.specular_texture = specular_texture;
	plane = new sgl::BoundedPlane(settings);
	car = new sgl::GameObject3D(
		0.f, 1500.f, -20.f, 0.f, 0.f, 0.f,
		new btBoxShape(btVector3(5.f, 2.5f, 5.f)), 2000.f,
		new sgl::Model("assets\\3d models\\lada\\source\\Wavefront\\testtt.obj")
	);
	sgl::StefanGraphics::addObject(plane);
	sgl::StefanGraphics::addObject(car);
	sgl::StefanGraphics::mainLoop();
	sgl::StefanGraphics::terminate();
	delete plane;
	delete car->get3DModel();
	delete car;
	sgl::StefanPhysics::terminate();
	return 0;
}