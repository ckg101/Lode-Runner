#ifndef GAMEPLAY_H
#define GAMEPLAY_H

class GAMEPLAY
{
	public:
		GAMEPLAY(IDirect3DDevice9* d, PLATFORM* p, HWND &hWnd, int screen_width, int screen_height);
		~GAMEPLAY(void);
		void Render(IDirect3DSurface9* &buf);
		void MovePlayer1Right(void);
		void MovePlayer1Left(void);
		void MovePlayer1Up(void);
		void MovePlayer1Down(void);
		int LoadLevel(unsigned int levelNbr);
		void Exit(void);
		wchar_t* GetMusicFileName(void);

	private:
		IDirect3DDevice9* d3ddev;
		PLATFORM* platform;
		SPRITE** player;
		SPRITE** monk;
		wchar_t** musicFileName;
		unsigned int gold;

};

#endif