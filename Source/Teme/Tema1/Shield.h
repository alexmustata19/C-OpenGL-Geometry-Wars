/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/
#pragma once
#include "Object.h"

class Shield : public Object {
public:
	static glm::vec3 color;
	static float sx, sy;
	static float radius;
	Shield() {};
	~Shield() {};
	Shield(float tx, float ty, float sx, float sy, float r, glm::vec3 color) : Object(tx, ty, sx, sy, r, color) {};
	Shield(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color) : Object(tx, ty, sx, sy, c, s, color) {};
};