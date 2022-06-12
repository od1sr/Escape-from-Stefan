#ifndef _LIGHT_CLASS_
#define _LIGHT_CLASS_

#include "glm/vec4.hpp"
#include "Shader.h"
#include <string>
#include <vector>

using glm::vec4;

namespace sgl
{

	class Light
	{
	public:
		class Exception
		{
		public:
			std::string message;

			Exception(std::string msg);
		};

		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
	protected:
		bool is_turned_on;
		static std::vector<void(*)(Shader&)> draw_handlers;

		void setBaseLight(Shader &shader, std::string &index) const;
		virtual void draw(Shader &shader) const = 0;
	public:
		static void DrawLights(Shader &shader);
		Light(vec4 ambient, vec4 diffuse, vec4 specular);
		Light();
		virtual void addToShaderProgram(Shader &shader) = 0;
		void turnOn() { is_turned_on = true; };
		void turnOff() { is_turned_on = false; };
	};

}

#endif // !_LIGHT_CLASS_