#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include <math.h>
#include "sprite.h"
#include "gameplay.h"

GAMEPLAY::GAMEPLAY(IDirect3DDevice9* d, PLATFORM* p)
{
	d3ddev = d;
	platform = p;
	player1 = NULL;
}

GAMEPLAY::~GAMEPLAY(void)
{
}

int GAMEPLAY::LoadLevel(unsigned int levelNbr)
{
	platform->
}
