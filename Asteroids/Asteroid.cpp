#include "Asteroid.h"
#include <cstdlib>
#include <ctime>

Asteroid::Asteroid() {
	radius = ((rand() % 3) * 5) + 50; //can be 50, 55, 60
	generateOutline();
	int spawnRNG = rand() % 2; // if 0, spawn on the horizontal margin. if 1, spawn on the vertical margin
	int outerMargin = 50; //pixel margin outside of window for wrapping
	if (spawnRNG)
		position = glm::vec3(0 - outerMargin, ofRandom(0 - outerMargin, ofGetWindowHeight()) + outerMargin, 0);
	else
		position = glm::vec3(ofRandom(0 - outerMargin, ofGetWindowWidth()) + outerMargin, 0 - outerMargin, 0);
	velocity = glm::vec3((rand() % 50 + 30) * (1 - 2 * (rand() % 2)), (rand() % 50 + 30) * (1 - 2 * (rand() % 2)), 0);
	torque = ofRandom(-6000, 6000);
	hasSplit = false;
	lifespan = 20;
}

Asteroid::Asteroid(bool split) {
	radius = ((rand() % 3) * 5) + 50; //can be 50, 55, 60
	if (split) {
		radius /= 2.0;
	} else {
		int spawnRNG = rand() % 2; // if 0, spawn on the horizontal margin. if 1, spawn on the vertical margin
		int outerMargin = 50; //pixel margin outside of window for wrapping
		if (spawnRNG)
			position = glm::vec3(0 - outerMargin, ofRandom(0 - outerMargin, ofGetWindowHeight()) + outerMargin, 0);
		else
			position = glm::vec3(ofRandom(0 - outerMargin, ofGetWindowWidth()) + outerMargin, 0 - outerMargin, 0);
	}
	generateOutline();

	position = glm::vec3(ofRandom(0, ofGetWindowWidth()), ofRandom(0, ofGetWindowHeight()), 0);
	velocity = glm::vec3((rand() % 50 + 30) * (1 - 2 * (rand() % 2)), (rand() % 50 + 30) * (1 - 2 * (rand() % 2)), 0);
	torque = ofRandom(-6000, 6000);
	hasSplit = split;
	lifespan = 20;
}

void Asteroid::draw() {
	ofPushMatrix();
	ofMultMatrix(getTransform());
	ofSetColor(ofColor::white);
	outline.draw();
	ofPopMatrix();
}

bool Asteroid::inside(glm::vec3 p) { //assumes circle collision
	float distance = glm::distance(p, position);
	return distance <= radius;
}

void Asteroid::generateOutline() {
	float angleDelta = 360.0 / (radius / 5);

	for (int i = 0; i < radius / 5; i++) {
		glm::vec3 newPoint = glm::vec4((rand() % ((int)radius / 2)) + (radius * 3 / 4), 0, 0, 0) * glm::rotate(glm::mat4(1.0), glm::radians(angleDelta * i), glm::vec3(0, 0, -1));
		outline.addVertex(newPoint);
	}
	outline.close();
}

//-----------------------------------------
void AsteroidSystem::add(const Asteroid & p) {
	asteroids.push_back(p);
}

void AsteroidSystem::remove(int i) {
	asteroids.erase(asteroids.begin() + i);
}

void AsteroidSystem::update() {
	// check if empty and just return
	if (asteroids.size() == 0) return;

	vector<Asteroid>::iterator p = asteroids.begin();
	vector<Asteroid>::iterator tmp;

	// check which particles have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, we need to use an iterator.
	//
	while (p != asteroids.end()) {
		if (p->lifespan != -1 && p->getAge() > p->lifespan) {
			tmp = asteroids.erase(p);
			p = tmp;
		} else
			p++;
	}

	// update forces on all particles first
	//
	for (int i = 0; i < asteroids.size(); i++) {
		for (int k = 0; k < forces.size(); k++) {
			forces[k]->updateForce(&asteroids[i]);
		}
	}

	// integrate all the particles in the store
	//
	for (int i = 0; i < asteroids.size(); i++)
		asteroids[i].update();
}

//  draw the particle cloud
//
void AsteroidSystem::draw() {
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i].draw();
	}
}

void AsteroidSystem::split(Asteroid ast, int index) {
	for (int i = 0; i < 4; i++) {
		Asteroid a = Asteroid(true);
		a.birthtime = ofGetElapsedTimeMillis();
		a.position = ast.position;
		add(a);
	}
	remove(index);
}

//----------------------------------------

AsteroidEmitter::AsteroidEmitter() {
	sys = new AsteroidSystem();
	createdSys = true;
	init();
}

//AsteroidEmitter::~AsteroidEmitter() {
//	if (createdSys) delete sys;
//}

void AsteroidEmitter::init() {
	velocity = glm::vec3(1000, 0, 0);
	started = false;
	spawning = true;
	lastSpawned = 0;
	radius = 1; //arbitrary
	particleRadius = 1; //also arbitrary
	visible = true;
	type = DirectionalEmitter;
	rate = 0.1;
	lifespan = 20;

	for (int i = 0; i < 4; i++) {
		Asteroid asteroid;
		asteroid.lifespan = lifespan;
		asteroid.birthtime = ofGetElapsedTimeMillis();
		sys->add(asteroid);
	}
}

void AsteroidEmitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate) && spawning) { //spawns 4 asteroids at the same time
		for (int i = 0; i < 4; i++) {
			Asteroid asteroid;
			asteroid.lifespan = lifespan;
			asteroid.birthtime = time;
			sys->add(asteroid);
		}
		lastSpawned = time;
	}
	sys->update();
}

void AsteroidEmitter::draw() {
	sys->draw();
}
