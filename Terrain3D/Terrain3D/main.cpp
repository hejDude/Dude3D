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

#include "main.h"


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

	// utworzenie okna
	//--------------------------------------------------------------------------------
	hWnd = CreateWindow(
		_T( "TerrainWndClass" ),	// identyfikator klasy okna
		_T( "Terrain 2D => Terrain 3D" ),	// wyswietli sie na pasku tytulu
		WS_SYSMENU | WS_MINIMIZEBOX,	// styl okna
		CW_USEDEFAULT, CW_USEDEFAULT,	// pozycja x,y
		800, 600,	// szerokosc i wysokosc
		NULL, NULL,
		hInstance,	// uchwyt instancji programu
		NULL );
	//--------------------------------------------------------------------------------

	// utworzenie D3DDevice (zwraca 0 w przypadku wystapienia bledu - zakonczenie dzialania aplikacji)
	if( !MakeD3DDevice() )
		goto WinMain_end;


	//--------------------------------------------------------------------------------
	position = D3DXVECTOR3( (ground.width/2 + 0.5), 0.5, (ground.length/2 + 0.5) );
	direction = D3DXVECTOR3(0,0,1);
	headDirection = D3DXVECTOR3(0,1,0);
	//--------------------------------------------------------------------------------


	// utworzenie i wypelnienie buforow wierzcholkow
	//--------------------------------------------------------------------------------
	if( !MakeVBuffors() )	// (zwraca 0 w przypadku wystapienia bledu - zakonczenie dzialania aplikacji)
		goto WinMain_end;
	
	FillVBuffors();
	//--------------------------------------------------------------------------------


	pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	//pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	D3DXCreateTextureFromFile( pD3DDevice, _T("ground1.jpg"), &pTexture );

	ZeroMemory(&keys, sizeof(char) * 256);
	//--------------------------------------------------------------------------------

	// wyswietlanie utworzonego okna
	ShowWindow( hWnd, SW_SHOWDEFAULT );	



	//	petla komunikatow i renderowania
	//--------------------------------------------------------------------------------
	MSG msg;
	for(;;)
	{
		if( !PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			Input();

			Render();
			continue;
		}
        
		if(msg.message == WM_QUIT)
			break;
 
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	//--------------------------------------------------------------------------------



	//	do tego bloku mozna wejsc tylko poprzez instrukcje goto
	//--------------------------------------------------------------------------------
	if(0)
	{
		WinMain_end:
		msg.wParam = 1;
	}
	//--------------------------------------------------------------------------------

	// czyszczenie pamieci
	ClearMemory();

	//	wyrejestrowanie klasy okna
	UnregisterClass( _T("TerrainWndClass"), hInstance );

	//	zwracamy wartosc komunikatu WM_QUIT
	return msg.wParam;
}

/************************************************************************************************************************************/







///////////////////////
/// Pozostale funkcje programu :
///////////////////////

/************************************************************************************************************************************/
// funkcja przetwarzajaca komunikaty okna
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );	// przesylamy komunikat konczacy petle programu
			return 0;
		case WM_KEYDOWN:
			keys[wParam]=1;
			return 0;
		case WM_KEYUP:
			keys[wParam]=0;
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );	// przetwarzanie komunikatow w sposob domyslny
}

/************************************************************************************************************************************/
// renderowanie sceny
void Render()
{
	position_real = 0.9*position_real + 0.1*position;
    direction_real = 0.9*direction_real + 0.1*direction;
    headDirection_real = 0.9*headDirection_real + 0.1*headDirection;

	// czyszczenie ekranu i bufora glebi
    pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
 
    hr = pD3DDevice->BeginScene();
    if( FAILED( hr ) )
        return;
 
    /*static int rotation;
	macierz swiata (transformuje wszystkie wierzcholki na scenie)
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
	D3DXMatrixRotationY( &matWorld, ++rotation / 200.0f );
    pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
	*/
 
    // macierz widoku (okresla pozycje i obrot kamery)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(
		&matView,
		&position_real,
		&(position_real+direction_real),
		&headDirection_real);
	pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	/*
    D3DXMATRIXA16 matView;
    D3DXVECTOR3 vEyePt( 0.0f, 1.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
	*/
 
    // macierz projekcji
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI / 1.9f,
		8.0/6.0,
		0.1f,
		100.0f );
	pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	/*
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(
        &matProj, D3DX_PI / 3.0f, 1.0f, 1.0f, 100.0f );
    pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	*/


	pD3DDevice->SetFVF( FVF_VERTEX );	// wybor uzywanej struktury wierzcholka


	pD3DDevice->SetTexture( 0, pTexture );		 // wybor tekstury
	pD3DDevice->SetStreamSource( 0, pVBtiles, 0, sizeof( Vertex ) );	// wybor bufora wierzcholkow
	//pD3DDevice->SetIndices( pIBsciany );
	//pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0U, num_v, 0U, num_sciany*2 );	// rysowanie kwadratow
	pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, num_tiles*2 );	// rysowanie "kafelek"


    pD3DDevice->EndScene();
    pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

/************************************************************************************************************************************/
// przetwarzanie klawiszy
void Input()
{
	if(keys['W'])
	{
        Move(direction);
        keys['W']=0;
	}
	if(keys['S'])
	{
        Move(-direction);
        keys['S']=0;
	}
	if(keys['A'])
	{
        D3DXVECTOR3 v;
        D3DXVec3Cross(&v, &direction, &headDirection);
        Move(v);
        keys['A']=0;
	}
	if(keys['D'])
	{
        D3DXVECTOR3 v;
        D3DXVec3Cross(&v, &headDirection, &direction);
        Move(v);
        keys['D']=0;
	}
	if(keys[VK_SPACE])
	{
        Move(headDirection);
        keys[VK_SPACE]=0;
	}
	if(keys[VK_CONTROL])
	{
        Move(-headDirection);
        keys[VK_CONTROL]=0;
	}
	if(keys[VK_LEFT])
	{
        D3DXMATRIX mat;
        D3DXMatrixRotationAxis(&mat, &headDirection, -D3DX_PI/2);
        D3DXVec3TransformCoord(&direction, &direction, &mat);
 
        keys[VK_LEFT]=0;
	}
	if(keys[VK_RIGHT])
	{
        D3DXMATRIX mat;
        D3DXMatrixRotationAxis(&mat, &headDirection, D3DX_PI/2);
        D3DXVec3TransformCoord(&direction, &direction, &mat);
 
        keys[VK_RIGHT]=0;
	}
	if(keys[VK_UP])
	{
        D3DXVECTOR3 v = direction;
        direction = -headDirection;
        headDirection = v;
 
        keys[VK_UP]=0;
	}
	if(keys[VK_DOWN])
	{
        D3DXVECTOR3 v = direction;
        direction = headDirection;
        headDirection = -v;
 
        keys[VK_DOWN]=0;
	}
	if(keys[VK_DELETE])
	{
        D3DXMATRIX mat;
        D3DXMatrixRotationAxis(&mat, &direction, D3DX_PI/2);
        D3DXVec3TransformCoord(&headDirection, &headDirection, &mat);
 
        keys[VK_DELETE]=0;
	}
	if(keys[VK_NEXT])
	{
        D3DXMATRIX mat;
        D3DXMatrixRotationAxis(&mat, &direction, -D3DX_PI/2);
        D3DXVec3TransformCoord(&headDirection, &headDirection, &mat);
 
        keys[VK_NEXT]=0;
	}
}

/************************************************************************************************************************************/
// poruszanie
void Move(D3DXVECTOR3& vec)
{
	if(vec.x> 0.9) position += vec;
    if(vec.x<-0.9) position += vec;
    if(vec.y> 0.9) position += vec;
    if(vec.y<-0.9) position += vec;
    if(vec.z> 0.9) position += vec;
    if(vec.z<-0.9) position += vec;
}

/************************************************************************************************************************************/
// utworzenie Direct3D Device (zwraca 0 w przypadku wystapienia bledu)
int MakeD3DDevice()
{
	pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(!pD3D) 
		return 0;
 
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
 
	hr = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&pD3DDevice );
	if(FAILED(hr)) 
		return 0;

	return 1;
}

/************************************************************************************************************************************/
// utworzenie buforow wierzcholkow i zablokowanie ich (zwraca 0 w przypadku wystapienia bledu)
int MakeVBuffors()
{
	num_tiles = ground.width * ground.length;		// ilosc "kafelek" = szerokosc * dlugosc (terenu)
	num_v = num_tiles * 3 * 2;			// ilosc "kafelek" * 3 * 2 (bo dwa trojkaty)
 
	hr = pD3DDevice->CreateVertexBuffer(
		num_v * sizeof( Vertex ),
		0,
		FVF_VERTEX,
		D3DPOOL_DEFAULT,
		&pVBtiles,
		NULL );
	if( FAILED( hr ) )
		return 0;

 
	// zablokowanie buforow
	hr = pVBtiles->Lock( 0, 0, ( void** )&pVertices, 0 );
	if( FAILED( hr ) ) 
		return 0;

	return 1;
}

/************************************************************************************************************************************/
// wypelnienie buforow wierzcholkow i odblokowanie ich
void FillVBuffors()
{
	int vi= 0;	// vertex index

	for(int x=0 ; x < ground.width ; x++)
		for(int z=0 ; z < ground.length ; z++)
		{
			pVertices[vi] = Vertex(x+0.0f, 0.0f, z+0.0f, 0.0f, 0.0f);
			pVertices[vi+1] = Vertex(x+1.0f, 0.0f, z+1.0f, 1.0f, 1.0f);
			pVertices[vi+2] = Vertex(x+1.0f, 0.0f, z+0.0f, 1.0f, 0.0f);

			pVertices[vi+3] = Vertex(x+0.0f, 0.0f, z+0.0f, 0.0f, 0.0f);
			pVertices[vi+4] = Vertex(x+0.0f, 0.0f, z+1.0f, 0.0f, 1.0f);
			pVertices[vi+5] = Vertex(x+1.0f, 0.0f, z+1.0f, 1.0f, 1.0f);

			vi += 6;
		}
	

	// odblokowanie buforow
	pVBtiles->Unlock();
}

/************************************************************************************************************************************
// utworzenie i wypelnienie bufora indeksow
void MakeIBuffor()
{
	UINT *pS;
	pD3DDevice->CreateIndexBuffer(
        num_sciany*2*3*sizeof( UINT ),
        D3DUSAGE_DONOTCLIP,
        D3DFMT_INDEX32,
        D3DPOOL_DEFAULT,
        &pIBsciany,
        NULL );
	pIBsciany->Lock( 0U, 0U, (void**)&pS, 0 );
 
	UINT *pP;
	pD3DDevice->CreateIndexBuffer(
        num_podlogi*2*3*sizeof( UINT ),
        D3DUSAGE_DONOTCLIP,
        D3DFMT_INDEX32,
        D3DPOOL_DEFAULT,
        &pIBpodlogi,
        NULL );
	pIBpodlogi->Lock( 0U, 0U, (void**)&pP, 0 );
        
	for(int z = 0; z < mapa_size[ 2 ]; ++z)
		for(int y = 0; y < mapa_size[ 1 ]; ++y)
			for(int x = 0; x < mapa_size[ 0 ]; ++x)
				for(int wymiar = 0; wymiar < 3; ++wymiar)
					for(int strona = 0; strona < 2; ++strona)
						if( !MAPA( x,y,z ).przejscie[ wymiar ][ strona ] )
						{
							if( wymiar == 1 )
							{
								*pP++ = V(x,y+strona,z);
								*pP++ = V(x+strona,y+strona,z+1-strona);
								*pP++ = V(x+1-strona,y+strona,z+strona);
 
								*pP++ = V(x+1,y+strona,z+1);
								*pP++ = V(x+1-strona,y+strona,z+strona);
								*pP++ = V(x+strona,y+strona,z+1-strona);
							}
							else if( wymiar == 0 )
							{
								 *pS++ = V(x+strona, y, z);
								 *pS++ = V(x+strona, y+1-strona, z+strona);
								 *pS++ = V(x+strona, y+strona, z+1-strona);
 
								 *pS++ = V(x+strona, y+1, z+1);
								 *pS++ = V(x+strona, y+strona, z+1-strona);
								 *pS++ = V(x+strona, y+1-strona, z+strona);
							}
							else    // wymiar == 2
							{
								*pS++ = V(x, y, z+strona);
								*pS++ = V(x+1-strona, y+strona, z+strona);
								*pS++ = V(x+strona, y+1-strona, z+strona);
 
								*pS++ = V(x+1, y+1, z+strona);
								*pS++ = V(x+strona, y+1-strona, z+strona);
								*pS++ = V(x+1-strona, y+strona, z+strona);
							}
						}

}

/************************************************************************************************************************************/
// czyszczenie pamieci
void ClearMemory()
{
	//	zwolnienie interfejsow Direct3D:
	//--------------------------------------------------------------------------------
	if( pTexture ) pTexture->Release();
	if( pVBtiles ) pVBtiles->Release();

	if( pD3DDevice ) pD3DDevice->Release();
    if( pD3D ) pD3D->Release();
	//--------------------------------------------------------------------------------
}

/************************************************************************************************************************************/








/************************************************************************************************************************************/
// Zmienne globalne :

HRESULT hr;
HWND hWnd;
//ifstream file;
char keys[256];

Terrain ground(100, 100);


IDirect3D9              *pD3D = 0;
IDirect3DDevice9        *pD3DDevice = 0;
IDirect3DVertexBuffer9	*pVBtiles = 0;
IDirect3DIndexBuffer9	*pIBtiles = 0;
IDirect3DTexture9       *pTexture = 0;

D3DXVECTOR3 position;
D3DXVECTOR3 direction;
D3DXVECTOR3 headDirection;
D3DXVECTOR3 position_real;
D3DXVECTOR3 direction_real;
D3DXVECTOR3 headDirection_real;

int num_tiles = 0;

int num_v = 0;
Vertex *pVertices;
/************************************************************************************************************************************/


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/