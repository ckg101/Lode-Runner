#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include <stdio.h>
#include <math.h>
#include <comdef.h>
#include <ShObjIdl.h>
#include "player.h"

PLAYER::PLAYER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	//::SPRITE(d, nbr_of_frames, screen_width, screen_height);
	goldCollected = 0;
	itemHeld = 0;
	score = 0;
	energy = 0;
	energyStart = 0;
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

void PLAYER::backBarFrame(void)
{
	if(frameState < 77 || frameState > 80)
	{
		frameState = 77;
	}
	else if(frameState < 80)
	{
		frameState++;
	}
	else
		frameState = 77;
}

void PLAYER::nextBarFrame(void)
{
	if(frameState < 73 || frameState > 76)
	{
		frameState = 73;
	}
	else if(frameState < 76)
	{
		frameState++;
	}
	else
		frameState = 73;
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

bool PLAYER::drillFrame(void)
{
	if((frameState <37) || (frameState > 46))
	{
		frameState = 37;
		return true;
	}
	else if(frameState < 46)
	{
		frameState++;
		return true;
	}
	else
		frameState = 37;
	return false;
}

bool PLAYER::pickRightFrame(void)
{
	if((frameState < 47) || (frameState > 59))
	{
		frameState = 47;
		return true;
	}
	else if(frameState < 59)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
}

bool PLAYER::pickLeftFrame(void)
{
	if((frameState < 60) || (frameState > 72))
	{
		frameState = 60;
		return true;
	}
	else if(frameState < 72)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
}

bool PLAYER::ropeFrame(void)
{
	if((frameState < 83) || (frameState > 101))
	{
		frameState = 83;
		return true;
	}
	else if(frameState < 101)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
}

bool PLAYER::gooRightFrame(void)
{
	if(frameState < 102 || frameState > 109)
	{
		frameState = 102;
		return true;
	}
	else if(frameState < 109)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
}

bool PLAYER::gooLeftFrame(void)
{
	if(frameState < 110 || frameState > 117)
	{
		frameState = 110;
		return true;
	}
	else if(frameState < 117)
	{
		frameState++;
		return true;
	}
	else
		frameState = 0;
	return false;
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