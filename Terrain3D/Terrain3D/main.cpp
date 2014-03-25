/*-------------------------------------------------------------------------------------------
						---->>>> Terrain 2D => 3D <<<<----

Projekt zaliczeniowy z przedmiotu MO3D

Wydzia³ AEI, Informatyka SSM, Sem. 3


Autorzy projektu :
1) Dudek Piotr
2) Sakwerda Bart³omiej
3) Widenka Grzegorz
4) Wra¿eñ Micha³
-------------------------------------------------------------------------------------------*/

#include "stdafx.h"

// funkcja przetwarzajaca komunikaty okna
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// renderowanie sceny
void Render();
// wczytanie terenu2D z pliku
void LoadTerrain();


// wskazniki do interfejsow DirectX
IDirect3D9              *pD3D = 0;
IDirect3DDevice9        *pD3DDevice = 0;
IDirect3DVertexBuffer9  *pVB = 0;
IDirect3DTexture9       *pTexture = 0;

// typ kodujacy wynik wywolania funkcji (32 bity) - kod bledu (domyslnie E_FAIL), lub kod sukcesu (domyslnie S_OK)
HRESULT hr;


// struktura wierzcholka
struct Vertex
{
    FLOAT x, y, z;
	FLOAT u, v;
};
#define FVF_VERTEX ( D3DFVF_XYZ | D3DFVF_TEX1 )

int num_v = 4;



// Glowna funkcja programu :
/************************************************************************************************************************************/
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{

	// zdefiniowanie i rejestracja klasy okna
	//--------------------------------------------------------------------------------
	WNDCLASSEX wc;		// struktura klasy okna
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );	// zerowanie pól struktury
	wc.cbSize = sizeof( WNDCLASSEX );	// rozmiar struktury
	wc.style = CS_CLASSDC;	// styl klasy okna
	wc.lpfnWndProc = WndProc;	// wsk do f. przetwarzajacej komunikaty okna
	wc.hInstance = hInstance;	// uchwyt instancji programu
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );	// uchwyt do kursora
	wc.lpszClassName = _T( "TerrainWndClass" );	// identyfikator klasy okna
	RegisterClassEx( &wc );		// rejestracja klasy okna
	//--------------------------------------------------------------------------------

	// utworzenie i wyswietlenie okna, utworzenie D3DDevice oraz utworzenie bufora wierzcholkow
	//--------------------------------------------------------------------------------
	HWND hWnd = CreateWindow(
		_T( "TerrainWndClass" ),	// identyfikator klasy okna
		_T( "Terrain 2D => Terrain 3D" ),	// wyswietli sie na pasku tytulu
		WS_SYSMENU | WS_MINIMIZEBOX,	// styl okna
		CW_USEDEFAULT, CW_USEDEFAULT,	// pozycja x,y
		800, 600,	// szerokosc i wysokosc
		NULL, NULL,
		hInstance,	// uchwyt instancji programu
		NULL );

	// utworzenie D3DDevice
	//--------------------------------------------------------------------------------
	pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(!pD3D) goto WinMain_end;
 
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
 
	HRESULT hr;
	hr = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&pD3DDevice );
	if(FAILED(hr)) goto WinMain_end;
	//--------------------------------------------------------------------------------

	// utworzenie bufora wierzcholkow
	//--------------------------------------------------------------------------------
	hr = pD3DDevice->CreateVertexBuffer(
		num_v * sizeof( Vertex ),
		0,
		FVF_VERTEX,
		D3DPOOL_DEFAULT,
		&pVB,
		NULL );
	if( FAILED( hr ) ) goto WinMain_end;
 
	Vertex* pVertices;
	hr = pVB->Lock( 0, 0, ( void** )&pVertices, 0 );
	if( FAILED( hr ) ) goto WinMain_end;
 
	pVertices[0].x = -1.0f;
	pVertices[0].y = 0.0f;
	pVertices[0].z = 0.0f;
	pVertices[0].u = 0.0f;
	pVertices[0].v = 0.0f;
 
	pVertices[1].x = 1.0f;
	pVertices[1].y = 0.0f;
	pVertices[1].z = 0.0f;
	pVertices[1].u = 1.0f;
	pVertices[1].v = 0.0f;
 
	pVertices[2].x = 0.0f;
	pVertices[2].y = 1.73f;
	pVertices[2].z = 0.0f;
	pVertices[2].u = 0.5f;
	pVertices[2].v = 1.0f;
 
	pVB->Unlock();
	//--------------------------------------------------------------------------------

	pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );


	LoadTerrain();	// wczytanie terenu2D z pliku

	ShowWindow( hWnd, SW_SHOWDEFAULT );	// wyswietlanie utworzonego okna
	//--------------------------------------------------------------------------------


	//	petla komunikatow i renderowania
	//--------------------------------------------------------------------------------
	MSG msg;
	for(;;)
	{
		if( !PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			Render();
			continue;
		}
        
		if(msg.message == WM_QUIT)
			break;
 
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	//--------------------------------------------------------------------------------


	//	do tego bloku mozna wejsc tylko poprzez instrukcje goto:
	if(0)
	{
		WinMain_end:
		msg.wParam = 1;
	}


	//	zwolnienie interfejsow Direct3D:
	if( pTexture ) pTexture->Release();
	if( pVB ) pVB->Release();
	if( pD3DDevice ) pD3DDevice->Release();
    if( pD3D ) pD3D->Release();

	//	wyrejestrowanie klasy okna
	UnregisterClass( _T("TerrainWndClass"), hInstance );

	//	zwracamy wartosc komunikatu WM_QUIT
	return msg.wParam;
}
/************************************************************************************************************************************/


// Pozostale funkcje programu
/************************************************************************************************************************************/
// funkcja przetwarzajaca komunikaty okna
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );	// przesylamy komunikat konczacy petle programu
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );	// przetwarzanie komunikatow w sposob domyslny
}

/************************************************************************************************************************************/
// renderowanie sceny
void Render()
{
	// czyszczenie ekranu i bufora glebi:
    pD3DDevice->Clear(
        0,
        NULL,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB( 50, 40, 30 ),
        1.0f,
        0 );
 
    hr = pD3DDevice->BeginScene();
    if( FAILED( hr ) )
        return;
 
    static int rotation;
 
    // macierz swiata (transformuje wszystkie wierzcholki na scenie)
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
	D3DXMatrixRotationY( &matWorld, ++rotation / 200.0f );
    pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
 
    // macierz widoku (okresla pozycje i obrot kamery)
    D3DXMATRIXA16 matView;
    D3DXVECTOR3 vEyePt( 0.0f, 1.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
 
    // macierz projekcji:
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(
        &matProj, D3DX_PI / 3.0f, 1.0f, 1.0f, 100.0f );
    pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
 
    // wybor tekstury:
    pD3DDevice->SetTexture( 0, pTexture );
 
    // wybor bufora wierzcholkow:
    pD3DDevice->SetStreamSource( 0, pVB, 0, sizeof( Vertex ) );
 
    // wybor uzywanej struktury wierzcholka:
    pD3DDevice->SetFVF( FVF_VERTEX );
 
    // rysowanie kwadratow:
    pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, num_v/3 );
 
    pD3DDevice->EndScene();
    pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

/************************************************************************************************************************************/
// wczytanie terenu2D z pliku
void LoadTerrain()
{
	D3DXCreateTextureFromFile(
    pD3DDevice, _T("Terrain1.bmp"), &pTexture );
}

/************************************************************************************************************************************/


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/