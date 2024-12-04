#pragma once
#include <Windows.h>
#include<strstream>
#include<math.h>
#include <GL/freeglut.h>

class Point3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) {
		x = dx; y = dy; z = dz;
	};
	void set(Point3& p) {
		x = p.x; y = p.y; z = p.z;
	}
	Point3(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}
	Point3() {
		x = y = z = 0;
	}
};


class Vector3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) {
		x = dx; y = dy; z = dz;
	};
	//const 붙임
	void set(const Vector3& v){
		x = v.x; y = v.y; z = v.z;
	}
	void flip() {
		x = -x; y + -y; z = -z;
	}
	void setDiff(Point3& a, Point3& b) {
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
	}
	void normalize();

	Vector3(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}
	// 복사 생성자 오류(직접 생성하면 생기는 문제. 앞에 const를 붙여 해결)
	Vector3(const Vector3& v) {
		x = v.x; y = v.y; z = v.z;
	}
	Vector3() {
		x = y = z = 0.0;
	}
	Vector3 cross(const Vector3& b) const;
	float dot(Vector3 b);

	float magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

};

class Camera {
public:
	Point3 eye; // 시점 좌표계 원점
	Vector3 u, v, n; // 시점 좌표계를 구성하는 정규 직교 기저
	float aspect, nearDist, farDist, viewAngle; //gluPerspextive의 파라미터들 시점 변환 후 이용
	
	void setModelViewMatrix(); //시점 변환에 따라 모델 뷰 행렬 조정
	Camera(void);
	
	//비행 시뮬레이터의 회전 변환 함수들 
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);

	void slide(double du, double dv, double dn); // 이동 변환

	void set(Point3 Eye, Point3 look, Vector3 up);// 벡터로
	void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ); // 점으로
	void setShape(float viewAngle, float aspext, float Near, float Far); //화면 정의
	void setAspect(float asp); // 종행비 정의
	void getShape(float& viewAngle, float& aspext, float& Near, float& Far); //화면 구성값 보기
	void rotAxes(Vector3& a, Vector3& b, float angle); //시점 좌표계 축 회전
	void setDefaultCamera();

};
