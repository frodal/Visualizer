#include "PreCompiledHeader.h"
#include "TestRayTracing.h"

#include "Renderer.h"

Test::TestRayTracing::TestRayTracing(std::string& name)
	: Test(name), width(1280), height(720),
	textureSlot(0), camera(45.0f, 0.1f, 100.f)
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

	Renderer renderer;
	renderer.EnableBlend();

	vertexArray = std::make_unique<VertexArray>();
	vertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));
	VertexBufferLayout bufferLayout;
	bufferLayout.Push<float>(2);
	bufferLayout.Push<float>(2);
	vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

	indexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(lengthof(indices)));

	shader = std::make_unique<Shader>("resources/shaders/Texture.glsl");
	shader->Bind();
	shader->SetUniform1i("u_Texture", textureSlot);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	shader->SetUniformMat4f("u_MVP", projection * view);

	texture = std::make_unique<Texture>();

	// Setup scene
	scene.skyEmissionPower = 0.8f;

	Material& pinkSphereMaterial = scene.Materials.emplace_back();
	pinkSphereMaterial.Albedo = { 1.0f, 0.0f, 1.0f };
	pinkSphereMaterial.Roughness = 0.0f;

	Material& greenSphereMaterial = scene.Materials.emplace_back();
	greenSphereMaterial.Albedo = { 0.4f, 0.8f, 0.4f };
	greenSphereMaterial.Roughness = 0.1f;

	Material& orangeSphereMaterial = scene.Materials.emplace_back();
	orangeSphereMaterial.Albedo = { 0.8f, 0.5f, 0.2f };
	orangeSphereMaterial.Roughness = 0.1f;
	orangeSphereMaterial.EmissionColor = orangeSphereMaterial.Albedo;
	orangeSphereMaterial.EmissionPower = 5.0f;

	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;
		scene.Spheres.push_back(sphere);
	}

	{
		Sphere sphere;
		sphere.Position = { 2.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 2;
		scene.Spheres.push_back(sphere);
	}

	{
		Sphere sphere;
		sphere.Position = { 0.0f, -101.0f, 0.0f };
		sphere.Radius = 100.0f;
		sphere.MaterialIndex = 1;
		scene.Spheres.push_back(sphere);
	}
}

Test::TestRayTracing::~TestRayTracing()
{
}

void Test::TestRayTracing::OnUpdate(float deltaTime)
{
	if (camera.OnUpdate(deltaTime))
		rayTracingRenderer.ResetFrameIndex();
}

void Test::TestRayTracing::OnRender()
{
	width = window->GetWidth();
	height = window->GetHeight();

	rayTracingRenderer.OnResize(width, height);
	camera.OnResize(width, height);

	rayTracingRenderer.Render(scene, camera);
	texture->UpdateTexture(reinterpret_cast<unsigned char*>(rayTracingRenderer.GetFinalImage()), width, height);
	
	Renderer renderer;
	texture->Bind(textureSlot);
	renderer.Draw(*vertexArray, *indexBuffer, *shader);
}

void Test::TestRayTracing::OnImGuiRender()
{
	ImGui::Text("Settings");
	ImGui::Separator();
	ImGui::Checkbox("Accumulate", &rayTracingRenderer.GetSettings().Accumulate);
	if (ImGui::Button("Reset"))
		rayTracingRenderer.ResetFrameIndex();

	ImGui::Begin("Scene");
	ImGui::Separator();
	ImGui::ColorEdit3("Sky color", glm::value_ptr(scene.skyColor));
	ImGui::DragFloat("Sky emission power", &scene.skyEmissionPower, 0.05f, 0.0f, FLT_MAX);
	ImGui::Separator();
	for (size_t i = 0; i < scene.Spheres.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Sphere %d", i);

		Sphere& sphere = scene.Spheres[i];
		ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
		ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
		ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)scene.Materials.size() - 1);

		ImGui::Separator();

		ImGui::PopID();
	}

	for (size_t i = 0; i < scene.Materials.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Material %d", i);

		Material& material = scene.Materials[i];
		ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
		ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
		ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

		ImGui::Separator();

		ImGui::PopID();
	}

	ImGui::End();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test::TestRayTracing::SetWindow(Window* window)
{
	this->window = window;
	width = window->GetWidth();
	height = window->GetHeight();

	camera.SetWindow(window);
}
