#ifndef PLATFORM_H
#define PLATFORM_H
#include "sprite.h"

class PLATFORM
{
	public:
		PLATFORM(IDirect3DDevice9* d, int screen_width, int screen_height);
		~PLATFORM();
		int initialize(unsigned int nbr_of_blocks, unsigned int nbr_of_types);
		int addPlatform(unsigned int blockNbr, unsigned int _type);
		void deinitialize(void);
		int loadBlocks(wchar_t* name);
		void setBlock(unsigned int blockNbr);
		void renderPlatform(IDirect3DSurface9* &buf);
		void GetBlockCoordinates(unsigned int blockNbr, int &x, int &y);
		unsigned int getBlockNbr(int x, int y);
		unsigned short getIsOccupied(unsigned int blockNbr);
		bool getIsSelected(unsigned int blockNbr);
		unsigned int getSelectedTypeNbr(void);
		IMAGE* getImage(int type);
	private:
		SPRITE** blocks;
		SPRITE* temp_sprite;
		IMAGE* sheet;			// this holds the sprite sheet
		SPRITE** menu;
		unsigned short* isOccupied;
		bool* isSelected;
		unsigned int* type;		// hold the type ID of each block
		unsigned int nbrOfBlocks;
		unsigned int nbrOfTypes;
		int screenWidth;
		int screenHeight;
		IDirect3DDevice9* d3ddev;

};

#endif