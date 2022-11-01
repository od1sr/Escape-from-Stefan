#include "CameraFPS.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using glm::vec3;
using sgl::CameraFPS;
static const vec3 vec_up(0.f, 1.f, 0.f);
static const float max_pitch = glm::radians(89.f);

template <typename T>
static inline int sign(T value)
{
	return (value < 0 ? -1 : 1);
}
void CameraFPS::setFrontAndRight()
{
	front = direction;
	front.y = 0.f;
	front = glm::normalize(front);
	right.x = -front.z;
	right.z = front.x;
}

CameraFPS::CameraFPS(vec3 _pos, vec3 _direction)
	: direction(_direction), pos(_pos), front(0.f), right(0.f)
{
	direction = glm::normalize(direction);
	setFrontAndRight();
}

CameraFPS::CameraFPS(vec3 position)
	: CameraFPS(position, glm::vec3(0.f, 0.f, -1.f))
{
}
CameraFPS::CameraFPS() : CameraFPS(vec3(0.f))
{
}
vec3 CameraFPS::getDirection() const
{
	return direction;
}
vec3 CameraFPS::getPos() const
{
	return pos;
}
glm::mat4 &CameraFPS::getViewMatrix() const
{
	glm::mat4 view = glm::lookAt(pos, pos + direction, glm::vec3(0.f, 0.1f, 0.f));
	return view;
}

void CameraFPS::rotate(float delta_yaw, float delta_pitch)
{
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), (float)delta_yaw, vec_up);
	float current_pitch, proj_length;
	proj_length = sqrt(direction.x * direction.x + direction.z * direction.z);
	current_pitch = atan(direction.y / proj_length);
	if (abs(current_pitch + delta_pitch) > max_pitch)
		delta_pitch = (max_pitch - abs(current_pitch)) * sign(delta_pitch);
	r = glm::rotate(r, delta_pitch, right);
	glm::vec4 d;
	d.x = direction.x;
	d.y = direction.y;
	d.z = direction.z;
	direction = r*d;
	setFrontAndRight();

}
void CameraFPS::rotateDeg(float delta_yaw, float delta_pitch)
{
	rotate(glm::radians(delta_yaw), glm::radians(delta_pitch));
}
void CameraFPS::directionalMovement(MoveMode mode, float offset)
{
	float cos_a = cos(atan2(front.x, front.z)),
		sin_a = sin(atan2(front.x, front.z));
	glm::vec3 horizontal_right(0.f, 0.f, 0.f);
	switch (mode)
	{
	case sgl::straight:
		pos += front * offset;
		break;
	case sgl::right:
		pos += right * offset;
		break;
	case sgl::up:
		pos.y += offset;
		break;
	default:
		break;
	}
}
void CameraFPS::movement(vec3 &offset)
{
	pos += offset;
}
void CameraFPS::setPosition(vec3 &new_pos)
{
	pos = new_pos;
}
void CameraFPS::setDirection(glm::vec3 &new_direction)
{
	direction = new_direction;
	front = vec3(direction.x, 0.f, direction.z);
	front = glm::normalize(front);
}
CameraFPS::~CameraFPS()
{
}
