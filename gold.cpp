#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include "gold.h"

GOLD::GOLD(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height) 
: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isCollected = false;
	isHidden = false;
}

GOLD::~GOLD()
{
}

int GOLD::renderSprite(D3DLOCKED_RECT& locked_rect)
{
	if (isCollected == false && isHidden == false)
		SPRITE::renderSprite(locked_rect);
	return 1;
}