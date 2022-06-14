#include "Light.h"
#include <string>

std::vector<void(*)(Shader&)> sgl::Light::draw_handlers;

void sgl::Light::setBaseLight(Shader &shader, std::string &index) const
{
	shader.setVec4(
		(index + ".settings.ambient").c_str(), ambient);
	shader.setVec4(
		(index + ".settings.diffuse").c_str(), diffuse);
	shader.setVec4(
		(index + ".settings.specular").c_str(), specular);
}

void sgl::Light::DrawLights(Shader &shader)
{
	int i = 0;
	while (i < draw_handlers.size())
		draw_handlers[i++](shader);
}

sgl::Light::Light(vec4 _ambient, vec4 _diffuse, vec4 _specular)
	: ambient(_ambient), diffuse(_diffuse), specular(_specular),
	is_turned_on(false)
{
}

sgl::Light::Light()	: Light(vec4(0.f), vec4(0.f), vec4(0.f))
{
}

sgl::Light::Exception::Exception(std::string msg) : message(msg)
{
}
