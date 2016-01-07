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
#include "platform.h"
#include "sound.h"
#include "gameplay.h"

GAMEPLAY::GAMEPLAY(IDirect3DDevice9* d, PLATFORM* p, HWND &hWnd, int screen_width, int screen_height)
{
	d3ddev = d;
	platform = p;
	player = NULL;
	musicFileName = NULL;

	player = (PLAYER**) malloc(sizeof(PLAYER*) * 2);
	for(unsigned int index = 0; index < 2; index++)
	{
		player[index] = new PLAYER(d, 27, screen_width, screen_height);
	}

	player[0]->loadBitmaps(L"Graphics\\block29_");
	player[0]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	player[0]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	//player[1]->loadBitmaps(L"Graphics\\block30_");
	//player[1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	//player[1]->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	musicFileName = (wchar_t**)malloc(sizeof(wchar_t*) * 10);
	
	for(unsigned int index = 0; index < 10; index++)
	{
		musicFileName[index] = (wchar_t*)malloc(sizeof(wchar_t) * 256);
		wsprintf(musicFileName[index], L"Sound\\world%d.MID", index+1);
	}
}

GAMEPLAY::~GAMEPLAY(void)
{
	if(player)
	{
		for(unsigned int index = 0; index < 2; index++)
			delete player[index];
	}
	free(player);
	if(musicFileName)
	{
		for(unsigned int index = 0; index < 10; index++)
		{
			free(musicFileName[index]);
		}
		free(musicFileName);
	}
}

void GAMEPLAY::Render(IDirect3DSurface9* &buf)
{
	platform->renderPlatform(buf);
	player[0]->renderSprite(buf);
}

int GAMEPLAY::LoadLevel(unsigned int levelNbr)
{
	platform->LoadLevel(L"level1.lvl");
	POINT p;
	p = platform->GetStartingCoordinatesOfPlayer(0);
	player[0]->x_pos = p.x;
	player[0]->y_pos = p.y;
	platform->SetIsPlaying(true);
	return 1;
}

int GAMEPLAY::LoadLevel(void)
{
	POINT p;
	platform->LoadLevel();
	p = platform->GetStartingCoordinatesOfPlayer(0);
	player[0]->x_pos = p.x;
	player[0]->y_pos = p.y;
	platform->SetIsPlaying(true);
	return 1;
}

void GAMEPLAY::Exit(void)
{
	platform->SetIsPlaying(false);
}

wchar_t* GAMEPLAY::GetMusicFileName(void)
{
	return musicFileName[platform->GetWorldNbr()];
}

void GAMEPLAY::MovePlayer1Right(void)
{
	unsigned int res;
	// detect for hitting walls
	res = platform->GetType(platform->getBlockNbr(player[0]->x_pos+15, player[0]->y_pos));
	if( res > BLOCK_HOLLOW)
	{
		player[0]->x_pos+=3;
		player[0]->nextFrame();
	}
	else
		player[0]->setFrameState(0);	// if hit a wall set the frame to the idle
}

void GAMEPLAY::MovePlayer1Left(void)
{
	unsigned int res;
	// detect for hitting walls
	res = platform->GetType(platform->getBlockNbr(player[0]->x_pos-3, player[0]->y_pos));
	if(res > BLOCK_HOLLOW)
	 {
		player[0]->x_pos-=3;
		player[0]->backFrame();
	}
	else
		player[0]->setFrameState(0);	// if hit a wall set the frame to the idle
	
}

void GAMEPLAY::MovePlayer1Down(void)
{
	unsigned int res;
	unsigned int blockNbr;
	int x, y;
	blockNbr = platform->getBlockNbr(player[0]->x_pos+10, player[0]->y_pos+24);
	res = platform->GetType(blockNbr);
	if(res == BLOCK_LADDER)
	{
		platform->GetBlockCoordinates(blockNbr, x, y);
		player[0]->x_pos = x;
		player[0]->y_pos+=3;
		player[0]->climbDownFrame();
	}
}

void GAMEPLAY::MovePlayer1Up(void)
{
	unsigned int res;
	unsigned int blockNbr;
	int x, y;
	blockNbr = platform->getBlockNbr(player[0]->x_pos+10, player[0]->y_pos);
	res = platform->GetType(blockNbr);
	if(res == BLOCK_LADDER)
	{
		platform->GetBlockCoordinates(blockNbr, x, y);
		player[0]->x_pos = x;
		player[0]->y_pos-=3;
		player[0]->climbUpFrame();
	}
}
