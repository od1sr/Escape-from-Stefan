#include "Player.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "config.h"

#define CAMERA_SHIFT(height) glm::vec3(0.f, (height)*CAMERA_RELATIVE_SHIFT/2.f, 0.f)

sgl::Player::Player(PlayerSettings &settings)
{
	camera = new CameraFPS(
		settings.position + CAMERA_SHIFT(settings.height),
		settings.view_direction
	);
	height = settings.height;
	btCollisionShape *collision_shape = new btCapsuleShape(settings.radius, settings.height);
	initRigidBody(settings.position.x, settings.position.y, settings.position.z, 0.f, 0.f, 0.f,
		collision_shape, settings.mass);
	rigid_body->setAngularFactor(btVector3(0.f, 0.f, 0.f));
	is_standing = false;
	current_x_walking = current_z_walking = 0;
	current_walking_velocity = btVector3(0.f, 0.f, 0.f);
}

void sgl::Player::update()
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	glm::vec3 pos = glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()) + 
		CAMERA_SHIFT(height);
	camera->setPosition(pos);
	if (current_x_walking || current_z_walking)
	{
		current_x_walking = current_z_walking = 0;
		rigid_body->setLinearVelocity(rigid_body->getLinearVelocity() - current_walking_velocity);
		current_walking_velocity.setValue(0.f, 0.f, 0.f);
	}
}

void sgl::Player::rotateCamera(float yaw, float pitch)
{
	camera->rotate(yaw, pitch);
}

objectID sgl::Player::getObjectID() const
{
	return (objectID)(PhysicalBody::getObjectID() | objectID::PLAYER);
}

void sgl::Player::moveDirectionaly(char x_direction, char z_direction)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	glm::vec3 pos(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
	glm::vec3 first_cam_pos = pos + CAMERA_SHIFT(height);
	camera->setPosition(first_cam_pos);
	if (x_direction != 0)
	{
		if (current_x_walking == x_direction)
		{
			if (current_z_walking == z_direction)
				return;
		}
		else
			camera->directionalMovement(sgl::straight, x_direction);
	}
	if (z_direction != 0)
	{
		if (current_z_walking == z_direction)
		{
			if (current_x_walking == x_direction)
				return;
		}
		else
			camera->directionalMovement(sgl::right, z_direction);
	}
	current_x_walking = x_direction;
	current_z_walking = z_direction;
	glm::vec3 cam_pos = camera->getPos();
	btVector3 new_pos(cam_pos.x, transform.getOrigin().y(), cam_pos.z);
	current_walking_velocity = (new_pos - transform.getOrigin()).normalize() * PLAYER_SPEED;
	current_walking_velocity.setY(0);
	rigid_body->setLinearVelocity(rigid_body->getLinearVelocity() + current_walking_velocity);
	camera->setPosition(first_cam_pos);
}

bool sgl::Player::tryToJump()
{
	if (!is_standing)
		return false;
	rigid_body->setLinearVelocity(btVector3(0.f, PLAYER_JUMP_VELOCTY, 0.f));
	is_standing = false;
	return true;
}

sgl::Player::~Player()
{
	delete camera;
}
