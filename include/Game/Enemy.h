#ifndef _ENEMY_CLASS_H_
#define _ENEMY_CLASS_H_

#include "Model.h"
#include "PhysicalBody.h"
#include "config.h"
#include "Player.h"
#include <chrono>

namespace sgl
{
	class StefanPhysics;

	struct EnemySettings
	{
		glm::vec3 position;
		float radius, height;
		glm::vec2 view_direction; // only for x and z; an enemy can not 
		uint hp; // health
		uint damage;
		float speed;
		Model *model;
		Player* player_to_chase;
		float mass;
	};

	class Enemy : public PhysicalBody, public IDrawable
	{
		friend class StefanPhysics;
	private:
		uint hp; 
		uint damage;
		float speed;
		Model *model;
		glm::vec3 view_direction; 
		Player *victim;
		bool is_following_victim;
		std::chrono::milliseconds last_hit_time;;

		void tryToFollowVictimIfItIsPossible(bool has_followed_victim_recently);
		void moveRandomlyIfNotFollowingVictim(bool has_followed_victim_recently);
		void setRandomViewDirection();
		void checkIfKDRatioIsOver();
		void collideCallback(ITransformable *other, btManifoldPoint &cp) override;
	public:
		Enemy(EnemySettings &settings);
		void draw(Shader &shader) const override;
		void hitByThePlayer(int damage_done);
		void update() override;
		inline objectID getObjectID() const override {return (objectID)(PhysicalBody::getObjectID() | objectID::ENEMY);}

	};
}

#endif // !_ENEMY_CLASS_H_