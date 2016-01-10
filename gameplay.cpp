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
#include "digger.h"
#include "gameplay.h"

GAMEPLAY::GAMEPLAY(IDirect3DDevice9* d, IXAudio2* xa, PLATFORM* p, HWND &hWnd, int screen_width, int screen_height)
{
	d3ddev = d;
	platform = p;
	player = NULL;
	digger = NULL;
	musicFileName = NULL;
	soundFileName = NULL;
	isFalling = false;
	//fallingSound = NULL;
	isEnteringLevel = false;
	soundEffect = NULL;
	isEnteringLevelSound = false;
	isDiggingRight = false;
	isDiggingLeft = false;	
	nbrOfGold = 0;
	gold = NULL;

	player = (PLAYER**) malloc(sizeof(PLAYER*) * 2);
	for(unsigned int index = 0; index < 2; index++)
	{
		player[index] = new PLAYER(d, 37, screen_width, screen_height);
	}

	player[PLAYER1]->loadBitmaps(L"Graphics\\block29_");
	player[PLAYER1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	player[PLAYER1]->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	digger = new DIGGER(d, 14, screen_width, screen_height);
	digger->loadBitmaps(L"Graphics\\block37_");
	digger->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	digger->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	//player[1]->loadBitmaps(L"Graphics\\block30_");
	//player[1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	//player[1]->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	musicFileName = (wchar_t**)malloc(sizeof(wchar_t*) * 10);
	
	for(unsigned int index = 0; index < 10; index++)
	{
		musicFileName[index] = (wchar_t*)malloc(sizeof(wchar_t) * 256);
		wsprintf(musicFileName[index], L"Sound\\world%d.MID", index+1);
	}

	soundFileName = (wchar_t**)malloc(sizeof(wchar_t*) * 9);
	for(unsigned int index = 0; index < 9; index++)
	{
		soundFileName[index] = (wchar_t*)malloc(sizeof(wchar_t) * 256);
	}
	wsprintf(soundFileName[SOUND_BEGIN_LEVEL], L"Sound\\beginlevel.wav");
	wsprintf(soundFileName[SOUND_FALLING], L"Sound\\falling.wav");
	wsprintf(soundFileName[SOUND_LANDING], L"Sound\\landing.wav");
	wsprintf(soundFileName[SOUND_DIGGER], L"Sound\\digger.wav");
	wsprintf(soundFileName[SOUND_GOLD], L"Sound\\gold.wav");

	soundEffect = (SOUND**)malloc(sizeof(SOUND*) * 5);
	soundEffect[SOUND_BEGIN_LEVEL] = new SOUND(xa);
	soundEffect[SOUND_BEGIN_LEVEL]->loadWAVFile(soundFileName[SOUND_BEGIN_LEVEL]);
	soundEffect[SOUND_FALLING] = new SOUND(xa);
	soundEffect[SOUND_FALLING]->loadWAVFile(soundFileName[SOUND_FALLING]);
	soundEffect[SOUND_LANDING] = new SOUND(xa);
	soundEffect[SOUND_LANDING]->loadWAVFile(soundFileName[SOUND_LANDING]);
	soundEffect[SOUND_DIGGER] = new SOUND(xa);
	soundEffect[SOUND_DIGGER]->loadWAVFile(soundFileName[SOUND_DIGGER]);
	soundEffect[SOUND_GOLD] = new SOUND(xa);
	soundEffect[SOUND_GOLD]->loadWAVFile(soundFileName[SOUND_GOLD]);
	
	//soundEffect[SOUND_BEGIN_LEVEL]
}

GAMEPLAY::~GAMEPLAY(void)
{
	if(player)
	{
		for(unsigned int index = 0; index < 2; index++)
			delete player[index];
	}
	free(player);
	if(digger)
		delete digger;
	if(musicFileName)
	{
		for(unsigned int index = 0; index < 10; index++)
		{
			free(musicFileName[index]);
		}
		free(musicFileName);
	}
	if(soundFileName)
	{
		for(unsigned int index = 0; index < 9; index++)
		{
			free(soundFileName[index]);
		}
		free(soundFileName);
	}
	if(soundEffect)
	{
		for(unsigned int index = 0; index < 5; index++)
		{
			delete soundEffect[index];
		}
		free(soundEffect);
	}
	if(gold)
	{
		for(unsigned int index = 0; index < nbrOfGold; index++)
		{
			if(gold[index])
			{
				delete gold[index];
				gold[index] = NULL;
			}
		}
		free(gold);
		gold = NULL;
	}
	//if(fallingSound)
	//	delete fallingSound;
	//if(landingSound)
	//	delete landingSound;
}

void GAMEPLAY::Render(IDirect3DSurface9* &buf)
{
	Gravity();
	CollectGold();

	if(isEnteringLevel == true)
	{
		if(isEnteringLevelSound == false)
		{
			soundEffect[SOUND_BEGIN_LEVEL]->startWAVFile();
			isEnteringLevelSound = true;
		}
		Sleep(75);
		Player1EntersLevel();
	}
	platform->renderPlatform(buf);

	if(isDiggingRight == true)
	{
		DigRightPlayer1();
		Sleep(75);
		digger->renderSprite(buf);
	}
	if(isDiggingLeft == true)
	{
		DigLeftPlayer1();
		Sleep(50);
		digger->renderSprite(buf);
	}
	

	for(unsigned int index = 0; index < nbrOfGold; index++)
	{
		if(gold[index])
			if(gold[index]->isCollected == false)
				gold[index]->renderSprite(buf);
	}
	player[PLAYER1]->renderSprite(buf);
}

int GAMEPLAY::LoadLevel(unsigned int levelNbr)
{
	unsigned int counter = 0;
	platform->LoadLevel(L"level1.lvl");
	wchar_t fileName[256];
	POINT p;
	p = platform->GetStartingCoordinatesOfPlayer(0);
	player[PLAYER1]->x_pos = p.x;
	player[PLAYER1]->y_pos = p.y;

	for(unsigned int index = 0; index < platform->nbrOfBlocks; index++)
	{
		if((platform->type[index] >= BLOCK_GOLD_COIN) && (platform->type[index] <= BLOCK_GOLD_BARS))
			nbrOfGold++;
	}
	
	gold = (GOLD**)malloc(sizeof(GOLD*) * nbrOfGold);
	
	for(unsigned int index = 0; index < nbrOfGold; index++)
	{
		gold[index] = new GOLD(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
	}

	for(unsigned int index = 0; index < platform->nbrOfBlocks; index++)
	{
		if((platform->type[index] >= BLOCK_GOLD_COIN) && (platform->type[index] <= BLOCK_GOLD_BARS))
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			wsprintf(fileName, L"Graphics\\block%d_", platform->type[index]+1);
			gold[counter]->loadBitmaps(fileName);
			gold[counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			gold[counter]->x_pos = p.x;
			gold[counter]->y_pos = p.y;
			counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
	}

	platform->SetIsPlaying(true);
	isEnteringLevel = true;
	isEnteringLevelSound = false;
	return 1;
}

int GAMEPLAY::LoadLevel(void)
{
	POINT p;
	unsigned int counter = 0;
	wchar_t fileName[256];
	platform->LoadLevel();
	p = platform->GetStartingCoordinatesOfPlayer(0);
	player[PLAYER1]->x_pos = p.x;
	player[PLAYER1]->y_pos = p.y;


	for(unsigned int index = 0; index < platform->nbrOfBlocks; index++)
	{
		if((platform->type[index] >= BLOCK_GOLD_COIN) && (platform->type[index] <= BLOCK_GOLD_BARS))
			nbrOfGold++;
	}
	
	gold = (GOLD**)malloc(sizeof(GOLD*) * nbrOfGold);
	
	for(unsigned int index = 0; index < nbrOfGold; index++)
	{
		gold[index] = new GOLD(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
	}

	for(unsigned int index = 0; index < platform->nbrOfBlocks; index++)
	{
		if((platform->type[index] >= BLOCK_GOLD_COIN) && (platform->type[index] <= BLOCK_GOLD_BARS))
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			wsprintf(fileName, L"Graphics\\block%d_", platform->type[index]+1);
			gold[counter]->loadBitmaps(fileName);
			gold[counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			gold[counter]->x_pos = p.x;
			gold[counter]->y_pos = p.y;
			counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
	}


	platform->SetIsPlaying(true);
	isEnteringLevel = true;
	isEnteringLevelSound = false;
	return 1;
}

void GAMEPLAY::Exit(void)
{
	if(gold)
	{
		for(unsigned int index = 0; index < nbrOfGold; index++)
		{
			if(gold[index])
			{
				delete gold[index];
				gold[index] = NULL;
			}
		}
		free(gold);
		gold = NULL;
	}
	platform->SetIsPlaying(false);
}

wchar_t* GAMEPLAY::GetMusicFileName(void)
{
	return musicFileName[platform->GetWorldNbr()];
}

void GAMEPLAY::MovePlayer1Right(void)
{
	unsigned int res, blockNbr;
	unsigned int res2;	// used for the ID of the block below the player
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false)
	{
		// get the next block to determine if the player can move or not
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+25, player[PLAYER1]->y_pos);
		res2 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+25));
		// detect for hitting walls
		res = platform->GetType(blockNbr);
		if( res > BLOCK_HOLLOW && (player[PLAYER1]->x_pos+24 < 767))
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			if(res2 == BLOCK_SLOW)
				player[PLAYER1]->x_pos+=1;
			else
				player[PLAYER1]->x_pos+=4;
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->nextFrame();
		}
		else
			player[PLAYER1]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
}

void GAMEPLAY::MovePlayer1Left(void)
{
	unsigned int res, res2, blockNbr;
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false)
	{
		// detect for hitting walls
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos-1, player[PLAYER1]->y_pos);
			res2 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+25));
			res = platform->GetType(blockNbr);
		if(res > BLOCK_HOLLOW && (player[PLAYER1]->x_pos > 0))
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			if(res2 == BLOCK_SLOW)
				player[PLAYER1]->x_pos-=1;
			else
				player[PLAYER1]->x_pos-=4;
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->backFrame();
		}
		else
			player[PLAYER1]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
	
}

void GAMEPLAY::MovePlayer1Down(void)
{
	unsigned int res;
	unsigned int blockNbr;
	int x, y;

	if(isEnteringLevel == false)
	{
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+10, player[PLAYER1]->y_pos+24);
		res = platform->GetType(blockNbr);
		if(res == BLOCK_LADDER || res == BLOCK_REGULAR_WITH_LADDER)
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->x_pos = x;
			player[PLAYER1]->y_pos+=4;
			player[PLAYER1]->climbDownFrame();
		}
	}
}

void GAMEPLAY::MovePlayer1Up(void)
{
	unsigned int res, res2;
	unsigned int blockNbr, prevBlockNbr;
	int x, y;

	if(isEnteringLevel == false)
	{
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
		prevBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+23);
		//currentBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
		res2 = platform->GetType(prevBlockNbr);
		res = platform->GetType(blockNbr);
		if(res == BLOCK_LADDER || res == BLOCK_REGULAR_WITH_LADDER && (player[PLAYER1]->y_pos > 0))
		{
		//if(res2 == BLOCK_LADDER || res2 == BLOCK_REGULAR_WITH_LADDER || res2 == BLOCK_REGULAR)
		
				platform->GetBlockCoordinates(blockNbr, x, y);
				player[PLAYER1]->x_pos = x;
				if(player[PLAYER1]->y_pos-4 < 0)
					player[PLAYER1]->y_pos = 0;
				else
					player[PLAYER1]->y_pos-=4;
				player[PLAYER1]->climbUpFrame();
		
		}
		else if (res2 == BLOCK_LADDER || res2 == BLOCK_REGULAR_WITH_LADDER)
		{
		
				platform->GetBlockCoordinates(blockNbr, x, y);
				player[PLAYER1]->x_pos = x;
				if(player[PLAYER1]->y_pos-2 < 0)
					player[PLAYER1]->y_pos = 0;
				else
					player[PLAYER1]->y_pos-=2;
				player[PLAYER1]->climbUpFrame();
		}
		else
		{
			//platform->GetBlockCoordinates(blockNbr, x, y);
			//player[PLAYER1]->x_pos = x;
			//player[PLAYER1]->y_pos = y;
			player[PLAYER1]->setFrameState(0);
		}
	}

}

void GAMEPLAY::DigRightPlayer1(void)
{
	unsigned int res, blockNbr, blockNbr2;
	unsigned int res2;	// used for the ID of the block to the right and below the player
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false)
	{
		// get the next block to determine if the player can dig or not
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+25, player[PLAYER1]->y_pos);
		blockNbr2 = platform->getBlockNbr(player[PLAYER1]->x_pos+25, player[PLAYER1]->y_pos+25);
		res2 = platform->GetType(blockNbr2);
		// detect for hitting walls
		res = platform->GetType(blockNbr);
		if( res2 == BLOCK_REGULAR)
		{
			platform->GetBlockCoordinates(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos), x, y);
			player[PLAYER1]->x_pos = x;
			player[PLAYER1]->y_pos = y;
			platform->GetBlockCoordinates(blockNbr2, x, y);
			digger->x_pos = x;
			digger->y_pos = y;
			isDiggingRight = digger->digRightFrame();
			platform->DestroyBlock(blockNbr2);
			soundEffect[SOUND_DIGGER]->startWAVFile();
		}
		else
			player[PLAYER1]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false &&isDiggingRight == true)
	{
		isDiggingRight = digger->digRightFrame();
	}
}

void GAMEPLAY::DigLeftPlayer1(void)
{	
	unsigned int res, blockNbr, blockNbr2;   // blockNbr2 is the block to be destroyed
	unsigned int res2;	// used for the ID of the block to the right and below the player
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false)
	{
		// get the next block to determine if the player can dig or not
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos-1, player[PLAYER1]->y_pos);
		blockNbr2 = platform->getBlockNbr(player[PLAYER1]->x_pos-12, player[PLAYER1]->y_pos+25);
		res2 = platform->GetType(blockNbr2);
		// detect for hitting walls
		res = platform->GetType(blockNbr);
		if( res2 == BLOCK_REGULAR)
		{
			platform->GetBlockCoordinates(platform->getBlockNbr(player[PLAYER1]->x_pos+12, player[PLAYER1]->y_pos), x, y);
			player[PLAYER1]->x_pos = x;
			player[PLAYER1]->y_pos = y;
			platform->GetBlockCoordinates(blockNbr2, x, y);
			digger->x_pos = x;
			digger->y_pos = y;
			isDiggingLeft = digger->digLeftFrame();
			platform->DestroyBlock(blockNbr2);
			soundEffect[SOUND_DIGGER]->startWAVFile();
		}
		else
			player[PLAYER1]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == true &&isDiggingRight == false)
	{
		isDiggingLeft = digger->digLeftFrame();
	}
}

void GAMEPLAY::Gravity(void)
{
	unsigned int res, res2, blockNbr;
	int x, y;
	blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+12, player[PLAYER1]->y_pos+24);
	res = platform->GetType(blockNbr);
	// get ID of current block
	res2 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos+12, player[PLAYER1]->y_pos));

	if( (res == BLOCK_EMPTY || res == BLOCK_HOLLOW) && (res2 != BLOCK_LADDER) && (player[PLAYER1]->y_pos <= 743))
	{
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+12, player[PLAYER1]->y_pos);
		platform->GetBlockCoordinates(blockNbr, x, y);
		player[PLAYER1]->x_pos = x;
		if(player[PLAYER1]->y_pos+5 > 745)
		{
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, 760);
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->y_pos = y;
		}
		else
			player[PLAYER1]->y_pos+=5;
		player[PLAYER1]->fallingFrame();
		if(isFalling == false)
			soundEffect[SOUND_FALLING]->startWAVFile();
		isFalling = true;
	}
	else
	{
		if(isFalling == true)
		{
			soundEffect[SOUND_FALLING]->stopWAVFile();
			//fallingSound->stopWAVFile();
			//landingSound->startWAVFile();
			soundEffect[SOUND_LANDING]->startWAVFile();
			player[PLAYER1]->setFrameState(0);
		}
		isFalling = false;
	}
}

void GAMEPLAY::Sounds(void)
{
	//fallingSound->playWAVFile();
	//landingSound->playWAVFile();
	for(unsigned int index = 0; index < 5; index++)
	{
		soundEffect[index]->playWAVFile();
	}
}

void GAMEPLAY::Player1EntersLevel(void)
{
	isEnteringLevel = player[PLAYER1]->enterLevel();
}

void GAMEPLAY::CollectGold(void)
{
	unsigned int res;
	int x, y;
	for(unsigned int index = 0; index < nbrOfGold; index++)
	{
		res = platform->getBlockNbr(gold[index]->x_pos, gold[index]->y_pos);
		platform->GetBlockCoordinates(res,x,y);
		if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
			&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
		{
			if(gold[index]->isCollected == false)
			{
				gold[index]->isCollected = true;
				// collect gold
				player[PLAYER1]->goldCollected++;
				soundEffect[SOUND_GOLD]->startWAVFile();
			}
		}
	}
}