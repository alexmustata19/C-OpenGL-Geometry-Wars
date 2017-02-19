#include "Ship.h"

float Ship::sx = 1.5;
float Ship::sy = 1.5;
glm::vec3 Ship::color = glm::vec3(1, 0.5, 0); //Portocaliu
glm::vec3 Ship::invincibleColor = glm::vec3(0.5, 0.5, 0.5); //Gri
float Ship::radius = 1.75;
float Ship::speed = 1;

Ship::Ship() {}

Ship::~Ship() {}
