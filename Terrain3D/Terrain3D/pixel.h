// Klasa Pixel

#ifndef _pixel_h_
#define _pixel_h_

#include "stdafx.h"


/************************************************************************************************************************************/
///////////////////////
/// Klasa przechowywujaca informacje o pojedynczych pikselach na odczytywanej mapie :
///////////////////////

class Pixel
{
public :
	int	x, y;		// polozenie piksela
	int colorLevel;	// poziom jasnosci koloru (jakiejs tam skladowej)

	Pixel(int x, int y, int level) : x(x), y(y), colorLevel(level) {}
};

/************************************************************************************************************************************/


#endif


/*-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/