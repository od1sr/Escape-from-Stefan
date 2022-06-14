#ifndef _PROJECT_LIGHT_CLASS_H
#define _PROJECT_LIGHT_CLASS_H

#include "DirectionalLight.h"
#include "PointLight.h"

namespace sgl
{
    const int MAX_PROJECT_LIGHTS_NUMBER = 10;

    class ProjectLight :
        public PointLight
    {
    private:
        static int number_of_lights;
        static ProjectLight* lights[MAX_PROJECT_LIGHTS_NUMBER];
        static bool draw_handler_is_initialized;
        vec3 direction;
    public:
        float inside_angle;
        float outside_angle;

    private:
        static void drawHandler(Shader &shader);
        void draw(Shader &shader) const override;
    public:
        ProjectLight(
            float constant,
            float linear,
            float quadratic,
            vec4 ambient,
            vec4 diffuse,
            vec4 specular,
            vec3 position,
            vec3 direction,
            float inside_angle,
            float outside_angle
        );
        ProjectLight();
        vec3 getDirection() const { return direction; };
        void setDirection(vec3 new_direction) { direction = glm::normalize(new_direction); };
        void addToShaderProgram(Shader &shader) override;
        ~ProjectLight();
    };
}

#endif // !_PROJECT_LIGHT_CLASS_H