#include "camera.h"
#include <iostream>
Camera::Camera(void) {
	setDefaultCamera(); // 생성자 초기화 함수를 별도로 호출
}

void Camera::setDefaultCamera(void) {
	setShape(45.0f, 640 / (float)480, 0.1f, 200.0f);// 화면 구성 값 정의
	Point3 eyePoint = Point3(10.0, 0.0, 0.0); //카메라 위치 지정
	Point3 lookPoint = Point3(0.0, 0.0, 0.0); //바라보는 시점 지정

	Vector3 upVector = Vector3(); //카메라 업 벡터 지정
	set(eyePoint, lookPoint, upVector);
}

void Camera::set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ) {
	Point3 Eye = Point3(eyeX, eyeY, eyeZ); //카메라, 시점, 업 벡터 정의
	Point3 look = Point3(lookX, lookY, lookZ);
	Vector3 up = Vector3(upX, upY, upZ);
	eye.set(Eye);// 카메라 위치 정의
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();
}

void Camera::set(Point3 Eye, Point3 look, Vector3 up) {
	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();
}

void Camera::setModelViewMatrix(void) {
	float m[16]; //모델 뷰 행렬에 넣을 행렬 값. 4×4=16개의 원소를 가짐.카메라 위치
	Vector3 eVec(eye.x, eye.y, eye.z);

	//회전 변환과 이동 변환 결과로 뷰 행렬 작성
	m[0] = u.x; m[4] = u.y; m[8] = u.z; m[12] = -eVec.dot(u);
	m[1] = v.x; m[5] = v.y; m[9] = v.z; m[13] = -eVec.dot(v);
	m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = -eVec.dot(n);
	m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m); //·행렬 m을 모델 뷰 행렬에 넣어줌.

}

//투상정의
void Camera::setShape(float vAngle, float asp, float nr, float fr) {
	viewAngle = vAngle; //시야각
	aspect = asp; //종횡비
	nearDist = nr; //전방 투상면
	farDist = fr; //후방 투상면
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //투상 행렬을 새로 정의하기 위해 항등 행렬 로드
	gluPerspective(viewAngle, aspect, nearDist, farDist); //원근 투상 정의
	glMatrixMode(GL_MODELVIEW);
}


//종횡비만 새로 정의
void Camera::setAspect(float asp) {
	aspect = asp; 
}


//원근 투상의 구성값 리턴
void Camera::getShape(float& vAngle, float& asp, float& nr, float& fr) {
	vAngle = viewAngle; 
	asp = aspect;
	nr = nearDist;
	fr = farDist;
}

//이동변환
void Camera :: slide(double du, double dv, double dn) {
	eye.x += du * u.x + dv * v.x + dn * n.x; //시점 좌표축의 정규화 기반 벡터에 이동 변환 행렬을 곱함.
	eye.y += du * u.y + dv * v.y + dn * n.y; //시점 좌표계가 이동한 효과
	eye.z += du * u.z + dv * v.z + dn * n.z;
	setModelViewMatrix();
}


//시점 좌표계를 이루는 기반 벡터를 기울여 회전 변환
void Camera::rotAxes(Vector3& a, Vector3& b, float angle) {
	float ang = 3.14159265 / 180 * angle; // 각을 라디안 단위로 변환
	float C = cos(ang), S = sin(ang); //변환하려는 각의 cos, sin 값을 구함.


	//이를 이용해 두 축을 angle만큼 회전
	Vector3 t(C * a.x + S * b.x, C * a.y + S * b.y, C * a.z + S * b.z);
	b.set(-S * a.x + C * b.x, -S * a.y + C * b.y, -S * a.z + C * b.z);
	a.set(t.x, t.y, t.z);

}


//n축을 기준으로 회전. 나머지 u, v축이 각도만큼 회전 모델 뷰 행렬에 적용
void Camera::roll(float angle) {
	rotAxes(u, v, angle);
	setModelViewMatrix();
}
//u축을 기준으로 회전, 나머지 n, v축이 각도만큼 회전 모델 뷰 행렬에 적용
void Camera::pitch(float angle) {
	rotAxes(n, v, angle);
	setModelViewMatrix();
}
//v축을 기준으로 회전, 나머지 n, v축이 각도만큼 회전 모델 뷰 행렬에 적용
void Camera::yaw(float angle) {
	rotAxes(u, n, angle);
	setModelViewMatrix();
}
//const 부침
Vector3 Vector3::cross(const Vector3& b) const {
	Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	return c;
}

float Vector3::dot(Vector3 b) { return x * b.x + y * b.y + z * b.z; }

void Vector3::normalize() {
	double sizeSq = x * x + y * y + z * z;
	if (sizeSq < 0.0000001) {
		std::cerr << "\nnormalize() sees vector (0, 0, 0)!";
		return;
	}
	float scaleFactor = 1.0 / (float)sqrt(sizeSq); //벡터를 그 벡터 크기로 나눠줌
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}


