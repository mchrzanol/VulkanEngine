#pragma once

#include "Libraries.h"

class ViewMatricies
{
private:
	glm::vec3 camFront;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float camSpeed;
	float sensitivity;
	glm::vec3 camPosition;
	glm::vec3 lastCamPosition;

	glm::vec3 camUp;
	glm::mat4 ViewMatrix;

	bool firstSet = true;
public:
	ViewMatricies() : camSpeed(0.005f), sensitivity(0.5f), camFront(0.0f, 0.0f, -1.0f),
		camPosition(0.f, 0.f, 1.f), camUp(0.f, 1.f, 0.f), ViewMatrix(1.0f)
	{};
	void update()
	{
		lastCamPosition = camPosition;
		ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, camUp);
	}
	void UpdateFront(std::pair<double, double>  deltapos)
	{
		if (firstSet) {
			firstSet = false;
			return;
		}


		yaw += (deltapos.first * sensitivity);
		pitch += (deltapos.second * sensitivity);
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		camFront.y = sin(glm::radians(pitch));
		camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camFront = glm::normalize(camFront);

	}
	inline void ChangePosz(float var) { camPosition += glm::vec3(var) * camFront; };
	inline void ChangePosx(float var) { camPosition += glm::normalize(glm::cross(camFront, camUp)) * glm::vec3(var); };
	inline void ChangeSen(float f) { sensitivity = f;  };
	inline const glm::mat4 GetView() { return ViewMatrix; };
	inline glm::vec3 GetCamPos() { return camPosition; };
};