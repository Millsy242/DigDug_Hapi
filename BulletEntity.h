/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// BulletEntity.h
*/
#pragma once
#include "Entity.h"
#include <HAPI_lib.h>
#include <math.h> 
#include "Vector2.h"
#include "Graphics.h"

class BulletEntity : public Entity
{
public:
	BulletEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames = 1);
	~BulletEntity();

	Eside GetSide() const override final {return Eside::eBullet;}
	void Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector) override final;
	void Render(Graphics &graphic) override;
	void Fired(eDirection direction, Vector2 begin) override final;	

private:
	eDirection TempDirection;

	void setDirection(eDirection direction)	{TempDirection = direction;}
};
