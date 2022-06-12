#ifndef _BOUNDED_PLAIN_GAME_OBJECT_H_
#define _BOUNDED_PLAIN_GAME_OBJECT_H_

#include "PlainObject.h"

namespace sgl
{
    struct BoundedPlaneSettings
    {
        float x, y, z;
        float pitch, yaw, roll;
        float mass;
        Texture diffuse_texture;
        Texture specular_texture;
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        int shininess;
        // width(x) and length(z) are from one side to other
        float width, length;
        BoundedPlaneSettings();
        PlainObjectSettings& convertToPlainSettings() const;
    };

    class BoundedPlane :
        public PlainObject
    {
    private:
        BoundedPlane(BoundedPlane&);
    public:
        BoundedPlane(const BoundedPlaneSettings &settings);
        void draw(Shader &shader) const override;
        float getWidth() const;
        float getLength() const;
        void setDiffuseColor(float r, float g, float b, float alpha) override;
        void setSpecularColor(float r, float g, float b, float alpha) override;
        ~BoundedPlane();
    };
}

#endif // !_BOUNDED_PLAIN_GAME_OBJECT_H_