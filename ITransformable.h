#ifndef _INTERFACE_TRANSFORMABLE_
#define _INTERFACE_TRANSFORMABLE_

#include "glm/vec3.hpp"

namespace sgl
{
    class ITransformable
    {
        virtual void move(float dx, float dy, float dz) = 0;
        virtual void setCoordinates(float x, float y, float z) = 0;
        virtual glm::vec3 getCoordinates() const = 0;
        virtual void rotate(float delta_pitch, float delta_yaw, float delta_roll) = 0;
        virtual void setRotation(float pitch, float yaw, float roll) = 0;
        virtual glm::vec3 getRotation() const = 0;
    };
}

#endif // !_INTERFACE_TRANSFORMABLE_