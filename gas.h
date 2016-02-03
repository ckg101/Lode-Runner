#ifndef GAS_H
#define GAS_H
#include "sprite.h"

class GAS : public SPRITE
{
	public:
		GAS(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~GAS();

		bool isCollected;
};

class GAS_SPRAY : public SPRITE
{
	public:
		GAS_SPRAY(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		bool FrameRight(void);
		bool FrameLeft(void);
		void Reset(void);
		
};

#endif