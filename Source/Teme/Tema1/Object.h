/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/
#pragma once
#include <Core/GPU/Mesh.h>

class Object
{
public:
	float tx, ty, sx, sy, r, c, s;
	glm::vec3 color;
	Object();
	Object(float tx, float ty, float sx, float sy, float r, glm::vec3 color) : tx(tx), ty(ty), sx(sx), sy(sy), r(r), color(color) {};
	Object(float tx, float ty, float sx, float sy, float c, float s, glm::vec3 color) : tx(tx), ty(ty), sx(sx), sy(sy), c(c), s(s), color(color) {};
	Mesh* mesh;
	~Object();
};

