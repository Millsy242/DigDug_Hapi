/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// TileEntity.cpp
*/
#include "TileEntity.h"

TileEntity::TileEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames) : Entity(spritename, spriteRect, NumberOfFrames)
{
}
TileEntity::~TileEntity()
{
}
void TileEntity::Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector)
{	
}
void TileEntity::setRectPos()
{
	MakeRectFollowEntity();
}
