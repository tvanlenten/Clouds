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
	Clouds!

	ImGui is a nice simple gui https://github.com/ocornut/imgui
*/
int main()
{
	Controller controller(WIDTH, HEIGHT, "Clouds!", false);
	auto camera = std::make_shared<Camera>(glm::ivec2(WIDTH, HEIGHT), glm::vec3(0.5, 0.0, -0.5), 90.0f, 0.01f, 20.0f, 0.1f, 1.75f);
	controller.addMovementEventHandeler(camera);
	controller.addMouseMovementEventHandeler(camera);
	GUI gui(controller.getWindow());
	auto screen = std::make_shared<RenderTarget>(glm::ivec2(WIDTH, HEIGHT));


	// create generators
	auto skyboxGenerator = std::make_shared<SkyboxGenerator>(glm::ivec2(512, 512));
	auto cloudGenerator = std::make_shared<CloudGenerator>(glm::ivec3(128), glm::i32(2));

	// generate textures
	auto skyboxTexture = skyboxGenerator->Generate(glm::vec3(2.0, 1.0, 0.0));
	auto cloudVolume = cloudGenerator->Generate();


	// create renderers
	auto skyboxRenderer = std::make_shared<SkyboxRenderer>(skyboxTexture);
	auto sceneRenderer = std::make_shared<SceneRenderer>();
	auto cloudRenderer = std::make_shared<CloudRenderer>(glm::ivec2(WIDTH, HEIGHT), cloudVolume);

	// create framebuffer to store depth from previous passes for cloudRenderer
	auto target = std::make_shared<RenderTarget>(glm::ivec2(WIDTH, HEIGHT));
	target->addColorAttachment();
	target->addDepthAttachment();
	target->bindAttachments();
	
	//DEBUG VALUES FOR TEXTURE
	auto texDebugShader = std::make_shared<Shader>("shaders/screen.vert", "shaders/debugCloudTex.frag", nullptr, false);
	float debuggingTex = 1.0f;
	float slice = 0.5;

	// main draw loop
	controller.showMouse();
	while (!controller.shouldQuit())
	{
		// get user input and start gui
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

		// Init Render Pass
		target->bind();
		target->clear();

		// draw skybox
		skyboxRenderer->Draw(target, camera);

		// draw scene
		sceneRenderer->Draw(target, skyboxTexture, camera);

		// draw clouds
		cloudRenderer->Draw(target, camera);

		// draw debug clound texture to screen quad
		if (debuggingTex == 1.0f) {			
			texDebugShader->Start();
			texDebugShader->Set("cloudVolume", 0);
			texDebugShader->Set("slice", slice);

			target->renderToTarget(); // draw screen quad
			texDebugShader->End();
		}
		
		// clear the screen before blitting
		screen->bind();
		screen->clear();
		// blit the framebuffer to the screen
		screen->blitToTarget(target, GL_COLOR_BUFFER_BIT, GL_LINEAR);


		// ImGui Test stuff
		ImGui::Begin("Test");
		ImGui::Text(std::string("Camera Front x: " + std::to_string(camera->Front.x)).c_str());
		ImGui::Text(std::string("Camera Front y: " + std::to_string(camera->Front.y)).c_str());
		ImGui::Text(std::string("Camera Front z: " + std::to_string(camera->Front.z)).c_str());
		sceneRenderer->Gui();
		skyboxRenderer->Gui();

		// Debug Tex
		ImGui::DragFloat("slice", &slice, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("Debug", &debuggingTex, 1.0f, 0.0f, 1.0f);

		ImGui::End();

		// swap buffers and end GUI
		gui.End();
		controller.swapBuffers();
	}

	return 0;
}