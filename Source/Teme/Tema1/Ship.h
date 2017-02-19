#pragma once

#include "Object.h"

class Ship : public Object
{
public:
	int lifes;
	static float radius;
	static float speed;
	static float sx, sy;
	static glm::vec3 color, invincibleColor;
	Ship();
	~Ship();
	Ship(float tx, float ty, float sx, float sy, float r, glm::vec3 color, int lifes) : Object(tx, ty, sx, sy, r, color), lifes(lifes) {};
	Ship(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color, int lifes) : Object(tx, ty, sx, sy, c, s, color), lifes(lifes) {};
};

