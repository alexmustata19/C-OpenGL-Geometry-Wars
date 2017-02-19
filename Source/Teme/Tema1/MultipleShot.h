/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/
#pragma once
#include "Object.h"

class MultipleShot : public Object {
public:
	static glm::vec3 color;
	static float sx, sy;
	static float radius;
	MultipleShot() {};
	~MultipleShot() {};
	MultipleShot(float tx, float ty, float sx, float sy, float r, glm::vec3 color) : Object(tx, ty, sx, sy, r, color) {};
	MultipleShot(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color) : Object(tx, ty, sx, sy, c, s, color) {};
};