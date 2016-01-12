#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <stdio.h>
#include <dinput.h>
#include <dsound.h>
#include <xaudio2.h>
#include <time.h>
#include "graphics.h"
#include "sound.h"
#include "controls.h"
#include "sprite.h"
#include "TitleScreen.h"
#include "cursor.h"
#include "platform.h"
#include "gameplay.h"
 
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "D3dx9.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
 
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define GAME_MODE_TITLE	0
#define GAME_MODE_EDITOR	1
#define GAME_MODE_PLAY	2

enum GAME_SPEED
{	SPEED5 = 0, SPEED4 = 25, SPEED3 = 50, SPEED2 = 75, SPEED1 = 100};
 
// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
IDirect3DSurface9* surface;
IDirectInput8* dinput;	// the pointer to our DirectInput interface
IDirectInputDevice8* dinput_keyboard;	
//IDirectSound8* dsound;
IXAudio2* xaudio;
RECT src;
//GRAPHICS* graphics;
CONTROLS* controls;
SOUND* music;
SOUND* buttonpress;
//SPRITE* sprite;
CURSOR* editorCursor;
CURSOR* titleCursor;
//BACKGROUND background;
PLATFORM* platform;
TITLESCREEN* titleScreen;
GAMEPLAY* gameplay;
int frameCounter;
int gameMode;
unsigned long seconds;
bool isRunning;

//unsigned char keyboardState[256];
 
// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
int initDInput(HWND hWnd);	// sets up and initializes DirectInput
//int initDSound(HWND hWnd);	// sets up and initializes DirectSound
int initXAudio(HWND hWnd);		// sets up and initializes XAudio
void RenderFrame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void cleanDInput(void);
void SetBackgroundColor(int r, int g, int b);
void CreateSurface();
int LoadBitmapImg(char* fileName);
IDirect3DSurface9* LoadBitmapImg2(char* fileName);
int LoadBitmapImg3(char* fileName, IDirect3DSurface9* &s);
void ProcessKeyboardInput(unsigned char k);
void ProcessMouseInput(DIMOUSESTATE* mouseState);
 
// the handle for the window, filled by a function
HWND hWnd;
 
// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);
 
// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
 
    // this struct holds information for the window class
    WNDCLASSEX wc;
	//DIMOUSESTATE mouseState;
 
    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
 
    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";
 
    // register the window class
    RegisterClassEx(&wc);
    
    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",    // name of the window class
                          L"LodeRunner",   // title of the window
                          //WS_EX_TOPMOST | WS_POPUP, 
						  WS_OVERLAPPEDWINDOW, //for windowed mode // window style
						  0,    // x-position of the window
                          0,    // y-position of the window
                          SCREEN_WIDTH,    // width of the window
                          SCREEN_HEIGHT,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL
 
    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);
 
    // set up and initialize Direct3D
    initD3D(hWnd);
	initDInput(hWnd);
	initXAudio(hWnd);
	//graphics = new GRAPHICS(d3ddev);
	controls = new CONTROLS(dinput);
	buttonpress = new SOUND(xaudio);
	buttonpress->loadWAVFile(L"Sound\\buttonpress.wav");

	music = new SOUND(xaudio);
	music->loadWAVFile(L"Sound\\LRMENU1.WAV");
	music->startWAVFile();
	
	//graphics->LoadBitmapImage(L"Graphics\\background1.bmp", background.surface, &background.parameters);
	//src = background.parameters;
	//sprite = new SPRITE(d3ddev, 9, SCREEN_WIDTH, SCREEN_HEIGHT);
	//sprite->loadBitmaps(L"Graphics\\explosion");
	//sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	editorCursor = new CURSOR(d3ddev, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
	titleCursor = new CURSOR(d3ddev, SCREEN_WIDTH, SCREEN_HEIGHT);
	controls->CreateKeyboard(hWnd);
	controls->CreateMouse(hWnd);

	platform = new PLATFORM(d3ddev, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!platform->initialize(1024, 32, 10))
		MessageBoxW(NULL, L"Error Initializing Platform", L"Error", MB_OK);
	//platform->loadBlocks(L"Graphics\\block");
	platform->LoadBlocks(WORLD_JUNGLE);
	titleScreen = new TITLESCREEN(d3ddev, SCREEN_WIDTH, SCREEN_HEIGHT);
	titleScreen->initialize();
	gameplay = new GAMEPLAY(d3ddev, xaudio, platform, hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	gameMode = GAME_MODE_TITLE;
	frameCounter = 0;
	//platform->addPlatform(0, BLOCK_REGULAR);
	//platform->addPlatform(1, 31);
	//platform->setBlock(0, 0, 0);
	//platform->setBlock(1, 100, 100);
	

    // enter the main loop:
 
    MSG msg;
	isRunning = true;
                    
    while(isRunning)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
 
        if(msg.message == WM_QUIT)
		{
			isRunning = false;
            break;
		}

		/*if(msg.message == MM_MCINOTIFY)
		{
			// if the MIDI file stops playing then repeat
			if(msg.wParam == MCI_NOTIFY_SUCCESSFUL)
			{
				if(music.getMIDIStatus())
				{
					//music.stopMIDIFile();
					music.playMIDIFile();
				}
			}
		}*/
 
        //if(KEY_DOWN(VK_ESCAPE))
	            //PostMessage(hWnd, WM_DESTROY, 0, 0);

		RenderFrame();
    }
 
    // clean up DirectX and COM
    cleanD3D();
	cleanDInput();
	//delete graphics;
	xaudio->Release();
	if(controls)
		delete controls;
	//delete wavfile;
	//delete sprite;
	if(editorCursor)
		delete editorCursor;
	if(titleCursor)
		delete titleCursor;
	if(platform)
	{
		platform->deinitialize();
		delete platform;
	}
	if(music)
		delete music;
	if(buttonpress)
		delete buttonpress;
	if(titleScreen)
		delete titleScreen;
	if(gameplay)
		delete gameplay;
    return msg.wParam;
}
 
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
				isRunning = false;
                //return 0;
            } break;
    }
 
    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}
 
// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
 
    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
 
    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // TRUE = WINDOWED
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
    // for Full Screen Mode
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
}

int initDInput(HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(GetModuleHandle(NULL), 
					   DIRECTINPUT_VERSION, 
					   IID_IDirectInput8, 
					   (void**)&dinput, 
					   NULL);
	if(hr != DI_OK)
	{
		MessageBoxW(NULL, L"DirectInput8Create fail", L"Error", MB_OK);
		return 0;
	}
	
	return 1; 

}

/*int initDSound(HWND hWnd)
{
	HRESULT hr;
	hr = DirectSoundCreate8(&DSDEVID_DefaultPlayback, &dsound, NULL);
	if(hr != DS_OK)
	{
		MessageBoxW(NULL, L"DirectSoundCreate8 Failed", L"Error", MB_OK);
		return 0;
	}
	return 1;
}*/

int initXAudio(HWND hWnd)
{
	HRESULT hr;
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	xaudio = NULL;
	hr = XAudio2Create(&xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(hr != S_OK)
	{
		MessageBoxW(NULL, L"XAudio2 Create Failed", L"Error", MB_OK);
		CoUninitialize();
		return 0;
	}

	hr = xaudio->CreateMasteringVoice(&pMasterVoice, 
								 XAUDIO2_DEFAULT_CHANNELS,
								 XAUDIO2_DEFAULT_SAMPLERATE,
								 0,
								 0,
								 NULL);
	if(hr != S_OK)
	{
		MessageBoxW(NULL, L"CreateMasteringVoice failed", L"Error", MB_OK);
		CoUninitialize();
		return 0;
	}
	return 1;
}
 
// this is the function used to render a single frame
void RenderFrame(void)
{
	unsigned long temp_time = GetTickCount() - seconds;
	do
	{
		frameCounter++;
		temp_time = GetTickCount() - seconds;

		if(gameMode == GAME_MODE_PLAY)
		{
			if(frameCounter <61)
			{
				Sleep(SPEED4);
				buttonpress->playWAVFile();
				//gameplay->Sounds();
				IDirect3DSurface9* backbuffer = NULL;
				d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
 
				d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

				gameplay->Render(backbuffer);

				ProcessKeyboardInput(controls->GetKeyboardInput());
				ProcessMouseInput(&controls->GetMouseInput());
				
				d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
				
			}
		}
		else
		{
		if(frameCounter < 61)
		{
			buttonpress->playWAVFile();
			
			IDirect3DSurface9* backbuffer = NULL;
			d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
 
			d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

			ProcessKeyboardInput(controls->GetKeyboardInput());
			ProcessMouseInput(&controls->GetMouseInput());

			if(gameMode == GAME_MODE_TITLE)
			{
				if(!music->playWAVFile())
					music->startWAVFile();
				titleScreen->Render(backbuffer);
				titleCursor->Render(backbuffer);
			}

			else if(gameMode == GAME_MODE_EDITOR)
			{
				
				//wavfile->playWAVFile();
				// clear the window to a black
		
	
	
				//render sprites
				//sprite->renderSprite(background.surface);

				/*RECT dest;
				dest.bottom = src.bottom;
				dest.top = 0;
				dest.left = 0;
				dest.right = src.right;
				d3ddev->StretchRect(background.surface, 
						&background.parameters, 
						backbuffer, 
						&dest, 
						D3DTEXF_NONE);*/
	
				platform->renderPlatform(backbuffer);
				editorCursor->Render(backbuffer);
			}
			else if(gameMode == GAME_MODE_PLAY)
			{
				gameplay->Render(backbuffer);
			}
 
			 d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
			}
		}
	}while(temp_time < 1000);

	seconds = GetTickCount();
	frameCounter=0;
}
 
// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}

void cleanDInput(void)
{
	dinput->Release();
}
 
void SetBackgroundColor(int r, int g, int b)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
    d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

void ProcessKeyboardInput(unsigned char k)
{
	if(gameMode == GAME_MODE_TITLE)
	{
		switch(k)
		{
			case DIK_ESCAPE:
				//MessageBoxW(NULL, L"Escape", L"Escape", MB_OK);
				PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
			case DIK_SPACE:
				gameMode = GAME_MODE_EDITOR;
				music->stopWAVFile();
			break;
		}
	}
	else if(gameMode == GAME_MODE_EDITOR)
	{
		switch(k)
		{
			case DIK_ESCAPE:
				//MessageBoxW(NULL, L"Escape", L"Escape", MB_OK);
				//PostMessage(hWnd, WM_DESTROY, 0, 0);
				gameMode = GAME_MODE_TITLE;
				platform->ClearLevel();
				music->startWAVFile();
				Sleep(250);
			break;
			case DIK_W:
				Sleep(500);
				if(!platform->SetWorldNbr(platform->GetWorldNbr()+1))	// go to the next world with each stroke 
				{
					platform->SetWorldNbr(WORLD_JUNGLE);	// once last world is reached the next one is the first world
					platform->LoadBlocks(WORLD_JUNGLE);
					platform->ResetLevelToCurrentWorld();
				}
				else
				{
					platform->LoadBlocks(platform->GetWorldNbr());
					platform->ResetLevelToCurrentWorld();
				}
				controls->GetKeyboardInput();
			break;
		}
	}
	else if(gameMode == GAME_MODE_PLAY)
	{
		switch(k)
		{
			case DIK_ESCAPE:
				gameMode = GAME_MODE_TITLE;
				gameplay->Exit();
				Sleep(250);
				music->startWAVFile();
				controls->GetKeyboardInput();
			break;
			case DIK_RIGHT:		// right key is pressed
				gameplay->MovePlayer1Right();
			break;
			case DIK_LEFT:		// left key is pressed
				gameplay->MovePlayer1Left();
			break;
			case DIK_DOWN:		// down key is pressed
				gameplay->MovePlayer1Down();
			break;
			case DIK_UP:		// up key is pressed
				gameplay->MovePlayer1Up();
			break;
			case DIK_X:
				gameplay->DigRightPlayer1();
			break;
			case DIK_Z:
				gameplay->DigLeftPlayer1();
			break;
			case DIK_C:
				gameplay->DrillPlayer1();
			break;
			case DIK_V:
				gameplay->PickRightPlayer1();
				Sleep(250);
			break;
		}
	}
}

void ProcessMouseInput(DIMOUSESTATE* mouseState)
{
	int x, y;
	int result;
	short res;
	//wchar_t str[256];
	POINT p;

	if(gameMode == GAME_MODE_TITLE)
	{
		titleScreen->GetButtonCoordinates(titleCursor->GetBlockCursor(), x, y);
		p = titleCursor->GetCursorPosition();	
		result = titleScreen->GetButtonNbr(p.x, p.y);
		titleCursor->SetButtonCursor(result);	
		if(result != TITLESCREEN_INVALID_BUTTON)
		{
			titleCursor->SetSelectionX_Pos(x);
			titleCursor->SetSelectionY_Pos(y);
		}
		titleCursor->MoveCursorX(mouseState->lX);
		titleCursor->MoveCursorY(mouseState->lY);

		if(mouseState->rgbButtons[0] & 0x80)
		{
			switch(result)
			{
				case TITLESCREEN_1PLAYER_BUTTON:
					buttonpress->startWAVFile();
					//music.stopMIDIFile();
					music->stopWAVFile();
					//MessageBoxW(hWnd, L"Not Implemented Yet", L"1 Player", MB_OK);
					gameplay->LoadLevel(0);
					gameMode = GAME_MODE_PLAY;
					//music.loadMIDIFile(hWnd, gameplay->GetMusicFileName());
					//music.playMIDIFile();
					controls->GetMouseInput();
				break;
				case TITLESCREEN_2PLAYER_BUTTON:
					buttonpress->startWAVFile();
					MessageBoxW(hWnd, L"Not Implemented Yet", L"2 Player", MB_OK);
					controls->GetMouseInput();
				break;
				case TITLESCREEN_EDITOR_BUTTON:
					gameMode = GAME_MODE_EDITOR;
					music->stopWAVFile();
					platform->SetIsPlaying(false);
					platform->ClearLevel();
					buttonpress->startWAVFile();
					controls->GetMouseInput();
				break;
				case TITLESCREEN_LOAD_BUTTON:
					buttonpress->startWAVFile();
					Sleep(250);
					music->stopWAVFile();
					controls->UnacquireMouse();
					gameplay->LoadLevel();
					gameMode = GAME_MODE_PLAY;
					//music.loadMIDIFile(hWnd, gameplay->GetMusicFileName());
					//music.playMIDIFile();
					controls->GetMouseInput();
				break;
				case TITLESCREEN_OPTIONS_BUTTON:
					buttonpress->startWAVFile();
					MessageBoxW(hWnd, L"Not Implemented Yet", L"Options", MB_OK);
					controls->GetMouseInput();
				break;
				case TITLESCREEN_EXIT_BUTTON:
					buttonpress->startWAVFile();
					music->stopWAVFile();
					PostMessage(hWnd, WM_DESTROY, 0, 0);
				break;
			}
		}

		
	}
	else if(gameMode == GAME_MODE_EDITOR)
	{
		platform->GetBlockCoordinates(editorCursor->GetBlockCursor(), x, y);
		// cursor for the level editor

		// Get the cursor position and then get the corresponding platform block number according
		// to the coordinates of the cursor
		p = editorCursor->GetCursorPosition();
	
		editorCursor->SetBlockCursor(platform->getBlockNbr(p.x, p.y));
		editorCursor->MoveCursorX(mouseState->lX);
		editorCursor->MoveCursorY(mouseState->lY);

		if(mouseState->rgbButtons[0] & 0x80)
		{
			res = platform->getIsOccupied(editorCursor->GetBlockCursor());
			if(res == IS_NOT_OCCUPIED || 
				res == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER || res == IS_OCCUPIED_TELEPORT)
			{
				platform->addPlatform(editorCursor->GetBlockCursor(), editorCursor->GetType());
				platform->setBlock(editorCursor->GetBlockCursor());
			}
			if(platform->getIsSelected(editorCursor->GetBlockCursor()) == true)
			{
				platform->setBlock(editorCursor->GetBlockCursor());
				editorCursor->SetType(platform->getSelectedTypeNbr());
			}
			if(platform->getBlockNbr(p.x, p.y) == BLOCK_SAVE_BUTTON+1024)
			{
				buttonpress->startWAVFile();
				//MessageBoxW(hWnd, L"Not Implemented Yet", L"SAVE", MB_OK);
				controls->UnacquireMouse();
				platform->SaveLevel();
			}
			if(platform->getBlockNbr(p.x, p.y) == BLOCK_LOAD_BUTTON+1024)
			{
				buttonpress->startWAVFile();
				//MessageBoxW(hWnd, L"Not Implemented Yet", L"LOAD", MB_OK);
				controls->UnacquireMouse();
				platform->LoadLevel();
			}
			controls->GetMouseInput();
		}
		if(mouseState->rgbButtons[1] & 0x80)
		{
			res = platform->getIsOccupied(editorCursor->GetBlockCursor());
			if(res == IS_OCCUPIED || IS_OCCUPIED_FULL ||
				res == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER || res == IS_OCCUPIED_TELEPORT)
			{
				platform->addPlatform(editorCursor->GetBlockCursor(), BLOCK_EMPTY);
			}
			//MessageBoxW(NULL, L"Not Implemented Yet", L"Remove Block", MB_OK);
			controls->GetMouseInput();
		}
		if(mouseState->rgbButtons[2] & 0x80)
		{
			//MessageBoxW(NULL, L"Middle Mouse Press", L"Action", MB_OK);
		}
		//mouseState->lX = mouseState->lX - pOrigin.lX;
		//mouseState->lY = mouseState.lY - pOrigin.lY;
	
		// move the selection box to the appropriate position
		editorCursor->SetSelectionX_Pos(x);
		editorCursor->SetSelectionY_Pos(y);
	}
	
}

