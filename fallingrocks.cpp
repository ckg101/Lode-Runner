#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include "fallingrocks.h"

FALLINGROCKS::FALLINGROCKS(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height) 
		: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isOpen = true;
	fell = false;
	timer = 0;
}

FALLINGROCKS::~FALLINGROCKS(void)
{
}

bool FALLINGROCKS::frame(void)
{
	if(frameState <= 2)
	{
		frameState++;
		return true;
	}
	else
		frameState = 3;
	return false;
}

void FALLINGROCKS::reset(void)
{
	frameState = 0;
}