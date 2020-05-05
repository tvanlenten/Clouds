#include "Sun.h"
#include "../ImGui/imgui.h"
#include <glm/gtx/transform.hpp>

Sun::Sun()
{
	_rot = 0.0;
	_axis = glm::normalize(glm::vec3(0.0, 1.0, -4.0));
	_power = 20.0;
}

Sun::Sun(glm::vec3 axis, float rot, float power)
{
	_rot = rot;
	_axis = axis;
	_power = power;
}

void Sun::Gui()
{
	ImGui::DragFloat3("Sun Axis", &_axis[0], 0.1, -5.0, 5.0);
	ImGui::DragFloat("Sun Rotation", &_rot, 0.1);
	ImGui::DragFloat("Sun Power", &_power, 0.1, 0.0, 50.0);
}

void Sun::SetAxis(glm::vec3 axis)
{
	_axis = axis;
}

void Sun::SetRotation(float rot)
{
	_rot = rot;
}

glm::vec3 Sun::GetDirection()
{
	glm::mat4 m = glm::rotate(_rot, glm::normalize(_axis));
	return glm::vec3(m * glm::vec4(0.0, 1.0, 0.0, 1.0));
}

float Sun::GetPower()
{
	return _power;
}
