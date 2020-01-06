#ifndef MONK_H
#define MONK_H

#include "sprite.h"

enum MONK_ROPE_STATE
{ MONK_BEGIN_ROPE_ANIMATION = 0, MONK_MIDDLE_ROPE_ANIMATION = 1, MONK_FINISH_ROPE_ANIMATION = 2};

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
	bool eatPlayer1Frame(void);
	bool hangMonkFrame(void);
	
	unsigned int goldCollected;
	bool isFalling; 
	bool isEnteringLevel;
	bool isClimbingBar;
	bool isReleased;
	unsigned int isEatingPlayer1;
	unsigned int isBeingHung;
};

#endif