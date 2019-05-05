#include "PreCompiledHeader.h"
#include "TestTexture2D.h"

#include "Renderer.h"

namespace Test {

	TestTexture2D::TestTexture2D(std::string& name)
		: textureSlot(0), projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		translationA(200, 200, 0), translationB(400, 200, 0)
	{
		testName = name;

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

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(positions, lengthof(positions) * sizeof(float));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, lengthof(indices));

		shader = std::make_unique<Shader>("resources/shaders/Texture.shader");
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
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

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
		ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}