#include "PreCompiledHeader.h"
#include "RayTracingRenderer.h"

#include <execution>

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
		uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
		uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
		uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	static uint32_t PCG_Hash(uint32_t input)
	{
		uint32_t state = input * 747796405u + 2891336453u;
		uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
		return (word >> 22u) ^ word;
	}

	static float RandomFloat(uint32_t& seed)
	{
		seed = PCG_Hash(seed);
		return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	}

	static glm::vec3 InUnitSphere(uint32_t& seed)
	{
		return glm::normalize(glm::vec3(RandomFloat(seed) * 2.0f - 1.0f, RandomFloat(seed) * 2.0f - 1.0f, RandomFloat(seed) * 2.0f - 1.0f));
	}

}

RayTracingRenderer::~RayTracingRenderer()
{
	delete[] m_FinalImage;
	delete[] m_ImageData;
	delete[] m_AccumulationData;
}

void RayTracingRenderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_width == width && m_height == height)
			return;
	}

	m_width = width;
	m_height = height;
	ResetFrameIndex();

	delete[] m_FinalImage;
	m_FinalImage = new uint32_t[width * height];

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[width * height];

	m_ImageHorizontalIter.resize(width);
	m_ImageVerticalIter.resize(height);
	for (uint32_t i = 0; i < width; i++)
		m_ImageHorizontalIter[i] = i;
	for (uint32_t i = 0; i < height; i++)
		m_ImageVerticalIter[i] = i;
}

void RayTracingRenderer::Render(const Scene& scene, const RayTracingCamera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, static_cast<size_t>(m_width) * static_cast<size_t>(m_height) * sizeof(glm::vec4));

#define MT 1
#if MT

	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this](uint32_t y)
		{
			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
			[this, y](uint32_t x)
				{
					glm::vec4 color = PerPixel(x, y);
					m_AccumulationData[x + y * m_width] += color;

					glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_width];
					accumulatedColor /= static_cast<float>(m_FrameIndex);

					accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
					m_ImageData[x + y * m_width] = Utils::ConvertToRGBA(accumulatedColor);
				});
		});

#else

	for (uint32_t y = 0; y < m_height; y++)
	{
		for (uint32_t x = 0; x < m_width; x++)
		{
			glm::vec4 color = PerPixel(x, y);
			m_AccumulationData[x + y * m_width] += color;

			glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_width];
			accumulatedColor /= static_cast<float>(m_FrameIndex);

			accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_width] = Utils::ConvertToRGBA(accumulatedColor);
		}
	}

#endif

	for (uint32_t y = 0; y < m_height; y++)
	{
		for (uint32_t x = 0; x < m_width; x++)
		{
			m_FinalImage[x + y * m_width] = m_ImageData[x + y * m_width];
		}
	}

	if (m_Settings.Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;
}

glm::vec4 RayTracingRenderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray{};
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(m_width)];

	glm::vec3 light(0.0f);
	glm::vec3 contribution(1.0f);

	uint32_t seed = x + y * m_width;
	seed *= m_FrameIndex;

	constexpr int bounces = 5;
	for (int i = 0; i < bounces; i++)
	{
		seed += i;

		RayTracingRenderer::HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f)
		{
			contribution *= m_ActiveScene->skyColor;
			light += m_ActiveScene->GetSkyEmission();
			break;
		}

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];

		contribution *= material.Albedo;
		light += material.GetEmission();

		constexpr float offset = std::numeric_limits<float>::epsilon();
		ray.Origin = payload.WorldPosition + payload.WorldNormal * offset;
		ray.Direction = glm::normalize(payload.WorldNormal + Utils::InUnitSphere(seed));
	}

	return glm::vec4(light * contribution, 1.0f);
}

RayTracingRenderer::HitPayload RayTracingRenderer::TraceRay(const Ray& ray)
{
	int closestSphereIndex = -1;
	float hitDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];
		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		// float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0.0f && closestT < hitDistance)
		{
			hitDistance = closestT;
			closestSphereIndex = static_cast<int>(i);
		}
	}

	if (closestSphereIndex < 0)
		return Miss(ray);

	return ClosestHit(ray, hitDistance, closestSphereIndex);
}

RayTracingRenderer::HitPayload RayTracingRenderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	RayTracingRenderer::HitPayload payload{};
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.Origin - closestSphere.Position;
	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);

	payload.WorldPosition += closestSphere.Position;

	return payload;
}

RayTracingRenderer::HitPayload RayTracingRenderer::Miss(const Ray& ray)
{
	RayTracingRenderer::HitPayload payload{};
	payload.HitDistance = -1.0f;
	return payload;
}