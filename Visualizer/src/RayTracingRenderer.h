#pragma once
#include "PreCompiledHeader.h"

#include "RayTracingCamera.h"

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0.0f;

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }
};

struct Sphere
{
	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;

	int MaterialIndex = 0;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
	glm::vec3 skyColor = glm::vec3(1.0f);
	float skyEmissionPower = 1.0f;

	glm::vec3 GetSkyEmission() const { return skyColor * skyEmissionPower; }
};

class RayTracingRenderer
{
public:
	struct Settings
	{
		bool Accumulate = true;
	};
public:
	RayTracingRenderer() = default;
	~RayTracingRenderer();

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const RayTracingCamera& camera);

	uint32_t* GetFinalImage() const { return m_FinalImage; }

	void ResetFrameIndex() { m_FrameIndex = 1; }
	Settings& GetSettings() { return m_Settings; }
private:
	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen

	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);
private:
	uint32_t* m_FinalImage = nullptr;
	uint32_t m_width = 0, m_height = 0;
	Settings m_Settings;

	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	const Scene* m_ActiveScene = nullptr;
	const RayTracingCamera* m_ActiveCamera = nullptr;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;

	uint32_t m_FrameIndex = 1;
};