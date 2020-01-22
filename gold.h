#ifndef GOLD_H
#define GOLD_H
#include "sprite.h"

class GOLD : public SPRITE
{
	public:
		GOLD(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height);
		~GOLD();
		int renderSprite(D3DLOCKED_RECT& locked_rect);
		bool isCollected;
		bool isHidden;
};

#endif