#include "DirectionalLight.h"
#include "PointLight.h"
#include "glm/mat4x4.hpp"

bool sgl::DirectionalLight::directional_draw_handler_is_set = false;
sgl::DirectionalLight *sgl::DirectionalLight::light = NULL;

sgl::DirectionalLight::DirectionalLight(vec3 _direction, vec4 _ambient, vec4 _diffuse, vec4 _specular)
	:	Light(_ambient, _diffuse, _specular), direction(glm::normalize(_direction))
{
	if (DirectionalLight::light != NULL)
		throw sgl::MaxNumberOfLightsExceeded("Max number of directional lights = 1", 1);
	if (!directional_draw_handler_is_set)
	{
		Light::draw_handlers.push_back(drawDirectionalLight);
		directional_draw_handler_is_set = true;
	}
}

sgl::DirectionalLight::DirectionalLight() : DirectionalLight(vec3(0.f), vec4(0.f), vec4(0.f), vec4(0.f))
{
}

void sgl::DirectionalLight::addToShaderProgram(Shader &shader)
{
	static std::string name = "d_light";
	light = this;
	shader.setVec3("d_light.direction", direction);
	Light::setBaseLight(shader, name);
}

vec3 sgl::rotateVector(vec3 vector, float roll, float yaw, float pitch)
{
	glm::mat4 rotation = glm::mat4(1.f);
	glm::vec4 new_direction =  vec4(vector, 0.f);
	rotation = glm::rotate(rotation, roll, vec3(1.f, 0.f, 0.f));
	rotation = glm::rotate(rotation, yaw, vec3(0.f, 1.f, 0.f));
	rotation = glm::rotate(rotation, pitch, vec3(0.f, 0.f, 1.f));
	new_direction = rotation * new_direction;
	return vec3(new_direction);
}

void sgl::DirectionalLight::draw(Shader &shader) const
{
	std::string name = "d_light";
	setBaseLight(shader, name);
	shader.setVec3("d_light.direction", direction);
}

void sgl::DirectionalLight::drawDirectionalLight(Shader &shader)
{
	if(light)
	{
		shader.setBool("directional_light_is_using", light->is_turned_on);
		light->draw(shader);
	}
	else
		shader.setBool("directional_light_is_using", false);
}

sgl::DirectionalLight::~DirectionalLight()
{
	light = NULL;
}