#pragma once
#include <Windows.h>
#include<strstream>
#include<math.h>
#include <GL/freeglut.h>
#include "camera.h"
#include "terrain.h"
#include "particle.h"
class MissilePoint3
{
public:
	float x, y, z;
	// 시점에서 생성됨
	void set(Point3& p) {
		x = p.x; y = p.y; z = p.z;
	}
	MissilePoint3(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}
	MissilePoint3() {
		x = y = z = 0;
	}
private:

};

class Missile {
public:
	MissilePoint3 position;
	Vector3 direction;
	bool active;
	float rotationMatrix[16];
	ParticleSystem explosionParticles;

	Missile(Point3& eye, const Vector3& dir);
	~Missile();
	void MissileDraw(float x, float y, float z, Camera missileV); //미사일 그리기
	void MoveMissile(double speed, Terrain* terrain);
	bool checkCollision(Terrain* terrain);
	ParticleSystem flameParticles;

};
