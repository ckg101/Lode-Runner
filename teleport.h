#pragma once
#include "sprite.h"
class TELEPORT :
	public SPRITE
{
	public:
		TELEPORT(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height);
		~TELEPORT();
};

