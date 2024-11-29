#pragma once
#include <Windows.h>
#include<strstream>
#include<math.h>
#include <GL/freeglut.h>
#include "camera.h"

class MissilePoint3
{
public:
	float x, y, z;
	// 시점에서 나감
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
	Missile();
	~Missile();
	void MissileDraw(float x, float y, float z); //미사일 그리기


};
