// Klasa main

#ifndef _main_h_
#define _main_h_

#include "stdafx.h"
#include "vertex.h"
#include "terrain.h"
#include "pixel.h"


/************************************************************************************************************************************/
///////////////////////
/// Funkcje :
///////////////////////


// funkcja przetwarzajaca komunikaty okna
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// renderowanie sceny
void Render();

// przetwarzanie klawiszy
void Input();
// poruszanie
void Move(D3DXVECTOR3& vec);

// utworzenie Direct3D Device (zwraca 0 w przypadku wystapienia bledu)
int MakeD3DDevice();
// utworzenie buforow wierzcholkow i zablokowanie ich (zwraca 0 w przypadku wystapienia bledu)
int MakeVBuffors();
// wypelnienie buforow wierzcholkow i odblokowanie ich
void FillVBuffors();
// utworzenie i wypelnienie bufora indeksow
//void MakeIBuffor();

// czyszczenie pamieci
void ClearMemory();

/************************************************************************************************************************************/



/************************************************************************************************************************************/
///////////////////////
/// Zmienne globalne :
///////////////////////


// typ kodujacy wynik wywolania funkcji (32 bity) - kod bledu (domyslnie E_FAIL), lub kod sukcesu (domyslnie S_OK)
extern HRESULT hr;

// okno WinAPI
extern HWND hWnd;

// strumien do odczytu danych z pliku
//extern ifstream file;

// tablica przechowujaca stan klawiszy (wcisniety lub nie)
extern char keys[256];

// obiekt ground klasy Terrain reprezentujacy powierzchnie
extern Terrain ground;


// wskazniki do interfejsow DirectX
//----------------------------------------------------------------------
extern IDirect3D9              *pD3D;
extern IDirect3DDevice9        *pD3DDevice;

extern IDirect3DVertexBuffer9	*pVBtiles;
//extern IDirect3DIndexBuffer9	*pIBtiles;
extern IDirect3DTexture9		*pTexture;
//----------------------------------------------------------------------

// wektory DirectX
//----------------------------------------------------------------------
extern D3DXVECTOR3 position;
extern D3DXVECTOR3 direction;
extern D3DXVECTOR3 headDirection;

extern D3DXVECTOR3 position_real;
extern D3DXVECTOR3 direction_real;
extern D3DXVECTOR3 headDirection_real;
//----------------------------------------------------------------------

/************************************************************************************************************************************/


#endif


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/