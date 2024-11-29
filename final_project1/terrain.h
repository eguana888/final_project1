#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <math.h>
#include "camera.h"
#include "util.h"



class Terrain {
private:
	typedef const char* pcStr;
	typedef unsigned int uint;
	typedef unsigned char ubyte;

	uint _texId_ground, _texId_water; //지형 텍스처와 물 텍스처의 ID
	AUX_RGBImageRec *img; //텍스처 이미지 객체와 연결되는 포인터
	ubyte* _height; // 지형 높이 배열과 연결하는 포인터
	GLint _map_width, _map_height; // 단위 지형의 가로 세로 크기
	GLuint baseTerrain; // 타일의 인덱스 값
	GLubyte minHeight; // 지형 데이터의 최소 높이
	static ubyte* readRawData(pcStr filename, uint width, uint height); // 지형 테이터 읽어오기

public:
	Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);

	void RenderTerrain(GLfloat, GLfloat); //지형 그리기

	~Terrain(void);//소멸자

	void TileTerrain(GLint); //LoD를 위한 타일링

	void RenderWater(void);//물 그리기
	GLfloat getHeight(GLfloat, GLfloat);//해당 위치의 높이 구하기
	GLint getTileX(GLfloat);//해당 위치의 x 타일 구하기
	GLint getTileY(GLfloat);//해당 위치의 y 타일 구하기
	GLfloat waterLevel; //수면 높이
};
