#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#define TITLESCREEN_1PLAYER_BUTTON	0
#define TITLESCREEN_2PLAYER_BUTTON	1
#define TITLESCREEN_EDITOR_BUTTON	2
#define TITLESCREEN_LOAD_BUTTON		3
#define TITLESCREEN_OPTIONS_BUTTON	4
#define TITLESCREEN_EXIT_BUTTON		5
#define TITLESCREEN_INVALID_BUTTON	10

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
	void Render(IDirect3DSurface9* &backbuffer);
};

#endif