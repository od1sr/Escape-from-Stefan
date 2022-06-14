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
    class PhysicalBody :
        public ITransformable
    {
    protected:
        btRigidBody *rigid_body;

        void initRigidBody(float x, float y, float z, float pitch, float yaw, float roll, 
            btCollisionShape *collision_shape, float mass);
        glm::mat4 getModelMatrix() const;
    private:
        PhysicalBody(PhysicalBody&figure);
    public:
        //the collision shape won't be copied and delted by object
        PhysicalBody(float x, float y, float z, float pitch, float yaw, float roll, btCollisionShape *collision_shape, float mass);
        PhysicalBody(float x, float y, float z, float pitch, float yaw, float roll);
        PhysicalBody(float x, float y, float z);
        PhysicalBody();
        virtual void move(float dx, float dy, float dz) override;
        virtual void setCoordinates(float x, float y, float z) override;
        virtual glm::vec3 getCoordinates() const override;
        virtual void rotate(float delta_pitch, float delta_yaw, float delta_roll) override;
        virtual glm::vec3 getRotation() const override;
        float getMass() const override;
        void setMass(float m) override;
        ~PhysicalBody();
    };
}

#endif //!_PHYSICAL_OBJECT_CLASS_