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
		void UnacquireMouse(void);
		void AcquireMouse(void);
		unsigned char GetKeyboardInput(void);
		DIMOUSESTATE GetMouseInput(void);
	protected:
		IDirectInput8* dinput;
		IDirectInputDevice8* dinput_keyboard;
		IDirectInputDevice8* dinput_mouse;
		unsigned char keyboardState[256];
		DIMOUSESTATE mouseState;
};

#endif