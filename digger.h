#ifndef DIGGER_H
#define DIGGER_H

#include "sprite.h"

class DIGGER :
	public SPRITE
{
public:
	DIGGER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~DIGGER(void);
	bool digRightFrame(void);
	bool digLeftFrame(void);


};

#endif
