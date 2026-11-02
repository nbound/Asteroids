#pragma once
#include "Particle.h"
class Asteroid : public Particle {
public:
	ofPolyline outline; //asteroid outline

	Asteroid();
	Asteroid(bool split);
	void draw();
	bool inside(glm::vec3 p);
	void generateOutline();
	bool hasSplit;
};

class AsteroidSystem : public ParticleSystem { //mimics ParticleSystem functionality with support for Asteroid
public:
	void add(const Asteroid &);
	void remove(int);
	void update();
	void draw();
	void split(Asteroid a, int index); //special function for splitting asteroids into 4
	vector<Asteroid> asteroids;
};

class AsteroidEmitter : public ParticleEmitter { //mimics ParticleEmitter functionality with support for Asteroid
public:
	AsteroidSystem * sys;

	AsteroidEmitter();
	//~AsteroidEmitter();
	void init();
	void update();
	void draw();
};
