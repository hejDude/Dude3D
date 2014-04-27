// Klasa Terrain

#ifndef _terrain_h_
#define _terrain_h_

#include "stdafx.h"


/************************************************************************************************************************************/
///////////////////////
/// Klasa reprezentujaca 3-wymiarowy teren wyswietlany w naszej symulacji :
///////////////////////

class Terrain
{
public :
	int width;
	int length;

	Terrain(int x, int z) : width(x), length (z) {}
};


// ilosc "kafelek", z ktorych sie sklada teren
extern int num_tiles;


// wczytanie terenu2D z pliku
//void LoadTerrain();
/************************************************************************************************************************************/


#endif


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/