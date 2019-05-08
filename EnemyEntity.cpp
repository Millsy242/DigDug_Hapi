/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// EnemyEntity.cpp
*/
#include "EnemyEntity.h"
#include "Graphics.h"
#include <stdlib.h>   
#include <stdio.h>     
#include <time.h>    
#include "Tiles.h"

EnemyEntity::EnemyEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames) : Entity(spritename, spriteRect, NumberOfFrames)
{
	srand((unsigned int)time(NULL)); // set random seed
}

EnemyEntity::~EnemyEntity()
{
}

void EnemyEntity::Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector)
{
	MakeRectFollowEntity();
	if (alive)
	{
		getCurrentTile(TileSet);

		oldPosition = position;
		lastDirection = Direction;		

		if (move_time <= 0)
		{
			if (EnemyState != eEnemyState::eBlowup1 && EnemyState != eEnemyState::eBlowup2 && EnemyState != eEnemyState::eBlowup3 && EnemyState != eEnemyState::eBlowup4)
			{
				//AI stuff
				if (numEnemies < 2)
				{
					//If this enemy is the only one left
					//Escape!
					EnemyState = eEnemyState::eEscape;
				}
				else				
					AI(TileSet);

				if (EnemyState == eEnemyState::eEscape)
				{
					//This section here is basically making the enemy go straight up until they are at the top of the map
					//Then go left until they cant go left anymore
					//After which kill them as they have escaped DigDug
					if (current->row > 2)
					{
						Direction = eDirection::eUp;
						move_time = framesToCrossOneTile;
					}
					else
					{
						Direction = eDirection::eLeft;
						move_time = framesToCrossOneTile;
					}

					if (current->col == 0 && current->row == 2)
					{
						alive = false;
						active = false; 
					}
				}			
			}
			else
			{
				//cant move when inflated
				Direction = eDirection::eStop;
			}
		}
		if (move_time > 0)
			move_time--;

		Movement(); //Do Movement		
	}
}

void EnemyEntity::Collision(std::vector<Entity*> EntityVector)
{
	if (active)
	{
		for (int i{ 0 }; i < EntityVector.size(); i++)
		{
			if (EntityVector.at(i)->GetSide() == Eside::eKillRock || EntityVector.at(i)->GetSide() == Eside::eNeutral || EntityVector.at(i)->GetSide() == Eside::eBullet && (EntityVector.at(i)->active && EntityVector.at(i)->alive)) //No Point checking for collisions when one doesnt need to
			{
				if ((position.x < EntityVector.at(i)->position.x + EntityVector.at(i)->getRect().Width()) && (position.x + getRect().Width() > EntityVector.at(i)->position.x) &&
					(position.y < EntityVector.at(i)->position.y + EntityVector.at(i)->getRect().Height()) && (position.y + getRect().Height() > EntityVector.at(i)->position.y))
				{
					if (EntityVector.at(i)->GetSide() == Eside::eNeutral)
					{
						if(EnemyState != eEnemyState::eEscape) //Move through rocks when escaping - not ideal solution
							position = oldPosition;
					}
					else if (EntityVector.at(i)->GetSide() == Eside::eKillRock)
					{
						RockPosInVector = i;
						EnemyState = eEnemyState::eSquashed;
						alive = false;
					}
					if (EntityVector.at(i)->GetSide() == Eside::eBullet)
					{
						if (HAPI.GetSoundStatus(PumpSoundID, null) != HAPI_MediaStatus::ePlaying)
							HAPI.PlaySound("Data//Sounds//23-Pumping.wav", options_2, PumpSoundID);

						if (EnemyState != eEnemyState::eBlowup1 && EnemyState != eEnemyState::eBlowup2 && EnemyState != eEnemyState::eBlowup3 && EnemyState != eEnemyState::eBlowup4)
						{
							Direction = eDirection::eStop;
							EnemyState = eEnemyState::eBlowup1;
						}
						else
						{
							if (EnemyState == eEnemyState::eBlowup1)
							{

								EnemyState = eEnemyState::eBlowup2;
							}
							else if (EnemyState == eEnemyState::eBlowup2)
							{

								EnemyState = eEnemyState::eBlowup3;
							}
							else if (EnemyState == eEnemyState::eBlowup3)
							{

								EnemyState = eEnemyState::eBlowup4;
							}
							else if (EnemyState == eEnemyState::eBlowup4)
							{

								alive = false;
								active = false;
							}
						}
						EntityVector.at(i)->Kill();
					}
				}
			}
		}
	}
	if (EnemyState == eEnemyState::eSquashed && alive == false)
		position.y = EntityVector.at(RockPosInVector)->position.y + getRect().Width();
}

void EnemyEntity::getCurrentTile(std::vector<Tile*> TileSet)
{
	for (auto p : TileSet)
	{
		if (p->Coordinates == position)
		{
			current = p;
		}
	}
}

void EnemyEntity::Movement()
{
	switch (Direction)
	{
	case  eDirection::eLeft:
		if (current->East != nullptr)
		{
			if (current->East->passable() || EnemyState == eEnemyState::eEscape)
			{
				position.x -= newspeed;
			}
		}
		break;
	case  eDirection::eRight:
		if (current->West != nullptr)
		{
			if (current->West->passable() || EnemyState == eEnemyState::eEscape)
			{
				position.x += newspeed;
			}
		}
		break;
	case  eDirection::eUp:
		if (current->North != nullptr)
		{
			if (current->North->passable() || EnemyState == eEnemyState::eEscape)
			{
				position.y -= newspeed;
			}
		}
		break;
	case  eDirection::eDown:
		if (current->South != nullptr)
		{
			if (current->South->passable() || EnemyState == eEnemyState::eEscape)
			{
				position.y += newspeed;
			}
		}
		break;
	case  eDirection::eStop:
		position = position;
		break;
	default:
		break;
	}
}

void EnemyEntity::AI(std::vector<Tile*> TileSet)
{
	if (alive && active)
	{
		int errorcounter{ 0 };
		bool end = false;
		while (!end)
		{
			switch (rand() % 5)
			{
			case 0:
				if (current->East != nullptr)
				{
					if (current->East->passable())
					{
						Direction = eDirection::eRight;
						move_time = framesToCrossOneTile;
						end = true;
					}
				}
				break;
			case 1:
				if (current->North != nullptr)
				{
					if (current->North->passable())
					{
						Direction = eDirection::eUp;
						move_time = framesToCrossOneTile;
						end = true;
					}
				}
				break;
			case 2:
				if (current->West != nullptr)
				{
					if (current->West->passable())
					{
						Direction = eDirection::eLeft;
						move_time = framesToCrossOneTile;
						end = true;
					}
				}
				break;
			case 3:
				if (current->South != nullptr)
				{
					if (current->South->passable())
					{
						Direction = eDirection::eDown;
						move_time = framesToCrossOneTile;
						end = true;
					}
				}
				break;
			case 4:
				Direction = eDirection::eStop;
				move_time = framesToCrossOneTile;
				break;
			default:
				break;
			}
			//prevent ai getting stuck in a loop where it cant move in any direction
			if (errorcounter > 4)
				end = true;
			errorcounter++;
		}
	}
}

void EnemyEntity::Render(Graphics & graphic)
{
	if (active)
	{
		graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y, frameNum);

		if (oldPosition != position)
		{
			frameNum++;
		}
		if (EnemyState == eEnemyState::eSneak || EnemyState == eEnemyState::eEscape)
		{
			if (frameNum >= 4)
				frameNum = 2;
		}
		else if (EnemyState == eEnemyState::eNormal || EnemyState == eEnemyState::eAttack || EnemyState == eEnemyState::eEscape)
		{
			if (frameNum >= 2)
				frameNum = 0;
		}
		if (EnemyState == eEnemyState::eSquashed)
		{
			HAPI.PlaySound("Data//Sounds//17-Monster Squashed.wav");

			frameNum = 4;

			if (HAPI.GetTime() - lastTimeUpdatedAnim >= animTime)
			{
				active = false;
				lastTimeUpdatedAnim = HAPI.GetTime();
			}
		}
		if (EnemyState == eEnemyState::eBlowup1)
		{
			frameNum = 5;
		}
		if (EnemyState == eEnemyState::eBlowup2)
		{
			frameNum = 6;
		}
		if (EnemyState == eEnemyState::eBlowup3)
		{
			frameNum = 7;
		}
		if (EnemyState == eEnemyState::eBlowup4)
		{
			frameNum = 8;
		}
	}
}