#ifndef AI_H
#define AI_H
#include "gameplay.h"

class AI
{
	public:
		AI(GAMEPLAY* g);
		~AI(void);

		void Process(void);
		void ProcessDumb(void);	// if DebugMode is enabled
		bool DetectCollisionWithOtherMonkRight(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkLeft(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkUp(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkDown(unsigned int monkNbr);
		bool DetectCollisionWithOtherMonkFallingDown(unsigned int monkNbr);
		void CollideWithRopetrapMonk(void);
		void MonkGravity(void);
		
	private:
		GAMEPLAY* gameplay;
};

#endif