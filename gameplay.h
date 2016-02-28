#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "platform.h"
#include "player.h"
#include "monk.h"
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
#include "ai.h"

enum SOUND_EFFECT
{ SOUND_BEGIN_LEVEL, SOUND_FALLING, SOUND_LANDING, SOUND_DIGGER, SOUND_GOLD, SOUND_DRILLING, SOUND_PICK, 
  SOUND_FALLINGROCKS, SOUND_SETROPETRAP, SOUND_PICKUP, SOUND_LASTGOLD, SOUND_ENTERPORTAL, SOUND_ENDLEVEL,
  SOUND_WALKSLOW, SOUND_GOO, SOUND_GAS, SOUND_EAT};

class GAMEPLAY
{
	public:
		friend class AI;
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
		void GooPlatformLeft(void);
		void UseGasRightPlayer1(void);
		void UseGasLeftPlayer1(void);
		void OpenExitDoor(void);
		bool MoveMonkRight(unsigned int monkNbr);
		bool MoveMonkLeft(unsigned int monkNbr);
		bool MoveMonkUp(unsigned int monkNbr);
		bool MoveMonkDown(unsigned int monkNbr);
		void MonkEatPlayer1(unsigned int monkNbr);
		int LoadLevel(unsigned int levelNbr, bool newGame);
		int LoadLevel(void);
		void Exit(void);
		void Gravity(void);
		void CollideWithMonkPlayer1(void);
		void MonkGravity(void);
		void ExitLevel(void);
		void KillPlayer1(void);
		void Sounds(void);
		wchar_t* GetMusicFileName(void);
		void Player1EntersLevel(void);
		void CollectGold(void);
		void PickupItem(void);
		void DropItem(void);
		bool leaveGameplay;
		unsigned int groupNbr;
		bool displayPlayer1;

	private:
		void UnallocateItems(void);
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		PLAYER** player;
		MONK** monk;			// enemies
		DIGGER* digger;
		FALLINGROCKS** fallingrocks;
		GOO_PLATFORM* gooPlatform;
		GAS_SPRAY* gasSpray;
		GOLD** gold;
		ROPETRAP** ropetrap;
		JACKHAMMER** jackhammer;
		PICK** pick;
		GOO** goo;
		GAS** gas;
		EXITDOOR** exitdoor;
		AI* ai;
		SPRITE* controls;

		wchar_t** musicFileName;
		wchar_t** soundFileName;
		wchar_t** levelFileName;
		unsigned int nbrOfMonks;
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
		unsigned char isGooPlatformLeft;
		unsigned char isUsingGasRight;
		unsigned char isUsingGasLeft;
		unsigned char isExitingLevel;
		//SOUND* fallingSound;
		//SOUND* landingSound;
		SOUND** soundEffect;
		SOUND* music;
};

#endif