/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// BackgroundEntity.cpp
*/
#include "BackgroundEntity.h"
#include "Graphics.h"

BackgroundEntity::BackgroundEntity(std::string spritename, Rectangle SpriteRect) : Entity(spritename, spriteRect)
{ 
}

BackgroundEntity::~BackgroundEntity()
{
}

void BackgroundEntity::Update(std::vector<Tile*> TileSet,int numEnemies, std::vector<Entity*> EntityVector)
{
	MakeRectFollowEntity();
	active = false;
	alive = false;
}

void BackgroundEntity::Render(Graphics &graphic)
{
	graphic.DrawSprite(SpriteName, (int)position.x, (int)position.y);
}
