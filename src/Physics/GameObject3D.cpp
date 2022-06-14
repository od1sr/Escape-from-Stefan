#include "GameObject3D.h"
#include "StefanPhysics.h"


sgl::GameObject3D::GameObject3D(float x, float y, float z, float pitch, float yaw, float roll, 
	btCollisionShape *collison_shape, float mass, Model *_model
)	:	PhysicalBody(x, y, z, pitch, yaw, roll, collison_shape, mass), model(_model)
{
}

sgl::GameObject3D::GameObject3D(float x, float y, float z, float pitch, float yaw, float roll, Model *_model)
	: GameObject3D(x, y, z, pitch, yaw, roll, new btEmptyShape(), 0.f, _model)
{
}

sgl::GameObject3D::GameObject3D(float x, float y, float z, Model *_model)
	: GameObject3D(x, y, z, 0.f, 0.f, 0.f, _model)
{
}

sgl::GameObject3D::GameObject3D(Model *_model)
	: GameObject3D(0.f, 0.f, 0.f, _model)
{
}

sgl::GameObject3D::GameObject3D()
	: GameObject3D(NULL)
{
}

void sgl::GameObject3D::draw(Shader &shader) const
{
	shader.setMat4("model", getModelMatrix());
	model->draw(shader);
}

sgl::Model* sgl::GameObject3D::set3DModel(sgl::Model *_model)
{
	Model *old_model = _model;
	model = _model;
	return old_model;
}

sgl::GameObject3D::~GameObject3D()
{
}