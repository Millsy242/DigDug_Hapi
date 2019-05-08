/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Entity.h
*/
#pragma once
#include "Vector2.h"
#include <HAPI_lib.h>
#include <string>
#include "Rectangle.h"

enum class Eside
{
	ePlayer,
	eEnemy,
	eKillRock,
	eBullet,
	eTile,
	eBackground,
	eNeutral,
};
enum class eDirection
{
	eStop,
	eLeft,
	eRight,
	eUp,
	eDown,
	eSquash
};
enum class eEnemyState
{
	eNULL, // For non-enemies
	eNormal, // action at the start of a level, random movement
	eAttack, // action when after x time, chase player 
	eSneak, // action when sneaking through soil
	eBlowup1, // action after 1 pump/bullet from player
	eBlowup2, // action after 2 pumps/bullets from player
	eBlowup3, // action after 3 pumps/bullets from player
	eBlowup4, // action after 4 pumps/bullets from player
	eEscape, // action when only 1 enemy left, go straight up and turn left
	eSquashed //show squashed texture
};
using namespace HAPISPACE;
struct Tile;
class Graphics;
class Entity
{
public:
	Entity(std::string spritename, Rectangle SpriteRect, int NumberOfFrames = 1);
	virtual ~Entity();

	virtual void Render(Graphics &graphic);
	void setPosition(Vector2 newPos) //Only use when creating Entity
	{
		position = newPos;
		startPos = position;
		MakeRectFollowEntity();
	}
	virtual Eside GetSide() const = 0; 
	virtual eEnemyState GetEnemyState() const { return eEnemyState::eNULL; };
	virtual void Update(std::vector<Tile*> TileSet, int numEnemies, std::vector<Entity*> EntityVector ) = 0;
	virtual void Movement(); //Move the Entity
	virtual void Collision(std::vector<Entity*> EntityVector) {};
	void edgecheck(int screenWidth, int screenHeight, Rectangle playArea);
	virtual void Fired(eDirection direction, Vector2 begin) {};
	void Kill() { alive = false; active = false; };	

	Rectangle getRect()
	{
		return spriteRect;
	}
	void setLives(int numlives)
	{
		Lives = numlives;
	}
	std::string getSpriteName() {return SpriteName;};
	bool getQUIT() { return QUIT; }
	int getLives() { return Lives; }
	int getScore() { return Score; }
	void setScore(int score) { Score = score; }

	bool alive{ true };
	bool active{ true };
	Vector2 position;	
protected:	

	std::string SpriteName; 
	int frameNum{ 0 };
	int numframes{ 1 };		
	int Lives{ 0 };
	int Score{ 0 };
	int RockPosInVector{ 0 };
	int framesToCrossOneTile = 15;
	int newspeed = 15 / framesToCrossOneTile;
	int move_time = 0;
	float null; //for sounds
	bool QUIT{ false };

	eDirection Direction;
	HAPI_TSoundOptions options;
	HAPI_TSoundOptions options_2;
	Vector2 oldPosition ;
	Vector2 startPos;
	Rectangle spriteRect;

	HAPISPACE::DWORD lastTimeUpdatedAnim{ 0 }; //ms
	HAPISPACE::DWORD animTime{ 500 }; //ms



	void MakeRectFollowEntity(bool force = false)
	{		
		if (oldPosition != position || force)
		{
			spriteRect.setPos(position.x,position.x+15, position.y,position.y+15);
		}
	}	
};
