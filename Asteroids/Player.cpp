#include "Player.h"
//-------Player subclass
Player::Player() {
	Particle::Particle();
	lifespan = -1; //immortal
	defaultSize = 50.0;
	thrustForce = 0.0;
	thrustingPower = 1000.0;
	turnTorque = 0;
	turningPower = 1200.0;
	damping = 0.99;
	angDamping = 0.96;
	alive = true;
	//define player outline facing right by default
	outline.addVertex(glm::vec3(-defaultSize / 2, -defaultSize / 2, 0));
	outline.addVertex(glm::vec3(-defaultSize / 4, 0, 0));
	outline.addVertex(glm::vec3(-defaultSize / 2, defaultSize / 2, 0));
	outline.addVertex(glm::vec3(defaultSize / 2, 0, 0));
	outline.close();
}

void Player::draw() {
	ofPushMatrix(); //store current transformation matrix
	ofMultMatrix(getTransform()); //apply this shape's transformation matrix
	//draw triangle
	ofSetColor(ofColor::white); //set color to white
	outline.draw();
	ofPopMatrix();
}

bool Player::inside(glm::vec3 p) {	//uses only three points for the sake of keeping bounding triangle convex
	glm::vec3 pObj = getPointInObjectSpace(p);

	glm::vec3 A = glm::vec3(-defaultSize / 2, -defaultSize / 2, 0);
	glm::vec3 B = glm::vec3(-defaultSize / 2, defaultSize / 2, 0);
	glm::vec3 C = glm::vec3(defaultSize / 2, 0, 0);

	double c1 = glm::cross(B - A, pObj - A).z;
	double c2 = glm::cross(C - B, pObj - B).z;
	double c3 = glm::cross(A - C, pObj - C).z;

	return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
}

void Player::die() {
	alive = false;
	thrustForce = 0;
	turnTorque = 0;
}
