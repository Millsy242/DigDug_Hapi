/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Sprite.cpp
*/
#include "Sprite.h"

Sprite::Sprite(int a)
{
}


Sprite::~Sprite()
{
	if(textureData != nullptr)
		delete[] textureData;	
}

Rectangle Sprite::loadTexture(std::string path, int frames) 
{
	if (!HAPI.LoadTexture(path, &textureData, width, height))
	{
		HAPI.UserMessage("Missing texture", "ERROR");
	}
	else
	{
		Frames = frames;
		frameSize = width / frames;
		
		SpriteRect = Rectangle(width, height);
		FrameRect = Rectangle(frameSize, height);
	}
	return getFrameRect();
}

void Sprite::FastBlit(BYTE *screen, const Rectangle &screenRect, int posX, int posY,int frameNumber)
{	
	BYTE *screenPnter = screen + (posX + posY*screenRect.Width()) * 4;
	BYTE *texturePnter = textureData;

	Rectangle newSource(0, FrameRect.Width(), 0, FrameRect.Height());
	Rectangle clippedRect(newSource);

	clippedRect.Translate(posX, posY);

	if (clippedRect.OutsideOf(screenRect))
		return;

	if (!screenRect.contains(clippedRect))
	{
		clippedRect.ClipTo(screenRect);
	}

	clippedRect.Translate(-posX, -posY);

	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	clippedRect.Translate(frameNumber*FrameRect.Width(), 0);

	for (int y{ 0 }; y < height; y++)
	{
		memcpy(screenPnter, texturePnter, FrameRect.Width() * 4);
		texturePnter += FrameRect.Width() * 4; //move texture pointer to next line
		screenPnter += screenRect.Width() * 4;  //move screen pointer to next line
	}
}
void Sprite::ClipBlit(BYTE *screen, const Rectangle &screenRect, int posX, int posY, int frameNumber)
{
	Rectangle newSource(0, FrameRect.Width(), 0, FrameRect.Height());
	Rectangle clippedRect(newSource);

	clippedRect.Translate(posX, posY);

	if (clippedRect.OutsideOf(screenRect))
		return;

	if (!screenRect.contains(clippedRect))
	{
		clippedRect.ClipTo(screenRect);
	}

	clippedRect.Translate(-posX, -posY);

	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	clippedRect.Translate(frameNumber*FrameRect.Width(), 0);

	BYTE *screenPnter = screen + (posX + posY*screenRect.Width()) * 4;
	BYTE *texturePnter = textureData + (clippedRect.Left + clippedRect.Top*SpriteRect.Width()) * 4;

	int EOLScreenIncrement = (screenRect.Width() - clippedRect.Width()) * 4;
	int EOLTextureIncrement = (SpriteRect.Width() - clippedRect.Width()) * 4;

	for (int y{ 0 }; y < clippedRect.Height(); y++)
	{
		for (int x{ 0 }; x < clippedRect.Width(); x++)
		{
			BYTE blue = texturePnter[0];
			BYTE green = texturePnter[1];
			BYTE red = texturePnter[2];
			BYTE alpha = texturePnter[3];

			if (alpha != 0 && alpha != 255)
			{
				//Blending
				screenPnter[0] = screenPnter[0] + ((alpha*(blue - screenPnter[0])) >> 8);
				screenPnter[1] = screenPnter[1] + ((alpha*(green - screenPnter[1])) >> 8);
				screenPnter[2] = screenPnter[2] + ((alpha*(red - screenPnter[2])) >> 8);
			}
			else
			{
				if (alpha == 255) //No point blending if pixel is not transparent
				{
					screenPnter[0] = blue;
					screenPnter[1] = green;
					screenPnter[2] = red;
				}
				//if (alpha == 0) Do nothing
			}

			texturePnter += 4;
			screenPnter += 4;
		}
		screenPnter += EOLScreenIncrement;
		texturePnter += EOLTextureIncrement;
	}
}
void Sprite::TransparentBlit(BYTE *screen, int screenWidth, int posX, int posY)
{
	BYTE *screenPnter = screen + (posX + posY * screenWidth) * 4;
	BYTE *texturePnter = textureData;

	int EOLScreenIncrement = (screenWidth - width) * 4; //used to advance to the next line of the texture

	for (int y{ 0 }; y < height; y++)
	{
		for (int x{ 0 }; x < width; x++)
		{
			BYTE blue = texturePnter[0];
			BYTE green = texturePnter[1];
			BYTE red = texturePnter[2];
			BYTE alpha = texturePnter[3];

			if (alpha != 0 && alpha != 255)
			{
				//Blending
				screenPnter[0] = screenPnter[0] + ((alpha*(blue - screenPnter[0])) >> 8);
				screenPnter[1] = screenPnter[1] + ((alpha*(green - screenPnter[1])) >> 8);
				screenPnter[2] = screenPnter[2] + ((alpha*(red - screenPnter[2])) >> 8);
			}
			else
			{
				if (alpha == 255) //No point blending if pixel is not transparent
				{
					screenPnter[0] = blue;
					screenPnter[1] = green;
					screenPnter[2] = red;
				}
				//if (alpha == 0) Do nothing
			}
			screenPnter += 4;
			texturePnter += 4;
		}
		screenPnter += EOLScreenIncrement;
	}
}