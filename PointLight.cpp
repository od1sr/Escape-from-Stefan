#include "PointLight.h"
#include <string>

int sgl::PointLight::number_of_lights = 0;
sgl::PointLight *sgl::PointLight::lights[MAX_POINT_LIGHTS_NUMBER];
bool sgl::PointLight::draw_handler_is_initialized = false;

void sgl::PointLight::drawPointLights(Shader &shader)
{
	int current_p_lights_num = 0;
	for (int i = 0; i < number_of_lights; i++)
	{
		if (!lights[i]->is_turned_on)
			moveTurnedOffLightToTail(i, lights, number_of_lights);
		if (lights[i]->is_turned_on)
		{
			lights[i]->draw(shader);
			++current_p_lights_num;
		}
		else
			break; // there are no one more turned on lights
	}
	if(current_p_lights_num != number_of_lights)
		shader.setInt("p_light_num", current_p_lights_num);
}

void sgl::PointLight::moveTurnedOffLightToTail(int light_index, PointLight **lights, int lights_number)
{
	for (int current_index = light_index; current_index < lights_number; ++current_index)
		if (lights[current_index]->is_turned_on)
		{
			PointLight *temp_light = lights[current_index];
			lights[current_index] = lights[light_index];
			lights[light_index] = temp_light;
			lights[lights_number]->index = current_index;
			lights[current_index]->index = light_index;
		}
}

void sgl::PointLight::addNewLightAndSetIndex(
	PointLight **lights, int *lights_number, int max_lights_number)
{
	if (*lights_number >= MAX_POINT_LIGHTS_NUMBER)
		throw MaxNumberOfLightsExceeded(
			"Max number of lights exceeded. Current number of lights = " + std::to_string(*lights_number),
			*lights_number
		);
	index = *lights_number;
	++(*lights_number);
	lights[index] = this;
}

sgl::PointLight::PointLight(float _const, float _linear, float _quadratic, 
	vec4 _ambient, vec4 _diffuse, vec4 _specular, vec3 position
) : Light(ambient, diffuse, specular), constant(_const), linear(_linear), quadratic(_quadratic), 
	pos(position), index(-1)
{
	if (!draw_handler_is_initialized)
	{
		Light::draw_handlers.push_back(drawPointLights);
		draw_handler_is_initialized = true;
	}
}

sgl::PointLight::PointLight() : PointLight(0.f, 0.f, 0.f, vec4(0.f), vec4(0.f), vec4(0.f), vec3(0.f))
{
}

void sgl::PointLight::addToShaderProgram(Shader &shader)
{
	addNewLightAndSetIndex(PointLight::lights, &PointLight::number_of_lights, MAX_POINT_LIGHTS_NUMBER);
	draw(shader);
}

void sgl::PointLight::draw(Shader &shader) const
{
	std::string str_index = "p_lights[" + std::to_string(index) + "]";
	Light::setBaseLight(shader, str_index);
	shader.setVec3((str_index + ".pos").c_str(), pos);
	setForce(shader, str_index);
}

void sgl::PointLight::setForce(Shader &shader, std::string &index) const
{
	shader.setFloat(
		(index + ".force.constant").c_str(), constant);
	shader.setFloat(
		(index + ".force.linear").c_str(), linear);
	shader.setFloat(
		(index + ".force.quadratic").c_str(), quadratic);
}

sgl::PointLight::~PointLight()
{
	is_turned_on = false;
	moveTurnedOffLightToTail(index, lights, number_of_lights);
	--number_of_lights;
}

sgl::MaxNumberOfLightsExceeded::MaxNumberOfLightsExceeded(std::string message, int _current_number_of_lights)
	:	Light::Exception(message), current_number_of_lights(_current_number_of_lights)
{
}