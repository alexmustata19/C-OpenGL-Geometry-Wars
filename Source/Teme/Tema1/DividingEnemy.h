/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/
#pragma once
#include "Object.h"

class DividingEnemy : public Object
{
public:
	DividingEnemy() {};
	~DividingEnemy() {};
	static float sx, sy;
	static glm::vec3 color;
	static float radius;
	float speed;
	int lifes;
	DividingEnemy(float tx, float ty, float sx, float sy, float r, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, r, color), lifes(1), speed(speed) {};
	DividingEnemy(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color, float speed) : Object(tx, ty, sx, sy, c, s, color), lifes(1), speed(speed) {};
};