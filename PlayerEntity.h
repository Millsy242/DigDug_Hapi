/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// PlayerEntity.h
*/
#pragma once
#include "Entity.h"
#include <HAPI_lib.h>
#include <math.h> 
#include <vector>
#include "Rectangle.h"

using namespace HAPISPACE;
class Graphics;
class PlayerEntity : public Entity
{
public:
	PlayerEntity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames = 1);
	~PlayerEntity();

	Eside GetSide() const override final {return Eside::ePlayer;};
	//Update 
	void Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector) override final;
	//Draw Entity to screen
	//Input: access to Graphic class
	void Render(Graphics &graphic) override;
	//Check for collisions
	//Input: Vector of Entities
	void Collision(std::vector<Entity*> EntityVector) override;

private:
	//ID's for Sounds
	int walkingSoundID{ 1 };
	int enemySoundID{ 2 };
	int ENDSOUNDID{ 3 };
	int FireSoundID{ 4 };	
};

