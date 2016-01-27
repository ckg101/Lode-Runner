#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "player.h"
#include "digger.h"
#include "sound.h"
#include "gold.h"
#include "fallingrocks.h"
#include "ropetrap.h"
#include "jackhammer.h"
#include "pick.h"
#include "goo.h"
#include "gas.h"
#include "exitdoor.h"

enum SOUND_EFFECT
{ SOUND_BEGIN_LEVEL, SOUND_FALLING, SOUND_LANDING, SOUND_DIGGER, SOUND_GOLD, SOUND_DRILLING, SOUND_PICK, 
  SOUND_FALLINGROCKS, SOUND_SETROPETRAP, SOUND_PICKUP, SOUND_LASTGOLD, SOUND_ENTERPORTAL, SOUND_ENDLEVEL};

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
		void PickLeftPlayer1(void);
		void FallingRocks(void);
		void SetUpRopePlayer1(void);
		void UseGooRightPlayer1(void);
		void GooPlatformRight(void);
		void UseGooLeftPlayer1(void);
		void OpenExitDoor(void);
		int LoadLevel(unsigned int levelNbr);
		int LoadLevel(void);
		void Exit(void);
		void Gravity(void);
		void ExitLevel(void);
		void Sounds(void);
		wchar_t* GetMusicFileName(void);
		void Player1EntersLevel(void);
		void CollectGold(void);
		void PickupItem(void);
		void DropItem(void);
		bool leaveGameplay;
		unsigned int groupNbr;
	private:
		void UnallocateItems(void);
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		PLAYER** player;
		DIGGER* digger;
		FALLINGROCKS** fallingrocks;
		GOO_PLATFORM* gooPlatform;
		SPRITE** monk;
		GOLD** gold;
		ROPETRAP** ropetrap;
		JACKHAMMER** jackhammer;
		PICK** pick;
		GOO** goo;
		GAS** gas;
		EXITDOOR** exitdoor;
		SPRITE* controls;

		wchar_t** musicFileName;
		wchar_t** soundFileName;
		wchar_t** levelFileName;
		unsigned int nbrOfGold;
		unsigned int nbrOfRopetrap;
		unsigned int nbrOfJackhammer;
		unsigned int nbrOfPick;
		unsigned int nbrOfGoo;
		unsigned int nbrOfGas;
		unsigned int nbrOfExitdoor;
		unsigned int nbrOfLevels;
		unsigned int currentLevel;
		bool isFalling;
		bool isEnteringLevel;
		bool isEnteringLevelSound;
		bool isDiggingRight;
		bool isDiggingLeft;
		bool isClimbingBar;
		bool isReleased;		// used for Gravity() and falling off bar
		bool isDone;			// once last gold is collected.
		unsigned char isDrilling;
		unsigned char isPickingRight;
		unsigned char isPickingLeft;
		unsigned char isFallingRocksRight;
		unsigned char isFallingRocksLeft;
		unsigned char isSettingUpRope;
		unsigned char isUsingGooRight;
		unsigned char isGooPlatformRight;
		unsigned char isUsingGooLeft;
		unsigned char isExitingLevel;
		//SOUND* fallingSound;
		//SOUND* landingSound;
		SOUND** soundEffect;
		SOUND* music;
};

#endif