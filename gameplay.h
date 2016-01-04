#pragma once

class GAMEPLAY
{
	public:
		GAMEPLAY(IDirect3DDevice9* d, PLATFORM* p);
		~GAMEPLAY(void);
		void MovePlayer1Right(void);
		void MovePlayer1Left(void);
		void MovePlayer1Up(void);
		void MovePlayer1Down(void);
		int LoadLevel(unsigned int levelNbr);
	private:
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		SPRITE** player;
		SPRITE** monk;
		unsigned int gold;

};
