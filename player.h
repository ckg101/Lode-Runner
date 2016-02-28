#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

enum PLAYERS { PLAYER1, PLAYER2, PLAYER_DIGGING };

class PLAYER : public SPRITE
{
public:
	PLAYER(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
	~PLAYER();

	void nextFrame(void);
	void backFrame(void);
	void nextBarFrame(void);
	void backBarFrame(void);
	void climbDownFrame(void);
	void climbUpFrame(void);
	void fallingFrame(void);
	void setFrameState(unsigned int index);
	bool enterLevel(void);
	bool digRightFrame(void);
	bool digLeftFrame(void);
	bool drillFrame(void);
	bool pickRightFrame(void);
	bool pickLeftFrame(void);
	bool ropeFrame(void);
	bool gooRightFrame(void);
	bool gooLeftFrame(void);
	unsigned int goldCollected;
	unsigned int itemHeld;
	unsigned int lives;
};

#endif