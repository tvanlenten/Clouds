#pragma once
#include <glm/glm.hpp>
#include "EventHandeling\Event.h"
#include "EventHandeling\EventHandeler.h"
#include <memory>

/*
	From learnopengl.com
*/

class Camera : public EventHandeler<MovementEvent>, public EventHandeler<MouseMovementEvent>
{
public:
	Camera(glm::ivec2 dimensions, glm::vec3 position, float fov, float near, float far, float rotationSpeed, float movementSpeed);
	~Camera();

	void setMoveSpeed(float moveSpeed);
	void setRotationSpeed(float rotationSpeed);

	glm::mat4 ViewMatrix();
	glm::mat4 ProjectionMatrix();
	glm::mat4 ProjectionViewMatrix();
	glm::mat4 InverseProjectionViewMatrix();

	void setPitch(float pitch);
	void setYaw(float yaw);

	//worldSpace -> clipSpace
	glm::vec3 project(glm::vec3 worldCoord);
	//clipSpace -> worldSpace
	glm::vec3 unProject(glm::vec3 clipSpaceCoord);

	void shouldUpdate(bool state);

	void handle(std::shared_ptr<MovementEvent> e) override;
	void handle(std::shared_ptr<MouseMovementEvent> e) override;
private:
	void updateCameraVectors();

public:
	float Yaw;
	float Pitch;
	float FOV;
	float Near;
	float Far;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::ivec2 Dimensions;
private:
	bool _shouldUpdate;
	float _moveSpeed;
	float _rotationSpeed;
	bool _constrainPitch;
};

