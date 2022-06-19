#include "PhysicalBody.h"
#include "btBulletDynamicsCommon.h"
#include "StefanPhysics.h"

sgl::PhysicalBody::PhysicalBody(float x, float y, float z,
	float pitch, float yaw, float roll, btCollisionShape *collision_shape, float mass)
{
	initRigidBody(x, y, z, pitch, yaw, roll, collision_shape, mass);
}

void sgl::PhysicalBody::initRigidBody(float x, float y, float z, float pitch, float yaw, float roll,
	btCollisionShape *collision_shape, float mass)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));
	btQuaternion rot;
	rot.setEuler(yaw, pitch, roll);
	transform.setRotation(rot);
	btVector3 local_inertia(0, 0, 0);
	if (mass != 0.f)
		collision_shape->calculateLocalInertia(mass, local_inertia);
	btDefaultMotionState *motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
	rigid_body = new btRigidBody(rb_info);
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rigid_body->setUserPointer(this);
	StefanPhysics::addRigidBody(rigid_body);
}

sgl::PhysicalBody::PhysicalBody(float x, float y, float z, float pitch, float yaw, float roll)
	: PhysicalBody(x, y, z, pitch, yaw, roll, new btEmptyShape(), 0.f)
{
}

sgl::PhysicalBody::PhysicalBody(float x, float y, float z)
	: PhysicalBody(x, y, z, 0.f, 0.f, 0.f)
{
}

sgl::PhysicalBody::PhysicalBody()
	: PhysicalBody(0.f, 0.f, 0.f)
{
}

void sgl::PhysicalBody::move(float dx, float dy, float dz)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btVector3 coordinates = transform.getOrigin();
	PhysicalBody::setCoordinates(coordinates.x() + dx, coordinates.y() + dy, coordinates.z() + dz);
}

void sgl::PhysicalBody::setCoordinates(float x, float y, float z)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	transform.setOrigin(btVector3(x, y, z));
	rigid_body->getMotionState()->setWorldTransform(transform);
}

glm::vec3 sgl::PhysicalBody::getCoordinates() const
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btVector3  pos = transform.getOrigin();
	return glm::vec3 { pos.x(), pos.y(), pos.z() };
}

void sgl::PhysicalBody::rotate(float pitch, float yaw, float roll)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot;
	rot.setEuler(yaw, pitch, roll);
	transform.setRotation(rot);
	rigid_body->getMotionState()->setWorldTransform(transform);
}

glm::vec3 sgl::PhysicalBody::getRotation() const
{
	glm::vec3 rotation;
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	transform.getRotation().getEulerZYX(rotation.z, rotation.y, rotation.x);
	return rotation;
}

float sgl::PhysicalBody::getMass() const
{
	return rigid_body->getMass();
}

void sgl::PhysicalBody::setMass(float m)
{
	btVector3 inertia(0, 0, 0);
	if (m != 0.f)
		rigid_body->getCollisionShape()->calculateLocalInertia(m, inertia);
	rigid_body->setMassProps(m, inertia);
}

glm::mat4 sgl::PhysicalBody::getModelMatrix() const
{
	glm::vec4 mat_vector[4];
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	transform.getOpenGLMatrix((float*)mat_vector);
	return glm::mat4(mat_vector[0], mat_vector[1],
		mat_vector[2], mat_vector[3]);
}

sgl::PhysicalBody::~PhysicalBody()
{
	StefanPhysics::removeCollisonObject(rigid_body);
	delete rigid_body->getMotionState();
	delete rigid_body->getCollisionShape();
	delete rigid_body;
}
