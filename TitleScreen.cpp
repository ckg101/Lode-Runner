#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include "sprite.h"
#include "TitleScreen.h"

TITLESCREEN::TITLESCREEN(IDirect3DDevice9* d, int screen_width, int screen_height)
{
	titleScreenImage = NULL;
	titleScreenImage = new SPRITE(d, 1, screen_width, screen_height);
	titleScreenImage->loadBitmaps(L"Graphics\\TitleScreen");
}

TITLESCREEN::~TITLESCREEN(void)
{
	if(titleScreenImage)
		delete titleScreenImage;
}

// Copy image to backbuffer to be displayed
void TITLESCREEN::Render(IDirect3DSurface9* &backbuffer)
{
	titleScreenImage->renderSprite(backbuffer);
}
