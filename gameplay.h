#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "player.h"
#include "sound.h"

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
		int LoadLevel(unsigned int levelNbr);
		int LoadLevel(void);
		void Exit(void);
		void Gravity(void);
		void Sounds(void);
		wchar_t* GetMusicFileName(void);

	private:
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		PLAYER** player;
		SPRITE** monk;
		wchar_t** musicFileName;
		unsigned int gold;
		bool isFalling;
		SOUND* fallingSound;
		SOUND* landingSound;
};

#endif