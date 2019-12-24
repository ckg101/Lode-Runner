#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include "DIGGER.h"

DIGGER::DIGGER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height) : SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
}

DIGGER::~DIGGER(void)
{
}

bool DIGGER::digRightFrame(void)
{
	if(frameState == 6 || frameState == 13)
	{
		frameState = 0;
		return true;
	}
	else if(frameState < 5)
	{
		frameState++;
		return true;
	}
	else if(frameState == 5)
	{
		frameState=6;
		return false;
	}
	return false;
}

bool DIGGER::digLeftFrame(void)
{
	if(frameState == 13 || frameState == 0)
	{
		frameState = 7;
		return true;
	}
	else if(frameState < 12)
	{
		frameState++;
		return true;
	}
	else if(frameState == 12)
	{
		frameState=13;
		return false;
	}
	return false;
}