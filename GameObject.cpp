#include "GameObject.h"
#include "StefanPhysics.h"


sgl::GameObject::GameObject(float x, float y, float z, float pitch, float yaw, float roll, const char *path_to_3dmodel)
	: VolumetricFigure(x, y, z, pitch, yaw, roll), model(), rigid_body(NULL)
{
	if (path_to_3dmodel)
		model.loadFromFile(path_to_3dmodel);
}

sgl::GameObject::GameObject(float x, float y, float z, const char *path_to_3dmodel)
	: GameObject(x, y, z, 0.f, 0.f, 0.f, path_to_3dmodel)
{
}

sgl::GameObject::GameObject(const char *path_to_3dmodel)
	: GameObject(0.f, 0.f, 0.f, path_to_3dmodel)
{
}

sgl::GameObject::GameObject()
	: GameObject(NULL)
{
}

void sgl::GameObject::draw(Shader &shader) const
{
	shader.setMat4("model", model_matrix);
	model.draw(shader);
}

void sgl::GameObject::setCoordinates(float x, float y, float z)
{
	VolumetricFigure::setCoordinates(x, y, z);
	setRBCoordinates();
}

void sgl::GameObject::setRBCoordinates()
{
	if (rigid_body)
	{
		btTransform transform;
		rigid_body->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		rigid_body->getMotionState()->setWorldTransform(transform);
	}
}

void sgl::GameObject::move(float dx, float dy, float dz)
{
	VolumetricFigure::move(dx, dy, dz);	
	setRBCoordinates();
}

void sgl::GameObject::rotate(float d_pitch, float d_yaw, float d_roll)
{
	VolumetricFigure::rotate(d_pitch, d_yaw, d_roll);
	setRBRotation();
}

void sgl::GameObject::setRBRotation()
{
	if (rigid_body)
	{
		btTransform transform;
		rigid_body->getMotionState()->getWorldTransform(transform);
		btQuaternion rot;
		rot.setEuler(rotation.x, rotation.y, rotation.z);
		transform.setRotation(rot);
		rigid_body->getMotionState()->setWorldTransform(transform);
	}
}

void sgl::GameObject::setRotation(float pitch, float yaw, float roll)
{
	VolumetricFigure::setRotation(pitch, yaw, roll);
	setRBRotation();
}

sgl::GameObject::~GameObject()
{
}