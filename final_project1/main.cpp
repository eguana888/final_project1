#include <stdio.h>
#include <math.h>
#include "terrain.h"
#include "Skybox.h"
#include <GL/freeglut.h>
#include "util.h"
#include <vector>
#include "missile.h"

//미사일들
std::vector<Missile> missiles;

//객체 선언
Terrain* terrain; //지형
Skybox* skybox; //스카이 박스
Camera cam; //카메라
Missile *missile;

void fireMissile();

bool keyPressed[256]; //키보드 입력 상황을 나타내는 배열
void myKeyboard(unsigned char key, int x, int y) { 
	keyPressed[key] = true; 
	if(keyPressed[' ']) { fireMissile(); }

}
//배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//배열이 false면 Key Up

void dispose();//종료 시 수행되는 함수
void Display();//디스플레이 콜백
void Idle();//아이들 콜백
void Reshape(GLint w, GLint h);//리셰이프 콜백
void fog();

void Display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
		skybox->draw();
	glPopMatrix();
	glPushMatrix();
		glScalef(1.0f, 0.2f, 1.0f);
		terrain->RenderTerrain(cam.eye.x, cam.eye.z);
	glPopMatrix();

	for (auto& missile : missiles) {
		missile.MissileDraw(cam.eye.x, cam.eye.y, cam.eye.z, cam);  // 각 미사일의 draw 메서드 호출

	}

	fog();//수면 아래 안개 효과
	glFlush();
	glutSwapBuffers(); //더블 버퍼링

		
}




void Idle() {//해당 키가 눌려 있는지 지속적으로 검사해 다중 입력을 가능하게 함.
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y < terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }
	//간단한 충돌 탐지.지표면 아래로 카메라가 내려갈 때는 지표면 위로 유지시킴.
	if (keyPressed['k']) { cam.pitch(-0.5); }
	if (keyPressed['i']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //아무것도 누르지 않을 때는 이동 없는 것으로 간주.모델 뷰 행렬을 유지 다시 그림



	for (auto& missile : missiles) {
		missile.MoveMissile(0.5f);  // 이동 속도
	}

	// 비활성화된 미사일 제거
	missiles.erase(std::remove_if(missiles.begin(), missiles.end(),
		[](const Missile& m) { return !m.active; }),
		missiles.end());


	glutPostRedisplay(); // 다시그림
}

//리셰이프 콜백
void Reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);//뷰 포트 설정
	glMatrixMode(GL_PROJECTION);//원근 투상을 위한 투상 행렬
	glLoadIdentity();//항등 행렬
	gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0); //원근 투상 설정
}


//프로그램 종료 시 객체 반환
void dispose() {
	delete terrain;
	delete skybox;
	//원래 책에는 있었지만 스택 메모리에 할당된 객체이기 때문에 delete로 
	//delete& cam;

}

//미사일 발사
void fireMissile() {
	printf("클릭함\n");
	printf("Camera Position: (%f, %f, %f)\n", cam.eye.x, cam.eye.y, cam.eye.z);
	printf("Camera Direction: (%f, %f, %f)\n", cam.n.x, cam.n.y, cam.n.z);
	Vector3 dir(-cam.n.x, -cam.n.y, -cam.n.z); // 카메라 방향의
	dir.normalize();
	Point3 camp;
	camp.set(cam.eye.x, cam.eye.y, cam.eye.z);
	missiles.emplace_back(camp, dir); // 미사일 생성
}

void fog() {
	//GL_FOG 활성화
	glEnable(GL_FOG);

	//안개의 농도 차이를 결정하는 파라미터(GL_LINEAR, GL_EXP, GL_EXP2)
	glFogi(GL_FOG_MODE, GL_EXP2);
	//시점의 높이(y)가 물 표면 높이보다 낮으면, 즉 물 안에 있다면
	GLfloat waterFogColor[4] = { 0.0, 0.6, 0.6, 5.0 };
	GLfloat fogColor[4] = { 0.75, 0.75, 0.75, 0.0 };
	if (cam.eye.y < (terrain->waterLevel - 75)) {
		//안개 색상을 waterFogColor(0.0, 0.6, 0.6, 1.0)으로 변경하고
		glFogfv(GL_FOG_COLOR, waterFogColor);
		//안개 거리를 waterFogDensity(0.075)로 변경해 물 안이라는 느낌을 주도록
		glFogf(GL_FOG_DENSITY, 0.075);
	}
	else {
		//아니면 안개 색상을 fogColor(0.7, 0.7, 0.7, 1.0)으로 변경하고
		glFogfv(GL_FOG_COLOR, fogColor);
		//안개 거리를 fogDensity(0.002)로 변경해 물 밖이라는 느낌을 주도록
		glFogf(GL_FOG_DENSITY, 0.001);

	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480); //윈도우 설정
	glutInitWindowPosition(400, 30);
	glutCreateWindow("CGproject_final"); 
	glutReshapeFunc(Reshape); //콜백 함수 등록
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	glEnable(GL_DEPTH_TEST); //깊이 버퍼, 후면 제거 활성화
	glEnable(GL_CULL_FACE); //지표면, 스카이 박스 객체 생성

	terrain = new Terrain("space/terrain1.raw",
		"space/snow512.bmp", 257, 257);

	skybox = new Skybox();

	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);//카메라의 초기 위치와 회전각
	//설정을 마친 카메라가 처음에 어디에서 어디를 보고 있을지 결정

	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);
	atexit(dispose);
	printf("JL: roll\n");
	printf("W: foward S: backwad A: left D: right\n");
	printf("IK: pitch\n");
	printf("QE: yaw\n\n");
	printf("1,2: solid, wire rendering\n");
	glutMainLoop();
	return 0;
}

