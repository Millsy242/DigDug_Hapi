/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// TileEntity.h
*/
#pragma once
#include "Entity.h"
class TileEntity :	public Entity
{
public:
	TileEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames = 1);
	~TileEntity();
	Eside GetSide() const override final { return Eside::eTile; };
	void Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector) override final;
	void setRectPos();
};

