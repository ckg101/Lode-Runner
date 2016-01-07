#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

class PLAYER : public SPRITE
{
public:
	PLAYER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~PLAYER();

	void nextFrame(void);
	void backFrame(void);
	void climbDownFrame(void);
	void climbUpFrame(void);
	void fallingFrame(void);
	void setFrameState(unsigned int index);
private:
	int test;
};

#endif