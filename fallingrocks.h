#pragma once
#include "sprite.h"

class FALLINGROCKS :
	public SPRITE
{
public:
	FALLINGROCKS(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~FALLINGROCKS(void);
	bool frame(void);
	void reset(void);

	bool isOpen;
	unsigned int blockNbr;
	unsigned int timer;
	bool fell;
};
