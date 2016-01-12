#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "player.h"
#include "digger.h"
#include "sound.h"
#include "gold.h"
#include "fallingrocks.h"

enum SOUND_EFFECT
{ SOUND_BEGIN_LEVEL, SOUND_FALLING, SOUND_LANDING, SOUND_DIGGER, SOUND_GOLD, SOUND_DRILLING};

class GAMEPLAY
{
	public:
		GAMEPLAY(IDirect3DDevice9* d, IXAudio2* xa, PLATFORM* p, HWND &hWnd, int screen_width, int screen_height);
		~GAMEPLAY(void);
		void Render(IDirect3DSurface9* &buf);
		void MovePlayer1Right(void);
		void MovePlayer1Left(void);
		void MovePlayer1Up(void);
		void MovePlayer1Down(void);
		void DigRightPlayer1(void);
		void DigLeftPlayer1(void);
		void DrillPlayer1(void);
		void PickRightPlayer1(void);
		void FallingRocksRight(void);
		int LoadLevel(unsigned int levelNbr);
		int LoadLevel(void);
		void Exit(void);
		void Gravity(void);
		void Sounds(void);
		wchar_t* GetMusicFileName(void);
		void Player1EntersLevel(void);
		void CollectGold(void);

	private:
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		PLAYER** player;
		DIGGER* digger;
		FALLINGROCKS** fallingrocks;
		SPRITE** monk;
		GOLD** gold;
		wchar_t** musicFileName;
		wchar_t** soundFileName;
		unsigned int nbrOfGold;
		bool isFalling;
		bool isEnteringLevel;
		bool isEnteringLevelSound;
		bool isDiggingRight;
		bool isDiggingLeft;
		unsigned char isDrilling;
		unsigned char isPickingRight;
		unsigned char isFallingRocksRight[4];
		IS_FALLING_ROCKS_INDEX isFallingRocksIndex[4];
		//SOUND* fallingSound;
		//SOUND* landingSound;
		SOUND** soundEffect;
		SOUND* music;
};

#endif