#pragma once
#ifndef BMP_H
#define BMP_H

#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include<string>

extern const double EPS;

//typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

//struct BITMAPFILEHEADER {
//  word bfType;
//	dword bfSize;
//	word bfReserved1;
//	word bfReserved2;
//	dword bfOffBits;
//};
//
//struct BITMAPINFOHEADER {
//	dword biSize;
//	long biWidth;
//	long biHeight;
//	word biPlanes;
//	word biBitCount;
//	dword biCompression;
//	dword biSizeImage;
//	long biXPelsPerMeter;
//	long biYPelsPerMeter;
//	dword biClrUsed;
//	dword biClrImportant;
//};
//
//struct RGBQUAD {
//	byte rgbBlue;
//	byte rgbGreen;
//	byte rgbRed;
//	byte rgbReserved;
//};

struct IMAGEDATA {
	BYTE red;
	BYTE green;
	BYTE blue;
};

class BMP {
public:
	BMP( int H = 0 , int W = 0 );
	~BMP();
	BITMAPFILEHEADER strHead;
	BITMAPINFOHEADER strInfo;
	bool ima_created;
	GLubyte* ima;

	void Release();
	void Allocate(int H, int W);
	int GetH() { return strInfo.biHeight; }
	int GetW() { return strInfo.biWidth; }

	void Initialize( int H , int W );
	void Input( std::string file );
	GLubyte* GetPixels();
};

#endif
