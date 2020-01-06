#include "AI.h"

AI::AI(GAMEPLAY* g)
{
	gameplay = g;
}

AI::~AI(void)
{
}

void AI::Process(void)
{
	bool res = false;
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(gameplay->monk[index]->x_pos < gameplay->player[PLAYER1]->x_pos)		// if player is to the right of the monk
		{
			res = DetectCollisionWithOtherMonkRight(index);
			if(gameplay->monk[index]->y_pos < gameplay->player[PLAYER1]->y_pos && res == false)
			{
				res = DetectCollisionWithOtherMonkDown(index);
				if(res == false)
					if(gameplay->MoveMonkDown(index) == false)
						gameplay->MoveMonkRight(index);
			}
			else if(gameplay->monk[index]->y_pos > gameplay->player[PLAYER1]->y_pos && res == false)
			{
				res = DetectCollisionWithOtherMonkUp(index);
				if(res == false)
					if(gameplay->MoveMonkUp(index) == false)
						gameplay->MoveMonkRight(index);
			}
			else
			{
				if(res == false)
					gameplay->MoveMonkRight(index);
			}
		
		}
		else if(gameplay->monk[index]->x_pos > gameplay->player[PLAYER1]->x_pos)	// if player is to the left of the monk
		{
			res = DetectCollisionWithOtherMonkLeft(index);
			if(gameplay->monk[index]->y_pos < gameplay->player[PLAYER1]->y_pos && res == false)
			{
				res = DetectCollisionWithOtherMonkDown(index);
				if(res == false)
					if(gameplay->MoveMonkDown(index) == false)
						gameplay->MoveMonkLeft(index);
			}
			else if(gameplay->monk[index]->y_pos > gameplay->player[PLAYER1]->y_pos && res == false)
			{
				res = DetectCollisionWithOtherMonkUp(index);
				if(res == false)
					if(gameplay->MoveMonkUp(index) == false)
						gameplay->MoveMonkLeft(index);
			}
			else
			{
				if(res == false)
					gameplay->MoveMonkLeft(index);
			}
		}
		else if(gameplay->monk[index]->y_pos < gameplay->player[PLAYER1]->y_pos)
		{
			res = DetectCollisionWithOtherMonkDown(index);
			if(res == false)
				gameplay->MoveMonkDown(index);
		}
		else if(gameplay->monk[index]->y_pos > gameplay->player[PLAYER1]->y_pos)
		{
			res = DetectCollisionWithOtherMonkUp(index);
			if(res == false)
				gameplay->MoveMonkUp(index);
		}
	}
	MonkGravity();
	CollideWithRopetrapMonk();
}

void AI::ProcessDumb(void)
{
	MonkGravity();
	CollideWithRopetrapMonk();
}

bool AI::DetectCollisionWithOtherMonkRight(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr && gameplay->monk[monkNbr]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
		{
			if(gameplay->monk[monkNbr]->x_pos+24 >= gameplay->monk[index]->x_pos 
				&& gameplay->monk[monkNbr]->x_pos+24 < gameplay->monk[index]->x_pos+2 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
				return true;
			else if(gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
			{
				gameplay->monk[index]->x_pos-=24;
				return false;
			}
		}
		
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkLeft(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr && gameplay->monk[monkNbr]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
		{
			if(gameplay->monk[monkNbr]->x_pos <= gameplay->monk[index]->x_pos+24 
				&& gameplay->monk[monkNbr]->x_pos > gameplay->monk[index]->x_pos+22 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
				return true;
			else if(gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
			{
				gameplay->monk[index]->x_pos+=24;
				return false;
			}
		}
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkDown(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr && gameplay->monk[monkNbr]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
			if(gameplay->monk[monkNbr]->y_pos+24 >= gameplay->monk[index]->y_pos 
				&& gameplay->monk[monkNbr]->y_pos+24 < gameplay->monk[index]->y_pos+2 
				&& gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
				return true;
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkUp(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr && gameplay->monk[monkNbr]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
			if(gameplay->monk[monkNbr]->y_pos <= gameplay->monk[index]->y_pos+24 
				&& gameplay->monk[monkNbr]->y_pos > gameplay->monk[index]->y_pos+22 
				&& gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
				return true;
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkFallingDown(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr && gameplay->monk[monkNbr]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
			if(gameplay->monk[monkNbr]->y_pos+24 >= gameplay->monk[index]->y_pos 
				&& gameplay->monk[monkNbr]->y_pos+24 < gameplay->monk[index]->y_pos+5 
				&& gameplay->monk[monkNbr]->x_pos <= gameplay->monk[index]->x_pos+24
				&& gameplay->monk[monkNbr]->x_pos >= gameplay->monk[index]->x_pos-24 && gameplay->monk[index]->isBeingHung == MONK_BEGIN_ROPE_ANIMATION)
				return true;
	}
	return false;
}

void AI::CollideWithRopetrapMonk(void)
{
	for (unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
		for (unsigned int index2 = 0; index2 < gameplay->nbrOfRopetrap; index2++)
			if (gameplay->monk[index]->x_pos + 12 >= gameplay->ropetrap[index2]->x_pos + 8 && 
				gameplay->monk[index]->x_pos + 12 <= gameplay->ropetrap[index2]->x_pos + 16
				&& gameplay->monk[index]->y_pos == gameplay->ropetrap[index2]->y_pos)
			{
				if (gameplay->ropetrap[index2]->isSet == true && gameplay->ropetrap[index2]->isUsed == false)
				{
					// if monk collides with unused set rope trap then hang the monk
					if (gameplay->HangMonk(index) == true)
						gameplay->ropetrap[index2]->isUsed = true;
					gameplay->ropetrap[index2]->isCollected = true;	// set ropetrap as collected so it will not draw to screen after being used
				}
			}

}

void AI::MonkGravity(void)
{
	unsigned int res, res2, res3, blockNbr, currentBlockNbr;
	int x, y;
	bool collision;

	for (unsigned int monkNbr = 0; monkNbr < gameplay->nbrOfMonks; monkNbr++)
	{
		blockNbr = gameplay->platform->getBlockNbr(gameplay->monk[monkNbr]->x_pos + 12, gameplay->monk[monkNbr]->y_pos + 24);
		res = gameplay->platform->GetType(blockNbr);
		// get ID of current block
		res2 = gameplay->platform->GetType(gameplay->platform->getBlockNbr(gameplay->monk[monkNbr]->x_pos + 12, gameplay->monk[monkNbr]->y_pos));

		currentBlockNbr = gameplay->platform->getBlockNbr(gameplay->monk[monkNbr]->x_pos, gameplay->monk[monkNbr]->y_pos);
		res3 = gameplay->platform->GetType(currentBlockNbr);

		if (gameplay->monk[monkNbr]->isClimbingBar == false)
		{
			if (res3 == BLOCK_BAR && res != BLOCK_LADDER)
			{
				if (gameplay->monk[monkNbr]->isReleased == false)
				{
					gameplay->platform->GetBlockCoordinates(currentBlockNbr, x, y);
					gameplay->monk[monkNbr]->y_pos = y;
					gameplay->monk[monkNbr]->x_pos = x;
					//monk[monkNbr]->setFrameState(73);
					gameplay->monk[monkNbr]->isClimbingBar = true;
					gameplay->monk[monkNbr]->isFalling = false;
					return;
				}
			}
			else
				gameplay->monk[monkNbr]->isReleased = false;

			if ((res == BLOCK_EMPTY || res == BLOCK_HOLLOW || res == BLOCK_BAR) && (res2 != BLOCK_LADDER) && (gameplay->monk[monkNbr]->y_pos <= 743))
			{
				blockNbr = gameplay->platform->getBlockNbr(gameplay->monk[monkNbr]->x_pos + 12, gameplay->monk[monkNbr]->y_pos);
				gameplay->platform->GetBlockCoordinates(blockNbr, x, y);
				gameplay->monk[monkNbr]->x_pos = x;
				if (gameplay->monk[monkNbr]->y_pos + 5 > 745)
				{
					blockNbr = gameplay->platform->getBlockNbr(gameplay->monk[monkNbr]->x_pos, 760);
					gameplay->platform->GetBlockCoordinates(blockNbr, x, y);
					gameplay->monk[monkNbr]->y_pos = y;
				}
				else
				{
					if (DetectCollisionWithOtherMonkFallingDown(monkNbr) == false)
						gameplay->monk[monkNbr]->y_pos += 5;
				}
				gameplay->monk[monkNbr]->fallingFrame();

				gameplay->monk[monkNbr]->isFalling = true;
			}
			else
			{
				if (gameplay->monk[monkNbr]->isFalling == true)
				{
					gameplay->monk[monkNbr]->isReleased = false;
					gameplay->monk[monkNbr]->setFrameState(0);
				}
				gameplay->monk[monkNbr]->isFalling = false;
			}
		}
	}
}
