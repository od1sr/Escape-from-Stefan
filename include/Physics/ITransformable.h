#ifndef _INTERFACE_TRANSFORMABLE_
#define _INTERFACE_TRANSFORMABLE_

#include "glm/vec3.hpp"
#include "config.h"

namespace sgl
{
    class StefanPhysics;

    class ITransformable
    {
        friend class StefanPhysics;
    protected:
        btRigidBody *rigid_body;

        virtual void update(float time_passed_since_last_update) = 0; //seconds
        virtual void collideCallback(ITransformable *other, btManifoldPoint &cp) = 0;
    public:
        virtual void move(float dx, float dy, float dz) = 0;
        virtual void setCoordinates(float x, float y, float z) = 0;
        virtual glm::vec3 getCoordinates() const = 0;
        virtual void rotate(float pitch, float yaw, float roll) = 0;
        virtual glm::vec3 getRotation() const = 0;
        virtual float getMass() const = 0;
        virtual void setMass(float m) = 0;
        virtual objectID getObjectID() const = 0; // IDs can be mixed using the '|' operator for the inheritance's sake
    };
}

#endif // !_INTERFACE_TRANSFORMABLE_