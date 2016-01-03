#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include "controls.h"


CONTROLS::CONTROLS(IDirectInput8* d)
{
	dinput = d;
}

int CONTROLS::CreateKeyboard(HWND hWnd)
{
	HRESULT hr;
	
	hr = dinput->CreateDevice(GUID_SysKeyboard, &dinput_keyboard, NULL);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"Create Keyboard failed", L"Error", MB_OK);
		return 0;
	}
	hr = dinput_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"Set Data Format Keyboard failed", L"Error", MB_OK);
		return 0;
	}
	hr = dinput_keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"SetCooperativeLevel keyboard failed", L"Error", MB_OK);
		return 0;
	}
	hr = dinput_keyboard->Acquire();
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"Acquire keyboard failed", L"Error", MB_OK);
		return 0;
	}
	return 1;
}

int CONTROLS::CreateMouse(HWND hWnd)
{
	HRESULT hr;
	hr = dinput->CreateDevice(GUID_SysMouse, &dinput_mouse, NULL);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"Create Mouse Failed", L"Error", MB_OK);
		return 0;
	}

	hr = dinput_mouse->SetDataFormat(&c_dfDIMouse);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"SetDataFormat Mouse Failed", L"Error", MB_OK);
		return 0;
	}

	hr = dinput_mouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE  | DISCL_FOREGROUND);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"SetCooperativeLevel Mouse Failed", L"Error", MB_OK);
		return 0;
	}
	hr = dinput_mouse->Acquire();
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"Acquire Mouse Failed", L"Error", MB_OK);
		return 0;
	}
	return 1;
}

unsigned char CONTROLS::GetKeyboardInput(void)
{
	HRESULT hr;

	hr = dinput_keyboard->GetDeviceState(sizeof(keyboardState), (void*)&keyboardState);
	if(hr == DI_OK)
	{
		for(unsigned char x = 1; x <= 237; x++)
		{
			if(keyboardState[x] & 0x80)
			{
				return x;
			}
		}

	}
	else
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			dinput_keyboard->Acquire();
		}
	}
	return 0;
}

DIMOUSESTATE CONTROLS::GetMouseInput(void)
{
	HRESULT hr;
	static tPOINT pOrigin = {0}; 
	static bool bInitialized = false;
	hr = dinput_mouse->Poll();
	if(FAILED(hr))
	{
		hr = dinput_mouse->Acquire();
		while( hr == DIERR_INPUTLOST )
				hr = dinput_mouse->Acquire();
		return mouseState;
	}

	hr = dinput_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mouseState);
	if(hr == DI_OK)
	{
		if( !bInitialized )
		{
			bInitialized = true;
			pOrigin.lX = mouseState.lX;
			pOrigin.lY = mouseState.lY;
		}
	}
		
	else
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			dinput_mouse->Acquire();
		}
	}
	return mouseState;
}

CONTROLS::~CONTROLS()
{
	dinput_keyboard->Unacquire();
	dinput_keyboard->Release();
	dinput_mouse->Unacquire();
	dinput_mouse->Release();
}

void CONTROLS::UnacquireMouse(void)
{
	dinput_mouse->Unacquire();
}

void CONTROLS::AcquireMouse(void)
{
	dinput_mouse->Acquire();
}
