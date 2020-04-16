#include "PreCompiledHeader.h"
#include "TestDiscreteCircle.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Test {

	TestDiscreteCircle::TestDiscreteCircle(std::string& name)
		: Test(name), projection(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(640, 360, 0))),
		model(glm::mat4(1.0f)),Ntriangles(6),radius(200),
		circleColor{ 0.0f, 0.0f, 1.0f, 1.0f }, backgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f }
	{
		Renderer renderer;
		renderer.EnableBlend();

		shader = std::make_unique<Shader>("resources/shaders/ClearColor.glsl");
		shader->Bind();
		shader->SetUniform4f("u_Color", circleColor[0], circleColor[1], circleColor[2], circleColor[3]);
	}

	TestDiscreteCircle::~TestDiscreteCircle()
	{
	}

	void TestDiscreteCircle::OnUpdate(float deltaTime)
	{
	}

	void TestDiscreteCircle::OnRender()
	{
		Renderer renderer;

		renderer.SetClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		renderer.Clear();

		float* positions = new float[(static_cast<size_t>(Ntriangles) + 1) * 2];

		positions[0] = 0.0f;
		positions[1] = 0.0f;
		for (int i = 1; i < Ntriangles + 1; i++)
		{
			positions[i*2] = glm::cos(-2 * glm::pi<float>() * (i - 1) / static_cast<float>(Ntriangles));
			positions[i*2 + 1] = glm::sin(-2 * glm::pi<float>() * (i - 1) / static_cast<float>(Ntriangles));
		}

		unsigned int* indices = new unsigned int[static_cast<size_t>(Ntriangles) * 3];

		for (int i = 0; i < Ntriangles; i++)
		{
			indices[i * 3] = 0;
			indices[i * 3 + 1] = i + 1;
			indices[i * 3 + 2] = i + 2;
		}
		indices[Ntriangles * 3 - 1] = 1;

		VertexArray vertexArray;
		VertexBuffer vertexBuffer(positions, (Ntriangles + 1) * 2 * sizeof(float));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, bufferLayout);

		IndexBuffer indexBuffer(indices, Ntriangles * 3);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, 0.0f));
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view * model);
		shader->SetUniform4f("u_Color", circleColor[0], circleColor[1], circleColor[2], circleColor[3]);

		renderer.Draw(vertexArray, indexBuffer, *shader);

		delete[] positions;
		delete[] indices;
	}

	void TestDiscreteCircle::OnImGuiRender()
	{
		ImGui::SliderInt("Number of triangles", &Ntriangles, 3, 100);
		ImGui::SliderFloat("Radius", &radius, 0.0f, 400.0f);
		ImGui::ColorEdit4("Circle color", circleColor);
		ImGui::ColorEdit4("Background color", backgroundColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}