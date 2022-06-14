#pragma once
#include "glm/glm.hpp"
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
namespace sgl
{
	enum MoveMode{straight, right, up};
	class CameraFPS
	{
	private:
		glm::vec3 direction;
		glm::vec3 pos;
		glm::vec3 front;
		glm::vec3 right;
		void setFrontAndRight();
	public:
		CameraFPS(glm::vec3 _pos, glm::vec3 _direction);
		CameraFPS(glm::vec3 position);
		CameraFPS();
		glm::vec3 getDirection() const;
		glm::vec3 getPos() const;
		glm::mat4& getViewMatrix() const;
		//set the absolute max value of pitch and yaw(radians) of the camera.
		//add a yaw and a pitch(radians) to a camera if they are not beyond their max value
		void rotate(float delta_yaw, float delta_pitch);
		void rotateDeg(float delta_yaw, float delta_pitch);
		//move the camera in the its direction
		void directionalMovement(MoveMode mode, float offset);
		//add offset vector to camera position
		void movement(glm::vec3 &offset);
		void setPosition(glm::vec3 &new_pos);
		void setDirection(glm::vec3& new_direction);
		~CameraFPS();
	};
}
#endif // !CAMERA_CLASS_H