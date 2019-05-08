/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Graphics.h
*/
#pragma once
#include <HAPI_lib.h>
#include <math.h> 
#include "Sprite.h"
#include "Rectangle.h"
#include <unordered_map>

class Sprite;

// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class Sprite;

class Graphics
{
private:
	std::unordered_map<std::string, Sprite*> SpriteMap;

	int screenWidth{ 0 }, screenHeight{ 0 };
	BYTE* screen = nullptr;
	Sprite *sprite = nullptr;
	Rectangle screenRect;
	bool showFPS{ true }; 
public:
	Graphics();
	~Graphics();
	//Setup Hapi
	//Input: window dimensions and optional fps display which defaults to true
	//Returns: a bool to show if Hapi was created correctly
	bool InitialiseHAPI(int Window_Width, int Window_Height, bool FPS = true) ;
	//Make a sprite
	//Input: filepath of texture + name of texture, optionally can provide number of frames if the texture is animated
	//Returns: the bounding Rectangle, this gives immediate access to the rectangle for collisions etc.
	Rectangle CreateSprite(const std::string &filepath, const std::string &name, int frames = 1);
	//Render Sprite with clipping
	//Input: name of sprite, position on screen, which frame to use
	//Returns: bool to show if the sprite could be rendered
	bool DrawSprite(const std::string & name, int posX, int posY, int frameNum = 0) const;
	//Render Sprite without clipping **NOT RECOMMENDED**
	//Input: name of sprite, position on screen, which frame to use
	//Returns: bool to show if the sprite could be rendered
	bool FastDrawSprite(const std::string & name, int posX, int posY, int frameNum = 1) const;
	//Clear the screen
	//Input: optionally provide a colour, black by default
	void ClearToColour(HAPI_TColour colour = HAPI_TColour::BLACK);
	//Set a pixel to a colour
	//Input: Position, COlour of pixel
	void Settocolour(int x, int y, HAPI_TColour colour);

	//Get height of Sprite
	//Input: Name of sprite
	//Return: Sprite Height (int)
	int getSpriteHeight(const std::string & name);
	//Get width of Sprite
	//Input: Name of sprite
	//Return: Sprite Width (int)
	int getSpriteWidth(const std::string & name);


}; 