#ifndef EXITDOOR_H
#define EXITDOOR_H
#include "sprite.h"

class EXITDOOR : public SPRITE
{
	public:
		EXITDOOR(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~EXITDOOR();
		bool Frame(void);

		bool isUnlocked;
};

#endif