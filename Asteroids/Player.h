#pragma once
#include "Particle.h"
class Player : public Particle {
public:
	Player();
	void draw();
	bool inside(glm::vec3 p);
	void die();

	ofPolyline outline; //outline of player shape

	float thrustForce;
	float thrustingPower;
	float turnTorque;
	float turningPower;
	bool alive;

	ofSoundPlayer engineSound;
};
