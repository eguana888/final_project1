#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "missile.h"

Missile::Missile(Point3& eye, const Vector3& dir) :active(true) {
    position.set(eye);      // �̻��� ���� ��ġ�� ī�޶��� eye ��ġ�� ����
    direction = dir;        // �̻��� ������ ī�޶��� n ���ͷ� ����
    direction.normalize();  // ���� ���͸� ����ȭ

    Vector3 up(0.0f, 1.0f, 0.0f); // ���� �� ����
    if (fabs(direction.dot(up)) > 0.99f) {
        up.set(1.0f, 0.0f, 0.0f); 
    }
    Vector3 xAxis = up.cross(direction);
    xAxis.normalize();
    Vector3 yAxis = direction.cross(xAxis);
    yAxis.normalize();
    Vector3 zAxis = direction; // Z�� (���� ����)

    // ��ȯ ��� ����
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

    // �̻��� ��ġ�� �̵�
    glTranslatef(position.x, position.y, position.z);


    // �̻��� ����� ��ġ��Ű�� ���� ��ȯ ��� ����
    glMultMatrixf(rotationMatrix);



    // �̻��� ��ü
    GLUquadric* quadric = gluNewQuadric();
    glColor3f(0.8f, 0.8f, 0.8f);  // ��ü ����
    gluCylinder(quadric, 5.0, 5.0, 50.0, 30, 30);  // �̻��� ��ü

    // �̻��� �պκ�
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 50.0);  // ��ü ���κ����� �̵�
    glColor3f(1.0f, 1.0f, 1.0f);     // �պκ� ����
    glutSolidCone(5.0, 15.0, 30, 30); // �պκ� ����
    glPopMatrix();

    gluDeleteQuadric(quadric);  // �޸� ����
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
