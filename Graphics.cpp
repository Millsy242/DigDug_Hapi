/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Graphics.cpp
*/
#include "Graphics.h"

Graphics::Graphics() 
{
}

Graphics::~Graphics()
{
	for (std::unordered_map<std::string, Sprite*>::iterator it = SpriteMap.begin(); it != SpriteMap.end(); ++it)
		delete it->second; 
}

bool Graphics::InitialiseHAPI(int Window_Width, int Window_Height, bool FPS)
{
	showFPS = FPS;
	screenWidth = Window_Width;
	screenHeight = Window_Height;
	if (!HAPI.Initialise(screenWidth, screenHeight, "DIGDUG"))
		return false;

	screenRect = Rectangle(screenWidth, screenHeight);
	screen = HAPI.GetScreenPointer();

	if(showFPS)
	HAPI.SetShowFPS(true, 0, 0, HAPI_TColour::YELLOW);
	
	return true;
}

Rectangle Graphics::CreateSprite(const std::string &filepath,const std::string &name, int frames)
{
	if (frames < 0)
		frames = 0;
	sprite = new Sprite(0);

	sprite->loadTexture(filepath, frames);

	SpriteMap[name] = sprite;
	
	return sprite->getFrameRect();
}

bool Graphics::DrawSprite(const std::string & name, int posX, int posY, int frameNum) const
{
	if (SpriteMap.find(name) == SpriteMap.end())
	{
		std::cerr << "can't find " << name << std::endl;
		return false;
	}
	
	SpriteMap.at(name)->ClipBlit(screen, screenRect, posX, posY, frameNum);
	return true;
}
bool Graphics::FastDrawSprite(const std::string & name, int posX, int posY, int frameNum)const
{
	if (SpriteMap.find(name) == SpriteMap.end())
	{
		std::cerr << "can't find " << name << std::endl;
		return false;
	}
		SpriteMap.at(name)->FastBlit(screen, screenRect, posX, posY,frameNum);
		return true;
	
}

void Graphics::ClearToColour(HAPI_TColour colour)
{
	if (colour == HAPI_TColour::BLACK) //Better Performance
		memset(screen, 0, screenWidth*screenHeight * 4);
	else
	{
		for (int i{ 0 }; i < (screenWidth*screenHeight); i++)
		{
			screen[i * 4] = colour.red;
			screen[i * 4 + 1] = colour.green;
			screen[i * 4 + 2] = colour.blue;
		}
	}
}
void Graphics::Settocolour(int x, int y, HAPI_TColour colour)
{
	unsigned int offset = (x + y*screenWidth) * 4;

	screen[offset] = colour.red;
	screen[offset + 1] = colour.green;
	screen[offset + 2] = colour.blue;
}

int Graphics::getSpriteHeight(const std::string & name)
{
	if (SpriteMap.find(name) == SpriteMap.end())
	{
		std::cerr << "can't find " << name << std::endl;
		return -1;
	}
	SpriteMap.at(name)->getFrameRect().Height();
	return -1;
}

int Graphics::getSpriteWidth(const std::string & name)
{
	if (SpriteMap.find(name) == SpriteMap.end())
	{
		std::cerr << "can't find " << name << std::endl;
		return -1;
	}
	SpriteMap.at(name)->getFrameRect().Width();
	return -1;
}

