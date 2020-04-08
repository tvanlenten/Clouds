#include "OpenGL\Controller.h"
#include "OpenGL\OpenGL.h"
#include "OpenGL\Shader.h"
#include "OpenGL\RenderTarget.h"
#include "OpenGL\VertexArray.h"
#include "OpenGL\MeshGenerator.h"
#include "OpenGL\Texture2D.h"
#include "OpenGL\Texture3D.h"
#include "OpenGL\TextureCubeMap.h"
#include "OpenGL\Camera.h"
#include "OpenGL\Buffer.h"

#include "Utils/StringUtils.h"

#include "ImGui\GUI.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL\TimeQuery.h"

#include <iostream>
#include <memory>


#include "Cloud/SkyboxGenerator.h"
#include "Cloud/CloudRenderer.h"
#include "Cloud/SceneRenderer.h"
#include "Cloud/CloudGenerator.h"
#include "Cloud/SkyboxRenderer.h"


//#define WIDTH 1920
//#define HEIGHT 1080

#define WIDTH 1280
#define HEIGHT 720

/*
	Renders a cool skybox I found
	Project will mainly exist in here or in cloud
	
	ImGui is a nice simple gui https://github.com/ocornut/imgui

	OpenGL is wrapped nicely in classes along with a GLFW controller to handel input and windows

*/
int main()
{
	Controller controller(WIDTH, HEIGHT, "OpenGL GUI", false);
	auto camera = std::make_shared<Camera>(glm::ivec2(WIDTH, HEIGHT), glm::vec3(0.5, 0.0, -0.5), 90.0f, 0.01f, 20.0f, 0.1f, 1.75f);
	controller.addMovementEventHandeler(camera);
	controller.addMouseMovementEventHandeler(camera);
	GUI gui(controller.getWindow());
	auto screen = std::make_shared<RenderTarget>(glm::ivec2(WIDTH, HEIGHT));


	//load test skybox
	std::string faces[] =
	{
		"textures/lakeCubemap/posx.jpg", "textures/lakeCubemap/negx.jpg",
		"textures/lakeCubemap/posy.jpg", "textures/lakeCubemap/negy.jpg",
		"textures/lakeCubemap/posz.jpg", "textures/lakeCubemap/negz.jpg"
	};
	auto skyboxTextue = std::make_shared<TextureCubeMap>(faces, false, false);
	
	auto skyboxRenderer = std::make_shared<SkyboxRenderer>(skyboxTextue);
	auto sceneRenderer = std::make_shared<SceneRenderer>();


	controller.showMouse();
	while (!controller.shouldQuit())
	{
		controller.update();
		gui.Start();

		if (controller.isRightMousePressed())
		{
			controller.hideMouse();
			camera->shouldUpdate(true);
		}
		else
		{
			controller.showMouse();
			camera->shouldUpdate(false);
		}

		screen->bind();
		screen->clear();

		skyboxRenderer->Draw(screen, camera->ProjectionMatrix(), camera->ViewMatrix());

		sceneRenderer->Draw(screen, skyboxTextue, camera->ProjectionViewMatrix(), camera->Position);

		/*
			ImGui test stuff
		*/
		ImGui::Begin("Test");
		ImGui::Text(std::string("Camera x: " + std::to_string(camera->Position.x)).c_str());
		ImGui::Text(std::string("Camera y: " + std::to_string(camera->Position.y)).c_str());
		ImGui::Text(std::string("Camera z: " + std::to_string(camera->Position.z)).c_str());

		sceneRenderer->Gui();

		ImGui::End();

		gui.End();
		controller.swapBuffers();
	}

	return 0;
}