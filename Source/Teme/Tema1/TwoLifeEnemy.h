#pragma once
#include "Object.h"
class TwoLifeEnemy : public Object
{
public:
	TwoLifeEnemy() {};
	~TwoLifeEnemy() {};
	int lifes;
	float speedOneLife, speedTwoLifes;
	float static radius;
	float startShrinkAnimationTime;
	static float sxOneLife, syOneLife, sxTwoLifes, syTwoLifes, shrinkTime;
	static glm::vec3 colorOneLife, colorTwoLifes;
	TwoLifeEnemy(float tx, float ty, float sx, float sy, float r, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, r, color), lifes(2), speedOneLife(2 * speed), speedTwoLifes(speed) {};
	TwoLifeEnemy(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, c, s, color), lifes(2), speedOneLife(2 * speed), speedTwoLifes(speed) {};
};

