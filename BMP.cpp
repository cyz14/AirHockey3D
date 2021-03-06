#include"BMP.h"
#include<cstdio>
#include<fstream>
#include<iostream>
#include<string>
#include<cmath>

using namespace std;

const double EPS = 1e-6;

BMP::BMP( int H , int W ) {
	Initialize( H , W );
	Allocate( H, W );
}

BMP::~BMP() {
	Release();
}

void BMP::Initialize( int H , int W ) {
	strHead.bfReserved1 = 0;
	strHead.bfReserved2 = 0;
	strHead.bfOffBits = 54;

	strInfo.biSize = strHead.bfOffBits - sizeof(strInfo);
	strInfo.biPlanes = 1;
	strInfo.biHeight = H;
	strInfo.biWidth = W;
	strInfo.biBitCount = 24;
	strInfo.biCompression = 0;
	strInfo.biSizeImage = H * W * 3;
	strInfo.biXPelsPerMeter = 0;
	strInfo.biYPelsPerMeter = 0;
	strInfo.biClrUsed = 0;
	strInfo.biClrImportant = 0;

	strHead.bfSize = strInfo.biSizeImage + strInfo.biBitCount;
}

void BMP::Release() {
	//for ( int i = 0 ; i < strInfo.biHeight ; i++ )
	//	delete[] ima[i];

	delete[] ima;
}

void BMP::Allocate(int H, int W) {
	//ima = new IMAGEDATA*[H];
	//for (int i = 0; i < H; i++)
	//	ima[i] = new IMAGEDATA[W];
	ima = new GLubyte[H * W * 3];
}

void BMP::Input( std::string file ) {
	Release();

	FILE *fpi;
	errno_t err;
	if ((err = fopen_s(&fpi, file.c_str(), "rb")) != 0) {
		printf("Failed to open file %s\n", file.c_str());
		fclose(fpi);
		exit(-1);
	}

	if (fread(&strHead, 1, sizeof(BITMAPFILEHEADER), fpi) < 1) {
		printf("Failed to load bmp file.\n");
		fclose(fpi);
		exit(-1);
	}
	if (strHead.bfType != 'MB') {
		printf("Not a bmp file.\n");
		fclose(fpi);
		exit(-1);
	}

	if (fread(&strInfo, 1, sizeof(BITMAPINFOHEADER), fpi) < strHead.bfOffBits - sizeof(BITMAPINFOHEADER)) {
		printf("Could not load bmp file.\n");
		fclose(fpi);
		exit(-1);
	}
	
	Allocate( strInfo.biHeight , strInfo.biWidth );

	int bitsize = strInfo.biSizeImage;
	if (bitsize == 0)
		bitsize = (strInfo.biWidth *
		strInfo.biBitCount + 7) / 8 *
		abs(strInfo.biHeight);

	int byteread = fread(ima, 1, bitsize, fpi);
	if (byteread < bitsize) {
		printf("Failed to read image data.\n");
		fclose(fpi);
		exit(-1);
	}

	fclose( fpi );
}

GLubyte* BMP::GetPixels() { 
	return (GLubyte *)ima;
}

