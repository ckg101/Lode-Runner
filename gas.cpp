#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include "gas.h"

GAS::GAS(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
			: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isCollected = false;
}

GAS::~GAS()
{
}

GAS_SPRAY::GAS_SPRAY(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
			: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
}

bool GAS_SPRAY::FrameRight(void)
{
	if(frameState < 9)
	{
		frameState++;
		return true;
	}
	if(frameState == 9)
	{
		frameState = 5;
		return true;
	}
	return false;
}

bool GAS_SPRAY::FrameLeft(void)
{
	if(frameState < 10)
	{
		frameState = 10;
		return true;
	}
	else if(frameState < 18)
	{
		frameState++;
		return true;
	}
	else if(frameState == 18)
	{
		frameState = 14;
		return true;
	}
	return false;
}

void GAS_SPRAY::Reset(void)
{
	frameState = 0;
}