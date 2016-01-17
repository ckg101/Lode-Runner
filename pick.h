#ifndef PICK_H
#define PICK_H
#include "sprite.h"

class PICK : public SPRITE
{
	public:
		PICK(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~PICK();

		bool isCollected;
};

#endif