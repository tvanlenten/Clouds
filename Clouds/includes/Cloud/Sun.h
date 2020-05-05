#pragma once
#include <glm/glm.hpp>


class Sun
{
public:
	Sun();
	Sun(glm::vec3 axis, float rot, float power);

	void Gui();

	void SetAxis(glm::vec3 axis);
	void SetRotation(float rot);
	glm::vec3 GetDirection();
	float GetPower();

private:
	glm::vec3 _direction;
	float _rot;
	glm::vec3 _axis;
	float _power;
};