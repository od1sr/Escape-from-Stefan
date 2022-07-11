#include "StefanGraphics.h"
#include "StefanPhysics.h"
#include "BoundedPlane.h"
#include "Texture.h"
#include "Player.h"

void cursorMovementCallback(float d_x, float d_y, void *player);
void keyboardHandler(sgl::Player* player);

int main()
{
	sgl::BoundedPlane *plane;
	sgl::GameObject3D *car;
	sgl::Player *player;
	sgl::PlayerSettings p_set{ glm::vec3(10.f, 100.f, -10.f), 1.f, 7.5f, 100.f, glm::vec3(0.f, 1.f, 5.f)};
	player = new sgl::Player(p_set);
	sgl::StefanPhysics::init();
	sgl::StefanGraphics::init("Minesraft", player->getCamera());
	sgl::StefanGraphics::setCursorMovementCallback(cursorMovementCallback, player);
	sgl::BoundedPlaneSettings settings;
	sgl::Texture diffuse_texture, specular_texture;
	settings.y = -5.f;
	settings.z = -20.f;
	settings.width = settings.length = 400.f;
	settings.roll = settings.yaw = 0.f;
	settings.shininess = 64;
	settings.pitch = glm::radians(-90.f);
	loadTexture(&diffuse_texture, "assets\\textures\\box.png", sgl::TextureType::DIFFUSE);
	settings.diffuse_texture = diffuse_texture;
	loadTexture(&specular_texture, "assets\\textures\\lightmap.png", sgl::TextureType::SPECULAR);
	settings.specular_texture = specular_texture;
	plane = new sgl::BoundedPlane(settings);
	sgl::StefanGraphics::addObject(plane);
	car = new sgl::GameObject3D(
		0.f, 500.f, -20.f, 0.f, 0.f, 0.f,
		new btBoxShape(btVector3(5.f, 2.5f, 5.f)), 2000.f,
		new sgl::Model("assets\\3d models\\lada\\source\\Wavefront\\testtt.obj")
	);
	sgl::StefanGraphics::addObject(car);
	sgl::StefanPhysics::addObject(plane);
	sgl::StefanPhysics::addObject(car);
	sgl::StefanPhysics::addObject(player);
	while (sgl::StefanGraphics::drawScene())
	{
		sgl::StefanPhysics::stepSimulation();
		keyboardHandler(player);
	}
	delete plane;
	delete car->get3DModel();
	delete car;
	delete player;
	sgl::StefanGraphics::terminate();
	sgl::StefanPhysics::terminate();
	return 0;
}

void cursorMovementCallback(float d_x, float d_y, void *player)
{
	((sgl::Player*)player)->rotateCamera(d_x * MOUSE_SESETIVITY, d_y * MOUSE_SESETIVITY);
}

void keyboardHandler(sgl::Player *player)
{
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_W))
		player->moveDirectionaly(1, 0);
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_S))
		player->moveDirectionaly(-1, 0);
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_D))
		player->moveDirectionaly(0, 1);
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_A))
		player->moveDirectionaly(0, -1);
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_SPACE))
		player->tryToJump();
	glm::vec3 pos = player->getCoordinates();
}