#ifndef CONTROLS_H
#define CONTROLS_H
#include "sprite.h"

typedef struct 
{
	long lX;
	long lY;
	long lZ;
} tPOINT;

class CONTROLS
{
	public:
		CONTROLS(IDirectInput8* d);
		~CONTROLS();
		int CreateKeyboard(HWND hWnd);
		int CreateMouse(HWND hWnd);
		unsigned char GetKeyboardInput(void);
		DIMOUSESTATE GetMouseInput(void);
	protected:
		IDirectInput8* dinput;
		IDirectInputDevice8* dinput_keyboard;
		IDirectInputDevice8* dinput_mouse;
		unsigned char keyboardState[256];
		DIMOUSESTATE mouseState;
};

class CURSOR
{
	public:
		CURSOR(IDirect3DDevice9* &d, unsigned int type_nbr, int screen_width, int screen_height);
		~CURSOR();
		int SetType(unsigned int type_nbr);
		unsigned int GetType(void);
		void Render(IDirect3DSurface9* &backbuffer);
		void SetX_Pos(int x);
		void SetY_Pos(int y);
		int blockCursor;
	protected:
		SPRITE* cursorGraphic;
		unsigned int typeNbr;			// ID for the image to be displayed as cursor from PLATFORM sheet
};

#endif