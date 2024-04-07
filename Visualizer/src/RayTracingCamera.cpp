#include "PreCompiledHeader.h"
#include "RayTracingCamera.h"


RayTracingCamera::RayTracingCamera(float verticalFOV, float nearClip, float farClip)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Position = glm::vec3(0.0f, 0.0f, 6.0f);
}

RayTracingCamera::RayTracingCamera(float verticalFOV, float nearClip, float farClip, Window* window)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Position = glm::vec3(0.0f, 0.0f, 6.0f);
	m_window = window;
}

bool RayTracingCamera::OnUpdate(float ts)
{
	if (!m_window)
		return false;

	glm::vec2 mousePos = m_window->GetMousePosition();
	glm::vec2 delta = (mousePos - m_LastMousePosition);
	m_LastMousePosition = mousePos;

	if (m_window->GetMouseButton(GLFW_MOUSE_BUTTON_2) != GLFW_PRESS)
	{
		m_window->SetCursorMode(GLFW_CURSOR_NORMAL);
		return false;
	}

	m_window->SetCursorMode(GLFW_CURSOR_DISABLED);

	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float speed = GetTranslationSpeed();

	// Movement
	if (m_window->GetKey(GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += m_ForwardDirection * speed * ts;
		moved = true;
	}
	else if (m_window->GetKey(GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position -= m_ForwardDirection * speed * ts;
		moved = true;
	}
	if (m_window->GetKey(GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position -= rightDirection * speed * ts;
		moved = true;
	}
	else if (m_window->GetKey(GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += rightDirection * speed * ts;
		moved = true;
	}
	if (m_window->GetKey(GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_Position -= upDirection * speed * ts;
		moved = true;
	}
	else if (m_window->GetKey(GLFW_KEY_E) == GLFW_PRESS)
	{
		m_Position += upDirection * speed * ts;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			glm::angleAxis(-yawDelta, upDirection)));
		m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
		RecalculateRayDirections();
	}

	return moved;
}

void RayTracingCamera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
	RecalculateRayDirections();
}

float RayTracingCamera::GetRotationSpeed()
{
	return 0.3f * 0.002f;
}

float RayTracingCamera::GetTranslationSpeed()
{
	return 5.0f;
}

void RayTracingCamera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), static_cast<float>(m_ViewportWidth), static_cast<float>(m_ViewportHeight), m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void RayTracingCamera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	m_InverseView = glm::inverse(m_View);
}

void RayTracingCamera::RecalculateRayDirections()
{
	m_RayDirections.resize(static_cast<size_t>(m_ViewportWidth) * static_cast<size_t>(m_ViewportHeight));

	for (uint32_t y = 0; y < m_ViewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			glm::vec2 coord = { static_cast<float>(x) / static_cast<float>(m_ViewportWidth), static_cast<float>(y) / static_cast<float>(m_ViewportHeight) };
			coord = coord * 2.0f - 1.0f; // -1 -> 1

			glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDirection = glm::vec3(m_InverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
			m_RayDirections[static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(m_ViewportWidth)] = rayDirection;
		}
	}
}