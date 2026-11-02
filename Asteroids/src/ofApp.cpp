#include "ofApp.h"
#include <cstdlib>
#include <ctime>

//--------------------------------------------------------------
void ofApp::setup(){
	srand(time(0));
	background.load("images/pexels-francesco-ungaro-998641.jpg");
	score = 0;
	player = Player();
	player.position = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	playerShooter.visible = false;
	playerShooter.start();
	asteroidSpawner.start();

	engineSound.load("sounds/rocketsound.mp3");
	engineSound.setLoop(true);
	engineSound.setVolume(0.5);
	asteroidBreakSound.load("sounds/rock-break.mp3");
	playerExplosionSound.load("sounds/loud-explosion.mp3");
	playerExplosionSound.setVolume(0.4);

	bulletTurbulence = new TurbulenceForce(glm::vec3(-1000.0, -1000.0, 0), glm::vec3(1000.0, 1000.0, 0));
	playerShooter.sys->addForce(bulletTurbulence);

	radialForce = new ImpulseRadialForce(10000.0);
	explosion.sys->addForce(radialForce);

	explosion.setVelocity(ofVec3f(0, 0, 0));
	explosion.oneShot = true;
	explosion.type = RadialEmitter;
	explosion.groupSize = 100;
	explosion.randomLife = true;
	explosion.visible = false;
	explosion.lifeMinMax = ofVec2f(0.5, 1.5);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSeedRandom();

	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(player.rotation), glm::vec3(0, 0, -1));
	if (player.thrustForce != 0) {
		glm::vec4 thrust = R * glm::vec4(player.thrustForce, 0.0, 0.0, 0.0);
		player.force += thrust;
	}
	if (player.turnTorque != 0) player.torque += player.turnTorque;

	player.update();

	//update shooters
	playerShooter.position = player.position + glm::vec3(R * glm::vec4(player.defaultSize / 2, 0, 0, 0));
	playerShooter.rotation = player.rotation;
	playerShooter.update();

	//collision check asteroid -> bullet
	vector<Asteroid>::iterator a = asteroidSpawner.sys->asteroids.begin();
	vector<Asteroid>::iterator tmp1;
	vector<Particle>::iterator tmp2;

	while (a != asteroidSpawner.sys->asteroids.end()) {
		bool collision = false;
		vector<Particle>::iterator b = playerShooter.sys->particles.begin();
		while (b != playerShooter.sys->particles.end()) {
			if (a->inside(b->position)) {
				if (!a->hasSplit) {
					int index = distance(asteroidSpawner.sys->asteroids.begin(), a);
					asteroidSpawner.sys->split(*a, index);
					a = asteroidSpawner.sys->asteroids.begin() + index;
				} else {
					tmp1 = asteroidSpawner.sys->asteroids.erase(a);
					a = tmp1;
				}
				tmp2 = playerShooter.sys->particles.erase(b);
				b = tmp2;
				collision = true;
				asteroidBreakSound.play();
				score += 10;
				break;
			} else {
				b++;
			}
		}
		if (!collision) a++;
	}

	//collision check player -> asteroid
	//check if any of the vertices of the asteroid is in player ship using advanced for loop
	for (Asteroid ast : asteroidSpawner.sys->asteroids) {
		for (glm::vec3 p : ast.outline.getVertices()) {
			p = ast.getTransform() * glm::vec4(p, 1.0);
			if (player.inside(p) && player.alive) {
				player.die();
				playerExplosionSound.play();
				playerShooter.spawning = false;
				explosion.start();
				break;
			}
		}
	}

	//update explosion
	explosion.position = player.position;
	explosion.update();

	asteroidSpawner.update();

	//update thrust sound
	if (player.thrustForce != 0 && !engineSound.isPlaying()) engineSound.play();
	else if (player.thrustForce == 0 && engineSound.isPlaying()) engineSound.stop();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	if(player.alive) player.draw();
	asteroidSpawner.draw();
	explosion.draw();
	playerShooter.draw();
	

	//draw text
	ofSetColor(ofColor::white);
	ofDrawBitmapString("Controls: WASD to move | Space to shoot | R to reset player and score", 10, 15);
	ofDrawBitmapString("Score: " + ofToString(score), 10, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'W':
	case 'w':
		if(player.alive)player.thrustForce = player.thrustingPower;
		break;
	case 'S':
	case 's':
		if (player.alive) player.thrustForce = -player.thrustingPower;
		break;
	case 'A':
	case 'a':
		if (player.alive) player.turnTorque = player.turningPower;
		break;
	case 'D':
	case 'd':
		if (player.alive) player.turnTorque = -player.turningPower;
		break;
	case 'R':
	case 'r':
		player = Player();
		player.position = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		explosion.fired = false;
		explosion.sys->reset();
		score = 0;
		break;
	case OF_KEY_SPACE:
		if (player.alive) playerShooter.spawning = true;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case 'W':
	case 'w':
	case 'S':
	case 's':
		player.thrustForce = 0;
		break;
	case 'A':
	case 'a':
	case 'D':
	case 'd':
		player.turnTorque = 0;
		break;
	case OF_KEY_SPACE:
		playerShooter.spawning = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (player.inside(glm::vec3(x, y, 0))) cout << "Inside player ship" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
