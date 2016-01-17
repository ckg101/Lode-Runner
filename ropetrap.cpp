#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include "ropetrap.h"

ROPETRAP::ROPETRAP(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
			: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isCollected = false;
	isSet = false;
}

ROPETRAP::~ROPETRAP()
{
}

void ROPETRAP::SetFrameStateToSet(void)
{
	frameState = 1;
}

void ROPETRAP::SetFrameStateToNotSet(void)
{
	frameState = 0;
}