#ifndef GOO_H
#define GOO_H
#include "sprite.h"

class GOO : public SPRITE
{
	public:
		GOO(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~GOO();

		bool isCollected;
		bool isUsed;
};

class GOO_PLATFORM : public SPRITE
{
	public: 
		GOO_PLATFORM(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		bool Frame(void);
		void Reset(void);
};

#endif