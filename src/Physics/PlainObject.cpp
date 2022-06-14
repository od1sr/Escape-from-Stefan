#include "PlainObject.h"
#include "logging.h"
#include "btBulletDynamicsCommon.h"

sgl::PlainObjectSettings::PlainObjectSettings()
	: x(0.f), y(0.f), z(0.f), pitch(0.f), yaw(0.f), roll(0.f), mass(0.f), collision_shape(NULL),
	diffuse_texture{ 0, TextureType::UNINITIALIZED }, specular_texture{ 0, TextureType::UNINITIALIZED },
	diffuse_color(0.f), specular_color(0.f), shininess(0)
{
}

sgl::PlainObject::PlainObject(const PlainObjectSettings &settings)
{
	initPlainObject(settings);
}

void sgl::PlainObject::initPlainObject(const PlainObjectSettings &settings)
{
	btCollisionShape *cp;
	if (settings.collision_shape == NULL)
		cp = new btEmptyShape();
	else
		cp = settings.collision_shape;
	initRigidBody(settings.x, settings.y, settings.z, settings.pitch, settings.yaw, settings.roll, 
		cp, settings.mass);
	if (settings.diffuse_texture.type != TextureType::UNINITIALIZED && 
		settings.specular_texture.type != TextureType::UNINITIALIZED
	)
		setTexture(settings.diffuse_texture, settings.specular_texture);
	setDiffuseColor(settings.diffuse_color.r, settings.diffuse_color.g, settings.diffuse_color.b, 
		settings.diffuse_color.a);
	setSpecularColor(settings.specular_color.r, settings.specular_color.g, settings.specular_color.b,
		settings.specular_color.a);
	shininess = settings.shininess;
}

void sgl::PlainObject::draw(Shader &shader) const
{
	shader.setMat4("model", getModelMatrix());
	if (diffuse_texture)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, diffuse_texture->id));
		shader.setInt("material.diffuse_maps[0]", 0);
		shader.setInt("material.diff_textures_num", 1);
	}
	else
		shader.setInt("material.diff_textures_num", 0);
	if (specular_texture)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE1));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, specular_texture->id));
		shader.setInt("material.specular_maps[0]", 1);
		shader.setInt("material.spec_textures_num", 1);
	}
	else
		shader.setInt("material.spec_textures_num", 0);
	shader.setInt("material.shininess", shininess);
	vao.use();
}

void sgl::PlainObject::setDiffuseColor(float r, float g, float b, float a)
{
	if (!diffuse_color)
		diffuse_color = new glm::vec4;
	diffuse_color->x = r;
	diffuse_color->y = g;
	diffuse_color->z = b;
	diffuse_color->w = a;
}

glm::vec4 sgl::PlainObject::getDiffuseColor() const
{
	if (diffuse_color)
		return *diffuse_color;
	else
		return glm::vec4(0.f);
}

void sgl::PlainObject::setSpecularColor(float r, float g, float b, float a)
{
	if (!specular_color)
		specular_color = new glm::vec4;
	specular_color->x = r;
	specular_color->y = g;
	specular_color->z = b;
	specular_color->w = a;
}

glm::vec4 sgl::PlainObject::getSpecularColor() const
{
	if (specular_color)
		return *specular_color;
	else
		return glm::vec4(0.f);
}

void sgl::PlainObject::setTexture(const Texture &_diffuse_texture, const Texture &_specular_texture)
{
	if (!diffuse_texture)
		diffuse_texture = new Texture;
	*diffuse_texture = _diffuse_texture;
	if (!specular_texture)
		specular_texture = new Texture;
	*specular_texture = _specular_texture;
}

sgl::PlainObject::~PlainObject()
{
	if (diffuse_texture)
		delete diffuse_texture;
	if (specular_texture)
		delete specular_texture;
	if (diffuse_color)
		delete diffuse_color;
	if (specular_color)
		delete specular_color;
}