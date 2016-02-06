#ifndef MONK_H
#define MONK_H

#include "sprite.h"

class MONK : public SPRITE
{
public:
	MONK(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~MONK();

	void nextFrame(void);
	void backFrame(void);
	void nextBarFrame(void);
	void backBarFrame(void);
	void climbDownFrame(void);
	void climbUpFrame(void);
	void fallingFrame(void);
	void setFrameState(unsigned int index);
	bool enterLevel(void);
	
	unsigned int goldCollected;
};

#endif