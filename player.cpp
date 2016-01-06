#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <stdio.h>
#include <math.h>
#include <comdef.h>
#include <ShObjIdl.h>
#include "player.h"

PLAYER::PLAYER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	//::SPRITE(d, nbr_of_frames, screen_width, screen_height);
}

PLAYER::~PLAYER()
{
}

void PLAYER::nextFrame(void)
{
	if(frameState > 8)
		frameState = 0;
	else if(frameState < 8)
		frameState++;
	else
		frameState = 0;
}

void PLAYER::backFrame(void)
{
	if(frameState < 9)
	{
		frameState = 9;
	}
	else if(frameState < 17)
	{
		frameState++;
	}
	else
		frameState = 9;
}

void PLAYER::downFrame(void)
{
	if(frameState != 18)
	{
		frameState = 18;
	}
}

void PLAYER::upFrame(void)
{
	if(frameState < 19 || frameState == 20)
	{
		frameState = 19;
	}
	else
		frameState++;
}

void PLAYER::fallingFrame(void)
{
}