/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// BackgroundEntity.h
*/
#pragma once
#include "Entity.h"

class Graphics;

class BackgroundEntity : public Entity
{
public:
	BackgroundEntity(std::string spritename, Rectangle SpriteRect);
	~BackgroundEntity();

	Eside GetSide() const override final { return Eside::eBackground; };
	void Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector) override final;
	void Render(Graphics &graphic) override;
};

