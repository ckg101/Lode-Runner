#ifndef LEVEL_H
#define LEVEL_H
#include "sprite.h"

class LEVEL_EDITOR
{
	public:
		LEVEL_EDITOR(IDirect3DDevice9* d, nbr_of_blocks);
		~LEVEL_EDITOR();
		unsigned int GetBlockNbr(int &x, int &y);	// this will give you the block number according
													// to the coordinates entered
		POINT GetCoordinates(unsigned int block_nbr);	// this will give you the coordinates
														// according to the block number entered
		void Render(IDirect3DSurface9* &buf);							//
	private:
		unsigned int* blocks;		// this will hold the type ID for each block
		POINT* coordinates;
		unsigned int nbrOfBlocks;
		SPRITE** graphics;			// this holds the image to be displayed

		IDirect3DDevice9* d3ddev;

};

#endif