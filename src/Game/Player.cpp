#include "Player.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "config.h"
#include <cmath>

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
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rigid_body->setActivationState(DISABLE_DEACTIVATION);
	rigid_body->setAngularFactor(btVector3(0.f, 0.f, 0.f));
	is_standing = false;
	hp = settings.health_points;
}

void sgl::Player::update(float deltatime)
{
	btTransform transform;
	rigid_body->getMotionState()->getWorldTransform(transform);
	glm::vec3 pos = glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()) + 
		CAMERA_SHIFT(height);
	camera->setPosition(pos);
	if (rigid_body->getLinearVelocity().y() <=- 0.006f)
		is_standing = false;
	if (is_standing && last_landing_time.count())
	{
		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		double time_from_grounding = (double)(now - last_landing_time).count();
		if (time_from_grounding > PERIOD_OF_REST_AFTER_JUMP)
			time_from_grounding = PERIOD_OF_REST_AFTER_JUMP;
		double cos_prod = cos((time_from_grounding / PERIOD_OF_REST_AFTER_JUMP) * (M_PI) - M_PI / 2);
		pos.y -= cos_prod * height * PLAYER_CAM_DOWN_SHIFT_AFTER_GROUNDING;
		camera->setPosition(pos);
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

void sgl::Player::setWalking(float deltatime, char x_direction, char z_direction)
{
	// player is staying put and function is called to move him nowhere
	if ((!x_direction) && (!z_direction))
		return;
	glm::vec3 walk_vector = camera->getDirection();
	walk_vector.y = 0;
	walk_vector = glm::normalize(walk_vector);
	glm::vec3 camera_right = walk_vector;
	camera_right.x = -walk_vector.z;
	camera_right.z = walk_vector.x;
	walk_vector *= (float)z_direction;
	camera_right *= (float)x_direction;
	walk_vector = !z_direction && !x_direction ? glm::vec3(0.f) : (glm::normalize(walk_vector+camera_right) * 
		(is_standing ? PLAYER_SPEED : PLAYER_SPEED_WHEN_JUMPING)) * deltatime;
	if (last_landing_time.count())
	{
		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		walk_vector *= sin((
			fmax(
				fmin((now - last_landing_time).count(), PERIOD_OF_REST_AFTER_JUMP), 
				PERIOD_OF_REST_AFTER_JUMP / 4.f
			) / PERIOD_OF_REST_AFTER_JUMP
		) * M_PI / 2.f);
	}
	btTransform transform = rigid_body->getWorldTransform();
	transform.getOrigin().setX(transform.getOrigin().x() + walk_vector.x);
	transform.getOrigin().setZ(transform.getOrigin().z() + walk_vector.z);
	rigid_body->setWorldTransform(transform);
}

bool sgl::Player::tryToJump()
{
	if (!is_standing)
		return false;
	rigid_body->setLinearVelocity(btVector3(0.f, PLAYER_JUMP_VELOCTY, 0.f));
	is_standing = false;
	return true;
}

void sgl::Player::collideCallback(ITransformable *other, btManifoldPoint &cp)
{
	if (abs(cp.m_normalWorldOnB.y()) > sin(MIN_ANGLE_OF_SURFACE_TO_JUMP))
		setAsGrounded();
}

void sgl::Player::setAsGrounded()
{
	if (is_standing)
		return;
	is_standing = true;
	last_landing_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}

void sgl::Player::hitByEnemy(int damage)
{
	hp -= damage;
	std::cout << "Hit the player: " << hp + damage << " - " << damage << " = " << hp << std::endl;
}

sgl::Player::~Player()
{
	delete camera;
}
