/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// EnemyEntity.h
*/
#pragma once
#include "Entity.h"
#include <HAPI_lib.h>
#include <math.h> 

using namespace HAPISPACE;
class EnemyEntity :
	public Entity
{
public:
	EnemyEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames=1);
	~EnemyEntity();

	Eside GetSide() const override final { return Eside::eEnemy; };
	eEnemyState GetEnemyState() const override final{return EnemyState;	}
	void Update(std::vector<Tile*> TileSet,int numEnemies, std::vector<Entity*> EntityVector) override final;
	void Collision(std::vector<Entity*> EntityVector) override;
	void Render(Graphics &graphic) override;
	void getCurrentTile(std::vector<Tile*> TileSet);
	void Movement() override final; 
	void AI(std::vector<Tile*> TileSet);
private:
	eEnemyState EnemyState{ eEnemyState::eNormal };
	eDirection lastDirection;
	Tile *current{ nullptr };
	int PumpSoundID{ 0 };
};

