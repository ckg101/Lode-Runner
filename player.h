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
	void downFrame(void);
	void upFrame(void);
	void fallingFrame(void);
};

#endif