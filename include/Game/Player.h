#ifndef _PLAYER_CLASS_
#define _PLAYER_CLASS_

#include "PhysicalBody.h"
#include "CameraFPS.h"
#include <chrono>


namespace sgl
{
	// Player's body is a capsule

	class StefanPhysics;

	struct PlayerSettings
	{
		glm::vec3 position; // this is the coordinates of the center of the collison shape
		float radius, height;
		float mass;
		glm::vec3 view_direction;
		int health_points;
	};

	class Player : public PhysicalBody
	{
	private:
		CameraFPS *camera;
		float height;
		bool is_standing; // on the ground
		std::chrono::milliseconds last_landing_time; 
		int hp;

		void collideCallback(ITransformable *other, btManifoldPoint &cp) override;
		void setAsGrounded();
	public:
		Player(PlayerSettings &settings);
		void update(float time_passed_since_last_update) override;
		void rotateCamera(float yaw, float pitch);
		objectID getObjectID() const override;
		inline const CameraFPS* getCamera() const { return camera; };
		// 1 means walk straight along axis, 0 - stay put along axis, -1 - walk bakwards along axis, time_passed - seconds
		void setWalking(float time_passed, char x_direction= 0, char z_direction= 0);
		bool tryToJump(); // return true if jumped else false
		void hitByEnemy(int damage);
		~Player();
	};
}

#endif // !_PLAYER_CLASS_