#ifndef AI_H
#define AI_H
#include "gameplay.h"

class AI
{
	public:
		AI(GAMEPLAY* g);
		~AI(void);

		void Process(void);
		bool DetectCollisionWithOtherMonkRight(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkLeft(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkUp(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkDown(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkFallingDown(unsigned int monkNbr);
	private:
		GAMEPLAY* gameplay;
};

#endif