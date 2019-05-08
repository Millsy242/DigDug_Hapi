/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// World.h
*/
#pragma once

#include <vector>
#include "Rectangle.h"
#include <HAPI_lib.h>

class Graphics; 
class Entity;
class EnemyEntity;
class TileEntity;
class BulletEntity;
struct Tile; 
class World
{
public:
	World(int a);
	~World();
	void Run();
private:
//No need for Internal World Functions to be Public
	/*Setup:
	//should be universal for all levels
	//i.e. Only loads data into memory, thus reducing load times between levels
	//Use SetupComplete (bool) to check
	*/
	bool Setup(); 
	void Update();
	bool SetupLevel(std::vector<int> Grid, std::vector<int> ImageID, std::vector<std::string> ImageName, const int pooka, const int fygar, const int rock ); //Load data from xml file for each level
	void loadLevel(int score = 0);
	void Reset();
	bool clearscreen{ false };
	int getDeltaTime();
	bool loadfromXML(const std::string &Filename);
	void PrintTiles(); //DEBUG ONLY
	void setlevelnotofile(int levelno);
	std::string getlevelnofromfile();

	Graphics *Graphic{ nullptr };
	std::vector<Entity*> EntityVector; 	
	std::vector<Tile*> TileSet; 
	int Levelnum{ 0 };
	int Tiles_X{ 0 };
	int Tiles_Y{ 0 };
	int Lives{ 3 }; 
	int Player_Score{ 0 }; 
	bool exit = false;
	bool SetupComplete{ false };

	HAPISPACE::DWORD lastTimeUpdated = 0 ; //ms	
	//Keep track of Number of Enemies alive
	int numEnemies{ 0 }; 
	//Rectangles
	Rectangle playArea;

	Rectangle BackgroundRect;
	Rectangle RockRect;
	Rectangle PlayerRect;
	Rectangle PookasRect;
	Rectangle FygarRect;
	Rectangle BulletRect;
	
	Rectangle Tile0Rect;
	Rectangle Tile1Rect;
	Rectangle Tile2Rect;
	Rectangle Tile3Rect;
	Rectangle Tile4Rect;
	Rectangle Tile5Rect;
	Rectangle TileERect;
};

