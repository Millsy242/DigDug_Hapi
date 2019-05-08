/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// RockEntity.cpp
*/
#include "RockEntity.h"
#include "Tiles.h"

RockEntity::RockEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames) : Entity(spritename, spriteRect, NumberOfFrames)
{
	oldPosition = position;
}


RockEntity::~RockEntity()
{
}

void RockEntity::Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector)
{
	MakeRectFollowEntity();
	oldPosition = position;
	if (active)
	{		
		if (move_time <= 0)
		{		
			for (int i{ 0 }; i < TileSet.size(); i++)
			{
				if (TileSet[i]->Coordinates == position)
				{
					if (i + 28 < TileSet.size())
					{						
						if (TileSet.at(i + 14)->getActive())
						{
							timetomove = true;
							move_time = framesToCrossOneTile;
						}
						else
							timetomove = false; 
					}
				}
			}	
		}
		if (move_time > 0)
			move_time--;

		if (timetomove)
			Movenextturn();

		if (oldPosition.y < position.y)
		{
			hasfallen = true;
			if (HAPI.GetSoundStatus(FallSoundID, null) != HAPI_MediaStatus::ePlaying)
				HAPI.PlaySound("Data//Sounds//20-Rock Fall.wav", options_2, FallSoundID);
		}
	}
}

void RockEntity::Render(Graphics & graphic)
{
	if (active)
	{
		if (oldPosition.y == position.y && hasfallen == true)
		{
			alive = false;
		}

		if (!alive && hasfallen)
		{
			if (HAPI.GetSoundStatus(CrashSoundID, null) != HAPI_MediaStatus::ePlaying)
				HAPI.PlaySound("Data//Sounds//19-Rock Dropped on the Floor.wav", options_2, CrashSoundID);
			if (HAPI.GetTime() - lastTimeUpdatedAnim >= animTime)
			{
				frameNum++;
				lastTimeUpdatedAnim = HAPI.GetTime();
			}
		}
		if (frameNum < numframes + 2)
			graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y, frameNum);
		else
			active = false;
	}
}