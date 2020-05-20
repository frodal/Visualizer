#include "PreCompiledHeader.h"
#include "TestMandelbrot.h"

#include "Renderer.h"

namespace Test {

	TestMandelbrot::TestMandelbrot(std::string& name)
		: Test(name), width(1280), height(720),
		projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height))),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), 
		position(glm::dvec2(-0.5, 0.0)),
		scale(1.0), MaxIterations(5000)
	{
		/* Vertex positions*/
		float positions[] = {
								   0.0f,                       0.0f, 0.0f, 0.0f,
			  static_cast<float>(width),                       0.0f, 1.0f, 0.0f,
			  static_cast<float>(width), static_cast<float>(height), 1.0f, 1.0f,
								   0.0f, static_cast<float>(height), 0.0f, 1.0f
		};

		/* Triangle vertex indices (position[index])*/
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, lengthof(indices));

		shader = std::make_unique<Shader>("resources/shaders/Mandelbrot.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view);
		shader->SetUniform1i("MaxIterations", MaxIterations);
		shader->SetUniform1d("scale", scale);
		shader->SetUniform2d("pos", position.x, position.y);
		shader->SetUniform1f("aspectRatio", static_cast<float>(width) / height);
	}

	TestMandelbrot::~TestMandelbrot()
	{

	}

	void TestMandelbrot::OnUpdate(float deltaTime)
	{
	}

	void TestMandelbrot::OnRender()
	{
		Renderer renderer;

		shader->Bind();
		shader->SetUniform1i("MaxIterations", MaxIterations);
		shader->SetUniform1d("scale", scale);
		shader->SetUniform2d("pos", position.x, position.y);

		renderer.Draw(*vertexArray, *indexBuffer, *shader);
	}

	void TestMandelbrot::OnImGuiRender()
	{
		ImGui::Dummy(ImVec2(19.0f, 0.0f)); ImGui::SameLine();
		bool moveUp = ImGui::ArrowButton("Up", ImGuiDir_Up);
		bool moveLeft = ImGui::ArrowButton("Left", ImGuiDir_Left); ImGui::SameLine();
		bool moveDown = ImGui::ArrowButton("Down", ImGuiDir_Down); ImGui::SameLine();
		bool moveRight = ImGui::ArrowButton("Right", ImGuiDir_Right);
		if (moveLeft)
		{
			position.x -= scale;
		}
		else if (moveUp)
		{
			position.y += scale;
		}
		else if (moveDown)
		{
			position.y -= scale;
		}
		else if (moveRight)
		{
			position.x += scale;
		}

		if (ImGui::Button("Zoom in"))
		{
			scale /= 1.1;
		}
		else if (ImGui::Button("Zoom out"))
		{
			scale *= 1.1;
		}

		ImGui::SliderInt("Max iterations", &MaxIterations, 1000, 10000);
		
		ImGui::Text("Scale %e", scale);
		ImGui::Text("Position (%.16f, %.16f)", position.x, position.y);

		if (ImGui::Button("Reset"))
		{
			scale = 1.0;
			position = { -0.5,0.0 };
		}
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}