#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{
	Mesh * CreateCircle(std::string name, glm::vec3 color);

	Mesh * CreateShip(std::string name, glm::vec3 color);

	Mesh * CreateMissle(std::string name, glm::vec3 color);

	Mesh * CreateBonusLife(std::string name, glm::vec3 color);

	Mesh * CreateLifeBar(std::string name, glm::vec3 color);

	Mesh * CreateMultipleShot(std::string name, glm::vec3 color);

	Mesh * CreateInvincibility(std::string name, glm::vec3 color);

	Mesh * CreateSnowFlake(std::string name, glm::vec3 color);

	Mesh * CreateBigBang(std::string name, glm::vec3 color);

	Mesh * CreateShield(std::string name, glm::vec3 color);

	Mesh * CreateSlowDown(std::string name, glm::vec3 color);

}

