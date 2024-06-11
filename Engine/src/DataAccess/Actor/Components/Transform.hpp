#pragma once
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

	[[nodiscard]] const glm::mat4 GetMat() const;
};
