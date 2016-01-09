#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <math.h>
#include "sprite.h"
#include "platform.h"
#include "sound.h"
#include "gameplay.h"

GAMEPLAY::GAMEPLAY(IDirect3DDevice9* d, IXAudio2* xa, PLATFORM* p, HWND &hWnd, int screen_width, int screen_height)
{
	d3ddev = d;
	platform = p;
	player = NULL;
	musicFileName = NULL;
	isFalling = false;
	fallingSound = NULL;

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

	fallingSound = new SOUND(xa);
	fallingSound->loadWAVFile(L"Sound\\falling.wav");
	landingSound = new SOUND(xa);
	landingSound->loadWAVFile(L"Sound\\landing.wav");
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
	if(fallingSound)
		delete fallingSound;
	if(landingSound)
		delete landingSound;
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
	unsigned int res, blockNbr;
	int x, y;
	if(isFalling == false)
	{
		blockNbr = platform->getBlockNbr(player[0]->x_pos+25, player[0]->y_pos);
		// detect for hitting walls
		res = platform->GetType(blockNbr);
		if( res > BLOCK_HOLLOW && (player[0]->x_pos+24 < 767))
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[0]->x_pos+=4;
			player[0]->y_pos = y;
			player[0]->nextFrame();
		}
		else
			player[0]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
}

void GAMEPLAY::MovePlayer1Left(void)
{
	unsigned int res, blockNbr;
	int x, y;
	if(isFalling == false)
	{
		// detect for hitting walls
			blockNbr = platform->getBlockNbr(player[0]->x_pos-1, player[0]->y_pos);
			res = platform->GetType(blockNbr);
		if(res > BLOCK_HOLLOW && (player[0]->x_pos > 0))
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[0]->x_pos-=4;
			player[0]->y_pos = y;
			player[0]->backFrame();
		}
		else
			player[0]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
	
}

void GAMEPLAY::MovePlayer1Down(void)
{
	unsigned int res;
	unsigned int blockNbr;
	int x, y;
	blockNbr = platform->getBlockNbr(player[0]->x_pos+10, player[0]->y_pos+24);
	res = platform->GetType(blockNbr);
	if(res == BLOCK_LADDER || res == BLOCK_REGULAR_WITH_LADDER)
	{
		platform->GetBlockCoordinates(blockNbr, x, y);
		player[0]->x_pos = x;
		player[0]->y_pos+=4;
		player[0]->climbDownFrame();
	}
}

void GAMEPLAY::MovePlayer1Up(void)
{
	unsigned int res, res2;
	unsigned int blockNbr, prevBlockNbr;
	int x, y;

	blockNbr = platform->getBlockNbr(player[0]->x_pos, player[0]->y_pos);
	prevBlockNbr = platform->getBlockNbr(player[0]->x_pos, player[0]->y_pos+23);
	//currentBlockNbr = platform->getBlockNbr(player[0]->x_pos, player[0]->y_pos);
	res2 = platform->GetType(prevBlockNbr);
	res = platform->GetType(blockNbr);
	if(res == BLOCK_LADDER || res == BLOCK_REGULAR_WITH_LADDER && (player[0]->y_pos > 0))
	{
		//if(res2 == BLOCK_LADDER || res2 == BLOCK_REGULAR_WITH_LADDER || res2 == BLOCK_REGULAR)
		
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[0]->x_pos = x;
			if(player[0]->y_pos-4 < 0)
				player[0]->y_pos = 0;
			else
				player[0]->y_pos-=4;
			player[0]->climbUpFrame();
		
	}
	else if (res2 == BLOCK_LADDER || res2 == BLOCK_REGULAR_WITH_LADDER)
	{
		
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[0]->x_pos = x;
			if(player[0]->y_pos-2 < 0)
				player[0]->y_pos = 0;
			else
				player[0]->y_pos-=2;
			player[0]->climbUpFrame();
	}
	else
	{
		//platform->GetBlockCoordinates(blockNbr, x, y);
		//player[0]->x_pos = x;
		//player[0]->y_pos = y;
		player[0]->setFrameState(0);
	}

}

void GAMEPLAY::Gravity(void)
{
	unsigned int res, blockNbr;
	int x, y;
	blockNbr = platform->getBlockNbr(player[0]->x_pos+12, player[0]->y_pos+24);
	res = platform->GetType(blockNbr);
	//blockNbr2 = platform->getBlockNbr(player[0]->x_pos-10, player[0]->y_pos+24);
	//res2 = platform->GetType(blockNbr2);
	if(res == BLOCK_EMPTY && (player[0]->y_pos <= 743))
	{
		blockNbr = platform->getBlockNbr(player[0]->x_pos+12, player[0]->y_pos);
		platform->GetBlockCoordinates(blockNbr, x, y);
		player[0]->x_pos = x;
		if(player[0]->y_pos+5 > 745)
		{
			blockNbr = platform->getBlockNbr(player[0]->x_pos, 760);
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[0]->y_pos = y;
		}
		else
			player[0]->y_pos+=5;
		player[0]->fallingFrame();
		if(isFalling == false)
			fallingSound->startWAVFile();
		isFalling = true;
	}
	else
	{
		if(isFalling == true)
		{
			fallingSound->stopWAVFile();
			landingSound->startWAVFile();
			player[0]->setFrameState(0);
		}
		isFalling = false;
	}
}

void GAMEPLAY::Sounds(void)
{
	fallingSound->playWAVFile();
	landingSound->playWAVFile();
}