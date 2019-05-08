/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// World.cpp
*/
#include "World.h"
#include "Graphics.h"
#include "EnemyEntity.h"
#include "BackgroundEntity.h"
#include "BulletEntity.h"
#include "PlayerEntity.h"
#include "RockEntity.h"
#include "TileEntity.h"
#include <HAPI_lib.h>
#include "Rectangle.h"
#include "Vector2.h"
#include "Tiles.h"
#include <string>
#include <fstream>

#define screenWidth 210
#define screenHeight 270
#define DEBUG false

using namespace HAPISPACE;

World::World(int a = 0)
{
}

World::~World()
{
	delete Graphic;

	for (auto p : EntityVector)
		delete p;
	for (auto p : TileSet)
		delete p; 
}

void World::Run()
{
	Graphic = new Graphics();
	if (!Graphic->InitialiseHAPI(screenWidth, screenHeight))
		return;
	if (Setup())
		loadLevel();
}
std::string World::getlevelnofromfile()
{
	//Load level Data from config file
	std::string numfromfile;
	std::ifstream level;
	level.open("Data\\CurrentLevel.txt");
	level >> numfromfile; 
	level.close();	
	Levelnum = atoi(numfromfile.c_str());
	return numfromfile;	
	
}
void World::setlevelnotofile(int levelno)
{
	//Write level Data to config file
	std::ofstream Level;
	Level.open("Data\\CurrentLevel.txt");
	Level << std::to_string(levelno);
	Level.close();
}
void World::loadLevel(int score)
{	
	//Load Level 
	std::string Level;
	Level = "Level_" + getlevelnofromfile();
	Reset();
	Player_Score = score;
	if (loadfromXML("Data\\" + Level + ".xml"))
	{
		Update();
	}
	else
	{
		HAPI_UserResponse response;
		HAPI.UserMessage("I seem to have ran out of levels\nWould you like to restart from level 1?", "Oops, Level Error", HAPI_ButtonType::eButtonTypeYesNo, &response);
		if (response == HAPI_UserResponse::eUserResponseYes)
		{
			Reset();
			loadLevel(Player_Score);
		}
		else
			return;
	}
}

void World::Reset()
{
	//Reset World values to avoid duplicating everything when loading next level
	Levelnum = 1;
	setlevelnotofile(Levelnum);
	Tiles_X = 0;
	Tiles_Y = 0;
	Lives = 3;
	exit = false;
	SetupComplete = false;
	lastTimeUpdated = 0;
	while (!EntityVector.empty())
		EntityVector.pop_back();
	while (!TileSet.empty())
		TileSet.pop_back(); 
}

bool World::Setup()
{
	//Setup Textures and get Rectangles
	BackgroundRect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\background.tga", "background")));
	RockRect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\Rock_2.png", "Rock", 3)));
	PlayerRect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\Player.png", "Player", 9)));
	PookasRect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\Pookas.png", "Pookas", 9)));
	FygarRect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\Fygar.png", "Fygar", 10)));
	BulletRect  = Rectangle((Graphic->CreateSprite("Data\\Textures\\Bullet.png", "Bullet",4)));

	if (DEBUG)
	{
		Tile0Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile0D.png", "Tile0")));
		Tile1Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile1D.png", "Tile1")));
		Tile2Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile2D.png", "Tile2")));
		Tile3Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile3D.png", "Tile3")));
		Tile4Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile4D.png", "Tile4")));
		Tile5Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile5D.png", "Tile5")));
	}
	else
	{
		Tile0Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile0.png", "Tile0")));
		Tile1Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile1.png", "Tile1")));
		Tile2Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile2.png", "Tile2")));
		Tile3Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile3.png", "Tile3")));
		Tile4Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile4.png", "Tile4")));
		Tile5Rect = Rectangle((Graphic->CreateSprite("Data\\Textures\\Tile5.png", "Tile5")));
	}		
	TileERect =  Rectangle((Graphic->CreateSprite("Data\\Textures\\TileE.png", "TileE"))); //error tile texture

	return true;
}

bool World::loadfromXML(const std::string &Filename)
{
	//Actually Load from XML file
	std::vector<int> Grid;
	std::vector<int> ImageID;
	std::vector<std::string> ImageName;

	// HAPI provides a class for working with XML files
	// We use the constructor that loads and parses the file:
	CHapiXML xml(Filename);

	// Check it has loaded OK
	if (xml.GetRootNode() == nullptr)
		return false;

	CHapiXMLNode *node = xml.GetFirstNodeWithName("Map_Grid");
	if (!node)
		return false;

	std::vector<CHapiXMLNode*> TileNodes = xml.GetAllNodesWithName("Tile");
	if (TileNodes.empty())
		return false;

	std::vector<CHapiXMLNode*> numpookaNodes = xml.GetAllNodesWithName("numPooka");
	if (numpookaNodes.empty())
		return false;
	std::vector<CHapiXMLNode*> numfygarNodes = xml.GetAllNodesWithName("numFygar");
	if (numfygarNodes.empty())
		return false;
	std::vector<CHapiXMLNode*> numrockNodes = xml.GetAllNodesWithName("numRock");
	if (numrockNodes.empty())
		return false;

	CHapiXMLAttribute GridCell_;
	CHapiXMLAttribute ImageID_;
	CHapiXMLAttribute ImageName_;
	for (auto p : TileNodes)
	{
		p->GetAttributeWithName("GridCell", GridCell_);
		p->GetAttributeWithName("ImageID", ImageID_);
		p->GetAttributeWithName("ImageName", ImageName_);
		Grid.push_back(GridCell_.AsInt());
		ImageID.push_back(ImageID_.AsInt());
		ImageName.push_back(ImageName_.AsString());
	}
	if (DEBUG)
	{
	for (int i{ 0 }; i < Grid.size(); i++)
		std::cout << Grid[i] << ": " << ImageID[i] << ", " << ImageName[i] << std::endl;
	}
	CHapiXMLAttribute numPooka_;
	CHapiXMLAttribute numFygar_;
	CHapiXMLAttribute numRock_;
	for (int i{ 0 }; i<1; i++)
	{
		numpookaNodes[i]->GetAttributeWithName("Total", numPooka_);
		numfygarNodes[i]->GetAttributeWithName("Total", numFygar_);
		numrockNodes[i]->GetAttributeWithName("Total", numRock_);
	}

	return SetupLevel(Grid, ImageID, ImageName, numPooka_.AsInt(), numFygar_.AsInt(), numRock_.AsInt());
}

bool World::SetupLevel(std::vector<int> Grid, std::vector<int> ImageID, std::vector<std::string> ImageName, const int pooka, const int fygar, const int rock)
{
	//Setup Level data using XML Values

	//Worlk out number of Tiles in X and Y
	const int Tiles_X = screenWidth / 15; //14
	const int Tiles_Y = screenHeight / 15; //18	

	//Set these using XML File
	int TotalRocks{ rock };
	int TotalPookas{ pooka };
	int TotalFygar{ fygar };
	int DigDugpos{ -1 };  //Position in Vector
	std::vector<int> RockPos;
	std::vector<int> PookaPos;
	std::vector<int> FygarPos;

	//Setup Tile System
	int iter{ 0 };
	for (int y{ 0 }; y < Tiles_Y; y++)
	{
		for (int x{ 0 }; x < Tiles_X; x++)
		{
			TileEntity *newtile = nullptr;
			bool MakeHalfTilesWalkable = false;
			bool remove = false;
			switch (ImageID[iter])
			{
			case -1:
				newtile = new TileEntity("Tile5", Tile5Rect);
				remove = true;
				break;
			case 4:
				newtile = new TileEntity("Tile0", Tile0Rect); // half tile for 'ground' layer, same style as tile1
				MakeHalfTilesWalkable = true;
				break;
			case 0:
				newtile = new TileEntity("Tile1", Tile1Rect);
				break;
			case 1:
				newtile = new TileEntity("Tile2", Tile2Rect);
				break;
			case 2:
				newtile = new TileEntity("Tile3", Tile3Rect);
				break;
			case 3:
				newtile = new TileEntity("Tile4", Tile4Rect);
				break;
			case 5:
				RockPos.push_back(iter);
				remove = true;
				newtile = new TileEntity("Tile5", Tile5Rect);
				break;
			case 6:
				PookaPos.push_back(iter);
				remove = true;
				newtile = new TileEntity("Tile5", Tile5Rect);
				break;
			case 7:
				DigDugpos = iter; 
				remove = true;
				newtile = new TileEntity("Tile5", Tile5Rect);
				break;
			case 8:
				FygarPos.push_back(iter); 
				remove = true;
				newtile = new TileEntity("Tile5", Tile5Rect);
				break;
			default:
				newtile = new TileEntity("TileE", TileERect);
				break;
			}
			iter++;

			if (newtile != nullptr)
			{
				Tile *tile = new Tile(y, x, int(TileSet.size()), newtile, Vector2(float(x * 15), float(y * 15)));
				if (remove)
				{
					tile->setInactive();
					tile->setpassable(false); 
				}
				if(MakeHalfTilesWalkable)
					tile->setpassable(false);

				TileSet.push_back(tile);
			}
			else
			{
				HAPI.UserMessage("Tile creation failed! \nI will attempt to continue past the error", "ERROR");
			}
		}
	}

	//Ensure Player is first entity in the vector for potential (probably negligable) speedups
	//Sometimes Player needs to perform actions when other entities don't, this might help, if there are a lot of entities!
	PlayerEntity *Player = new PlayerEntity("Player", PlayerRect, 9);
	if (DigDugpos == -1)
		Player->setPosition(Vector2(200, 30));
	else
		Player->setPosition(TileSet[DigDugpos]->Coordinates);
	Player->setLives(Lives);
	Player->setScore(Player_Score); 
	EntityVector.push_back(Player);

	//Number of Rocks needed for the game
	for (int numRocks{ 0 }; numRocks < TotalRocks; numRocks++)
	{		
		RockEntity *Rock = new RockEntity("Rock", RockRect, 1);
		Rock->setPosition(TileSet[RockPos[numRocks]]->Coordinates);
		EntityVector.push_back(Rock);
	}
	//Number of Pookas enemies
	for (int numPookas{ 0 }; numPookas < TotalPookas; numPookas++)
	{
		EnemyEntity *Pookas = new EnemyEntity("Pookas", PookasRect, 9);
		Pookas->setPosition(TileSet[PookaPos[numPookas]]->Coordinates);
		EntityVector.push_back(Pookas);
	}
	//Number of Fygar enemies
	for (int numFygar{ 0 }; numFygar < TotalFygar; numFygar++)
	{
		EnemyEntity *Fygar = new EnemyEntity("Fygar", FygarRect, 9);
		Fygar->setPosition(TileSet[FygarPos[numFygar]]->Coordinates);
		EntityVector.push_back(Fygar);
	}
	//Bullet - can only have 1 onscreen at any one time. This is because in real DigDug the 'bullet' is a pump which in this case is a singular bullet. 
	BulletEntity *Bullet = new BulletEntity("Bullet", BulletRect,4);
	EntityVector.push_back(Bullet);
	
	//Ensure Background is the last entity in the vector for potential (probably negligable) speedups
	//Other Entities need to perform actions when the Background doesn't, this might help in these cases!
	BackgroundEntity *Background = new BackgroundEntity("background", BackgroundRect);
	EntityVector.push_back(Background);
	PrintTiles(); 
	playArea = Rectangle(0, screenWidth, 3 * 15, int(Tiles_Y*(Tiles_Y*0.925))); // NOTE: CHANGE THIS TO WORK WITH ANY SCREEN RESOLUTION!!!

//Make sure Tiles are updated
	for (auto a : TileSet)
	{
		a->Update(TileSet);
	}
	//Avoid graphics glitches by checking if the background fills the screen, if it doesnt remember to clear the screen every loop
	if (BackgroundRect.Width() < screenWidth || BackgroundRect.Height() < screenHeight)
		clearscreen = true;

	return true;
}

void World::Update()
{	
	const int frameDuration = 17;
	const int maxTimePerFrame = 200;
	int accumulatedTime = 0;
	
		while (HAPI.Update() && !exit) //returns true until user closes window
		{
			accumulatedTime += getDeltaTime();
			if (accumulatedTime > maxTimePerFrame)
			{
				accumulatedTime = maxTimePerFrame;
			}
			//Ensure Fps doesnt change User experiance
			while (accumulatedTime >= frameDuration)
			{
				accumulatedTime -= frameDuration;
				numEnemies = 0;
				for (auto p : EntityVector)
				{
					if (p->GetSide() == Eside::eEnemy && p->alive)
						numEnemies++;
				}
				for (auto p : EntityVector)
				{
					p->Update(TileSet, numEnemies, EntityVector);
					p->edgecheck(screenWidth, screenHeight, playArea);
					p->Collision(EntityVector);
				}
			}
			for (auto a : TileSet)
			{
				a->Update(TileSet);
			}
			if (clearscreen)
				Graphic->ClearToColour(HAPI_TColour::BLACK);

			for (auto p : EntityVector)
			{
				if (p->GetSide() == Eside::eBackground)
					p->Render(*Graphic);
			}
			for (auto a : TileSet)
			{
				a->Update(TileSet);
				a->Tentity->Render(*Graphic);
			}

			for (auto p : EntityVector)
			{
				if (p->GetSide() != Eside::eBackground)
					p->Render(*Graphic);
			}

			for (int L{ 0 }; L < Lives; L++)
			{
				//Display Lives of player
				Graphic->DrawSprite("Player", 15 * L, screenHeight - 15, 1); 
			}
			for (auto p : EntityVector)
			{
				if (p->GetSide() == Eside::ePlayer)
				{
					HAPI.RenderText(screenWidth - 75, 0, HAPI_TColour::WHITE, "Score: " + std::to_string(p->getScore()));
					Lives = p->getLives();
					if (p->getQUIT())
					{
						std::cout << "QUIT" << std::endl;
						return;
					}
				}
			}
			if (numEnemies == 0)
			{
				Levelnum++;
				exit = true;		

			}
		
		}
	if (exit)
	{
		setlevelnotofile(Levelnum);
		return loadLevel(Player_Score);
	}
}
int World::getDeltaTime()
{	
	int accumTime = HAPI.GetTime() - lastTimeUpdated;
	lastTimeUpdated = HAPI.GetTime();
	return accumTime;
}

void World::PrintTiles()
{
	if (DEBUG)
	{
		for (int i{ 0 }; i < TileSet.size(); i++)
			std::cout << TileSet[i]->col << ", " << TileSet[i]->row << ", " << TileSet[i]->VectorPos << ", " << ((TileSet[i]->col * 14) + TileSet[i]->row) << std::endl;
	}
}