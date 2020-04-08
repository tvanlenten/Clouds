#pragma once
#include <glm/glm.hpp>

enum class MouseButton
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2
};

enum class Action
{
	RELEASE = 0,
	PRESS = 1,
	REPEAT = 2
};

struct Event
{
	double deltaTime;
};

struct MovementEvent : Event
{
	bool Forward;
	bool Backward;
	bool Left;
	bool Right;
	bool Up;
	bool Down;
};

struct KeyPressEvent : Event //TODO FIX
{
	int key;
	int scancode;
	int action;
	int mods; //TODO
};

struct MouseMovementEvent : Event
{
	glm::ivec4 Mouse;
	glm::vec4 ClipSpaceMouse;
};

struct MouseButtonEvent : Event
{
	MouseButton button;
	Action action;
	//int mods; //TODO
};

struct ScrollEvent : Event
{
	double xoffset;
	double yoffset;
};

//screen size change event