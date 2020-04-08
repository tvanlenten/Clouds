#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::ivec2 dimensions, glm::vec3 position, float fov, float near, float far, float rotationSpeed, float movementSpeed)
{
	Dimensions = dimensions;
	Position = position;
	FOV = fov;
	Near = near;
	Far = far;
	Pitch = -90.0f;
	Yaw = 0.0f;
	Up = glm::vec3(0.0, 1.0, 0.0);
	Front = glm::vec3(0.0, 0.0, -1.0);
	Right = glm::vec3(1.0, 0.0, 0.0);
	_constrainPitch = true;
	_moveSpeed = movementSpeed;
	_rotationSpeed = rotationSpeed;
	_shouldUpdate = true;
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::setMoveSpeed(float moveSpeed)
{
	_moveSpeed = moveSpeed;
}

void Camera::setRotationSpeed(float rotationSpeed)
{
	_rotationSpeed = rotationSpeed;
}

glm::mat4 Camera::ViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::ProjectionMatrix()
{
	return glm::perspective(glm::radians(FOV), (float)Dimensions.x / (float)Dimensions.y, Near, Far);
}

glm::mat4 Camera::ProjectionViewMatrix()
{
	return ProjectionMatrix() * ViewMatrix();
}

glm::mat4 Camera::InverseProjectionViewMatrix()
{
	return glm::inverse(ProjectionViewMatrix());
}

void Camera::setPitch(float pitch)
{
	Pitch = pitch;
	updateCameraVectors();
}

void Camera::setYaw(float yaw)
{
	Yaw = yaw;
	updateCameraVectors();
}


glm::vec3 Camera::project(glm::vec3 worldCoord)
{
	glm::vec4 pt = ProjectionViewMatrix() * glm::vec4(worldCoord, 1.0f);
	glm::vec3 p = glm::vec3(pt.x, pt.y, pt.z) / pt.w;

	return p;//glm::project(worldCoord, view(), proj(), glm::ivec4(0, 0, Dimensions.x, Dimensions.y));
}

glm::vec3 Camera::unProject(glm::vec3 clipSpaceCoord)
{
	glm::vec4 pt = InverseProjectionViewMatrix() * glm::vec4(clipSpaceCoord, 1.0f);
	glm::vec3 p = glm::vec3(pt.x, pt.y, pt.z) / pt.w;

	return p;//glm::unProject(screenCoord, view(), proj(), glm::ivec4(0, 0, Dimensions.x, Dimensions.y));
}

void Camera::shouldUpdate(bool state)
{
	_shouldUpdate = state;
}

void Camera::handle(std::shared_ptr<MovementEvent> e)
{
	if (!_shouldUpdate)
		return;

	float velocity = _moveSpeed * e->deltaTime;
	if (e->Forward)
		Position += Front * velocity;
	if (e->Backward)
		Position -= Front * velocity;
	if (e->Left)
		Position -= Right * velocity;
	if (e->Right)
		Position += Right * velocity;
	if (e->Up)
		Position += Up * velocity;
	if (e->Down)
		Position -= Up * velocity;
}

void Camera::handle(std::shared_ptr<MouseMovementEvent> e)
{
	if (!_shouldUpdate)
		return;

	Yaw += float(e->Mouse.z) * _rotationSpeed;
	Pitch += float(e->Mouse.w) * _rotationSpeed;
	if (_constrainPitch)
	{
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}
	updateCameraVectors();
}


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, glm::vec3(0.0,1.0,0.0)));
	Up = glm::normalize(glm::cross(Right, Front));
}
