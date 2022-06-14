#ifndef _POINT_LIGHT_CLASS_
#define _POINT_LIGHT_CLASS_

#include "Light.h"
#include "glm/vec3.hpp"
#include <string>

using glm::vec3;

namespace sgl
{
    const int MAX_POINT_LIGHTS_NUMBER = 10;

    class MaxNumberOfLightsExceeded :
        Light::Exception
    {
    public:
        int current_number_of_lights;

        MaxNumberOfLightsExceeded(std::string message, int current_number_of_lights);
    };

    class PointLight :
        public Light
    {
    private:
        static int number_of_lights;
        static PointLight* lights[MAX_POINT_LIGHTS_NUMBER];
        static bool draw_handler_is_initialized;
    public:
        float constant;
        float linear;
        float quadratic;
        vec3 pos;
    protected:
        int index;

        virtual void draw(Shader &shader) const override;
        void setForce(Shader &shader, std::string &index) const;
    private:
        static void drawPointLights(Shader &shader);
    protected:
        void addNewLightAndSetIndex(PointLight **lights, int *lights_number, int max_lights_number);
        static void moveTurnedOffLightToTail(int light_index, PointLight **lights, int lights_number);
    public:
        PointLight(float constant, float linear, float quadratic, 
            vec4 ambient, vec4 diffuse, vec4 specular, vec3 position);
        PointLight();
        virtual void addToShaderProgram(Shader &shader) override;
        ~PointLight();
    };

}

#endif // !_POINT_LIGHT_CLASS_