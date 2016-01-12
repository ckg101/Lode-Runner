#pragma once
#include "sprite.h"

struct IS_FALLING_ROCKS_INDEX
{
	bool is;
	unsigned int blockNbr;
};

class FALLINGROCKS :
	public SPRITE
{
public:
	FALLINGROCKS(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~FALLINGROCKS(void);
	bool frame(void);
	void reset(void);
};
