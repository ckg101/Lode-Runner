#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <math.h>

#include "cursor.h"

///////////////////////////////////////////////////////////////////////////////
// CURSOR CLASS
//////////////////////////////////////////////////////////////////////////////

CURSOR::CURSOR(IDirect3DDevice9* &d, unsigned int type_nbr, int screen_width, int screen_height)
{
	cursorGraphic = NULL;
	selectionGraphic = NULL;
	cursorGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	cursorGraphic->loadBitmaps(_wcsdup(L"Graphics\\block33_"));
	cursorGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	cursorGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	selectionGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	selectionGraphic->loadBitmaps(_wcsdup(L"Graphics\\block34_"));
	selectionGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	selectionGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	typeNbr = 0;
	blockCursor = 0;
	screenWidth = screen_width;
	screenHeight = screen_height;
	displaySelectionGraphic = true;
}

CURSOR::CURSOR(IDirect3DDevice9* &d, int screen_width, int screen_height)
{
	cursorGraphic = NULL;
	selectionGraphic = NULL;
	cursorGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	cursorGraphic->loadBitmaps(_wcsdup(L"Graphics\\block33_"));
	cursorGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	cursorGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	selectionGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	selectionGraphic->loadBitmaps(_wcsdup(L"Graphics\\TitleSelection"));
	selectionGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	selectionGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	typeNbr = 0;
	blockCursor = 0;
	screenWidth = screen_width;
	screenHeight = screen_height;
	displaySelectionGraphic = false;
}

CURSOR::~CURSOR()
{
	if(cursorGraphic)
		delete cursorGraphic;
	if(selectionGraphic)
		delete selectionGraphic;
}

int CURSOR::SetType(unsigned int type_nbr)
{
	typeNbr = type_nbr;
	return 1;
}

unsigned int CURSOR::GetType(void)
{
	return typeNbr;
}

void CURSOR::Render(D3DLOCKED_RECT &backbuffer)
{
	if(displaySelectionGraphic == true)
		selectionGraphic->renderSprite(backbuffer);
	cursorGraphic->renderSprite(backbuffer);
}

void CURSOR::SetSelectionX_Pos(int x)
{
	selectionGraphic->x_pos = x;
}

void CURSOR::SetSelectionY_Pos(int y)
{
	selectionGraphic->y_pos = y;
}

void CURSOR::MoveCursorX(int qty)
{
	if(cursorGraphic->x_pos+qty < screenWidth-24 && cursorGraphic->x_pos+qty >= 0)
		cursorGraphic->x_pos+=qty;
}

void CURSOR::MoveCursorY(int qty)
{
	if(cursorGraphic->y_pos+qty < screenHeight-23 && cursorGraphic->y_pos+qty >=0)
		cursorGraphic->y_pos+=qty;
}

POINT CURSOR::GetCursorPosition(void)
{
	POINT p;
	p.x = cursorGraphic->x_pos;
	p.y = cursorGraphic->y_pos;
	return p;
}

void CURSOR::SetCursorPosition(int x, int y)
{
	cursorGraphic->x_pos = x;
	cursorGraphic->y_pos = y;
}

int CURSOR::GetBlockCursor(void)
{
	return blockCursor;
}

int CURSOR::SetBlockCursor(int c)
{
	if(blockCursor < 1024 && blockCursor >= 0)
	{
		blockCursor = c; 
		return 1;
	}
	else if(blockCursor >= 1024 && blockCursor < 1024+32)
	{
		blockCursor = c;
		return 2;
	}
	else if(blockCursor == 1024+BLOCK_SAVE_BUTTON)
	{
		blockCursor = c;
		return 3;
	}
	else if(blockCursor == 1024+BLOCK_LOAD_BUTTON)
	{
		blockCursor = c;
		return 4;
	}
	return 0;
}

int CURSOR::SetButtonCursor(int c)
{
	if(c < 6 && c >= 0)
	{
		displaySelectionGraphic = true;
		blockCursor = c;
		return 1;
	}
	displaySelectionGraphic = false;
	return 0;
}