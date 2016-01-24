#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include "goo.h"

GOO::GOO(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height)
			: SPRITE(d, nbr_of_frames, screen_width, screen_height)
{
	isCollected = false;
	isUsed = false;
}

GOO::~GOO()
{
}