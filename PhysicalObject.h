#ifndef _PHYSICAL_OBJECT_CLASS_
#define _PHYSICAL_OBJECT_CLASS_

#include "IDrawable.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "ITransformable.h"
#include "btBulletCollisionCommon.h"

namespace sgl
{
    class PhysicalObject :
        public ITransformable
    {
    protected:
        btRigidBody *rigid_body;

        void initRigidBody(float x, float y, float z, float pitch, float yaw, float roll, 
            btCollisionShape *collision_shape, float mass);
        void initRigidBody(btRigidBody *rb);
    private:
        PhysicalObject(PhysicalObject&figure);
    public:
        //the rigid_body won't be copied and delted by object
        PhysicalObject(btRigidBody *rigid_body);
        //the collision shape won't be copied and delted by object
        PhysicalObject(float x, float y, float z, float pitch, float yaw, float roll, btCollisionShape *collision_shape, float mass);
        PhysicalObject(float x, float y, float z, float pitch, float yaw, float roll);
        PhysicalObject(float x, float y, float z);
        PhysicalObject();
        virtual void move(float dx, float dy, float dz) override;
        virtual void setCoordinates(float x, float y, float z) override;
        virtual glm::vec3 getCoordinates() const override;
        virtual void rotate(float delta_pitch, float delta_yaw, float delta_roll) override;
        virtual void setRotation(float pitch, float yaw, float roll) override;
        virtual glm::vec3 getRotation() const override;
        ~PhysicalObject();
    };
}

#endif //!_PHYSICAL_OBJECT_CLASS_