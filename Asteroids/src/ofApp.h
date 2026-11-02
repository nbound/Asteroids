#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../Shape.h"
#include "../Player.h"
#include "../Particle.h"
#include "../Asteroid.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		Player player;
		bool playerShooting = false;
		ParticleEmitter playerShooter = ParticleEmitter(BulletType); //initialized in .h due to a bug that occurred when trying to do it in setup()
		AsteroidEmitter asteroidSpawner = AsteroidEmitter();
		ParticleEmitter explosion = ParticleEmitter();

		TurbulenceForce * bulletTurbulence;
		ImpulseRadialForce * radialForce;

		int score;



		ofSoundPlayer engineSound;
		ofSoundPlayer asteroidBreakSound;
		ofSoundPlayer playerExplosionSound;

		ofImage background;
};
