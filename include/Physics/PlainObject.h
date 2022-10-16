#ifndef _PLAIN_OBJECT_CLASS_H_
#define _PLAIN_OBJECT_CLASS_H_

#include "PhysicalBody.h"
#include "VAO.h"
#include "glm/matrix.hpp"
#include "Texture.h"
#include "btBulletCollisionCommon.h"

namespace sgl
{
    struct PlainObjectSettings
    {
        float x, y, z;
        float pitch, yaw, roll;
        float mass;
        btCollisionShape *collision_shape;
        Texture diffuse_texture;
        Texture specular_texture;
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        int shininess;
        PlainObjectSettings();
    };
    class PlainObject :
        public PhysicalBody, public IDrawable
    {
    protected:
        VAO vao;
        Texture *diffuse_texture;
        Texture *specular_texture;
        glm::vec4 *diffuse_color;
        glm::vec4 *specular_color;

        void initPlainObject(const PlainObjectSettings &settings);
    private:
        PlainObject(PlainObject&);
    public:
        unsigned int shininess;

    protected:
        PlainObject(const PlainObjectSettings &settings);
    public:
        virtual void draw(Shader &shader) const override;
        
        virtual void setDiffuseColor(float r, float g, float b, float a);
        virtual void setSpecularColor(float r, float g, float b, float a);
        glm::vec4 getDiffuseColor() const;
        glm::vec4 getSpecularColor() const;
        void setTexture(const Texture &diffuse_texture, const Texture &specular_texture);
        virtual objectID getObjectID() const override;
        ~PlainObject();
    };
}

#endif // !_PLAIN_OBJECT_CLASS_H_