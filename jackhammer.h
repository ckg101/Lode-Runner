#ifndef JACKHAMMER_H
#define JACKHAMMER_H
#include "sprite.h"

class JACKHAMMER : public SPRITE
{
	public:
		JACKHAMMER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~JACKHAMMER();

		bool isCollected;
};

#endif