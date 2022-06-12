#include "PhysicalObject.h"
#include "btBulletDynamicsCommon.h"
#include "StefanPhysics.h"


sgl::PhysicalObject::PhysicalObject(btRigidBody *_rigid_body)
{
	initRigidBody(_rigid_body);
}

void sgl::PhysicalObject::initRigidBody(btRigidBody *rb)
{
	rigid_body = rb;
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rigid_body->setUserPointer(this);
	StefanPhysics::addRigidBody(rigid_body);
}

sgl::PhysicalObject::PhysicalObject(float x, float y, float z,
	float pitch, float yaw, float roll, btCollisionShape *collision_shape, float mass)
{
	initRigidBody(x, y, z, pitch, yaw, roll, collision_shape, mass);
}

void sgl::PhysicalObject::initRigidBody(float x, float y, float z, float pitch, float yaw, float roll, 
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
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
	rigid_body = new btRigidBody(rb_info);
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rigid_body->setUserPointer(this);
	StefanPhysics::addRigidBody(rigid_body);
}

sgl::PhysicalObject::PhysicalObject(float x, float y, float z, float pitch, float yaw, float roll)
	: PhysicalObject(x, y, z, pitch, yaw, roll, new btEmptyShape(), 0.f)
{
}

sgl::PhysicalObject::PhysicalObject(float x, float y, float z)
	: PhysicalObject(x, y, z, 0.f, 0.f, 0.f)
{
}

sgl::PhysicalObject::PhysicalObject()
	: PhysicalObject(0.f, 0.f, 0.f)
{
}

void sgl::PhysicalObject::move(float dx, float dy, float dz)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btVector3 coordinates = transform.getOrigin();
	PhysicalObject::setCoordinates(coordinates.x() + dx, coordinates.y() + dy, coordinates.z() + dz);
}

void sgl::PhysicalObject::setCoordinates(float x, float y, float z)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	transform.setOrigin(btVector3(x, y, z));
	rigid_body->getMotionState()->setWorldTransform(transform);
}

glm::vec3 sgl::PhysicalObject::getCoordinates() const
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btVector3  pos = transform.getOrigin();
	return glm::vec3 { pos.x(), pos.y(), pos.z() };
}

void sgl::PhysicalObject::rotate(float delta_pitch, float delta_yaw, float delta_roll)
{
	glm::vec3 rotation = getRotation();
	setRotation(rotation.x + delta_pitch, rotation.y + delta_yaw, rotation.z + delta_roll);
}

void sgl::PhysicalObject::setRotation(float pitch, float yaw, float roll)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot;
	rot.setEuler(yaw, pitch, roll);
	transform.setRotation(rot);
}

glm::vec3 sgl::PhysicalObject::getRotation() const
{
	glm::vec3 rotation;
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	transform.getRotation().getEulerZYX(rotation.y, rotation.x, rotation.z);
	return rotation;
}

sgl::PhysicalObject::~PhysicalObject()
{
	StefanPhysics::removeCollisonObject(rigid_body);
	delete rigid_body->getMotionState();
	delete rigid_body->getCollisionShape();
	delete rigid_body;
}
