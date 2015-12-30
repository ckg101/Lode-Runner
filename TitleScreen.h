#ifndef TITLESCREEN_H
#define TITLESCREEN_H

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