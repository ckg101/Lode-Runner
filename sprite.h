#ifndef SPRITE_H
#define SPRITE_H

#include "graphics.h"

#define ANIMATION_AUTOMATIC_LOOP	1
#define ANIMATION_TRIGGERED_SEQ		2
#define BLOCK_REGULAR 0
#define BLOCK_SOLID	1

class SPRITE
{
	public:
		SPRITE(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height);
		~SPRITE();
		int loadBitmaps(wchar_t* name);
		int copyBitmaps(IMAGE* image, int frame_nbr);
		int setAnimationType(int a);
		int renderSprite(IDirect3DSurface9* &backbuffer);
		void nextFrame(void);
		void setTransparencyColor(unsigned long color);
		int x_pos;
		int y_pos;
	protected:
		IMAGE* frames;
		int nbrOfFrames;
		IDirect3DDevice9* d3ddev;
		int frameState;
		int screenWidth;
		int screenHeight;
		int animationType;
		unsigned long transparencyColor;
};

class PLATFORM
{
	public:
		PLATFORM(IDirect3DDevice9* d, int screen_width, int screen_height);
		~PLATFORM();
		int initialize(unsigned int nbr_of_blocks, unsigned int nbr_of_types);
		int addPlatform(unsigned int blockNbr, unsigned int type);
		void deinitialize(void);
		int loadBlocks(wchar_t* name);
		void setBlock(unsigned int blockNbr);
		void renderPlatform(IDirect3DSurface9* &buf);
		void GetBlockCoordinates(unsigned int blockNbr, int &x, int &y);
		unsigned int getBlockNbr(int x, int y);
		bool getIsOccupied(unsigned int blockNbr);
		IMAGE* getImage(int type);
	private:
		SPRITE** blocks;
		IMAGE* sheet;			// this holds the sprite sheet
		bool* isOccupied;
		unsigned int nbrOfBlocks;
		unsigned int nbrOfTypes;
		int screenWidth;
		int screenHeight;
		IDirect3DDevice9* d3ddev;

};

class CURSOR
{
	public:
		CURSOR(IDirect3DDevice9* &d, unsigned int type_nbr, int screen_width, int screen_height);
		~CURSOR();
		int SetType(unsigned int type_nbr);
		unsigned int GetType(void);
		void Render(IDirect3DSurface9* &backbuffer);
		void SetSelectionX_Pos(int x);
		void SetSelectionY_Pos(int y);
		void MoveCursorX(int qty);
		void MoveCursorY(int qty);
		int GetBlockCursor(void);
		int SetBlockCursor(int c);
		POINT GetCursorPosition(void);
	private:
		int blockCursor;
		SPRITE* cursorGraphic;
		SPRITE* selectionGraphic;
		unsigned int typeNbr;			// ID for the image to be displayed as cursor from PLATFORM sheet
		int screenWidth;
		int screenHeight;
};

#endif