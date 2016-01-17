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
	fallingrocks = NULL;
	musicFileName = NULL;
	soundFileName = NULL;
	isFalling = false;
	//fallingSound = NULL;
	isEnteringLevel = false;
	soundEffect = NULL;
	isEnteringLevelSound = false;
	isDiggingRight = false;
	isDiggingLeft = false;	
	isPickingRight = 0;
	isPickingLeft = 0;
	isDrilling = 0;
	isSettingUpRope = 0;
	nbrOfRopetrap = 0;
	nbrOfJackhammer = 0;
	nbrOfPick = 0;
	
	nbrOfGold = 0;
	gold = NULL;		// allocated in LoadLevel()
	music = NULL;
	ropetrap = NULL;	// allocated in LoadLevel()
	jackhammer = NULL;
	pick = NULL;

	player = (PLAYER**) malloc(sizeof(PLAYER*) * 2);
	for(unsigned int index = 0; index < 2; index++)
	{
		player[index] = new PLAYER(d, 102, screen_width, screen_height);
	}

	player[PLAYER1]->loadBitmaps(L"Graphics\\block29_");
	player[PLAYER1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	player[PLAYER1]->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	digger = new DIGGER(d, 14, screen_width, screen_height);
	digger->loadBitmaps(L"Graphics\\block37_");
	digger->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	digger->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	fallingrocks = (FALLINGROCKS**)malloc(sizeof(FALLINGROCKS*) * 4);
	for(unsigned int index = 0; index < 4; index++)
	{
		fallingrocks[index] = new FALLINGROCKS(d, 4, screen_width, screen_height);
		fallingrocks[index]->loadBitmaps(L"Graphics\\block38_");
		fallingrocks[index]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
		fallingrocks[index]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	}
	//player[1]->loadBitmaps(L"Graphics\\block30_");
	//player[1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	//player[1]->setAnimationType(ANIMATION_TRIGGERED_SEQ);

	musicFileName = (wchar_t**)malloc(sizeof(wchar_t*) * 10);
	
	for(unsigned int index = 0; index < 10; index++)
	{
		musicFileName[index] = (wchar_t*)malloc(sizeof(wchar_t) * 256);
		wsprintf(musicFileName[index], L"Sound\\world%d.WAV", index+1);
	}

	soundFileName = (wchar_t**)malloc(sizeof(wchar_t*) * 10);
	for(unsigned int index = 0; index < 10; index++)
	{
		soundFileName[index] = (wchar_t*)malloc(sizeof(wchar_t) * 256);
	}
	wsprintf(soundFileName[SOUND_BEGIN_LEVEL], L"Sound\\beginlevel.wav");
	wsprintf(soundFileName[SOUND_FALLING], L"Sound\\falling.wav");
	wsprintf(soundFileName[SOUND_LANDING], L"Sound\\landing.wav");
	wsprintf(soundFileName[SOUND_DIGGER], L"Sound\\digger.wav");
	wsprintf(soundFileName[SOUND_GOLD], L"Sound\\gold.wav");
	wsprintf(soundFileName[SOUND_DRILLING], L"Sound\\drilling.wav");
	wsprintf(soundFileName[SOUND_PICK], L"Sound\\pick.wav");
	wsprintf(soundFileName[SOUND_FALLINGROCKS], L"Sound\\fallingrocks.wav");
	wsprintf(soundFileName[SOUND_SETROPETRAP], L"Sound\\setropetrap.wav");
	wsprintf(soundFileName[SOUND_PICKUP], L"Sound\\pickup.wav");

	soundEffect = (SOUND**)malloc(sizeof(SOUND*) * 10);
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
	soundEffect[SOUND_DRILLING] = new SOUND(xa);
	soundEffect[SOUND_DRILLING]->loadWAVFile(soundFileName[SOUND_DRILLING]);
	soundEffect[SOUND_SETROPETRAP] = new SOUND(xa);
	soundEffect[SOUND_SETROPETRAP]->loadWAVFile(soundFileName[SOUND_SETROPETRAP]);

	soundEffect[SOUND_PICK] = new SOUND(xa);
	soundEffect[SOUND_PICK]->loadWAVFile(soundFileName[SOUND_PICK]);

	soundEffect[SOUND_FALLINGROCKS] = new SOUND(xa);
	soundEffect[SOUND_FALLINGROCKS]->loadWAVFile(soundFileName[SOUND_FALLINGROCKS]);
	
	soundEffect[SOUND_PICKUP] = new SOUND(xa);
	soundEffect[SOUND_PICKUP]->loadWAVFile(soundFileName[SOUND_PICKUP]);

	music = new SOUND(xa);
	
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
	if(fallingrocks)
	{
		for(unsigned int index = 0; index < 4; index++)
		{
			delete fallingrocks[index];
		}
		free(fallingrocks);
	}
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
		for(unsigned int index = 0; index < 10; index++)
		{
			free(soundFileName[index]);
		}
		free(soundFileName);
	}
	if(soundEffect)
	{
		for(unsigned int index = 0; index < 9; index++)
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

	if(ropetrap)
	{
		for(unsigned int index = 0; index < nbrOfRopetrap; index++)
		{
			if(ropetrap[index])
			{
				delete ropetrap[index];
				ropetrap[index] = NULL;
			}
		}
		free(ropetrap);
	}

	if(jackhammer)
	{
		for(unsigned int index = 0; index < nbrOfJackhammer; index++)
		{
			if(jackhammer[index])
			{
				delete jackhammer[index];
				jackhammer[index] = NULL;
			}
		}
		free(jackhammer);
	}

	if(pick)
	{
		for(unsigned int index = 0; index < nbrOfPick; index++)
		{
			if(pick[index])
			{
				delete pick[index];
				pick[index] = NULL;
			}
		}
		free(pick);
	}

	if(music)
		delete music;
	//if(fallingSound)
	//	delete fallingSound;
	//if(landingSound)
	//	delete landingSound;
}

void GAMEPLAY::Render(IDirect3DSurface9* &buf)
{
	if(!music->playWAVFile())
		music->startWAVFile();
	
	Gravity();
	CollectGold();
	Sounds();

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
		digger->renderSprite(buf);
		DigRightPlayer1();
		Sleep(75);
	}
	if(isDiggingLeft == true)
	{
		digger->renderSprite(buf);
		DigLeftPlayer1();
		Sleep(50);
	}
	if((isDrilling >=1) && (isDrilling <= 3))
	{
		DrillPlayer1();
	}
	if(isSettingUpRope)
	{
		SetUpRopePlayer1();
	}
	if(isPickingRight)
	{
		PickRightPlayer1();
	}
	if(isPickingLeft)
	{
		PickLeftPlayer1();
	}
	for(unsigned int index = 0; index < 4; index++)
	{
		if(fallingrocks[index]->isOpen == false)
		{
			fallingrocks[index]->renderSprite(buf);
			FallingRocks();
		}
	}
	

	for(unsigned int index = 0; index < nbrOfGold; index++)
	{
		if(gold[index])
			if(gold[index]->isCollected == false)
				gold[index]->renderSprite(buf);
	}

	for(unsigned int index = 0; index < nbrOfRopetrap; index++)
	{
		if(ropetrap[index])
			if(ropetrap[index]->isCollected == false)
				ropetrap[index]->renderSprite(buf);
	}
	for(unsigned int index = 0; index < nbrOfJackhammer; index++)
	{
		if(jackhammer[index])
			if(jackhammer[index]->isCollected == false)
				jackhammer[index]->renderSprite(buf);
	}
	for(unsigned int index = 0; index < nbrOfPick; index++)
	{
		if(pick[index])
			if(pick[index]->isCollected == false)
				pick[index]->renderSprite(buf);
	}
	player[PLAYER1]->renderSprite(buf);
}

int GAMEPLAY::LoadLevel(unsigned int levelNbr)
{
	unsigned int counter = 0;
	unsigned int ropetrap_counter = 0;
	unsigned int jackhammer_counter = 0;
	unsigned int pick_counter = 0;
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
		else if(platform->type[index] == BLOCK_ROPE)
			nbrOfRopetrap++;
		else if(platform->type[index] == BLOCK_JACKHAMMER)
			nbrOfJackhammer++;
		else if(platform->type[index] == BLOCK_PICK)
			nbrOfPick++;
	}
	
	if(nbrOfGold)
	{
		gold = (GOLD**)malloc(sizeof(GOLD*) * nbrOfGold);
	
		for(unsigned int index = 0; index < nbrOfGold; index++)
		{
			gold[index] = new GOLD(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfRopetrap)
	{
		ropetrap = (ROPETRAP**)malloc(sizeof(ROPETRAP*) * nbrOfRopetrap);

		for(unsigned int index = 0; index < nbrOfRopetrap; index++)
		{
			ropetrap[index] = new ROPETRAP(platform->d3ddev, 2, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfJackhammer)
	{
		jackhammer = (JACKHAMMER**)malloc(sizeof(JACKHAMMER*) * nbrOfJackhammer);

		for(unsigned int index = 0; index < nbrOfJackhammer; index++)
		{
			jackhammer[index] = new JACKHAMMER(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfPick)
	{
		pick = (PICK**)malloc(sizeof(PICK*) * nbrOfPick);

		for(unsigned int index = 0; index < nbrOfPick; index++)
		{
			pick[index] = new PICK(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
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
		else if(platform->type[index] == BLOCK_ROPE)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			ropetrap[ropetrap_counter]->loadBitmaps(L"Graphics\\block19_");
			ropetrap[ropetrap_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			ropetrap[ropetrap_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			ropetrap[ropetrap_counter]->x_pos = p.x;
			ropetrap[ropetrap_counter]->y_pos = p.y;
			ropetrap_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
		else if(platform->type[index] == BLOCK_JACKHAMMER)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			jackhammer[jackhammer_counter]->loadBitmaps(L"Graphics\\block17_");
			jackhammer[jackhammer_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			jackhammer[jackhammer_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			jackhammer[jackhammer_counter]->x_pos = p.x;
			jackhammer[jackhammer_counter]->y_pos = p.y;
			jackhammer_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
		else if(platform->type[index] == BLOCK_PICK)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			pick[pick_counter]->loadBitmaps(L"Graphics\\block18_");
			pick[pick_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			pick[pick_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			pick[pick_counter]->x_pos = p.x;
			pick[pick_counter]->y_pos = p.y;
			pick_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
	}

	platform->SetIsPlaying(true);
	music->loadWAVFile(musicFileName[platform->GetWorldNbr()]);
	music->startWAVFile();
	isEnteringLevel = true;
	isEnteringLevelSound = false;
	return 1;
}

int GAMEPLAY::LoadLevel(void)
{
	POINT p;
	unsigned int counter = 0;
	unsigned int ropetrap_counter = 0;
	unsigned int jackhammer_counter = 0;
	unsigned int pick_counter = 0;
	wchar_t fileName[256];
	platform->LoadLevel();
	p = platform->GetStartingCoordinatesOfPlayer(0);
	player[PLAYER1]->x_pos = p.x;
	player[PLAYER1]->y_pos = p.y;


	for(unsigned int index = 0; index < platform->nbrOfBlocks; index++)
	{
		if((platform->type[index] >= BLOCK_GOLD_COIN) && (platform->type[index] <= BLOCK_GOLD_BARS))
			nbrOfGold++;
		else if(platform->type[index] == BLOCK_ROPE)
			nbrOfRopetrap++;
		else if(platform->type[index] == BLOCK_JACKHAMMER)
			nbrOfJackhammer++;
		else if(platform->type[index] == BLOCK_PICK)
			nbrOfPick++;
	}
	
	if(nbrOfGold)
	{
		gold = (GOLD**)malloc(sizeof(GOLD*) * nbrOfGold);
	
		for(unsigned int index = 0; index < nbrOfGold; index++)
		{
			gold[index] = new GOLD(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfRopetrap)
	{
		ropetrap = (ROPETRAP**)malloc(sizeof(ROPETRAP*) * nbrOfRopetrap);

		for(unsigned int index = 0; index < nbrOfRopetrap; index++)
		{
			ropetrap[index] = new ROPETRAP(platform->d3ddev, 2, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfJackhammer)
	{
		jackhammer = (JACKHAMMER**)malloc(sizeof(JACKHAMMER*) * nbrOfJackhammer);

		for(unsigned int index = 0; index < nbrOfJackhammer; index++)
		{
			jackhammer[index] = new JACKHAMMER(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
	}

	if(nbrOfPick)
	{
		pick = (PICK**)malloc(sizeof(PICK*) * nbrOfPick);

		for(unsigned int index = 0; index < nbrOfPick; index++)
		{
			pick[index] = new PICK(platform->d3ddev, 1, platform->screenWidth, platform->screenHeight);
		}
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
		else if(platform->type[index] == BLOCK_ROPE)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			ropetrap[ropetrap_counter]->loadBitmaps(L"Graphics\\block19_");
			ropetrap[ropetrap_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			ropetrap[ropetrap_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			ropetrap[ropetrap_counter]->x_pos = p.x;
			ropetrap[ropetrap_counter]->y_pos = p.y;
			ropetrap_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
		else if(platform->type[index] == BLOCK_JACKHAMMER)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			jackhammer[jackhammer_counter]->loadBitmaps(L"Graphics\\block17_");
			jackhammer[jackhammer_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			jackhammer[jackhammer_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			jackhammer[jackhammer_counter]->x_pos = p.x;
			jackhammer[jackhammer_counter]->y_pos = p.y;
			jackhammer_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
		else if(platform->type[index] == BLOCK_PICK)
		{
			platform->GetBlockCoordinates(index, (int&)p.x, (int&)p.y);
			pick[pick_counter]->loadBitmaps(L"Graphics\\block18_");
			pick[pick_counter]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			pick[pick_counter]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
			pick[pick_counter]->x_pos = p.x;
			pick[pick_counter]->y_pos = p.y;
			pick_counter++;
			platform->type[index] = BLOCK_EMPTY;
			platform->isOccupied[index] = IS_NOT_OCCUPIED;
		}
	}

	platform->SetIsPlaying(true);
	music->loadWAVFile(musicFileName[platform->GetWorldNbr()]);
	music->startWAVFile();
	isEnteringLevel = true;
	isEnteringLevelSound = false;
	return 1;
}

void GAMEPLAY::Exit(void)
{
	music->stopWAVFile();
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
	if(ropetrap)
	{
		for(unsigned int index = 0; index < nbrOfRopetrap; index++)
		{
			if(ropetrap[index])
			{
				delete ropetrap[index];
				ropetrap[index] = NULL;
			}
		}
		free(ropetrap);
	}
	ropetrap = NULL;
	if(jackhammer)
	{
		for(unsigned int index = 0; index < nbrOfJackhammer; index++)
		{
			if(jackhammer[index])
			{
				delete jackhammer[index];
				jackhammer[index] = NULL;
			}
		}
		free(jackhammer);
	}
	jackhammer = NULL;
	if(pick)
	{
		for(unsigned int index = 0; index < nbrOfPick; index++)
		{
			if(pick[index])
			{
				delete pick[index];
				pick[index] = NULL;
			}
		}
		free(pick);
	}
	pick = NULL;
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
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false && isDrilling == 0 
		&& isPickingRight == 0)
	{
		// get the next block to determine if the player can move or not
		blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+25, player[PLAYER1]->y_pos);
		res2 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+25));
		// detect for hitting walls
		res = platform->GetType(blockNbr);
		if( res > BLOCK_HOLLOW && res != BLOCK_ROCKS && res != BLOCK_BAR && res != BLOCK_LADDER && (player[PLAYER1]->x_pos+24 < 767))
		{	
			platform->GetBlockCoordinates(blockNbr, x, y);
			if(res2 == BLOCK_SLOW)
				player[PLAYER1]->x_pos+=1;
			else
				player[PLAYER1]->x_pos+=4;
			if(isClimbingBar == true)
			{
				player[PLAYER1]->x_pos+=8;
				isClimbingBar = false;
			}
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->nextFrame();
		}
		else if(res == BLOCK_BAR)
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->x_pos+=4;
			player[PLAYER1]->nextBarFrame();
			isClimbingBar = true;
		}
		else if(res == BLOCK_LADDER)
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->x_pos+=4;
			player[PLAYER1]->nextFrame();
			isClimbingBar = true;
		}
		else
			player[PLAYER1]->setFrameState(0);	// if hit a wall set the frame to the idle
	}
}

void GAMEPLAY::MovePlayer1Left(void)
{
	unsigned int res, res2, res3, blockNbr;
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false && isDrilling == 0
		&& isPickingRight == 0)
	{
		// detect for hitting walls
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos-1, player[PLAYER1]->y_pos);
			res2 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+25));
			res = platform->GetType(blockNbr);
			res3 = platform->GetType(platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos-5));
		if(res > BLOCK_HOLLOW && res != BLOCK_ROCKS && res != BLOCK_BAR && res!=BLOCK_LADDER && (player[PLAYER1]->x_pos > 0))
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			if(res2 == BLOCK_SLOW)
				player[PLAYER1]->x_pos-=1;
			else
				player[PLAYER1]->x_pos-=4;
			if(isClimbingBar == true)
			{
				player[PLAYER1]->x_pos-=12;
				isClimbingBar = false;
			}
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->backFrame();
		}
		else if(res == BLOCK_BAR)
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->x_pos-=4;
			player[PLAYER1]->backBarFrame();
			isClimbingBar = true;
		}
		else if(res == BLOCK_LADDER)
		{
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->x_pos-=4;
			player[PLAYER1]->backFrame();
			isClimbingBar = true;
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

	if(isEnteringLevel == false && isDrilling == 0)
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
		if(isClimbingBar == true)
			isClimbingBar = false;
	}
}

void GAMEPLAY::MovePlayer1Up(void)
{
	unsigned int res, res2;
	unsigned int blockNbr, prevBlockNbr;
	int x, y;

	if(isEnteringLevel == false && isDrilling == 0)
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
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false && isDrilling == 0)
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
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false &&isDiggingRight == true && isDrilling == 0)
	{
		isDiggingRight = digger->digRightFrame();
		
	}
}

void GAMEPLAY::DigLeftPlayer1(void)
{	
	unsigned int res, blockNbr, blockNbr2;   // blockNbr2 is the block to be destroyed
	unsigned int res2;	// used for the ID of the block to the right and below the player
	int x, y;
	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false && isDrilling == 0)
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
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == true &&isDiggingRight == false && isDrilling == 0)
	{
		isDiggingLeft = digger->digLeftFrame();
	}
}

void GAMEPLAY::DrillPlayer1(void)
{
	unsigned int res, blockNbr, currentBlockNbr; 
	int x,y;
	bool test;

	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0)
	{
		if(player[PLAYER1]->itemHeld == BLOCK_JACKHAMMER)
		{
			// get the block that needs to be drilled
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+26);
			res = platform->GetType(blockNbr);
			currentBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);

			if(res == BLOCK_SOLID)
			{
				platform->GetBlockCoordinates(currentBlockNbr, x, y);
				player[PLAYER1]->x_pos = x;
				player[PLAYER1]->y_pos = y;
				soundEffect[SOUND_DRILLING]->startWAVFile();
				isDrilling = player[PLAYER1]->drillFrame();
			}
		}
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling)
	{
		player[PLAYER1]->y_pos+=1;
		test = player[PLAYER1]->drillFrame();
		if(test == false)
			isDrilling++;
		if(isDrilling == 4)
		{
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos+2);
			platform->GetBlockCoordinates(blockNbr, x,y);
			player[PLAYER1]->x_pos = x;
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->setFrameState(0);
			platform->DestroyBlockPermanently(blockNbr);
			isDrilling = 0;
		}
	}
}

void GAMEPLAY::PickRightPlayer1(void)
{
	unsigned int res, res2, blockNbr, currentBlockNbr, nextBlockNbr;
	int x,y;
	bool test;

	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingRight == 0)
	{
		if(player[PLAYER1]->itemHeld == BLOCK_PICK)
		{
			// get the block that needs to be picked
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+32, player[PLAYER1]->y_pos-5);
			res = platform->GetType(blockNbr);
		
			currentBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
			nextBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos+32, player[PLAYER1]->y_pos);
			res2 = platform->GetType(nextBlockNbr);
			if(res >= BLOCK_REGULAR && res <= BLOCK_SLOW)
			{
			
				platform->GetBlockCoordinates(currentBlockNbr, x, y);
				player[PLAYER1]->x_pos = x;
				player[PLAYER1]->y_pos = y;
				
				player[PLAYER1]->pickRightFrame();
				isPickingRight++;
			}
		}
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingRight && (isPickingRight %3 == 0))
	{
		test = player[PLAYER1]->pickRightFrame();
		if(isPickingRight == 6)
			soundEffect[SOUND_PICK]->startWAVFile();
		if(test == false)
		{
			isPickingRight = 0;
			player[PLAYER1]->setFrameState(0);

			// find first open slot to play animation
			for(unsigned int index = 0; index < 4; index++)
			{
				if(fallingrocks[index]->isOpen == true)
				{
					nextBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
					nextBlockNbr++;
					platform->SetTypeToRocks(nextBlockNbr);
					platform->GetBlockCoordinates(nextBlockNbr, x, y);
					fallingrocks[index]->isOpen = false;				// this triggers the fallingrocks animation
					fallingrocks[index]->blockNbr = nextBlockNbr;
					fallingrocks[index]->x_pos = x;
					fallingrocks[index]->y_pos = y;
					break;
				}
			}
			
			//FallingRocksRight();
		}
		else
			isPickingRight++;
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingRight && (isPickingRight %3))
	{
		isPickingRight++;
	}
}

void GAMEPLAY::PickLeftPlayer1(void)
{
	unsigned int res, res2, blockNbr, currentBlockNbr, nextBlockNbr;
	int x,y;
	bool test;

	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingRight == 0 && isPickingLeft == 0)
	{
		if(player[PLAYER1]->itemHeld == BLOCK_PICK)
		{
			// get the block that needs to be picked
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos-12, player[PLAYER1]->y_pos-5);
			res = platform->GetType(blockNbr);
		
			currentBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
			nextBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos-12, player[PLAYER1]->y_pos);
			res2 = platform->GetType(nextBlockNbr);
			if(res >= BLOCK_REGULAR && res <= BLOCK_SLOW)
			{
			
				platform->GetBlockCoordinates(currentBlockNbr, x, y);
				player[PLAYER1]->x_pos = x;
				player[PLAYER1]->y_pos = y;
			
				player[PLAYER1]->pickLeftFrame();
				isPickingLeft++;

			}
		}
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingLeft && (isPickingLeft %3 == 0))
	{
		test = player[PLAYER1]->pickLeftFrame();
		if(isPickingLeft == 6)
			soundEffect[SOUND_PICK]->startWAVFile();
		if(test == false)
		{
			isPickingLeft = 0;
			player[PLAYER1]->setFrameState(0);
			//FallingRocksLeft();

			// find first open slot to play animation
			for(unsigned int index = 0; index < 4; index++)
			{
				if(fallingrocks[index]->isOpen == true)
				{
					nextBlockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
					nextBlockNbr--;
					platform->SetTypeToRocks(nextBlockNbr);
					platform->GetBlockCoordinates(nextBlockNbr, x, y);
					fallingrocks[index]->isOpen = false;				// this triggers the fallingrocks animation
					fallingrocks[index]->blockNbr = nextBlockNbr;
					fallingrocks[index]->x_pos = x;
					fallingrocks[index]->y_pos = y;
					break;
				}
			}
		}
		else
			isPickingLeft++;
	}
	else if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false 
		&& isDrilling == 0 && isPickingLeft && (isPickingLeft %3))
	{
		isPickingLeft++;
	}
}

void GAMEPLAY::FallingRocks(void)
{
	bool test;
	for(unsigned int index =0; index < 4; index++)
	{
		if(fallingrocks[index]->isOpen == false)
		{
			if(fallingrocks[index]->timer == 0)	// if the beginning of animation
			{
				fallingrocks[index]->frame();
				fallingrocks[index]->timer++;
				soundEffect[SOUND_FALLINGROCKS]->startWAVFile();
			}
			else if(fallingrocks[index]->timer % 3 && fallingrocks[index]->fell == false) // slow down animation
			{
				fallingrocks[index]->timer++;
			}
			else
			{
				if(fallingrocks[index]->fell == false)	
				{

					test = fallingrocks[index]->frame();
					if(test == false)
					{
						fallingrocks[index]->fell = true;
						fallingrocks[index]->timer++;
					}
					else
						fallingrocks[index]->timer++;
				}
				else
				{
					if(fallingrocks[index]->timer < 200)			// if the rocks fell then wait 200 frames
						fallingrocks[index]->timer++;				// before removing the rocks
					else
					{
						fallingrocks[index]->isOpen = true;
						fallingrocks[index]->fell = false;
						fallingrocks[index]->reset();
						fallingrocks[index]->timer = 0;
						platform->RemoveRocks(fallingrocks[index]->blockNbr);
					}
				}
			}
		}
	}
	
}

void GAMEPLAY::SetUpRopePlayer1(void)
{
	unsigned int res, blockNbr;   	
	int x, y;
	bool test;
	
	

	if(isFalling == false && isEnteringLevel == false && isDiggingLeft == false && isDiggingRight == false && isDrilling == 0
		&& isSettingUpRope == 0)
	{
		if(player[PLAYER1]->itemHeld == BLOCK_ROPE)
		{
			// get the current block
			blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
			platform->GetBlockCoordinates(blockNbr, x, y);
			player[PLAYER1]->x_pos = x;
			player[PLAYER1]->y_pos = y;
			player[PLAYER1]->ropeFrame();
			soundEffect[SOUND_SETROPETRAP]->startWAVFile();
			isSettingUpRope++;
		}
	}
	else if(isSettingUpRope %2)
	{
		test = player[PLAYER1]->ropeFrame();
		if(test == false)
		{
			isSettingUpRope = 0;
			player[PLAYER1]->setFrameState(0);
			for(unsigned int index = 0; index < nbrOfRopetrap; index++)
			{
				if(ropetrap[index]->isCollected == true)
				{
					blockNbr = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
					platform->GetBlockCoordinates(blockNbr, x, y);
					ropetrap[index]->isSet = true;
					ropetrap[index]->isCollected = false;
					ropetrap[index]->x_pos = x;
					ropetrap[index]->y_pos = y;
					ropetrap[index]->SetFrameStateToSet();
					player[PLAYER1]->itemHeld = 0;
				}
			}

		}
		else
			isSettingUpRope++;
	}
	else if(isSettingUpRope %2 == 0)
	{
		isSettingUpRope++;
	}
	else
	{
		player[PLAYER1]->setFrameState(0);	// if set the frame to the idle 
		isSettingUpRope = 0;
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

	if(isDrilling == 0 && isClimbingBar == false)
	{
	if( (res == BLOCK_EMPTY || res == BLOCK_HOLLOW || res == BLOCK_BAR) && (res2 != BLOCK_LADDER) && (player[PLAYER1]->y_pos <= 743))
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
}

void GAMEPLAY::Sounds(void)
{
	//fallingSound->playWAVFile();
	//landingSound->playWAVFile();
	for(unsigned int index = 0; index < 10; index++)
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

void GAMEPLAY::PickupItem(void)
{
	unsigned int res;
	int x, y;

	if(player[PLAYER1]->itemHeld)
	{
		DropItem();
		return;
	}


	for(unsigned int index = 0; index < nbrOfRopetrap; index++)
	{
		if(ropetrap[index]->isCollected == false && ropetrap[index]->isSet == false)
		{
			res = platform->getBlockNbr(ropetrap[index]->x_pos, ropetrap[index]->y_pos);
			platform->GetBlockCoordinates(res,x,y);
			if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
				&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
			{
				player[PLAYER1]->itemHeld = BLOCK_ROPE;
				ropetrap[index]->isCollected = true;
				soundEffect[SOUND_PICKUP]->startWAVFile();
				break;
			}
		}
	}

	for(unsigned int index = 0; index < nbrOfJackhammer; index++)
	{
		if(jackhammer[index]->isCollected == false)
		{
			res = platform->getBlockNbr(jackhammer[index]->x_pos, jackhammer[index]->y_pos);
			platform->GetBlockCoordinates(res,x,y);
			if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
				&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
			{
				player[PLAYER1]->itemHeld = BLOCK_JACKHAMMER;
				jackhammer[index]->isCollected = true;
				soundEffect[SOUND_PICKUP]->startWAVFile();
				break;
			}
		}
	}

	for(unsigned int index = 0; index < nbrOfPick; index++)
	{
		if(pick[index]->isCollected == false)
		{
			res = platform->getBlockNbr(pick[index]->x_pos, ropetrap[index]->y_pos);
			platform->GetBlockCoordinates(res,x,y);
			if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
				&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
			{
				player[PLAYER1]->itemHeld = BLOCK_PICK;
				pick[index]->isCollected = true;
				soundEffect[SOUND_PICKUP]->startWAVFile();
				break;
			}
		}
	}
}

void GAMEPLAY::DropItem(void)
{
	unsigned int res;
	int x, y;

	if(player[PLAYER1]->itemHeld)
	{
		switch(player[PLAYER1]->itemHeld)
		{
			case BLOCK_ROPE:
				for(unsigned int index = 0; index < nbrOfRopetrap; index++)
				{
					if(ropetrap[index]->isCollected == true && ropetrap[index]->isSet == false)
					{
						res = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
						platform->GetBlockCoordinates(res,x,y);
						{
							player[PLAYER1]->itemHeld = 0;
							ropetrap[index]->isCollected = false;
							ropetrap[index]->x_pos = x;
							ropetrap[index]->y_pos = y;
							soundEffect[SOUND_PICKUP]->startWAVFile();
						}
					}
				}
			break;

			case BLOCK_JACKHAMMER:
				for(unsigned int index = 0; index < nbrOfJackhammer; index++)
				{
					if(jackhammer[index]->isCollected == true)
					{
						res = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
						platform->GetBlockCoordinates(res,x,y);
						if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
							&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
						{
							player[PLAYER1]->itemHeld = 0;
							jackhammer[index]->isCollected = false;
							jackhammer[index]->x_pos = x;
							jackhammer[index]->y_pos = y;
							soundEffect[SOUND_PICKUP]->startWAVFile();
						}
					}
				}
			break;

			case BLOCK_PICK:
				for(unsigned int index = 0; index < nbrOfPick; index++)
				{
					if(pick[index]->isCollected == true)
					{
						res = platform->getBlockNbr(player[PLAYER1]->x_pos, player[PLAYER1]->y_pos);
						platform->GetBlockCoordinates(res,x,y);
						if( ((player[PLAYER1]->x_pos+12 >= x) && (player[PLAYER1]->x_pos+12 <= x+24)) 
							&& ((player[PLAYER1]->y_pos+12 >= y) && (player[PLAYER1]->y_pos+12 <= y+24)))
						{
							player[PLAYER1]->itemHeld = 0;
							pick[index]->isCollected = false;
							pick[index]->x_pos = x;
							pick[index]->y_pos = y;
							soundEffect[SOUND_PICKUP]->startWAVFile();
						}
					}
				}
			break;
		}
	}
}

