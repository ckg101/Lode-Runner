#ifndef AI_H
#define AI_H
#include "gameplay.h"

class AI
{
	public:
		AI(GAMEPLAY* g);
		~AI(void);

		void Process(void);

	private:
		GAMEPLAY* gameplay;
};

#endif