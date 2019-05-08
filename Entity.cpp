/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Entity.cpp
*/
#include "Entity.h"
#include "Graphics.h"
#include "Tiles.h"

Entity::Entity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames) : SpriteName(spritename), numframes(NumberOfFrames), spriteRect(SpriteRect)
{
	options = HAPI_TSoundOptions(0.2f, true);
	options_2 = HAPI_TSoundOptions(0.2f);
}

Entity::~Entity()
{
}

void Entity::Render(Graphics &graphic)
{
	//Default Render
	if (active)
	{
		graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y, frameNum);
		if (numframes > 1) //no point wasting time changing framenum, if theres only 1 available
		{
			if (HAPI.GetTime() - lastTimeUpdatedAnim >= animTime)
			{
				frameNum++;
				if (frameNum >= numframes)
					frameNum = 0;

				lastTimeUpdatedAnim = HAPI.GetTime();
			}
		}
	}
}

void Entity::Movement()
{
	//Movement using Timestep speed
	switch (Direction)
	{
	case  eDirection::eLeft:
		position.x -= newspeed;
		break;
	case  eDirection::eRight:
		position.x += newspeed;
		break;
	case  eDirection::eUp:
		position.y -= newspeed;
		break;
	case  eDirection::eDown:
		position.y += newspeed;
		break;
	default:
		break;
	}
}

void Entity::edgecheck(int screenWidth, int screenHeight, Rectangle playArea)
{
	//Check if entity has left the screen
	if (active)
	{ 
		if (GetSide() == Eside::eNeutral || GetSide() == Eside::eBackground || GetSide() ==Eside::eTile ) //Pointless checking Entities that don't move
			return;
		else
		{
			MakeRectFollowEntity(true);

			if (position.x + spriteRect.Width() > playArea.Width())
			{
				if (GetSide() == Eside::eBullet)
					Kill();
				position.x = oldPosition.x;
			}
			if (position.x < 0)
			{
				if (GetSide() == Eside::eBullet)
					Kill();
				position.x = oldPosition.x;
			}
			if (position.y + getRect().Height() > screenHeight - getRect().Height())
			{
				if (GetSide() == Eside::eBullet)
					Kill(); //Kill Bullet if it hits the edge 
				position.y = oldPosition.y;
			}
			if (position.y < playArea.Top - spriteRect.Height())
			{
				if (GetSide() == Eside::eBullet)
					Kill();
				position.y = oldPosition.y;
			}

			MakeRectFollowEntity();				
		}
	}
}
