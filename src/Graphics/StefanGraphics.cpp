#include "StefanGraphics.h"
#include "Texture.h"
#include "logging.h"
#include "config.h"
#include "StefanPhysics.h"
#include "StefanPhysics.h"
#include "Player.h"
#include "Window.h"

using sgl::StefanGraphics;

const sgl::CameraFPS *StefanGraphics::cam = NULL;
Shader StefanGraphics::game_shader, StefanGraphics::menu_shader;
glm::mat4 StefanGraphics::projection;
sgl::ProjectLight StefanGraphics::flashlight;
sgl::DirectionalLight StefanGraphics::directional_light;
std::vector<const sgl::IDrawable*> StefanGraphics::objects_to_draw;
std::vector<const sgl::BasicWidget*> StefanGraphics::interface_objects;

void StefanGraphics::init(const CameraFPS *camera)
{
	createShader();
	createLights();
	projection = glm::mat4(1.f);
	cam = camera;
}

void StefanGraphics::createShader()
{
	game_shader = Shader("shaders\\vertex shader.glsl", "shaders\\fragment shader.glsl");
	menu_shader = Shader("shaders\\menu vertex shader.glsl", "shaders\\menu fragment shader.glsl");
	Shader::CompileStatus status = game_shader.getCompilestatus();
	if (status != Shader::CompileStatus::OK)
		exit((int)status);
	status = menu_shader.getCompilestatus();
	if (status != Shader::CompileStatus::OK)
		exit((int)status);
	game_shader.use();
}

void StefanGraphics::createLights()
{
	flashlight.ambient = vec4(0.1f, 0.1f, 0.1f, 1.f);
	flashlight.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.f);
	flashlight.specular = vec4(0.5f, 0.5f, 0.5f, 1.f);
	flashlight.constant = 1.f;
	flashlight.linear = 0.014f;
	flashlight.quadratic = 0.0007;
	flashlight.setDirection(vec3(0.5f, -0.7f, 0.5f));
	flashlight.pos = vec3(-4.f, 5.f, -6.f);
	flashlight.inside_angle = glm::radians(20.f);
	flashlight.outside_angle = glm::radians(25.f);
	flashlight.turnOn();
	directional_light.setDirection(vec3(0.1f, -1.f, 0.f));
	directional_light.ambient = vec4(0.8f, 0.8f, 0.8f, 1.f);
	directional_light.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.f);
	directional_light.specular = vec4(0.1f);
	flashlight.addToShaderProgram(game_shader);
	directional_light.addToShaderProgram(game_shader);
	directional_light.turnOn();
}

void StefanGraphics::drawScene()
{
	glm::vec3 cam_direction = cam->getDirection(), cam_pos = cam->getPos();
	glm::mat4 view = cam->getViewMatrix();
	view = cam->getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f),
		(float)Window::getWindowWidth() / Window::getWindowHeight(),
		0.1f, 1000.0f); 
	GL_CALL(glClearColor(0.1f, 0.f, 0.f, 1.f));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
	game_shader.use();
	game_shader.setMat4("projection", projection);
	game_shader.setMat4("view", view);
	game_shader.setVec3("camera_pos", cam_pos);
	flashlight.setDirection(cam_direction);
	flashlight.pos = cam_pos;
	Light::DrawLights(game_shader);
	for (int i = objects_to_draw.size() - 1; i >= 0; --i)
		objects_to_draw[i]->draw(game_shader);
	drawInterface(projection);
	Window::swapBuffers();
}

void sgl::StefanGraphics::drawInterface(glm::mat4 projection)
{
	menu_shader.use();
	menu_shader.setMat4("projection", projection);
	for (int i = interface_objects.size() - 1; i >= 0; --i)
		interface_objects[i]->draw(menu_shader);
}

void sgl::StefanGraphics::setCamera(const CameraFPS *cam)
{
	sgl::StefanGraphics::cam = cam;
}

void sgl::StefanGraphics::addObject(const IDrawable *object)
{
	objects_to_draw.push_back(object);
}
