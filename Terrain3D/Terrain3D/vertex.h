// Klasa vertex

#ifndef _vertex_h_
#define _vertex_h_

#include "stdafx.h"


/************************************************************************************************************************************/
///////////////////////
/// Struktura reprezentujaca wierzcholek :
///////////////////////

struct Vertex
{
	// konstruktor
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : x(x), y(y), z(z), u(u), v(v) {}

    FLOAT x, y, z;
	FLOAT u, v;
};


// opis struktury dla Direct3D (FVF - Flexible Vertex Format)
#define FVF_VERTEX ( D3DFVF_XYZ | D3DFVF_TEX1 )

/************************************************************************************************************************************/


/************************************************************************************************************************************/
///////////////////////
/// Zmienne globalne :
///////////////////////

// ilosc wierzcholkow
extern int num_v;

// wskaznik na tablice wierzcholkow
extern Vertex *pVertices;
extern Vertex *pVertices2;

/************************************************************************************************************************************/


#endif


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/