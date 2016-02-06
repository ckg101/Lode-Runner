#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <stdio.h>
#include <math.h>
#include <comdef.h>
#include <ShObjIdl.h>
#include "monk.h"

MONK::MONK(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	goldCollected = 0;
}

MONK::~MONK()
{
}

void MONK::nextFrame(void)
{
	if(frameState > 6)
		frameState = 0;
	else if(frameState < 6)
		frameState++;
	else
		frameState = 0;
}

void MONK::backFrame(void)
{
	if(frameState < 7 || frameState > 13)
	{
		frameState = 7;
	}
	else if(frameState < 13)
	{
		frameState++;
	}
	else
		frameState = 7;
}

void MONK::backBarFrame(void)
{

}

void MONK::nextBarFrame(void)
{
}

void MONK::climbDownFrame(void)
{

}

void MONK::climbUpFrame(void)
{

}

void MONK::fallingFrame(void)
{
}

bool MONK::enterLevel(void)
{

}

void MONK::setFrameState(unsigned int index)
{
	frameState = index;
}