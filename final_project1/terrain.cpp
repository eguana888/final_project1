#include "terrain.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "util.h"


Terrain::Terrain(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height)
{
	//맵의 가로 세로 크기 설정
	_map_width = width;
	_map_height = height;

    img = NULL; //물과 지형의 비트맵 이미지 포인터 초기화
	_texId_ground = 0; _texId_water = 0;
	_height = 0;
	//지형 텍스처 설정
    img = LoadBMP(surfaceTexFile); //지형 텍스처 이미지 로드
	glGenTextures(1, &_texId_ground);//텍스처 만들기
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //텍스처 ID와 연결
	//2차원 텍스처 이미지 설정
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//매핑될 물체가 작은 경우 텍스처 선형 보간 방법
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //매핑될 물체가 클 경우 텍스처의 확장 방법 결정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//물 텍스처 설정
    const char* waterTexFile = "space/water.bmp";
    img = LoadBMP(waterTexFile); //물 이미지 로드
    glGenTextures(1, &_texId_water); //텍스처 ID와 연결
    glBindTexture(GL_TEXTURE_2D, _texId_water);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    //파일로 부터 가로 세도만금의 높이 데이티를 읽어 옴.
    _height = readRawData(heightFile, width, height);

    int j;
    //지형의 최솟값을 구함.
    for (j = 0, minHeight = 2 ^ 10; j < _map_width * height; j++) {
        if (minHeight > _height[j]) { minHeight = _height[j]; }
    }
    for (int i = 0; i < _map_width* _map_height; ++i) { // 배열의 첫 10개 값 출력
        std::cout << "Height[" << i << "] = " << static_cast<int>(_height[i]) << std::endl;
    }

    //물 높이는 최소 지면의 60 위치
    waterLevel = (GLfloat)minHeight + 100;
    //최소 높이를 기준으로 지형을 잘라냄
    for (int i = 0; i < 10 + 1; i++) _height[i] -= minHeight;

    for (int i = 0; i < 10; ++i) { // 배열의 첫 10개 값 출력
        std::cout << "Height[" << i << "] = " << static_cast<int>(_height[i]) << std::endl;
    }
    baseTerrain = glGenLists(5); //4단계 지형 타일, 물 등 5개의 디스플레이 리스트

    //레벨 1 디테일 지형
    glNewList(baseTerrain, GL_COMPILE);
    TileTerrain(1);
    glEndList();
    glNewList(baseTerrain + 1, GL_COMPILE);

    TileTerrain(4);
    glEndList();
    glNewList(baseTerrain + 2, GL_COMPILE);

    TileTerrain(8);
    glEndList();
    glNewList(baseTerrain + 3, GL_COMPILE);

    TileTerrain(16);
    glEndList();
    glNewList(baseTerrain + 4, GL_COMPILE);

    RenderWater(); //물 그리기
    glEndList();

}

Terrain::~Terrain() {
    if (_height != 0) { delete[] _height; }
    if (_texId_ground != 0) { glDeleteTextures(1, &_texId_ground); }
    if (_texId_water != 0) { glDeleteTextures(1, &_texId_water); }
    if (img != 0) { free(img->data); free(img); }
}

void Terrain::RenderTerrain(GLfloat x, GLfloat y) {
    int i, j;
    //타일링한 지형 디스플레이 리스트를 불러오는 함수
    //레벨 1 디테일 지형.카메라가 위치한 곳에 한 블록만 그림
    glPushMatrix();
    glTranslatef(getTileX(x) * (_map_width - 1), 0.0, getTileY(y) * (_map_height - 1));
    glCallList(baseTerrain);
    glPopMatrix();


    //레벨 4 디테일 지형
    //레벨 1 블록을 제외하고 3×3 단위로 그림.
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            if (!(i == 0 && j == 0)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0,
                    getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 1);
                glPopMatrix();

            }
        }
    }
    //레벨 8 디테일 지형
    //레벨 4 블록을 제외하고 5×5 단위로 그림.
    for (i = -2; i < 3; i++) {
        for (j = -2; j < 3; j++) {
            if ((i < -1 || i>1) || (j < -1 || j>1)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 2);
                glPopMatrix();
            }
        }
    }
    //레벨 16 디테일 지형
    //레벨 8 블록을 제외하고 7×7 단위로 그림,
    for (i = -3; i < 4; i++) {
        for (j = -3; j < 4; j++) {
            if ((i < -2 || i>2) || (j < -2 || j>2)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0,
                    getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 3);
                glPopMatrix();
            }
        }
    }
    //물 디스플레이 리스트 3×3 면적에 그림.
    for (i = -2; i < 3; i++) {
        for (j = -2; j < 3; j++) {
            glPushMatrix();
            glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width)-1), 0.0,
                getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
            glCallList(baseTerrain + 4);
            glPopMatrix();

        }

    }

}

void Terrain::TileTerrain(GLint levelOfDetail) { //LoD 파라미터를 받아 각 타일을 그리는 함수
    glEnable(GL_TEXTURE_2D);//텍스처 활성화
    glMatrixMode(GL_MODELVIEW);//모델 뷰 행렬
    glPushMatrix();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// 텍스처 환경 설정
    glBindTexture(GL_TEXTURE_2D, _texId_ground);//지형 텍스처 ID가 가리키는 텍스처로 아래 정점 연결
    float coordX = (float)1.0 / _map_width; //가로 이동 단위
    float coordZ = (float)1.0 / _map_height; //세로 이동 단위
    for (int z = 0; z < _map_height; z += levelOfDetail) {//x, z축을 기준으로 바닥에 매핑하려고
        glBegin(GL_TRIANGLE_STRIP); //연속 삼각형 그리기로 텍스처 매핑
        for (int x = 0; x < _map_width; x += levelOfDetail) {
            glTexCoord2f(coordX * x, coordZ * z); //텍스처 정점과
            glVertex3f(x, _height[z * _map_height + x], z); //물체 정점 연결
            glTexCoord2f(coordX * x, coordZ * (z + levelOfDetail)); //텍스처 정점과
            glVertex3f(x, _height[(z + levelOfDetail) * _map_height + x], z + levelOfDetail); //물체 정점 연결

        }
        glEnd();
    }


    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Terrain::RenderWater() { //물 그리기
    glEnable(GL_TEXTURE_2D); //텍스처 활성화
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //텍스처 환경 설정
    glEnable(GL_BLEND);//블렌딩 활성화
    glDisable(GL_CULL_FACE);//후면 제거 비활성화

    
        
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //블렌딩 방식 지정
    glColor4f(0.0, 0.4, 0.5, 0.7);

    glBindTexture(GL_TEXTURE_2D, _texId_water); //텍스처 매핑
    glBegin(GL_QUADS); //사각형 단위로 그리기
        glTexCoord2f(0.0, 0.0); glVertex3f(0.0, waterLevel, 0.0);//같은 물높이 유지하고 정점 연결
        glTexCoord2f(15.0, 0.0); glVertex3f(0.0, waterLevel, _map_height);
        glTexCoord2f(15.0, 15.0); glVertex3f(_map_height, waterLevel, _map_height);
        glTexCoord2f(0.0, 15.0); glVertex3f(_map_height, waterLevel, 0.0);
    glEnd();
    glEnable(GL_CULL_FACE);//후면 제거 활성화
    glDisable(GL_BLEND);//블렌드 비활성화
    glDisable(GL_TEXTURE_2D);

}


//.raw 파일을 읽는 함수.filename: 높이 맵 파일의 경로.w : 높이 맵의 가로 크기.h : 높이 맵의 세로 크기
Terrain::ubyte* Terrain::readRawData(pcStr filename, uint w, uint h) {
    FILE* f;
    fopen_s(&f, filename, "rb"); //지형 높이 데이터 raw 파일 열기
    //파일이 없을 경우    
    if (!f) {
        printf("file not found :%s\n", filename);
        throw std::exception();
    }

    //raw 파일은 헤더가 없어서 파일의 크기 = 이미지의 픽셀 수이기 때문에 width * height 만큼의 공간을 할당하면 모든 데이터를 읽어올 수 있음.
    ubyte * raw = new ubyte[w * h]; //raw 크기를 잡고
    fread(raw, sizeof(ubyte), w * h, f); //높이 데이터 읽어오기
    fclose(f);
    return raw;
    
}


GLfloat Terrain::getHeight(GLfloat _x, GLfloat _y) {
    //텍스처 이미지의 해당_x, _y에서 높이 데이터를 가져오는 함수
    int x, y;
    if (_x < 0) x = _map_width + ((int)_x % _map_width); //텍스처 이미지 범위를 넘어가면 동일 텍스처를 반복
    else x = ((int)_x % _map_width);
    if (_y < 0)y = _map_height + ((int)_y % _map_height);
    else y = ((int)_y % _map_height);
    //좌표 값을 텍스처 크기 안에 한정시킴.높이 데이터는 1차원 배열에 저장되어 있으므로 행렬 처리
    return (_height[y * _map_height + x] / 5 + 10); //해당 위치의 높이 값 리턴
}

//현제 카메라가 위치한 텍스처 타일의 블록 좌표를 리턴 : ×좌표
GLint Terrain::getTileX(GLfloat x) {

    if (x > 0) return ((int)x) / (int)_map_width;
    else return ((int)x) / (int)_map_width - 1;
}
//현재 카메라가 위치한 텍스치 타일의 블록 좌표를 리턴 : y좌표
GLint Terrain :: getTileY(GLfloat y) {
       
    if (y > 0)return ((int)y) / (int)_map_height;
    else return ((int)y) / (int)_map_height - 1;
    
}