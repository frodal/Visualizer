#include "PreCompiledHeader.h"
#include "TestTexture2D.h"

#include "Renderer.h"

namespace Test {

	TestTexture2D::TestTexture2D(std::string& name)
		: Test(name), textureSlot(0), projection(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		translationA(200, 200, 0), translationB(400, 200, 0)
	{
		/* Vertex positions*/
		float positions[] = {
			 -50.0f,-50.0f, 0.0f, 0.0f,
			  50.0f,-50.0f, 1.0f, 0.0f,
			  50.0f, 50.0f, 1.0f, 1.0f,
			 -50.0f, 50.0f, 0.0f, 1.0f
		};

		/* Triangle vertex indices (position[index])*/
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		Renderer renderer;
		renderer.EnableBlend();

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, lengthof(indices));

		shader = std::make_unique<Shader>("resources/shaders/Texture.glsl");
		shader->Bind();
		//shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
		texture = std::make_unique<Texture>("resources/textures/Pyramids.png");
		shader->SetUniform1i("u_Texture", textureSlot);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		// Update projection matrix with latest viewport size
		float width = static_cast<float>(window->GetWidth());
		float height = static_cast<float>(window->GetHeight());
		projection = glm::ortho(0.0f, width, 0.0f, height);
	}

	void TestTexture2D::OnRender()
	{
		Renderer renderer;
		renderer.SetClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		renderer.Clear();

		texture->Bind(textureSlot);

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // Translates the model
			glm::mat4 MVP = projection * view * model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", MVP);

			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // Translates the model
			glm::mat4 MVP = projection * view * model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", MVP);

			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		float width = static_cast<float>(window->GetWidth());
		float height = static_cast<float>(window->GetHeight());
		
		ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, glm::max(width, height));
		ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, glm::max(width, height));
		translationA.x = glm::min(translationA.x, width);
		translationA.y = glm::min(translationA.y, height);
		translationB.x = glm::min(translationB.x, width);
		translationB.y = glm::min(translationB.y, height);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}