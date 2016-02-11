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
	for(unsigned int index = 0; index < gameplay->nbrOfMonks; index++)
	{
		if(gameplay->monk[index]->x_pos < gameplay->player[PLAYER1]->x_pos)
		{
			gameplay->MoveMonkRight(index);
		}
		else if(gameplay->monk[index]->x_pos > gameplay->player[PLAYER1]->x_pos)
		{
			gameplay->MoveMonkLeft(index);
		}
		else if(gameplay->monk[index]->y_pos < gameplay->player[PLAYER1]->y_pos)
		{
			gameplay->MoveMonkDown(index);
		}
		else if(gameplay->monk[index]->y_pos > gameplay->player[PLAYER1]->y_pos)
		{
			gameplay->MoveMonkUp(index);
		}
	}
}
