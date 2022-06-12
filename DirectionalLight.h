#ifndef _DIRECTIONAL_LIGHT_CLASS_
#define _DIRECTIONAL_LIGHT_CLASS_

#include "Light.h"
#include "glm/vec3.hpp"

using glm::vec3;
using glm::vec4;

namespace sgl
{
    class DirectionalLight :
        public Light
    {
    private:
        vec3 direction;
        static DirectionalLight *light;
        static bool directional_draw_handler_is_set;

        static void drawDirectionalLight(Shader &shader);
        void draw(Shader &shader) const override;
    public:
        DirectionalLight(vec3 direction, vec4 ambient, vec4 diffuse, vec4 specular);
        DirectionalLight();
        void addToShaderProgram(Shader &shader) override;
        vec3 getDirection() const { return direction; };
        void setDirection(vec3 new_direction) { direction = glm::normalize(new_direction); };
        ~DirectionalLight();
    };
    vec3 rotateVector(vec3 vector, float roll = 0.f, float yaw = 0.f, float pitch = 0.f);
}

#endif // !_DIRECTIONAL_LIGHT_CLASS_