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

	uint _texId_ground, _texId_water; //���� �ؽ�ó�� �� �ؽ�ó�� ID
	AUX_RGBImageRec *img; //�ؽ�ó �̹��� ��ü�� ����Ǵ� ������
	ubyte* _height; // ���� ���� �迭�� �����ϴ� ������
	GLint _map_width, _map_height; // ���� ������ ���� ���� ũ��
	GLuint baseTerrain; // Ÿ���� �ε��� ��
	GLubyte minHeight; // ���� �������� �ּ� ����
	static ubyte* readRawData(pcStr filename, uint width, uint height); // ���� ������ �о����

public:
	Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);

	void RenderTerrain(GLfloat, GLfloat); //���� �׸���

	~Terrain(void);//�Ҹ���

	void TileTerrain(GLint); //LoD�� ���� Ÿ�ϸ�

	void RenderWater(void);//�� �׸���
	GLfloat getHeight(GLfloat, GLfloat);//�ش� ��ġ�� ���� ���ϱ�
	GLint getTileX(GLfloat);//�ش� ��ġ�� x Ÿ�� ���ϱ�
	GLint getTileY(GLfloat);//�ش� ��ġ�� y Ÿ�� ���ϱ�
	GLfloat waterLevel; //���� ����
};
