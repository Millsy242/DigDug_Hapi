
/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Tiles.h
*/
/*

In order to find position in vector:

(col * 14) + row

*/
#include "Vector2.h"
#include "TileEntity.h"
#include <stdlib.h>   
#include <time.h>

class Graphics;

struct Tile
{
	Tile(int rowpos, int colpos, int Vectorpos,TileEntity *T_Entity, Vector2 xy) : Tentity(T_Entity), Coordinates(xy),row(rowpos),col(colpos),VectorPos(Vectorpos){}
	
	~Tile() 
	{ 		
		delete Tentity;		
	}
	void Update(std::vector<Tile*> TileSet)
	{
		Tentity->setPosition(Coordinates);
		Tentity->setRectPos(); 
		//Set N/E/S/W values 
		if (col > 0)
			West = TileSet[VectorPos - 1];
		else
			West = nullptr;
		if (col < 13)
			East = TileSet[VectorPos + 1];
		else
			East = nullptr;
		if (row > 2)
			North = TileSet[VectorPos - 14];
		else
			North = nullptr;
		if (row < 16)
			South = TileSet[VectorPos + 14];
		else
			South = nullptr;
	}
	void Render(Graphics &graphic)
	{		
		if (Tentity->active)
			Tentity->Render(graphic);
	}
	bool getActive()
	{
		if (Tentity->active)
			return false;
		else
			return true;
	}
	bool passable()
	{
		if (Tentity->alive)
			return false;
		else
			return true;
	}
	void setpassable(bool pass)
	{
		//Can an Enemy move through the Tile?
		Tentity->alive = pass; 
	}
	void setInactive(bool trueorfalse = false)
	{
		Tentity->active = trueorfalse;
		setpassable(trueorfalse);
	}
	int Score()
	{
		//Get a random int to add to players score when s/he passes though
		return rand() % 3 + 2; 
	}
	
	//Store which tile is North/East/South/West of this one
	Tile *North = nullptr;
	Tile *East = nullptr;
	Tile *South = nullptr;
	Tile *West = nullptr;	
	 
	Vector2 Coordinates; //Position of Tile
	int col{ 0 }; //Which Column is the Tile in?
	int row{ 0 }; //Which Row is the TIle in?
	int VectorPos{ 0 }; //Where abouts in the TileSet Vector is the Tile in?
	
	TileEntity *Tentity = nullptr;	//Pointer to the TileEntity ontop of the Tile
};