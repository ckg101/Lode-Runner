#ifndef SPRITE_H
#define SPRITE_H

#include "graphics.h"

#define ANIMATION_AUTOMATIC_LOOP	1
#define ANIMATION_TRIGGERED_SEQ		2
#define BLOCK_REGULAR		0
#define BLOCK_SOLID			1
#define BLOCK_SLOW			2
#define BLOCK_HOLLOW		3
#define BLOCK_GOLD_COIN		4
#define BLOCK_GOLD_BUCKET	5
#define BLOCK_GOLD_SKULL	6
#define BLOCK_GOLD_BARS		7
#define BLOCK_LADDER		8
#define BLOCK_LADDER_END	9
#define BLOCK_BAR			10
#define BLOCK_TELEPORT_ENTRY		11
#define BLOCK_TELEPORT_EXIT			12
#define BLOCK_KEY_ORANGE			13
#define BLOCK_KEY_GREEN				14
#define BLOCK_KEY_RED				15
#define BLOCK_JACKHAMMER			16
#define BLOCK_PICK					17
#define BLOCK_ROPE					18
#define BLOCK_CAVE					19
#define BLOCK_EXIT_DOOR				20
#define BLOCK_EXIT_DOOR_ORANGE		21
#define BLOCK_EXIT_DOOR_GREEN		22
#define BLOCK_EXIT_DOOR_RED			23
#define BLOCK_BOMB_LARGE			24
#define BLOCK_BOMB_SMALL			25
#define BLOCK_GOO					26
#define BLOCK_GAS					27
#define BLOCK_PLAYER1				28
#define BLOCK_PLAYER2				29
#define BLOCK_MONK					30
#define BLOCK_MONK_RANDOM			31
#define BLOCK_SAVE_BUTTON			33
#define BLOCK_EMPTY					100
#define BLOCK_REGULAR_WITH_GOLD_COIN	(200+BLOCK_REGULAR+BLOCK_GOLD_COIN)
#define BLOCK_REGULAR_WITH_LADDER		(200+BLOCK_REGULAR+BLOCK_LADDER)
#define BLOCK_REGULAR_WITH_LADDER_END	(200+BLOCK_REGULAR+BLOCK_LADDER_END)
#define BLOCK_REGULAR_WITH_BOMB_SMALL	(200+BLOCK_REGULAR+BLOCK_BOMB_SMALL)
#define BLOCK_TELEPORT_ENTRY1		300
#define BLOCK_TELEPORT_ENTRY2		331
#define BLOCK_TELEPORT_ENTRY3		302
#define BLOCK_TELEPORT_ENTRY4		303
#define BLOCK_TELEPORT_ENTRY5		304
#define BLOCK_TELEPORT_EXIT1		305
#define BLOCK_TELEPORT_EXIT2		306
#define BLOCK_TELEPORT_EXIT3		307
#define BLOCK_TELEPORT_EXIT4		308
#define BLOCK_TELEPORT_EXIT5		309
#define IS_NOT_OCCUPIED					0
#define IS_OCCUPIED						1
#define IS_OCCUPIED_TELEPORT			2
#define IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER	3
#define IS_OCCUPIED_FULL				4

class SPRITE
{
	public:
		SPRITE(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height);
		~SPRITE();
		int loadBitmaps(wchar_t* name);
		int copyBitmaps(IMAGE* image, int frame_nbr);
		int setAnimationType(int a);
		int renderSprite(IDirect3DSurface9* &backbuffer);
		int CopyOntoBlock(IMAGE* image);
		void nextFrame(void);
		void setTransparencyColor(unsigned long color);
		IMAGE* getFrame(void);
		IMAGE* getFrame(unsigned int frameNbr);
		int x_pos;
		int y_pos;
	protected:
		IMAGE* frames;
		int nbrOfFrames;
		IDirect3DDevice9* d3ddev;
		int frameState;
		int screenWidth;
		int screenHeight;
		int animationType;
		unsigned long transparencyColor;
};



#endif