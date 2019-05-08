/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// BulletEntity.cpp
*/

#include "BulletEntity.h"

BulletEntity::BulletEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames) : Entity(spritename, spriteRect, NumberOfFrames)
{
	//Make sure Bullet isnt shown when created
	alive = false;
	active = false; 
}

BulletEntity::~BulletEntity()
{
}

void BulletEntity::Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector)
{
	if (active)
	{
	MakeRectFollowEntity();
	oldPosition = position;
			
		if (move_time <= 0)
		{		
			//Ensure Movement is inline with timestep
			if (TempDirection == eDirection::eLeft)
			{
				Direction = eDirection::eLeft;
				move_time = framesToCrossOneTile;
			}
			else if (TempDirection == eDirection::eRight)
			{
				Direction = eDirection::eRight;
				move_time = framesToCrossOneTile;
			}
			else if (TempDirection == eDirection::eUp)
			{
				Direction = eDirection::eUp;
				move_time = framesToCrossOneTile;
			}
			else if (TempDirection == eDirection::eDown)
			{
				Direction = eDirection::eDown;
				move_time = framesToCrossOneTile;
			}	
		}
		if (move_time > 0)
			move_time--;

		Movement();
	}
}

void BulletEntity::Fired(eDirection direction, Vector2 begin)
{
	//make the bullet start moving 
	active = true;
	alive = true;
	setDirection(direction);
	setPosition(begin);
}

void BulletEntity::Render(Graphics & graphic)
{
	if (oldPosition == position)
	{
		active = false;
		alive = false;
	}
	if (active)
	{
		switch (Direction)
		{
		case  eDirection::eLeft:
			frameNum = 3;
			break;
		case  eDirection::eRight:
			frameNum = 2;
			break;
		case  eDirection::eUp:
			frameNum = 0;
			break;
		case  eDirection::eDown:
			frameNum = 1;
			break;
		default:
			break;
		}
		graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y, frameNum);
	}
}


