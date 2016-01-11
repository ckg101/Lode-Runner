#ifndef PLATFORM_H
#define PLATFORM_H
#include "sprite.h"

typedef struct RESPAWN_BLOCK
{
	unsigned int nbrOfFrames;
	unsigned int ID;
	unsigned short occ;
};

class PLATFORM
{
	public:
		PLATFORM(IDirect3DDevice9* d, int screen_width, int screen_height);
		~PLATFORM();
		friend class GAMEPLAY;
		int initialize(unsigned int nbr_of_blocks, unsigned int nbr_of_types, unsigned char nbr_of_worlds);
		int addPlatform(unsigned int blockNbr, unsigned int _type);
		void deinitialize(void);
		int loadBlocks(wchar_t* name);
		int LoadBlocks(unsigned char world);
		void setBlock(unsigned int blockNbr);
		void renderPlatform(IDirect3DSurface9* &buf);
		void GetBlockCoordinates(unsigned int blockNbr, int &x, int &y);
		unsigned int getBlockNbr(int x, int y);
		unsigned short getIsOccupied(unsigned int blockNbr);
		bool getIsSelected(unsigned int blockNbr);
		unsigned int getSelectedTypeNbr(void);
		IMAGE* getImage(int type);
		unsigned char GetWorldNbr(void);
		int SetWorldNbr(unsigned char w);
		void ResetLevelToCurrentWorld(void);
		int SaveLevel(void);
		int LoadLevel(void);
		int LoadLevel(wchar_t* fileName);
		void ClearLevel(void);
		POINT GetStartingCoordinatesOfPlayer(unsigned int playerNbr);
		void SetIsPlaying(bool status);
		bool GetIsPlaying(void);
		unsigned int GetType(unsigned int blockNbr);
		void DestroyBlock(unsigned int blockNbr);
		void DestroyBlockPermanently(unsigned int blockNbr);
		void RespawnBlock(unsigned int blockNbr);
	private:
		SPRITE** blocks;
		SPRITE* temp_sprite;
		IMAGE* sheet;			// this holds the sprite sheet
		SPRITE** menu;
		SPRITE** background;
		unsigned char currentWorld;
		unsigned char nbrOfWorlds;
		unsigned short* isOccupied;
		bool* isSelected;
		unsigned int* type;		// hold the type ID of each block
		RESPAWN_BLOCK* respawnBlock;
		unsigned int nbrOfBlocks;
		unsigned int nbrOfTypes;
		int screenWidth;
		int screenHeight;
		bool isPlaying;
		IDirect3DDevice9* d3ddev;

};

#endif