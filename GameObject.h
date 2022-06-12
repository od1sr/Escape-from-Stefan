#ifndef _3D_OBJECT_CLASS_
#define _3D_OBJECT_CLASS_

#include "PhysicalObject.h"
#include "Model.h"
#include "glm/matrix.hpp"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"


namespace sgl
{
    class GameObject :
        public PhysicalObject
    {
    private:
        Model model;
        glm::vec3 size_scalar;
        btRigidBody *rigid_body;

    public:
        GameObject(float x, float y, float z, float pitch, float yaw, float roll, const char *path_to_3dmodel);
        GameObject(float x, float y, float z, const char *path_to_3dmodel);
        GameObject(const char *path_to_3dmodel);
        GameObject();
        void draw(Shader &shader) const override;
        ~GameObject();
    };
}

#endif // !_3D_OBJECT_CLASS_