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
}

bool AI::DetectCollisionWithOtherMonkRight(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr)
		{
			if(gameplay->monk[monkNbr]->x_pos+24 >= gameplay->monk[index]->x_pos 
				&& gameplay->monk[monkNbr]->x_pos+24 < gameplay->monk[index]->x_pos+2 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos)
				return true;
			else if(gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos)
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
		if(index != monkNbr)
		{
			if(gameplay->monk[monkNbr]->x_pos <= gameplay->monk[index]->x_pos+24 
				&& gameplay->monk[monkNbr]->x_pos > gameplay->monk[index]->x_pos+22 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos)
				return true;
			else if(gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos 
				&& gameplay->monk[monkNbr]->y_pos == gameplay->monk[index]->y_pos)
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
		if(index != monkNbr)
			if(gameplay->monk[monkNbr]->y_pos+24 >= gameplay->monk[index]->y_pos 
				&& gameplay->monk[monkNbr]->y_pos+24 < gameplay->monk[index]->y_pos+2 
				&& gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos)
				return true;
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkUp(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr)
			if(gameplay->monk[monkNbr]->y_pos <= gameplay->monk[index]->y_pos+24 
				&& gameplay->monk[monkNbr]->y_pos > gameplay->monk[index]->y_pos+22 
				&& gameplay->monk[monkNbr]->x_pos == gameplay->monk[index]->x_pos)
				return true;
	}
	return false;
}

bool AI::DetectCollisionWithOtherMonkFallingDown(unsigned int monkNbr)
{
	// detect to see if the monk will collide with another monk
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(index != monkNbr)
			if(gameplay->monk[monkNbr]->y_pos+24 >= gameplay->monk[index]->y_pos 
				&& gameplay->monk[monkNbr]->y_pos+24 < gameplay->monk[index]->y_pos+5 
				&& gameplay->monk[monkNbr]->x_pos <= gameplay->monk[index]->x_pos+24
				&& gameplay->monk[monkNbr]->x_pos >= gameplay->monk[index]->x_pos-24)
				return true;
	}
	return false;
}
