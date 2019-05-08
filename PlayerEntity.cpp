/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// PlayerEntity.cpp
*/
#include "PlayerEntity.h"
#include "Graphics.h"
#include "BulletEntity.h"
#include "Tiles.h"

PlayerEntity::PlayerEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames ) : Entity(spritename, spriteRect, NumberOfFrames)
{
}

PlayerEntity::~PlayerEntity()
{
	if (HAPI.GetSoundStatus(enemySoundID, null) == HAPI_MediaStatus::ePlaying)
		HAPI.StopSound(enemySoundID, true);
	if (HAPI.GetSoundStatus(walkingSoundID, null) == HAPI_MediaStatus::ePlaying)
		HAPI.StopSound(walkingSoundID, true);
}

void PlayerEntity::Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector)
{	 
	MakeRectFollowEntity();
	//Get Keyboard and Controller data
	 static const HAPI_TKeyboardData &kdata = HAPI.GetKeyboardData(); //only needs to be called once
	 const HAPI_TControllerData &cdata = HAPI.GetControllerData(0); // call every frame to ensure the controller hasnt been unplugged
	 
	 if (active)
	 {
		 oldPosition = position;
		 //controls - keyboard + controller dpad
		  //DigDug doesnt allow diagonals, therefore its easier to keep controls to the dpad of the controller
		 if (alive)
		 {
			 if (move_time <= 0)
			 {
				 if (kdata.scanCode[HK_LEFT] || (cdata.isAttached && cdata.digitalButtons[HK_DIGITAL_DPAD_LEFT])) // check if controller is attached to ensure the controller hasnt been unplugged
				 {					
					 Direction = eDirection::eLeft;
					 move_time = framesToCrossOneTile;					 
				 }
				 else if (kdata.scanCode[HK_RIGHT] || (cdata.isAttached && cdata.digitalButtons[HK_DIGITAL_DPAD_RIGHT]))
				 {					
					 Direction = eDirection::eRight;
					 move_time = framesToCrossOneTile;					 
				 }
				 else if (kdata.scanCode[HK_UP] || (cdata.isAttached && cdata.digitalButtons[HK_DIGITAL_DPAD_UP]))
				 {					 
					 Direction = eDirection::eUp;
					 move_time = framesToCrossOneTile;					
				 }
				 else if (kdata.scanCode[HK_DOWN] || (cdata.isAttached && cdata.digitalButtons[HK_DIGITAL_DPAD_DOWN]))
				 {					
					 Direction = eDirection::eDown;
					 move_time = framesToCrossOneTile;					 
				 }
				 else
					 Direction = eDirection::eStop;

				 if (kdata.scanCode[HK_SPACE] || (cdata.isAttached && cdata.digitalButtons[HK_DIGITAL_A]))
				 {					 
					 for (auto b : EntityVector)
					 {
						 if (b->GetSide() == Eside::eBullet)
							 b->Fired(Direction, position);
					 }
					 Direction = eDirection::eStop;
					 //Play sound if harpoon is fired
					 if (HAPI.GetSoundStatus(FireSoundID, null) != HAPI_MediaStatus::ePlaying)
						 HAPI.PlaySound("Data//Sounds//22-Harpoon.wav", options_2, FireSoundID);
				 }
			 }
			 if (move_time > 0)
				 move_time--;
			 
			 Movement();
			 
			 //Tiles
			 //Check if Tile has been dug through before
			 //if it has: move through
			 //if it hasnt: dig through it
			 for (auto T : TileSet)
			 {
				 if (!T->getActive())
				 {
					 T->Tentity->setRectPos();

					 if ((position.x < T->Tentity->position.x + T->Tentity->getRect().Width()) && (position.x + spriteRect.Width() > T->Tentity->position.x) &&
						 (position.y < T->Tentity->position.y + T->Tentity->getRect().Height()) && (position.y + spriteRect.Height() > T->Tentity->position.y))
					 {
						 T->setInactive();
						 Score += T->Score();
					 }
				 }			 
			 }
			 //SOUNDS
			 {
				 if (oldPosition != position && (Direction != eDirection::eStop && Direction != eDirection::eSquash)) //moving
				 {
					 //play sound
					 if (HAPI.GetSoundStatus(walkingSoundID, null) != HAPI_MediaStatus::ePlaying)
						 HAPI.PlaySound("Data//Sounds//03-DigDug Walking.wav", options, walkingSoundID);
					 if (HAPI.GetSoundStatus(enemySoundID, null) == HAPI_MediaStatus::ePlaying)
						 HAPI.StopSound(enemySoundID);
				 }
				 else //not moving
				 {
					 //dont play sound
					 if (HAPI.GetSoundStatus(walkingSoundID, null) == HAPI_MediaStatus::ePlaying)
					 {
						 HAPI.StopSound(walkingSoundID, true);
						 if (HAPI.GetSoundStatus(enemySoundID, null) != HAPI_MediaStatus::ePlaying)
							 HAPI.PlaySound("Data//Sounds//24-Monster Moving.wav", options, enemySoundID);
					 }
				 }
			 }
		 }
		 else if (!alive)
		 {
			 HAPI.StopSound(walkingSoundID, true);
			 Lives--;
			 if (Lives >= 1)
			 {
				 alive = true;
				 position = startPos;
			 }
			 else
			 {
				 //quickly stop and start the ending tune, to make it easier to unpause later
				 HAPI.PlaySound("Data//Sounds//11-Game Over Music.wav", options_2, ENDSOUNDID); 
				 HAPI.StopSound(ENDSOUNDID,true);
				 active = false;
			 }
		}
	 }
	 else
	 {	 
		 //unpause tune and after its finished: quit the game
		 if (HAPI.GetSoundStatus(ENDSOUNDID, null) == HAPI_MediaStatus::ePaused)
			 HAPI.PlaySound("Data//Sounds//11-Game Over Music.wav", options_2, ENDSOUNDID);
		 if (HAPI.GetSoundStatus(ENDSOUNDID, null) != HAPI_MediaStatus::ePlaying)
			 QUIT = true; 
	 }
}

void PlayerEntity::Collision(std::vector<Entity*> EntityVector)
{
	if (alive)
	{
		for (int i{ 0 }; i < EntityVector.size(); i++)
		{
			if (EntityVector.at(i)->GetSide() != GetSide() && EntityVector.at(i)->GetSide()!=Eside::eBullet)
			{
				if (EntityVector.at(i)->alive && EntityVector.at(i)->active)
				{
					if ((position.x < EntityVector.at(i)->position.x + EntityVector.at(i)->getRect().Width()) && (position.x + getRect().Width() > EntityVector.at(i)->position.x) &&
						(position.y < EntityVector.at(i)->position.y + EntityVector.at(i)->getRect().Height()) && (position.y + getRect().Height() > EntityVector.at(i)->position.y))
					{
						if (EntityVector.at(i)->GetSide() == Eside::eEnemy) 
						{
							if (EntityVector.at(i)->GetEnemyState() != eEnemyState::eBlowup1 &&
								EntityVector.at(i)->GetEnemyState() != eEnemyState::eBlowup2 &&
								EntityVector.at(i)->GetEnemyState() != eEnemyState::eBlowup3 &&
								EntityVector.at(i)->GetEnemyState() != eEnemyState::eBlowup4 &&
								EntityVector.at(i)->GetEnemyState() != eEnemyState::eSquashed) // Dont check for collisions with enemies which have been shot
							{
								alive = false;
								Direction = eDirection::eRight;
								HAPI.PlaySound("Data//Sounds//09-Monster touched DigDug.wav");
								return;
							}				
						}
						else if (EntityVector.at(i)->GetSide() == Eside::eKillRock) //if rock is falling with DigDug underneath -> kill DigDug
						{
							RockPosInVector = i;
							alive = false;
							Direction = eDirection::eSquash;
						}
						else
							position = oldPosition;
					}
				}
			}
		}
	}
}

void PlayerEntity::Render(Graphics &graphic)
{
	if (oldPosition != position && alive)
	{
		frameNum++;

		if (Direction == eDirection::eRight)
		{
			if (frameNum >= 2)
				frameNum = 0;
		}
		if (Direction == eDirection::eLeft)
		{
			if (frameNum >= 4)
				frameNum = 2;
		}
		if (Direction == eDirection::eDown)
		{
			if (frameNum >= 6)
				frameNum = 4;
		}
		if (Direction == eDirection::eUp)
		{
			if (frameNum >= 8)
				frameNum = 6;
		}

	}
	if (Direction == eDirection::eSquash)
	{
		frameNum = 9;
	}

		graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y, frameNum);

}