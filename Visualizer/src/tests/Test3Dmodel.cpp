#include "PreCompiledHeader.h"
#include "Test3Dmodel.h"
#include "Renderer.h"
#include "Window.h"

namespace Test {

	Test3Dmodel::Test3Dmodel(std::string& name)
		: Test(name), projection(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, 0.1f, 1000.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -500.0f))),
		model(glm::mat4(1.0f)), size(100.0f), speed(1.0f), rotationAngle(0),
		modelColor{ 1.0f, 1.0f, 1.0f, 1.0f },
		backgroundColor{ 0.7f, 0.7f, 0.7f, 1.0f }
	{

		model3D = std::make_unique<Model>("resources/models/backpack/backpack.obj");

		Renderer renderer;
		renderer.EnableBlend();
		renderer.EnableDepth();

		shader = std::make_unique<Shader>("resources/shaders/Model3D.glsl");
	}

	Test3Dmodel::~Test3Dmodel()
	{
	}

	void Test3Dmodel::OnUpdate(float deltaTime)
	{
		// Update projection matrix with latest viewport size
		float halfWidth = static_cast<float>(window->GetWidth()) / 2.0f;
		float halfHeight = static_cast<float>(window->GetHeight()) / 2.0f;
		projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 1000.0f);

		// Update model scale and rotation
		model = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size)); // Scales the model
		rotationAngle += deltaTime * speed;
		model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // rotates the model about the vector
	}

	void Test3Dmodel::OnRender()
	{
		// Binds shader and sets uniforms in shader
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view * model);
		shader->SetUniformMat4f("u_NormalMatrix", glm::transpose(glm::inverse(model)));
		shader->SetUniformMat4f("u_ModelMatrix", model);
		shader->SetUniform4f("u_Color", modelColor[0], modelColor[1], modelColor[2], modelColor[3]);

		// Render
		Renderer renderer;
		renderer.SetClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		renderer.Clear();
		renderer.Draw(*model3D, *shader);
	}

	void Test3Dmodel::OnImGuiRender()
	{
		ImGui::SliderFloat("3D model size", &size, 20.0f, 150.0f);
		ImGui::SliderFloat("Rotation speed", &speed, 0.0f, 10.0f);
		ImGui::ColorEdit4("3D model tint color", modelColor);
		ImGui::ColorEdit4("Background color", backgroundColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}