#include "Enemy.h"
#include "btBulletCollisionCommon.h"
#include "glm/gtx/vector_angle.hpp"
#include "StefanPhysics.h"
#include <iostream>
#include "GLFW/glfw3.h"

sgl::Enemy::Enemy(EnemySettings &settings)	
	:	sgl::GameObject3D(settings.position.x, settings.position.y, settings.position.z, 
		0.f, settings.view_direction.y, 0.f, new btCapsuleShape(settings.radius, settings.height), settings.mass, settings.model)
{
	hp = settings.hp;
	damage = settings.damage;
	speed = settings.speed;
	view_direction = glm::normalize(glm::vec3(settings.view_direction.x, settings.view_direction.y, 0.f));
	victim = settings.player_to_chase;
	is_following_victim = false;
	last_hit_time = std::chrono::milliseconds(0);
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rigid_body->setActivationState(DISABLE_DEACTIVATION);
	rigid_body->setAngularFactor(btVector3(0.f, 0.f, 0.f));
}

void sgl::Enemy::draw(Shader &shader) const
{
	shader.setMat4("model", getModelMatrix());
	model->draw(shader);
}

void sgl::Enemy::hitByThePlayer(int damage_done)
{
	hp -= damage_done;
}

void sgl::Enemy::update(float deltatime)
{
	checkIfKDRatioIsOver();
	bool has_followed_victim_recently = is_following_victim;
	tryToFollowVictimIfItIsPossible(has_followed_victim_recently, deltatime);
	moveRandomlyIfNotFollowingVictim(has_followed_victim_recently, deltatime);
}

void sgl::Enemy::tryToFollowVictimIfItIsPossible(bool has_followed_victim_recently, float deltatime)
{
	glm::vec3 player_pos = victim->getCoordinates();
	glm::vec3 m_pos = getCoordinates();
	glm::vec3 direction_to_player = player_pos - m_pos;
	if (!last_hit_time.count() && glm::length(direction_to_player) <= ENEMYS_MAX_VIEW_DIST &&
		glm::angle(direction_to_player, view_direction) < HALF_FOV)
	{
		float eye_level = ((btCapsuleShape*)(rigid_body->getCollisionShape()))->getHalfHeight() * 2 *
			CAMERA_RELATIVE_SHIFT + m_pos.y;
		btCollisionWorld::ClosestRayResultCallback raytest_cb(btVector3(m_pos.x, eye_level, m_pos.z),
			btVector3(direction_to_player.x, direction_to_player.y, direction_to_player.z));
		StefanPhysics::rayTest(raytest_cb);
		if (is_following_victim = !raytest_cb.hasHit())
		{
			glm::vec3 old_vd = view_direction;
			view_direction = glm::normalize(direction_to_player);
			rotate(0.f, glm::angle(view_direction, old_vd), 0.f);
			m_pos += view_direction * speed * deltatime;
			setCoordinates(m_pos.x, m_pos.y, m_pos.z);
		}
	}
}

void sgl::Enemy::moveRandomlyIfNotFollowingVictim(bool has_followed_victim_recently, float deltatime)
{
	if (has_followed_victim_recently && !is_following_victim)
		setRandomViewDirection();
	if (!is_following_victim)
	{
		glm::vec3 m_pos = getCoordinates();
		m_pos += view_direction * speed * deltatime;
		setCoordinates(m_pos.x, m_pos.y, m_pos.z);
	}
}

void sgl::Enemy::setRandomViewDirection()
{
	switch (std::rand() % 2)
	{
	case (0):
		view_direction = glm::vec3(1.f, 0.f, 0.f);
		break;
	case (1):
		view_direction = glm::vec3(0.f, 0.f, 1.f);
		break;
	}
	glm::vec3 old_vd = view_direction;
	view_direction *= std::rand() % 2 ? 1.f : -1.f;
	rotate(0.f, glm::angle(view_direction, old_vd), 0.f);
}

void sgl::Enemy::checkIfKDRatioIsOver()
{
	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	if (now.count() - last_hit_time.count() >= ENEMYS_KILL_DEATH_RATIO)
		last_hit_time = std::chrono::milliseconds(0);
}

void sgl::Enemy::collideCallback(ITransformable *other, btManifoldPoint &cp)
{
	if (other->getObjectID() & objectID::PLAYER)
	{
		last_hit_time = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		((Player*)other)->hitByEnemy(damage);
	}
	else if (!other->getObjectID() & objectID::BOUNDED_PLANE)
		setRandomViewDirection();
}
