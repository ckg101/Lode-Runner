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
	if(frameState > 10)
		frameState = 1;
	else if(frameState < 9)
		frameState++;
	else
		frameState = 1;
}

void PLAYER::backFrame(void)
{
	if(frameState < 11 || frameState > 21)
	{
		frameState = 11;
	}
	else if(frameState < 19)
	{
		frameState++;
	}
	else
		frameState = 11;
}

void PLAYER::climbDownFrame(void)
{
	if(frameState != 21)
	{
		frameState = 21;
	}
}

void PLAYER::climbUpFrame(void)
{
	if(frameState < 22 || frameState == 23)
	{
		frameState = 22;
	}
	else
		frameState++;
}

void PLAYER::fallingFrame(void)
{
	if((frameState < 24) || (frameState > 26))
		frameState = 24;
	else if(frameState < 26)
		frameState++;
	else
		frameState = 24;
}

bool PLAYER::enterLevel(void)
{
	if(frameState < 27 || frameState > 36)
	{
		frameState = 27;
		return true;
	}
	else if(frameState < 36)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
}

void PLAYER::setFrameState(unsigned int index)
{
	frameState = index;
}