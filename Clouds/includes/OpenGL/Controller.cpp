#include "Controller.h"
#include <stdio.h>
#include "EventHandeling\EventHandeler.h"
#include "EventHandeling\Event.h"

GLFWwindow* Controller::_window = nullptr;
glm::ivec2 Controller::_windowDims = glm::ivec2(0);
bool Controller::_firstMouse = false;
glm::ivec4 Controller::_mouse = glm::ivec4(0);
glm::vec4 Controller::_clipSpaceMouse = glm::vec4(0.0);
double Controller::_deltaTime = 0.0;
double Controller::_prevTime = 0.0;
std::vector<std::shared_ptr<EventHandeler<KeyPressEvent>>> Controller::_keyPressEventHandelers = std::vector<std::shared_ptr<EventHandeler<KeyPressEvent>>>();
std::vector<std::shared_ptr<EventHandeler<MouseMovementEvent>>> Controller::_mouseMovementEventHandelers = std::vector<std::shared_ptr<EventHandeler<MouseMovementEvent>>>();
std::vector<std::shared_ptr<EventHandeler<MouseButtonEvent>>> Controller::_mouseButtonEventHandelers = std::vector<std::shared_ptr<EventHandeler<MouseButtonEvent>>>();
std::vector<std::shared_ptr<EventHandeler<ScrollEvent>>> Controller::_scrollEventHandelers = std::vector<std::shared_ptr<EventHandeler<ScrollEvent>>>();
std::vector<std::shared_ptr<EventHandeler<MovementEvent>>> Controller::_movementEventHandelers = std::vector<std::shared_ptr<EventHandeler<MovementEvent>>>();

Controller::Controller(int width, int height, const char* name, bool fullscreen)
{
	this->_windowDims = glm::ivec2(width, height);

	//INIT GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create GLFW window
	_window = (fullscreen) ?
		glfwCreateWindow(width, height, name, glfwGetPrimaryMonitor(), nullptr) :
		glfwCreateWindow(width, height, name, nullptr, nullptr);

	if (_window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(_window);

	//INIT GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		exit(-1);
	}

	//set up cursor
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(_window, cursor);

	//setup Input
	glfwSetKeyCallback(_window, keyPressCallback);
	glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	glfwSetCursorPosCallback(_window, mouseMovementCallback);
	glfwSetScrollCallback(_window, scrollCallback);
	_firstMouse = true;
	_mouse = glm::ivec4(0);
	_clipSpaceMouse = glm::vec4(0.0);
	_prevTime = 0.0;
}

Controller::~Controller()
{
	_keyPressEventHandelers.clear();
	_mouseMovementEventHandelers.clear();
	_mouseButtonEventHandelers.clear();
	_scrollEventHandelers.clear();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

bool Controller::isKeyPressed(int key)
{
	return glfwGetKey(_window, key) == GLFW_PRESS;
}

bool Controller::isLeftMousePressed()
{
	return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool Controller::isMiddleMousePressed()
{
	return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
}

bool Controller::isRightMousePressed()
{
	return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

GLFWwindow* Controller::getWindow()
{
	return _window;
}

glm::ivec4 Controller::getMouse()
{
	return _mouse;
}

glm::vec4 Controller::getClipSpaceMouse()
{
	return _clipSpaceMouse;
}

double Controller::getDeltaTime()
{
	return _deltaTime;
}

void Controller::keyPressCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	std::shared_ptr<KeyPressEvent> keyPressEvent = std::make_shared<KeyPressEvent>();
	keyPressEvent->deltaTime = _deltaTime;
	keyPressEvent->key = key;
	keyPressEvent->scancode = scancode;
	keyPressEvent->action = action;
	keyPressEvent->mods = mods;
	for (int i = 0; i < _keyPressEventHandelers.size(); i++)
	{
		_keyPressEventHandelers[i]->handle(keyPressEvent);
	}
}

void Controller::mouseMovementCallback(GLFWwindow * window, double x, double y)
{
	if (_firstMouse)
	{
		_firstMouse = false;
		_mouse = glm::vec4(x, y, 0.0, 0.0);
	}
	else
	{
		glm::ivec2 old = glm::ivec2(_mouse.x, _mouse.y);
		_mouse = glm::ivec4(x, y, x - old.x, old.y - y);
		_clipSpaceMouse = glm::vec4(_mouse.x, _windowDims.y - _mouse.y, _mouse.z, _mouse.w) / glm::vec4(float(_windowDims.x), float(_windowDims.y), float(_windowDims.x), float(_windowDims.y));
		_clipSpaceMouse = _clipSpaceMouse * 2.0f - 1.0f;
	}
	std::shared_ptr<MouseMovementEvent> mouseMovementEvent = std::make_shared<MouseMovementEvent>();
	mouseMovementEvent->deltaTime = _deltaTime;
	mouseMovementEvent->Mouse = _mouse;
	mouseMovementEvent->ClipSpaceMouse = _clipSpaceMouse;
	for (int i = 0; i < _mouseMovementEventHandelers.size(); i++)
	{
		_mouseMovementEventHandelers[i]->handle(mouseMovementEvent);
	}
}

void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::shared_ptr<MouseButtonEvent> mouseButtonEvent = std::make_shared<MouseButtonEvent>();
	mouseButtonEvent->deltaTime = _deltaTime;
	mouseButtonEvent->button = static_cast<MouseButton>(button);
	mouseButtonEvent->action = static_cast<Action>(action);
	for (int i = 0; i < _mouseButtonEventHandelers.size(); i++)
	{
		_mouseButtonEventHandelers[i]->handle(mouseButtonEvent);
	}
}

void Controller::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Controller::processMovementKeys()
{
	std::shared_ptr<MovementEvent> movementEvent = std::make_shared<MovementEvent>();
	movementEvent->deltaTime = _deltaTime;
	bool didMove = false;

	movementEvent->Forward = (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS);
	movementEvent->Backward = (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS);
	movementEvent->Left = (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS);
	movementEvent->Right = (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS);
	movementEvent->Up= (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS);
	movementEvent->Down = (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS);

	for (int i = 0; i < _movementEventHandelers.size(); i++)
	{
		_movementEventHandelers[i]->handle(movementEvent);
	}
}

bool Controller::shouldQuit()
{
	return glfwWindowShouldClose(_window);
}

void Controller::showMouse()
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Controller::hideMouse()
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Controller::update()
{
	glfwPollEvents();
	double currTime = glfwGetTime();
	_deltaTime = glfwGetTime() - _prevTime;
	_prevTime = currTime;
	processMovementKeys();
}

void Controller::swapBuffers()
{
	glfwSwapBuffers(_window);
}

void Controller::addKeyPressEventHandeler(std::shared_ptr<EventHandeler<KeyPressEvent>> eventHandeler)
{
	_keyPressEventHandelers.push_back(eventHandeler);
}

void Controller::addMouseMovementEventHandeler(std::shared_ptr<EventHandeler<MouseMovementEvent>> eventHandeler)
{
	_mouseMovementEventHandelers.push_back(eventHandeler);
}

void Controller::addMouseButtonEventHandeler(std::shared_ptr<EventHandeler<MouseButtonEvent>> eventHandeler)
{
	_mouseButtonEventHandelers.push_back(eventHandeler);
}

void Controller::addScrollEventHandeler(std::shared_ptr<EventHandeler<ScrollEvent>> eventHandeler)
{
	_scrollEventHandelers.push_back(eventHandeler);
}

void Controller::addMovementEventHandeler(std::shared_ptr<EventHandeler<MovementEvent>> eventHandeler)
{
	_movementEventHandelers.push_back(eventHandeler);
}

double Controller::getTime()
{
	return glfwGetTime();
}

