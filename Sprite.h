/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Sprite.h
*/
#pragma once
#include <HAPI_lib.h>
#include <math.h> 
#include "Rectangle.h"
using namespace HAPISPACE;
class Sprite
{
public:
	Sprite(int a);
	~Sprite();

	//Load the texture
	//Input: filepath, number of frames
	//Return: Rectangle
	Rectangle loadTexture(std::string path, int frames) ;
	//Fast Render without clipping or transparency
	//Input: screen pointer,size of screen, position,which frame to render
	void FastBlit(BYTE *screen, const Rectangle &screenRect, int posX, int posY, int frameNumber);
	//Transparent supporting Render without clipping
	//Input: screen pointer,size of screen, position
	void TransparentBlit(BYTE *screen, int screenWidth, int posX, int posY);
	//Render with clipping and transparency
	//Input: screen pointer,size of screen, position,which frame to render
	void ClipBlit(BYTE *screen, const Rectangle &screenRect, int posX, int posY, int frameNumber);
	//Return Rectangle of sprite
	Rectangle getFrameRect() { return FrameRect; };

private:
	Rectangle SpriteRect;
	Rectangle FrameRect;

	BYTE *textureData = nullptr;

	int height{ 0 }, width{ 0 };
	int Frames, frameSize;

	int tempx{ 0 };
	int tempy{ 0 };


};

