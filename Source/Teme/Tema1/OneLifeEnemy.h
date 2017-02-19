#pragma once
#include "Object.h"

class OneLifeEnemy : public Object
{
public:
	OneLifeEnemy() {};
	~OneLifeEnemy() {};
	static float sx, sy;
	static float radius;
	float speed;
	int lifes;
	OneLifeEnemy(float tx, float ty, float sx, float sy, float r, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, r, color), lifes(1), speed(speed) {};
	OneLifeEnemy(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, c, s, color), lifes(1), speed(speed) {};
};