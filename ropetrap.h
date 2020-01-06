#ifndef ROPETRAP_H
#define ROPETRAP_H
#include "sprite.h"

class ROPETRAP : public SPRITE
{
	public:
		ROPETRAP(IDirect3DDevice9 *d, int nbr_of_frames, int screen_width, int screen_height);
		~ROPETRAP();
		void SetFrameStateToSet(void);
		void SetFrameStateToNotSet(void);

		bool isCollected;
		bool isSet;
		bool isUsed;
};

#endif