#ifndef _PLAYER_CLASS_
#define _PLAYER_CLASS_

#include "PhysicalBody.h"
#include "CameraFPS.h"

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
	};

	class Player : public PhysicalBody
	{
		friend class StefanPhysics;
	private:
		CameraFPS *camera;
		float height;
		bool is_standing; // on the ground
		btVector3 current_walking_velocity;

	public:
		Player(PlayerSettings &settings);
		void update() override;
		void rotateCamera(float yaw, float pitch);
		objectID getObjectID() const override;
		inline const CameraFPS* getCamera() const { return camera; };
		// 1 means walk straight along axis, 0 - stay put along axis, -1 - walk bakwards along axis
		void setWalking(char x_direction= 0, char z_direction= 0); 
		bool tryToJump(); // return true if jumped else false
		~Player();
	};
}

#endif // !_PLAYER_CLASS_