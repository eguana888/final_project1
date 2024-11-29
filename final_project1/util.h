#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>

typedef struct _AUX_RGBImageRec {
    GLint sizeX, sizeY;
    unsigned char* data;
} AUX_RGBImageRec;

AUX_RGBImageRec* LoadBMP(const char* filename);