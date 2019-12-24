#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include "exitdoor.h"

EXITDOOR::EXITDOOR(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
			: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isUnlocked = false;
	beingUsed = false;
}

EXITDOOR::~EXITDOOR()
{
}

bool EXITDOOR::Frame(void)
{
	if(frameState < 12)
	{
		frameState++;
		return true;
	}
	else if(frameState < 15)
	{
		frameState++;
		return true;
	}
	else if(frameState == 15)
	{
		frameState = 12;
		return true;
	}
	return false;
}

bool EXITDOOR::ExitingFrame(void)
{
	if(frameState < 16)
	{
		frameState = 16;
		return true;
	}
	else if(frameState < 22)
	{
		frameState++;
		return true;
	}
	else if(frameState == 22)
	{
		frameState = 23;
		return false;
	}
}