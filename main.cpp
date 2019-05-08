/*
// Daniel Havey S6291311
// Games Engine Construction : Year 2 : Term 1 : 2017/18
// Main.cpp
*/

#include "World.h"

//Memory leak detection
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

void HAPI_Main()
{	
	World abc(0);
	abc.Run();	
}
