/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/
#pragma once
#include "Object2D.h"
#include <Core/Engine.h>

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(4, 0, 0), color),
		VertexFormat(glm::vec3(3.69, 1.52, 0), color),
		VertexFormat(glm::vec3(2.82, 2.82, 0), color),
		VertexFormat(glm::vec3(1.52, 3.69, 0), color),
		VertexFormat(glm::vec3(0, 4, 0), color),
		VertexFormat(glm::vec3(-1.52, 3.69, 0), color),
		VertexFormat(glm::vec3(-2.82, 2.82, 0), color),
		VertexFormat(glm::vec3(-3.69, 1.52, 0), color),
		VertexFormat(glm::vec3(-4, 0, 0), color),
		VertexFormat(glm::vec3(-3.69, -1.52, 0), color),
		VertexFormat(glm::vec3(-2.82, -2.82, 0), color),
		VertexFormat(glm::vec3(-1.52, -3.69, 0), color),
		VertexFormat(glm::vec3(0, -4, 0), color),
		VertexFormat(glm::vec3(1.52, -3.69, 0), color),
		VertexFormat(glm::vec3(2.82, -2.82, 0), color),
		VertexFormat(glm::vec3(3.69, -1.52, 0), color)
	};

	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	circle->SetDrawMode(GL_LINE_LOOP);

	circle->InitFromData(vertices, indices);
	return circle;
}

/*Functia returneaza tipul de mesh care va fi folosit
la desenarea navei si a inamicilor.*/
Mesh* Object2D::CreateShip(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0,0,0), color),
		VertexFormat(glm::vec3(2, 1.5, 0), color),
		VertexFormat(glm::vec3(2, -1.5, 0), color),
		VertexFormat(glm::vec3(-2, 1.5, 0), color),
		VertexFormat(glm::vec3(-2, -1.5, 0), color)
	};

	Mesh* ship = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 3, 0, 4, 2 };
	ship->InitFromData(vertices, indices);
	return ship;
}

/*Functia creeaza tipul de mesh care va fi folosit la
desenarea proiectilelor.*/
Mesh* Object2D::CreateMissle(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0.5, 0), color),
		VertexFormat(glm::vec3(1, -0.5, 0), color),
		VertexFormat(glm::vec3(-1, 0.5, 0), color),
		VertexFormat(glm::vec3(-1, -0.5, 0), color)
	};

	Mesh* missle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 2, 1,
		0, 4, 2,
		0, 3, 4,
		0, 1, 3
	};
	missle->InitFromData(vertices, indices);
	return missle;
}

/*Functia returneaza mesh-ul care va fi folosit la desenarea
simbololui de viata bonus.*/
Mesh* Object2D::CreateBonusLife(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0.5, 0), color),
		VertexFormat(glm::vec3(1, -0.5, 0), color),
		VertexFormat(glm::vec3(0.5, -1, 0), color),
		VertexFormat(glm::vec3(-0.5, -1, 0), color),
		VertexFormat(glm::vec3(-1, 0.5, 0), color),
		VertexFormat(glm::vec3(-1, -0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 1, 0), color),
		VertexFormat(glm::vec3(0.5, 1, 0), color)
	};

	Mesh* bonusLife = new Mesh(name);
	std::vector<unsigned short> indices = {
		0,2,1,
		0,4,3,
		0,6,5,
		0,8,7
	};
	bonusLife->InitFromData(vertices, indices);
	return bonusLife;
}

/*Functia returneaza mesh-ul care va fi folosit la reprezentarea
numarului de vieti al jucatorului.*/
Mesh* Object2D::CreateLifeBar(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0.5, 0), color),
		VertexFormat(glm::vec3(1, -0.5, 0), color),
		VertexFormat(glm::vec3(-1, 0.5, 0), color),
		VertexFormat(glm::vec3(-1, -0.5, 0), color)
	};

	Mesh* lifeBar = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 2, 1,
		0, 4, 2,
		0, 3, 4,
		0, 1, 3
	};
	lifeBar->InitFromData(vertices, indices);
	return lifeBar;
}

/*Functia creeaza simbolul pentru bonusul de proiectile
lansate simultan.*/
Mesh* Object2D::CreateMultipleShot(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0.5, -0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, -0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0.5, 0, 0), color),
		VertexFormat(glm::vec3(1, 0.5, 0), color),
		VertexFormat(glm::vec3(1, -0.5, 0), color),
		VertexFormat(glm::vec3(0, -0.5, 0), color),
		VertexFormat(glm::vec3(0.5, -1, 0), color),
		VertexFormat(glm::vec3(-0.5, -1, 0), color),
		VertexFormat(glm::vec3(-0.5, 0, 0), color),
		VertexFormat(glm::vec3(-1, -0.5, 0), color),
		VertexFormat(glm::vec3(-1, 0.5, 0), color),
		VertexFormat(glm::vec3(0, 0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 1, 0), color),
		VertexFormat(glm::vec3(0.5, 1, 0), color)
	};

	Mesh* multipleShot = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 2, 1,
		0, 3, 2,
		0, 4, 3,
		0, 1, 4,
		5,7,6,
		8,10,9,
		11,13,12,
		14,16,15
	};
	multipleShot->InitFromData(vertices, indices);
	return multipleShot;
}

/*Functia returneaza mesh-ul pentru desenarea simbolului
de invincibilitate.*/
Mesh* Object2D::CreateInvincibility(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(-1, 0, 0), color),
		VertexFormat(glm::vec3(-0.5, -0.5, 0), color),
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(0.5, -0.5, 0), color)
	};

	Mesh* inv = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 1, 7,
		7, 5, 6,
		5, 3, 4,
		3, 1, 2
	};
	inv->InitFromData(vertices, indices);
	return inv;
}

/*Functia returneaza simbolul pentru desenarea
simbolului de inghetare a inamicilor.*/
Mesh* Object2D::CreateSnowFlake(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.5, 0, 0), color),
		VertexFormat(glm::vec3(0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0, 0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 0, 0), color),
		VertexFormat(glm::vec3(-0.5, -0.5, 0), color),
		VertexFormat(glm::vec3(0, -0.5, 0), color),
		VertexFormat(glm::vec3(0.5, -0.5, 0), color),

		VertexFormat(glm::vec3(1.5, 0, 0), color),
		VertexFormat(glm::vec3(1.5, 1.5, 0), color),
		VertexFormat(glm::vec3(0, 1.5, 0), color),
		VertexFormat(glm::vec3(-1.5, 1.5, 0), color),
		VertexFormat(glm::vec3(-1.5, 0, 0), color),
		VertexFormat(glm::vec3(-1.5, -1.5, 0), color),
		VertexFormat(glm::vec3(0, -1.5, 0), color),
		VertexFormat(glm::vec3(1.5, -1.5, 0), color)
	};

	Mesh* snow = new Mesh(name);
	std::vector<unsigned short> indices = {
		7,8,1,
		0,9,2,
		1,10,3,
		2,11,4,
		3,12,5,
		4,13,6,
		5,14,7,
		6,15,0
	};
	snow->InitFromData(vertices, indices);
	return snow;
}

/*Functia returneaza simbolul pentru distrugerea
tuturor inamicilor prezenti in joc.*/
Mesh* Object2D::CreateBigBang(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0.5, 1.5, 0), color),
		VertexFormat(glm::vec3(-0.5, 1.5, 0), color),
		VertexFormat(glm::vec3(-1.5, 0.5, 0), color),
		VertexFormat(glm::vec3(-1.5, -0.5, 0), color),
		VertexFormat(glm::vec3(-0.5, -1.5, 0), color),
		VertexFormat(glm::vec3(0.5, -1.5, 0), color),
		VertexFormat(glm::vec3(1.5, -0.5, 0), color)
	};

	Mesh* bb = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 1
	};
	bb->InitFromData(vertices, indices);
	return bb;
}

/*Functia creeaza mesh-ul pentru desenarea scutului bonus.*/
Mesh* Object2D::CreateShield(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, -2, 0), color),
		VertexFormat(glm::vec3(1, 1, 0), color),
		VertexFormat(glm::vec3(-1, 1, 0), color)
	};

	Mesh* sh = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 3, 1,
		0, 1, 2
	};
	sh->InitFromData(vertices, indices);
	return sh;
}

/*Functia returneaza mesh-ul pentru desenarea
simbolului de incetinire.*/
Mesh* Object2D::CreateSlowDown(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.5, 0, 0), color),
		VertexFormat(glm::vec3(0, 0.5, 0), color),
		VertexFormat(glm::vec3(0, -0.5, 0), color),
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.5, 0.5, 0), color),
		VertexFormat(glm::vec3(0.5, -0.5, 0), color)
	};

	Mesh* sd = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 2, 1,
		3, 5, 4
	};
	sd->InitFromData(vertices, indices);
	return sd;
}