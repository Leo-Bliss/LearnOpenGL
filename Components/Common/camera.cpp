#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>



namespace Hub
{
	Camera::Camera() :
		_position(Vector3(0.0f, 0.0f, 0.3f)),
		_up(Vector3(0.0f, 1.0f, 0.0f)),
		_yaw(-90.0f),
		_pitch(0.0f),
		_front(Vector3(0.0f, 0.0f, -1.0f)),
		_worldUp(Vector3(0.0f, 1.0f, 0.0f)),
		_moveSpeed(3.0f),
		_mouseSensitivity(0.1f),
		_fov(45.0f)

	{
		updateCameraVector();
	}

	Matrix4 Camera::getViewMatrix()
	{
		return glm::lookAt(_position, _position + _front, _up);
	}

	void Camera::processKeyBoard(CameraMovement dirction, float deltaTime)
	{
		float velocity = _moveSpeed * deltaTime;
		if (dirction == CameraMovement::FORWORD)
		{
			_position += _front * velocity;
		}
		if (dirction == CameraMovement::BACKWORD)
		{
			_position -= _front * velocity;
		}
		if (dirction == CameraMovement::LEFT)
		{
			_position -= _right * velocity;
		}
		if (dirction == CameraMovement::RIGHT)
		{
			_position += _right * velocity;
		}
	}

	void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch /*= true*/)
	{
		xOffset *= _mouseSensitivity;
		yOffset *= _mouseSensitivity;
		_yaw += xOffset;
		_pitch += yOffset;
		if (constrainPitch && std::fabs(_pitch) > 89.0f)
		{
			_pitch = _pitch > 0.0f ? 89.0f : -89.0f;
		}
		updateCameraVector();
	}

	void Camera::processMouseScroll(float yOffset)
	{
		_fov -= yOffset;
		if (_fov < 1.0f)
		{
			_fov = 1.0f;
		}
		else if (_fov > 85.0f)
		{
			_fov = 85.0f;
		}
	}

	void Camera::setPosition(const Vector3& pos)
	{
		_position = pos;
	}

	Hub::Vector3 Camera::getPosition()
	{
		return _position;
	}

	float Camera::getFov()
	{
		return _fov;
	}

	void Camera::setFov(float fov)
	{
		_fov = fov;
	}

	void Camera::updateCameraVector()
	{
		Vector3 front;
		front.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
		front.y = std::sin(_pitch);
		front.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
		_front = glm::normalize(front);
		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up = glm::normalize(glm::cross(_right, _front));
	}

}