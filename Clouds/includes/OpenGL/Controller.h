#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct KeyPressEvent;
struct MouseMovementEvent;
struct MouseButtonEvent;
struct ScrollEvent;
struct MovementEvent;

template<class T>
class EventHandeler;

class Controller
{
public:
	Controller(int width, int height, const char* name, bool fullscreen);
	~Controller();

	bool isKeyPressed(int key);
	bool isLeftMousePressed();
	bool isMiddleMousePressed();
	bool isRightMousePressed();
	void showMouse();
	void hideMouse();

	void update();
	bool shouldQuit();
	double getTime();
	void swapBuffers();

	void addKeyPressEventHandeler(std::shared_ptr<EventHandeler<KeyPressEvent>> eventHandeler);
	void addMouseMovementEventHandeler(std::shared_ptr<EventHandeler<MouseMovementEvent>> eventHandeler);
	void addMouseButtonEventHandeler(std::shared_ptr<EventHandeler<MouseButtonEvent>> eventHandeler);
	void addScrollEventHandeler(std::shared_ptr<EventHandeler<ScrollEvent>> eventHandeler);
	void addMovementEventHandeler(std::shared_ptr<EventHandeler<MovementEvent>> eventHandeler);

	GLFWwindow* getWindow();
	glm::ivec4 getMouse();
	glm::vec4 getClipSpaceMouse();
	double getDeltaTime();

private:
	static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMovementCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void processMovementKeys();

private:
	static GLFWwindow* _window;
	static glm::ivec2 _windowDims;
	static bool _firstMouse;
	static glm::ivec4 _mouse;
	static glm::vec4 _clipSpaceMouse;
	static double _deltaTime;
	static double _prevTime;
	static std::vector<std::shared_ptr<EventHandeler<KeyPressEvent>>> _keyPressEventHandelers;
	static std::vector<std::shared_ptr<EventHandeler<MouseMovementEvent>>> _mouseMovementEventHandelers;
	static std::vector<std::shared_ptr<EventHandeler<MouseButtonEvent>>> _mouseButtonEventHandelers;
	static std::vector<std::shared_ptr<EventHandeler<ScrollEvent>>> _scrollEventHandelers;
	static std::vector<std::shared_ptr<EventHandeler<MovementEvent>>> _movementEventHandelers;
};

