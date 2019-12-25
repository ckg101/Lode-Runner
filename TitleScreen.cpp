#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include "sprite.h"
#include "TitleScreen.h"

TITLESCREEN::TITLESCREEN(IDirect3DDevice9* d, int screen_width, int screen_height)
{
	titleScreenImage = NULL;
	titleScreenImage = new SPRITE(d, 1, screen_width, screen_height);
	titleScreenImage->loadBitmaps(_wcsdup(L"Graphics\\TitleScreen"));
	buttonCoordinate = NULL;
}

TITLESCREEN::~TITLESCREEN(void)
{
	if(titleScreenImage)
		delete titleScreenImage;
	if(buttonCoordinate)
		free(buttonCoordinate);
}

// Copy image to backbuffer to be displayed
void TITLESCREEN::Render(D3DLOCKED_RECT &backbuffer)
{
	titleScreenImage->renderSprite(backbuffer);
}

int TITLESCREEN::initialize(void)
{
	// initial coordinates of the first block
	int x = 0;
	int y = 0;
	int counter = 0;
	nbrOfButtons = 6;

	buttonCoordinate = (POINT*)malloc(sizeof(POINT) * nbrOfButtons);
	if(buttonCoordinate == NULL)
		return 0;
	
		// set each block as false to represent an empty block
	//isOccupied = (unsigned char*)malloc(sizeof(unsigned char) * nbr_of_blocks);
	//if(isOccupied == NULL)
		//return -1;
	//memset((unsigned char*)isOccupied, (unsigned char)IS_NOT_OCCUPIED, sizeof(unsigned char) * nbr_of_blocks);

	
	// set the coordinates of the blocks
	buttonCoordinate[TITLESCREEN_1PLAYER_BUTTON].x = 78;
	buttonCoordinate[TITLESCREEN_1PLAYER_BUTTON].y = 579;
	buttonCoordinate[TITLESCREEN_2PLAYER_BUTTON].x = 221;
	buttonCoordinate[TITLESCREEN_2PLAYER_BUTTON].y = 579; 
	buttonCoordinate[TITLESCREEN_EDITOR_BUTTON].x = 361;
	buttonCoordinate[TITLESCREEN_EDITOR_BUTTON].y = 579;
	buttonCoordinate[TITLESCREEN_LOAD_BUTTON].x = 497;
	buttonCoordinate[TITLESCREEN_LOAD_BUTTON].y = 579;
	buttonCoordinate[TITLESCREEN_OPTIONS_BUTTON].x = 636;
	buttonCoordinate[TITLESCREEN_OPTIONS_BUTTON].y = 579;
	buttonCoordinate[TITLESCREEN_EXIT_BUTTON].x = 782;
	buttonCoordinate[TITLESCREEN_EXIT_BUTTON].y = 579;

	return 1;
}

void TITLESCREEN::GetButtonCoordinates(unsigned int buttonNbr, int &x, int &y)
{
	if(buttonNbr >= 0 && buttonNbr < nbrOfButtons)
	{
		x = buttonCoordinate[buttonNbr].x;
		y = buttonCoordinate[buttonNbr].y;
	}
}

unsigned int TITLESCREEN::GetButtonNbr(int x, int y)
{
	for(int index = 0; index < nbrOfButtons; index++)
	{
		if(x >= buttonCoordinate[index].x && x <= buttonCoordinate[index].x+95)
			if(y >= buttonCoordinate[index].y && y <= buttonCoordinate[index].y+95)
				return index;
	}
	return 10;
}