#ifndef TITLESCREEN_H
#define TITLESCREEN_H

enum TITLE_SCREEN_BUTTONS {TITLESCREEN_1PLAYER_BUTTON, 
							TITLESCREEN_2PLAYER_BUTTON, 
							TITLESCREEN_EDITOR_BUTTON,
							TITLESCREEN_LOAD_BUTTON,
							TITLESCREEN_OPTIONS_BUTTON,
							TITLESCREEN_EXIT_BUTTON,
							TITLESCREEN_INVALID_BUTTON = 10};

class TITLESCREEN
{
	public:
		TITLESCREEN(IDirect3DDevice9* d, int screen_width, int screen_height);
		~TITLESCREEN(void); 
		int initialize(void);
		void GetButtonCoordinates(unsigned int blockNbr, int &x, int &y);
		unsigned int GetButtonNbr(int x, int y);
		//unsigned int getBlockNbr(int x, int y);
		//unsigned char getIsOccupied(unsigned int blockNbr);
		//bool getIsSelected(unsigned int blockNbr);
		//unsigned int getSelectedTypeNbr(void);
		//IMAGE* getImage(int type);
	private:
		SPRITE* titleScreenImage;
		POINT* buttonCoordinate;
		unsigned int nbrOfButtons;
public:
	// Copy image to backbuffer to be displayed
	void Render(D3DLOCKED_RECT &backbuffer);
};

#endif