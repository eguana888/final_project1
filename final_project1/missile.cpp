#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "missile.h"

Missile::Missile(Point3& eye, const Vector3& dir) :active(true) {
    position.set(eye);      // 미사일 시작 위치를 카메라의 eye 위치로 설정
    direction = dir;        // 미사일 방향을 카메라의 n 벡터로 설정
    direction.normalize();  // 방향 벡터를 정규화

    Vector3 up(0.0f, 1.0f, 0.0f); // 월드 업 벡터
    if (fabs(direction.dot(up)) > 0.99f) {
        up.set(1.0f, 0.0f, 0.0f); 
    }
    Vector3 xAxis = up.cross(direction);
    xAxis.normalize();
    Vector3 yAxis = direction.cross(xAxis);
    yAxis.normalize();
    Vector3 zAxis = direction; // Z축 (시점 방향)

    // 변환 행렬 생성
    rotationMatrix[0] = xAxis.x; rotationMatrix[4] = yAxis.x; rotationMatrix[8] = zAxis.x; rotationMatrix[12] = 0.0f;
    rotationMatrix[1] = xAxis.y; rotationMatrix[5] = yAxis.y; rotationMatrix[9] = zAxis.y; rotationMatrix[13] = 0.0f;
    rotationMatrix[2] = xAxis.z; rotationMatrix[6] = yAxis.z; rotationMatrix[10] = zAxis.z; rotationMatrix[14] = 0.0f;
    rotationMatrix[3] = 0.0f;    rotationMatrix[7] = 0.0f;    rotationMatrix[11] = 0.0f;    rotationMatrix[15] = 1.0f;

}

Missile::~Missile() {

}
void Missile::MissileDraw(float x, float y, float z, Camera missileV) {
    if (!active) return;

    glPushMatrix();

    // 미사일 위치로 이동
    glTranslatef(position.x, position.y, position.z);


    // 미사일 방향과 일치시키기 위해 변환 행렬 적용
    glMultMatrixf(rotationMatrix);



    // 미사일 몸체
    GLUquadric* quadric = gluNewQuadric();
    glColor3f(0.8f, 0.8f, 0.8f);  // 몸체 색상
    gluCylinder(quadric, 5.0, 5.0, 50.0, 30, 30);  // 미사일 몸체

    // 미사일 앞부분
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 50.0);  // 몸체 끝부분으로 이동
    glColor3f(1.0f, 1.0f, 1.0f);     // 앞부분 색상
    glutSolidCone(5.0, 15.0, 30, 30); // 앞부분 생성
    glPopMatrix();

    gluDeleteQuadric(quadric);  // 메모리 해제
    glPopMatrix();


}


void Missile::MoveMissile(double speed) {
    if (!active) return;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;
    if (position.z < -2000.0f || position.z > 2000.0f ||
        position.x < -2000.0f || position.x > 2000.0f ||
        position.y < -2000.0f || position.y > 2000.0f) {
        active = false;
    }
}

