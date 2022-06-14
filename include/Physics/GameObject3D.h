#ifndef _GAME_OBJECT_CLASS_
#define _GAME_OBJECT_CLASS_

#include "PhysicalBody.h"
#include "Model.h"
#include "glm/matrix.hpp"

namespace sgl
{
    class GameObject3D :
        public PhysicalBody, public IDrawable
    {
    protected:
        // model won't be copied and deleted in destructor
        Model *model;

    public:
        GameObject3D(float x, float y, float z, float pitch, float yaw, float roll, 
            btCollisionShape *collison_shape, float mass, Model *model);
        GameObject3D(float x, float y, float z, float pitch, float yaw, float roll, Model *model);
        GameObject3D(float x, float y, float z, Model *model);
        GameObject3D(Model *model);
        GameObject3D();
        void draw(Shader &shader) const override;
        Model* set3DModel(Model *model); //return pointer to the old model
        ~GameObject3D();
    };
}

#endif // !_GAME_OBJECT_CLASS_