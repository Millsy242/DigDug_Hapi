/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Rectangle.h
*/
#pragma once
#include <algorithm>
#include <string>
#include <iostream>
class Rectangle
{
public:
	int Left{ 0 }, Right{ 0 }, Top{ 0 }, Base{ 0 };
	
	Rectangle() = default;
	~Rectangle() {};
	Rectangle(int left, int right, int top, int base) : Left(left), Right(right), Top(top), Base(base) {}
	Rectangle(int width, int height) : Left(0), Right(width), Top(0), Base(height){}

	void printRECT(std::string IdentifyingName = "NAME_NOT_GIVEN")
	{
		std::cout << "\n\\\\\\nPrinting Rectangle " << IdentifyingName << ": \nLeft: " << Left << "\nRight: " << Right << "\nTop: " << Top << "\nBase: " << Base << "\nWidth: " << Width() << "\nHeight: " << Height()<< std::endl;
	}

	//calculate and return width
	int Width() const
	{
		return Right - Left;
	}
	//calculate and return height
	int Height() const
	{
		return Base - Top; 
	}

	void ClipTo(const Rectangle &other)
	{
		if (Left < other.Left)
			Left = other.Left;
		if (Right > other.Right)
			Right = other.Right;
		if (Top < other.Top)
			Top = other.Top;
		if (Base > other.Base)
			Base = other.Base;		
	}
	void Translate(int dx, int dy)
	{
		Left += dx;
		Right += dx;
		Top += dy;
		Base += dy;
	}
	void setPos(float dx, float dy)
	{
		Right = int(dx + Width());
		Left = int(dx);
		Base = int(dy + Height());
		Top = int(dy);
	}
	void setPos(float left,float right,float top,float base)
	{
		Right = int(right);
		Left = int(left);
		Base = int(base);
		Top = int(top);
	}

	 bool contains(const Rectangle &other) const
	{
		if (Left<other.Left && Right > other.Right && Top < other.Top && Base > other.Base)
			return true;
		else
			return false;
	}
	 bool OutsideOf(const Rectangle &other) const //if this rect is outside of other
	{
		if (Left > other.Right) { return true; };
		if (Top > other.Base) { return true; };
		if (Right < other.Left) { return true; };
		if (Base < other.Top) { return true; };

		return false;
	}

	 bool Intersects(const Rectangle &other) const
	{
		if (Left < other.Left && Right > other.Left && Base > other.Top && Top < other.Top)
			return true;
		else
			return false;
	}	
};

