#pragma once
#include "imgui.h"
#include "GLFW\glfw3.h"

class GUI
{
public:
	GUI(GLFWwindow* window);
	~GUI();
	void Start();
	void End();
};

