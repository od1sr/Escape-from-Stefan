#include "ProjectLight.h"

using namespace sgl;

int ProjectLight::number_of_lights = 0;
ProjectLight *sgl::ProjectLight::lights[MAX_PROJECT_LIGHTS_NUMBER];
bool ProjectLight::draw_handler_is_initialized = false;

ProjectLight::ProjectLight(
	float constant, 
	float linear, 
	float quadratic, 
	vec4 ambient, 
	vec4 diffuse, 
	vec4 specular, 
	vec3 position, 
	vec3 _direction, 
	float _inside_angle,
	float _outside_angle

) : PointLight(constant, linear, quadratic, ambient, diffuse, specular, position), direction(_direction), 
	inside_angle(_inside_angle), outside_angle(_outside_angle)
{
}

ProjectLight::ProjectLight() : ProjectLight(0.f, 0.f, 0.f, vec4(0.f), vec4(0.f), vec4(0.f), vec3(0.f), vec3(0.f), 0.f, 0.f)
{
	if (!draw_handler_is_initialized)
	{
		Light::draw_handlers.push_back(drawHandler);
		draw_handler_is_initialized = true;
	}
}

void ProjectLight::addToShaderProgram(Shader &shader)
{
	addNewLightAndSetIndex((PointLight**)lights, &number_of_lights, MAX_PROJECT_LIGHTS_NUMBER);
	draw(shader);
}

void ProjectLight::drawHandler(Shader &shader)
{
	int current_p_lights_num = 0;
	for (int i = 0; i < number_of_lights; i++)
	{
		if (!lights[i]->is_turned_on)
			moveTurnedOffLightToTail(i, (PointLight**)lights, number_of_lights);
		if (lights[i]->is_turned_on)
		{
			lights[i]->draw(shader);
			++current_p_lights_num;
		}
		else
			break; // there are no one more turned on lights
	}
	shader.setInt("prj_light_num", current_p_lights_num);
}

void ProjectLight::draw(Shader &shader) const
{
	std::string str_index = "prj_lights[" + std::to_string(index) + "]";
	setBaseLight(shader, str_index);
	setForce(shader, str_index);
	shader.setVec3((str_index + ".pos").c_str(), pos);
	shader.setVec3((str_index + ".direction").c_str(), direction);
	shader.setFloat((str_index + ".inside_angle").c_str(), inside_angle);
	shader.setFloat((str_index + ".outside_angle").c_str(), outside_angle);
}

sgl::ProjectLight::~ProjectLight()
{
	is_turned_on = false;
	moveTurnedOffLightToTail(index, (PointLight**)lights, number_of_lights);
	--number_of_lights;
}