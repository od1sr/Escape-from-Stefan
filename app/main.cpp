#include "StefanGraphics.h"
#include "StefanPhysics.h"
#include "BoundedPlane.h"
#include "Texture.h"
#include "Player.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Enemy.h"

void cursorMovementCallback(float d_x, float d_y, void *player);
void keyboardHandler(sgl::Player *player, float deltatime);

int main()
{
	std::srand(std::time(nullptr));
	sgl::BoundedPlane *plane;
	sgl::GameObject3D *car;
	sgl::Player *player;
	sgl::Enemy *enemy;
	sgl::PlayerSettings p_set;
	sgl::EnemySettings e_set;
	p_set.position = glm::vec3(10.f, 5.f, -10.f);
	e_set.position = p_set.position - glm::vec3(5.f, -10.f, 5.f);
	p_set.radius = e_set.radius = 1.f;
	p_set.height = e_set.height = 7.5f;
	p_set.mass = e_set.mass = 10000.f;
	p_set.view_direction = glm::vec3(0.f, 1.f, 5.f);
	e_set.view_direction = glm::vec2(0.f, -1.f);
	p_set.health_points = e_set.hp = 100;
	e_set.damage = 10;
	e_set.speed = 15.f;
	player = new sgl::Player(p_set);
	e_set.player_to_chase = player;
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
		0.f, 10.f, -20.f, 0.f, 0.f, 0.f,
		new btBoxShape(btVector3(8.f, 5.f, 10.f)), 2000.f,
		new sgl::Model("assets\\3d models\\lada\\source\\Wavefront\\testtt.obj")
	);
	e_set.model = new sgl::Model("assets\\3d models\\cool guy\\Cool guy low poly.obj");
	enemy = new sgl::Enemy(e_set);
	sgl::StefanPhysics::addObject(plane);
	sgl::StefanGraphics::addObject(car);
	sgl::StefanPhysics::addObject(car);
	sgl::StefanPhysics::addObject(player);
	sgl::StefanPhysics::addObject(enemy);
	sgl::StefanGraphics::addObject(enemy);
	std::chrono::microseconds last_time = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	while (sgl::StefanGraphics::drawScene())
	{
		std::chrono::microseconds now = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		float deltatime = (now.count() - last_time.count()) * 0.000001f;
		last_time = now;
		sgl::StefanPhysics::stepSimulation(deltatime);
		keyboardHandler(player, deltatime);
	}
	delete car->get3DModel();
	delete car;
	delete player;
	delete enemy->get3DModel();
	delete enemy;
	delete plane;
	sgl::StefanGraphics::terminate();
	sgl::StefanPhysics::terminate();
	return 0;
}

void cursorMovementCallback(float d_x, float d_y, void *player)
{
	((sgl::Player*)player)->rotateCamera(d_x * MOUSE_SESETIVITY, d_y * MOUSE_SESETIVITY);
}

void keyboardHandler(sgl::Player *player, float deltatime)
{
	char x_moving_direction = 0, z_moving_direction = 0;
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_W))
		z_moving_direction += 1;
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_S))
		z_moving_direction -= 1;
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_D))
		x_moving_direction += 1;
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_A))
		x_moving_direction -= 1;
	player->setWalking(deltatime, x_moving_direction, z_moving_direction);
	if (sgl::StefanGraphics::keyIsPressed(GLFW_KEY_SPACE))
		player->tryToJump();
}