/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// RockEntity.h
*/
#pragma once
#include "Entity.h"
#include <HAPI_lib.h>
#include <math.h> 
#include "Vector2.h"
#include "Graphics.h"

class RockEntity :public Entity
{
public:
	RockEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames = 1);
	~RockEntity();

	Eside GetSide() const override final 
	{
		if (position.y == oldPosition.y)
			return Eside::eNeutral;
		else
			return Eside::eKillRock;
	};
	void Update( std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector) override final;

	void Render(Graphics &graphic) override;

private:
	int CrashSoundID{ 1 };
	int FallSoundID{ 2 };
	bool timetomove{ false };
	int moved{ 0 };
	bool hasfallen{ false };
	void Movenextturn()
	{
		if (moved < 45)
		{
			moved++; 
			return;
		}
		else
			position.y += newspeed;
	}
	
};
