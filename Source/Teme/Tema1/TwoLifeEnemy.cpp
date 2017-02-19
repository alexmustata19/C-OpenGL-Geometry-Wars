#pragma once
#include "TwoLifeEnemy.h"

float TwoLifeEnemy::sxOneLife = 0.75;
float TwoLifeEnemy::syOneLife = 0.75;
float TwoLifeEnemy::sxTwoLifes = 2;
float TwoLifeEnemy::syTwoLifes = 2;
float TwoLifeEnemy::radius = 1.75;
float TwoLifeEnemy::shrinkTime = 0.25;
glm::vec3 TwoLifeEnemy::colorOneLife = glm::vec3(0.3, 0, 0.6); //Mov
glm::vec3 TwoLifeEnemy::colorTwoLifes = glm::vec3(0, 0, 1); //Albastru
