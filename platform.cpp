#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
#include <xaudio2.h>
#include <stdio.h>
#include <math.h>
#include <comdef.h>
#include <ShObjIdl.h>

#include "platform.h"
////////////////////////////////////////////////////////////////////////////////////////////
// PLATFORM CLASS
////////////////////////////////////////////////////////////////////////////////////////////

PLATFORM::PLATFORM(IDirect3DDevice9* d, int screen_width, int screen_height)
{
	d3ddev = d;
	screenWidth = screen_width;
	screenHeight = screen_height;
	nbrOfBlocks = 0;
	nbrOfWorlds = 0;
	blocks = NULL;
	isOccupied = NULL;
	isSelected = NULL;
	background = NULL;
	sheet = NULL;
	menu = NULL;
	type = NULL;
	respawnBlock = NULL;
	temp_sprite = NULL;
	currentWorld = WORLD_JUNGLE;
	isPlaying = false;
}

PLATFORM::~PLATFORM()
{
	if(blocks)
		free(blocks);
	if(isOccupied)
		free(isOccupied);
	if(sheet)
		free(sheet);
	if(isSelected)
		free(isSelected);
	if(type)
		free(type);
	if(respawnBlock)
		free(respawnBlock);
	if(background)
		free(background);
}

// nbr_of_blocks MUST BE SQUARE
int PLATFORM::initialize(unsigned int nbr_of_blocks, unsigned int nbr_of_types, unsigned char nbr_of_worlds)
{
	// initial coordinates of the first block
	int x = 0;
	int y = 0;
	int counter = 0;
	blocks = (SPRITE**)malloc(sizeof(SPRITE*) * nbr_of_blocks);
	if(blocks == NULL)
		return 0;
	
	type = (unsigned int*)malloc(sizeof(unsigned int) * nbr_of_blocks);
	if(type == NULL)
		return -1;
	for(unsigned int index = 0; index < nbr_of_blocks; index++)
		type[index] = BLOCK_EMPTY;

	respawnBlock = (RESPAWN_BLOCK*)malloc(sizeof(RESPAWN_BLOCK) * nbr_of_blocks);
	if(respawnBlock == NULL)
		return -1;
	memset(respawnBlock,0,sizeof(unsigned int) * nbr_of_blocks);

	// set each block as false to represent an empty block
	isOccupied = (unsigned short*)malloc(sizeof(unsigned short) * nbr_of_blocks);
	if(isOccupied == NULL)
		return -1;
	memset((unsigned short*)isOccupied, IS_NOT_OCCUPIED, sizeof(unsigned short) * nbr_of_blocks);
	nbrOfBlocks = nbr_of_blocks;

	nbrOfTypes = nbr_of_types;

	for(unsigned int index = 0; index < nbrOfBlocks; index++)
	{
		blocks[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
		blocks[index]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
		if(blocks[index] == NULL)
			return -2;
	}
	
	for(unsigned int index_x = 0; index_x < (unsigned int)sqrt((double)nbrOfBlocks); index_x++)
	{
		for(unsigned int index_y = 0; index_y < (unsigned int)sqrt((double)nbrOfBlocks); index_y++)
		{
			// set the coordinates of the blocks
			blocks[counter]->x_pos = x;
			blocks[counter]->y_pos = y;
			x+=24;
			counter++;
		}
		y+=24;
		x = 0;
	}

	// load sprite sheet
	sheet = (IMAGE*)malloc(sizeof(IMAGE) * nbrOfTypes);
	if(sheet == NULL)
		return -3;

	menu = (SPRITE**)malloc(sizeof(SPRITE*) * (nbrOfTypes+3));
	if(menu == NULL)
		return -4;
	
	memset(menu, 0, sizeof(SPRITE*)*(nbrOfTypes+3));

	isSelected = (bool*)malloc(sizeof(bool) * nbr_of_types);
	if(isSelected == NULL)
		return -5;
	memset((bool*)isSelected, (bool)false, sizeof(bool) * nbr_of_types);
	
	for(unsigned int index = 0; index < nbrOfTypes; index++)
	{
		sheet[index].image = NULL;
	}

	// load the backgrounds
	nbrOfWorlds = nbr_of_worlds;
	background = (SPRITE**) malloc(sizeof(SPRITE*) * nbrOfWorlds);
	wchar_t txt[255];
	for(unsigned int index = 0; index < nbrOfWorlds; index++)
	{
		background[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
		if(background[index] == NULL)
			return -8;
		wsprintf(txt, L"Graphics\\world%d\\world%d_", index+1,index+1);
		background[index]->loadBitmaps(txt);
		background[index]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	}

	temp_sprite = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(temp_sprite == NULL)
		return -7;

	return 1;
}

void PLATFORM::deinitialize(void)
{
	unsigned int index;
	if(blocks == NULL)
		return;
	for(index = 0; index < nbrOfBlocks; index++)
	{
		if(blocks[index])
			delete blocks[index];
	}
	for(index = 0; index < nbrOfTypes; index++)
	{
		if(sheet[index].image)
			free(sheet[index].image);
	}
	for(index = 0; index < nbrOfTypes+3; index++)
	{
		if(menu[index])
		{
			delete menu[index];
			menu[index] = NULL;
		}
	}
	for(index = 0; index < nbrOfWorlds; index++)
	{
		if(background[index])
		{
			delete background[index];
			background[index] = NULL;
		}
	}
	delete temp_sprite;
	if(menu)
		free(menu);
}

int PLATFORM::loadBlocks(wchar_t* name)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	FILE* fp = NULL;
	unsigned long* lBits;
	int counter;
	int counter2;
	wchar_t fileName[255];
	unsigned char* temp_image = NULL;
	unsigned long* ltemp_image = NULL;
	unsigned int index = 0;

	while(index < nbrOfTypes)
	{
		
		counter = 0;
		counter2 = 0;
		swprintf(fileName, L"%s%d_0.bmp", name,index+1);
		fp = _wfopen(fileName, L"rb");
		if (fp == NULL)
		{
			MessageBox(NULL, L"file not exist", fileName, MB_OK);
			return 0;
		}
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

		sheet[index].image = (unsigned long*)malloc(bih.biHeight * bih.biWidth * 4);
		temp_image = (unsigned char*)malloc(bfh.bfSize);
		if(sheet[index].image == NULL)
		{
			MessageBox(NULL, L"Malloc image Error", L"error", MB_OK);
			return 0;
		}

		fread((unsigned char*)temp_image, bfh.bfSize, 1, fp);
		fclose(fp);

		// create memory for image
		
		sheet[index].parameters.top = 0;
		sheet[index].parameters.bottom = bih.biHeight;
		sheet[index].parameters.left = 0;
		sheet[index].parameters.right = bih.biWidth;

		//frames->surface->LockRect(&locked_rect, NULL, 0);
		//pBits = (unsigned char*) locked_rect.pBits;
		lBits = sheet[index].image;
		//pBits -= locked_rect.Pitch;
		//lBits = (unsigned long*) frames[index].image;

		//counter = bih.biWidth * bih.biHeight;

		for(int y = 0; y < bih.biHeight; y++)
		{
			for(int x = 0; x < bih.biWidth; x ++)
			{
				lBits[counter] = D3DCOLOR_XRGB(temp_image[counter2+2],temp_image[counter2+1],temp_image[counter2]);
				counter2+=3;
				counter++;
			}
		}

		free(temp_image);


		// flip image becuase BMP is stored
		// upside down
		counter = bih.biWidth * bih.biHeight - 1;
		counter2 = 0;
		ltemp_image = (unsigned long*)malloc(bih.biWidth * bih.biHeight * 4);
		for(int y = bih.biHeight; y > 0; y--)
		{
			for(int x = 0; x < bih.biWidth; x++)
			{
				ltemp_image[counter2] = lBits[counter];
				counter--;
				counter2++;
			}
		}

		memcpy((unsigned long*)lBits, (unsigned long*)ltemp_image, sizeof(unsigned long) * bih.biWidth * bih.biHeight);

		free(ltemp_image);
		
		index++;
	}

	int x, y;
	
	// load the menu
	
	for(unsigned int index = 0; index < nbrOfTypes; index++)
	{
		if(index == BLOCK_TELEPORT_ENTRY)
		{
			menu[BLOCK_TELEPORT_ENTRY] = new SPRITE(d3ddev, 6, screenWidth, screenHeight);
			if(menu[BLOCK_TELEPORT_ENTRY] == NULL)
				return -1;
			menu[BLOCK_TELEPORT_ENTRY]->loadBitmaps(_wcsdup(L"Graphics\\block12_"));
			menu[BLOCK_TELEPORT_ENTRY]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
		}
		else if(index == BLOCK_TELEPORT_EXIT)
		{
			menu[index] = new SPRITE(d3ddev, 6, screenWidth, screenHeight);
			if(menu[index] == NULL)
				return -1;
			menu[index]->loadBitmaps(_wcsdup(L"Graphics\\block13_"));
			menu[index]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
		}
		else
		{
			menu[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
			if(menu[index] == NULL)
				return -1;
			menu[index]->copyBitmaps(&sheet[index], 0);
		}
	}
	
	menu[nbrOfTypes] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes] == NULL)
		return -1;

	menu[nbrOfTypes]->loadBitmaps(_wcsdup(L"Graphics\\block34_"));
	menu[nbrOfTypes]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

	menu[nbrOfTypes+1] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes+1] == NULL)
		return -1;

	menu[nbrOfTypes+1]->loadBitmaps(_wcsdup(L"Graphics\\block35_"));
	menu[nbrOfTypes+1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

	x = 768;
	y = 0;
	counter = 0;
	menu[counter]->x_pos = x;
	menu[counter]->y_pos = y;

	for(unsigned int index_y = 0; index_y < 7; index_y++)
	{
		for(unsigned int index_x = 0; index_x < 5; index_x++)
		{
			// set the coordinates of the blocks
			menu[counter]->x_pos = x;
			menu[counter]->y_pos = y;
			x+=24;
			counter++;
			if(counter == nbrOfTypes)
				break;
		}
		y+=24;
		x=768;
	}

	menu[nbrOfTypes+1]->x_pos = x;
	menu[nbrOfTypes+1]->y_pos = y;
	
	return 1;
}

int PLATFORM::LoadBlocks(unsigned char world)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	FILE* fp = NULL;
	unsigned long* lBits;
	int counter;
	int counter2;
	wchar_t fileName[255];
	unsigned char* temp_image = NULL;
	unsigned long* ltemp_image = NULL;
	unsigned int index = 0;

	for(index = 0; index < nbrOfTypes; index++)
	{
		if(sheet[index].image)
		{
			free(sheet[index].image);
			sheet[index].image = NULL;
		}
		if(menu[index])
		{
			delete menu[index];
			menu[index] = NULL;
		}
	}

	index = 0;
	while(index < nbrOfTypes)
	{
		
		counter = 0;
		counter2 = 0;
		if((index >=0 && index <= 3) || (index == 19))
			swprintf(fileName, L"Graphics\\world%d\\block%d_0.bmp", world+1,index+1);
		else
			swprintf(fileName, L"Graphics\\block%d_0.bmp", index+1);
		fp = _wfopen(fileName, L"rb");
		if (fp == NULL)
		{
			MessageBox(NULL, L"file not exist", fileName, MB_OK);
			return 0;
		}
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

		sheet[index].image = (unsigned long*)malloc(bih.biHeight * bih.biWidth * 4);
		temp_image = (unsigned char*)malloc(bfh.bfSize);
		if(sheet[index].image == NULL)
		{
			MessageBox(NULL, L"Malloc image Error", L"error", MB_OK);
			return 0;
		}

		fread((unsigned char*)temp_image, bfh.bfSize, 1, fp);
		fclose(fp);

		// create memory for image
		
		sheet[index].parameters.top = 0;
		sheet[index].parameters.bottom = bih.biHeight;
		sheet[index].parameters.left = 0;
		sheet[index].parameters.right = bih.biWidth;

		//frames->surface->LockRect(&locked_rect, NULL, 0);
		//pBits = (unsigned char*) locked_rect.pBits;
		lBits = sheet[index].image;
		//pBits -= locked_rect.Pitch;
		//lBits = (unsigned long*) frames[index].image;

		//counter = bih.biWidth * bih.biHeight;

		for(int y = 0; y < bih.biHeight; y++)
		{
			for(int x = 0; x < bih.biWidth; x ++)
			{
				lBits[counter] = D3DCOLOR_XRGB(temp_image[counter2+2],temp_image[counter2+1],temp_image[counter2]);
				counter2+=3;
				counter++;
			}
		}

		free(temp_image);


		// flip image becuase BMP is stored
		// upside down
		/*counter = bih.biWidth * bih.biHeight - 1;
		counter2 = 0;
		ltemp_image = (unsigned long*)malloc(bih.biWidth * bih.biHeight * 4);
		for(int y = bih.biHeight; y > 0; y--)
		{
			for(int x = 0; x < bih.biWidth; x++)
			{
				ltemp_image[counter2] = lBits[counter];
				counter--;
				counter2++;
			}
		}*/

		ltemp_image = (unsigned long*)malloc(bih.biWidth * bih.biHeight * 4);
		counter = bih.biWidth * bih.biHeight-bih.biWidth;		// last pixel
		counter2 = 0;
		
		while(counter2 < bih.biWidth * bih.biHeight)
		{
			for(int x = 0; x < bih.biWidth; x++)
			{
				ltemp_image[counter2] = lBits[counter];
				counter++;
				counter2++;
			}
			
			counter-=bih.biWidth*2;
		}


		memcpy((unsigned long*)lBits, (unsigned long*)ltemp_image, sizeof(unsigned long) * bih.biWidth * bih.biHeight);

		free(ltemp_image);
		
		index++;
	}

	int x, y;
	
	// load the menu
	
	for(unsigned int index = 0; index < nbrOfTypes; index++)
	{
		if(index == BLOCK_TELEPORT_ENTRY)
		{
			menu[BLOCK_TELEPORT_ENTRY] = new SPRITE(d3ddev, 6, screenWidth, screenHeight);
			if(menu[BLOCK_TELEPORT_ENTRY] == NULL)
				return -1;
			menu[BLOCK_TELEPORT_ENTRY]->loadBitmaps(_wcsdup(L"Graphics\\block12_"));
			menu[BLOCK_TELEPORT_ENTRY]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
		}
		else if(index == BLOCK_TELEPORT_EXIT)
		{
			menu[index] = new SPRITE(d3ddev, 6, screenWidth, screenHeight);
			if(menu[index] == NULL)
				return -1;
			menu[index]->loadBitmaps(_wcsdup(L"Graphics\\block13_"));
			menu[index]->setAnimationType(ANIMATION_TRIGGERED_SEQ);
		}
		else
		{
			menu[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
			if(menu[index] == NULL)
				return -1;
			menu[index]->copyBitmaps(&sheet[index], 0);
		}
	}
	
	menu[nbrOfTypes] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes] == NULL)
		return -1;

	menu[nbrOfTypes]->loadBitmaps(_wcsdup(L"Graphics\\block34_"));
	menu[nbrOfTypes]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

	menu[nbrOfTypes+1] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes+1] == NULL)
		return -1;

	menu[nbrOfTypes+1]->loadBitmaps(_wcsdup(L"Graphics\\block35_"));
	menu[nbrOfTypes+1]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

	menu[nbrOfTypes+2] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes+2] == NULL)
		return -1;
	menu[nbrOfTypes+2]->loadBitmaps(_wcsdup(L"Graphics\\block36_"));
	menu[nbrOfTypes+2]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

	x = 768;
	y = 0;
	counter = 0;
	menu[counter]->x_pos = x;
	menu[counter]->y_pos = y;

	for(unsigned int index_y = 0; index_y < 7; index_y++)
	{
		for(unsigned int index_x = 0; index_x < 5; index_x++)
		{
			// set the coordinates of the blocks
			menu[counter]->x_pos = x;
			menu[counter]->y_pos = y;
			x+=24;
			counter++;
			if(counter == nbrOfTypes)
				break;
		}
		y+=24;
		x=768;
	}

	menu[nbrOfTypes+1]->x_pos = x;
	menu[nbrOfTypes+1]->y_pos = y;
	menu[nbrOfTypes+2]->x_pos = x+24;
	menu[nbrOfTypes+2]->y_pos = y;
	
	return 1;
}

int PLATFORM::addPlatform(unsigned int blockNbr, unsigned int _type)
{
	if(blockNbr >= nbrOfBlocks)
		return 0;
	if(_type >= nbrOfTypes && _type != BLOCK_EMPTY)
		return -1;

	if(_type == BLOCK_EMPTY)
	{
		type[blockNbr] = BLOCK_EMPTY;
		isOccupied[blockNbr] = IS_NOT_OCCUPIED;
		return 1;
	}
	//Check to see if a teleport entry block is placed already
	//then set the teleport number.  Maximum amount of teleports allowed in each level are 5
	if(isOccupied[blockNbr] == IS_OCCUPIED_TELEPORT && _type == BLOCK_TELEPORT_ENTRY)
	{
		Sleep(250);
		if(type[blockNbr] >= BLOCK_TELEPORT_ENTRY1 && type[blockNbr] <= BLOCK_TELEPORT_ENTRY5)
		{
			blocks[blockNbr]->copyBitmaps(menu[BLOCK_TELEPORT_ENTRY]->getFrame(type[blockNbr]-300+1), 0);
			type[blockNbr]++;
		}
		else
		{
			blocks[blockNbr]->copyBitmaps(menu[BLOCK_TELEPORT_ENTRY]->getFrame(1), 0);
			type[blockNbr] = BLOCK_TELEPORT_ENTRY1;
		}
	}

	// do the same for the exit teleport block
	else if(isOccupied[blockNbr] == IS_OCCUPIED_TELEPORT &&
		_type == BLOCK_TELEPORT_EXIT)
	{
		Sleep(250);
		if(type[blockNbr] >= BLOCK_TELEPORT_EXIT1 && type[blockNbr] <= BLOCK_TELEPORT_EXIT5)
		{
			blocks[blockNbr]->copyBitmaps(menu[BLOCK_TELEPORT_EXIT]->getFrame(type[blockNbr]-305+1), 0);
			type[blockNbr]++;
		}
		else
		{
			blocks[blockNbr]->copyBitmaps(menu[BLOCK_TELEPORT_EXIT]->getFrame(1), 0);
			type[blockNbr] = BLOCK_TELEPORT_EXIT1;
		}
	}
	// if the type of block does not allow another block to be placed inside
	else if(isOccupied[blockNbr] != IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER)
	{
		//blocks[blockNbr]->copyBitmaps(&sheet[_type], 0);
		blocks[blockNbr]->copyBitmaps(menu[_type]->getFrame(0), 0);
		type[blockNbr] = _type;
	}
	else if(isOccupied[blockNbr] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER)
	{
		// filter out which types can be added as a second layers
		if(_type == BLOCK_GOLD_COIN || _type == BLOCK_LADDER || _type == BLOCK_LADDER_END || _type == BLOCK_BOMB_SMALL)
		{
			temp_sprite->copyBitmaps(&sheet[_type], 0);
			temp_sprite->x_pos = blocks[blockNbr]->x_pos;
			temp_sprite->y_pos = blocks[blockNbr]->y_pos;
			temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			temp_sprite->CopyOntoBlock(blocks[blockNbr]->getFrame(), 24, 24);
			//if(type[blockNbr] != 0)
				type[blockNbr] = type[blockNbr] + 200 + _type;
		}
	}
		
	return 1;
}

void PLATFORM::setBlock(unsigned int blockNbr)
{
	if(blockNbr >= 0 && blockNbr < 1024)
	{
		if(isOccupied[blockNbr] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER)
			isOccupied[blockNbr]= IS_OCCUPIED_FULL;
		else if(isOccupied[blockNbr] == IS_NOT_OCCUPIED)
			isOccupied[blockNbr] = IS_OCCUPIED;
		if(type[blockNbr] == BLOCK_REGULAR)		// if its a regular ground block, allow another layer
			isOccupied[blockNbr] = IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER;
		if(type[blockNbr] == BLOCK_TELEPORT_ENTRY || 
			(type[blockNbr] >= BLOCK_TELEPORT_ENTRY1 && type[blockNbr] <= BLOCK_TELEPORT_ENTRY5) )
			isOccupied[blockNbr] = IS_OCCUPIED_TELEPORT;
		if(type[blockNbr] == BLOCK_TELEPORT_EXIT || 
			(type[blockNbr] >= BLOCK_TELEPORT_EXIT1 && type[blockNbr] <= BLOCK_TELEPORT_EXIT5) )
			isOccupied[blockNbr] = IS_OCCUPIED_TELEPORT;
		blocks[blockNbr]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	}
	else if(blockNbr >= 1024 && blockNbr < 1024+nbrOfTypes)
	{
		memset((bool*)isSelected, (bool)false, sizeof(bool)*nbrOfTypes);
		isSelected[blockNbr-1024] = true;
		menu[blockNbr-1024]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
		menu[nbrOfTypes]->x_pos = menu[blockNbr-1024]->x_pos;
		menu[nbrOfTypes]->y_pos = menu[blockNbr-1024]->y_pos;
	}
}

void PLATFORM::renderPlatform(D3DLOCKED_RECT &buf)
{
	unsigned int index;

	background[currentWorld]->renderSprite(buf);

	for(index = 0; index < nbrOfBlocks; index++)
	{	
		if(isOccupied[index] == IS_OCCUPIED || isOccupied[index] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER 
			|| isOccupied[index]== IS_OCCUPIED_FULL || isOccupied[index] == IS_OCCUPIED_TELEPORT)
		{
			if(isPlaying == true && type[index] != BLOCK_PLAYER1)
				blocks[index]->renderSprite(buf);
			if(isPlaying == false)
			{
				blocks[index]->renderSprite(buf);
			}
		}
		if(isPlaying == true)
		{
			if(respawnBlock[index].nbrOfFrames)
			{
				respawnBlock[index].nbrOfFrames--;
				if(respawnBlock[index].nbrOfFrames == 1)
				{
					// display the block again
					RespawnBlock(index);
				}
			}
		}
	}

	if(isPlaying == false)
	{
		for(index = 0; index < nbrOfTypes; index++)
		{
			menu[index]->renderSprite(buf);
			if(isSelected[index])
			{
				menu[nbrOfTypes]->renderSprite(buf);
			}
		}
		menu[nbrOfTypes+1]->renderSprite(buf);
		menu[nbrOfTypes+2]->renderSprite(buf);
	}
}

void PLATFORM::GetBlockCoordinates(unsigned int blockNbr, int &x, int &y)
{
	if(blockNbr >= 0 && blockNbr < nbrOfBlocks)
	{
		x = blocks[blockNbr]->x_pos;
		y = blocks[blockNbr]->y_pos;
	}
	if(blockNbr >= 1024 && blockNbr < 1024+nbrOfTypes)
	{
		x = menu[blockNbr-1024]->x_pos;
		y = menu[blockNbr-1024]->y_pos;
	}
	if(blockNbr == 1024+BLOCK_SAVE_BUTTON)
	{
		x = menu[33]->x_pos;
		y = menu[33]->y_pos;
	}
	if(blockNbr == 1024+BLOCK_LOAD_BUTTON)
	{
		x = menu[34]->x_pos;
		y = menu[34]->y_pos;
	}
}

unsigned int PLATFORM::getBlockNbr(int x, int y)
{
	for(int index = 0; index < nbrOfBlocks; index++)
	{
		if(x >= blocks[index]->x_pos && x <= blocks[index]->x_pos+23)
			if(y >= blocks[index]->y_pos && y <= blocks[index]->y_pos+23)
				return index;
	}

	for(int index = 0; index < nbrOfTypes; index++)
	{

		if(x >= menu[index]->x_pos && x <= menu[index]->x_pos+23)
			if(y >= menu[index]->y_pos && y <= menu[index]->y_pos+23)
				return index+1024;
	}
	if(x >= menu[BLOCK_SAVE_BUTTON]->x_pos && x <= menu[BLOCK_SAVE_BUTTON]->x_pos+23)
			if(y >= menu[BLOCK_SAVE_BUTTON]->y_pos && y <= menu[BLOCK_SAVE_BUTTON]->y_pos+23)
				return 1024+BLOCK_SAVE_BUTTON;
	if(x >= menu[BLOCK_LOAD_BUTTON]->x_pos && x <= menu[BLOCK_LOAD_BUTTON]->x_pos+23)
			if(y >= menu[BLOCK_LOAD_BUTTON]->y_pos && y <= menu[BLOCK_LOAD_BUTTON]->y_pos+23)
				return 1024+BLOCK_LOAD_BUTTON;
	return 0;
}

unsigned short PLATFORM::getIsOccupied(unsigned int blockNbr)
{
	if(blockNbr >= nbrOfBlocks)
		return 2;
	return isOccupied[blockNbr];
}

bool PLATFORM::getIsSelected(unsigned int blockNbr)
{
	if (((signed)blockNbr - 1024) < 0)
		return isSelected[0];
	return isSelected[blockNbr-1024];
}

unsigned int PLATFORM::getSelectedTypeNbr(void)
{
	for(int index = 0; index < nbrOfTypes; index++)
	{
		if(isSelected[index] == true)
			return index;
	}
	return 0;
}

IMAGE* PLATFORM::getImage(int type)
{
	return &sheet[type];
}

unsigned char PLATFORM::GetWorldNbr(void)
{
	return currentWorld;
}

int PLATFORM::SetWorldNbr(unsigned char w)
{
	if(w >= WORLD_JUNGLE && w <= WORLD_INDUSTRIAL)
	{
		currentWorld = w;
		return 1;
	}
	return 0;
}

void PLATFORM::ResetLevelToCurrentWorld(void)
{
	unsigned int _type;

	for(unsigned int index = 0; index < nbrOfBlocks; index++)
	{	
		if(isOccupied[index] == IS_OCCUPIED || isOccupied[index] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER 
			|| isOccupied[index]== IS_OCCUPIED_FULL || isOccupied[index] == IS_OCCUPIED_TELEPORT)
		{
			_type = type[index];
			if((_type >= 0 && _type <= 3) || (_type == 19))
				blocks[index]->copyBitmaps(&sheet[_type], 0);
			else if( _type == BLOCK_REGULAR_WITH_GOLD_COIN)
			{
				blocks[index]->copyBitmaps(&sheet[BLOCK_REGULAR], 0);
				temp_sprite->copyBitmaps(&sheet[BLOCK_GOLD_COIN], 0);
				temp_sprite->x_pos = blocks[index]->x_pos;
				temp_sprite->y_pos = blocks[index]->y_pos;
				temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
				temp_sprite->CopyOntoBlock(blocks[index]->getFrame(), 24, 24);
			}
			else if(_type == BLOCK_REGULAR_WITH_LADDER)
			{
				blocks[index]->copyBitmaps(&sheet[BLOCK_REGULAR], 0);
				temp_sprite->copyBitmaps(&sheet[BLOCK_LADDER], 0);
				temp_sprite->x_pos = blocks[index]->x_pos;
				temp_sprite->y_pos = blocks[index]->y_pos;
				temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
				temp_sprite->CopyOntoBlock(blocks[index]->getFrame(), 24, 24);
			}
			else if(_type == BLOCK_REGULAR_WITH_LADDER_END)
			{
				blocks[index]->copyBitmaps(&sheet[BLOCK_REGULAR], 0);
				temp_sprite->copyBitmaps(&sheet[BLOCK_LADDER_END], 0);
				temp_sprite->x_pos = blocks[index]->x_pos;
				temp_sprite->y_pos = blocks[index]->y_pos;
				temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
				temp_sprite->CopyOntoBlock(blocks[index]->getFrame(), 24, 24);
			}
			else if(_type == BLOCK_REGULAR_WITH_BOMB_SMALL)
			{
				blocks[index]->copyBitmaps(&sheet[BLOCK_REGULAR], 0);
				temp_sprite->copyBitmaps(&sheet[BLOCK_BOMB_SMALL], 0);
				temp_sprite->x_pos = blocks[index]->x_pos;
				temp_sprite->y_pos = blocks[index]->y_pos;
				temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
				temp_sprite->CopyOntoBlock(blocks[index]->getFrame(), 24, 24);
			}
		}
	}
}

int PLATFORM::SaveLevel(void)
{
	IFileDialog *pfd;
	wchar_t* fileName;
	FILE* fp;

    if (SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {
        if (SUCCEEDED(pfd->Show(NULL)))
        {
            IShellItem *psi;
            if (SUCCEEDED(pfd->GetResult(&psi)))
            {
                //PWSTR pszPath;
               /* if (SUCCEEDED(GetIDListName(psi, &pszPath)))
                {
                    MessageBox(NULL, pszPath, L"Selected Item", MB_OK);
                    CoTaskMemFree(pszPath);
                }*/
				if(!SUCCEEDED(psi->GetDisplayName(SIGDN_FILESYSPATH, &fileName)))
                {
                    MessageBox(NULL, L"GetIDListName() failed", NULL, NULL);
                }

				fp = _wfopen(fileName, L"wb");
				fwrite((unsigned char*)&currentWorld, sizeof(unsigned char), 1, fp);
				fwrite((unsigned int*)type, sizeof(unsigned int) * nbrOfBlocks, 1, fp);
				fwrite((unsigned short*)isOccupied, sizeof(unsigned short) * nbrOfBlocks, 1, fp);
				fclose(fp);
                psi->Release();
            }
        }
        pfd->Release();
    }
	return 1;
}

int PLATFORM::LoadLevel(void)
{
	IFileDialog *pfd;
	wchar_t* fileName;
	FILE* fp;
	HRESULT hr;

	//CoUninitialize();
	//hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
//	if(FAILED(hr))
		//MessageBoxW(NULL, L"Failed CoInitialize", L"Error", MB_OK);

    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {	
		//pfd->SetOptions(FOS_ALLNONSTORAGEITEMS);
		DWORD dwOptions;
		//pfd->GetOptions(&dwOptions);
		//pfd->SetOptions(dwOptions | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
		pfd->GetOptions(&dwOptions);
		pfd->SetOptions(dwOptions | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
        if (SUCCEEDED(pfd->Show(NULL)))
        {
            IShellItem *psi;
            if (SUCCEEDED(pfd->GetResult(&psi)))
            {
                //PWSTR pszPath;
               /* if (SUCCEEDED(GetIDListName(psi, &pszPath)))
                {
                    MessageBox(NULL, pszPath, L"Selected Item", MB_OK);
                    CoTaskMemFree(pszPath);
                }*/
				hr = psi->GetDisplayName( SIGDN_FILESYSPATH, &fileName);
				if(!SUCCEEDED(hr))
                {
                    MessageBox(NULL, L"GetIDListName() failed", NULL, NULL);
                }
				ClearLevel();
				fp = _wfopen(fileName, L"rb");
				fread((unsigned char*)&currentWorld, sizeof(unsigned char), 1, fp);
				fread((unsigned int*)type, sizeof(unsigned int) * nbrOfBlocks, 1, fp);
				fread((unsigned short*)isOccupied, sizeof(unsigned short) * nbrOfBlocks, 1, fp);
				fclose(fp);
				LoadBlocks(currentWorld);
				for(unsigned int index = 0; index < nbrOfBlocks; index++)
				{
					addPlatform(index, type[index]);
				}
				CoTaskMemFree(fileName);
				ResetLevelToCurrentWorld();
                psi->Release();
            }
        }
        pfd->Release();
    }
	return 1;
}

int PLATFORM::LoadLevel(wchar_t* fileName)
{
	FILE* fp;

	ClearLevel();
	fp = _wfopen(fileName, L"rb");
	fread((unsigned char*)&currentWorld, sizeof(unsigned char), 1, fp);
	fread((unsigned int*)type, sizeof(unsigned int) * nbrOfBlocks, 1, fp);
	fread((unsigned short*)isOccupied, sizeof(unsigned short) * nbrOfBlocks, 1, fp);
	fclose(fp);
	LoadBlocks(currentWorld);
	for(unsigned int index = 0; index < nbrOfBlocks; index++)
	{
		addPlatform(index, type[index]);
	}
	ResetLevelToCurrentWorld();
	return 1;
}

void PLATFORM::ClearLevel(void)
{
	currentWorld = WORLD_JUNGLE;
	for(unsigned int index = 0; index < nbrOfBlocks; index++)
		type[index] = BLOCK_EMPTY;
	memset((unsigned short*)isOccupied, IS_NOT_OCCUPIED, sizeof(unsigned short) * nbrOfBlocks);
	memset((bool*)isSelected, false, sizeof(bool) * nbrOfTypes);
	memset((RESPAWN_BLOCK*)respawnBlock, 0, sizeof(RESPAWN_BLOCK) * nbrOfBlocks);
	LoadBlocks(currentWorld);
}

POINT PLATFORM::GetStartingCoordinatesOfPlayer(unsigned int playerNbr)
{
	POINT p;
	for(unsigned int index = 0; index < nbrOfBlocks; index++)
	{
		if(playerNbr == 0)
		{
			if(type[index] == BLOCK_PLAYER1)
			{
				p.x = blocks[index]->x_pos;
				p.y = blocks[index]->y_pos;
				type[index] = BLOCK_EMPTY;
				isOccupied[index] = IS_NOT_OCCUPIED;
				return p;
			}
		}
		else if(playerNbr == 1)
		{
			if(type[index] == BLOCK_PLAYER2)
			{
				p.x = blocks[index]->x_pos;
				p.y = blocks[index]->y_pos;
				type[index] = BLOCK_EMPTY;
				isOccupied[index] = IS_NOT_OCCUPIED;
				return p;
			}
		}
	}
	p.x = 0;
	p.y = 0;
	return p;
}

void PLATFORM::SetIsPlaying(bool status)
{
	isPlaying = status;
}

unsigned int PLATFORM::GetType(unsigned int blockNbr)
{
	return type[blockNbr];
}

bool PLATFORM::GetIsPlaying(void)
{
	return isPlaying;
}

void PLATFORM::DestroyBlock(unsigned int blockNbr)
{
	respawnBlock[blockNbr].ID = type[blockNbr];
	respawnBlock[blockNbr].nbrOfFrames = 150;
	respawnBlock[blockNbr].occ = isOccupied[blockNbr];

	type[blockNbr] = BLOCK_EMPTY;
	isOccupied[blockNbr] = IS_NOT_OCCUPIED;
}

void PLATFORM::DestroyBlockPermanently(unsigned int blockNbr)
{
	//respawnBlock[blockNbr].ID = type[blockNbr];
	//respawnBlock[blockNbr].nbrOfFrames = 150;
	respawnBlock[blockNbr].occ = isOccupied[blockNbr];

	type[blockNbr] = BLOCK_EMPTY;
	isOccupied[blockNbr] = IS_NOT_OCCUPIED;
}

void PLATFORM::RespawnBlock(unsigned int blockNbr)
{
	type[blockNbr] = respawnBlock[blockNbr].ID;
	isOccupied[blockNbr] = respawnBlock[blockNbr].occ;
	memset((RESPAWN_BLOCK*)&respawnBlock[blockNbr],0, sizeof(RESPAWN_BLOCK));
}

void PLATFORM::SetTypeToRocks(unsigned int blockNbr)
{
	type[blockNbr] = BLOCK_ROCKS;
}

void PLATFORM::RemoveRocks(unsigned int blockNbr)
{
	type[blockNbr] = BLOCK_EMPTY;
}

void PLATFORM::SetTypeToSlow(unsigned int blockNbr)
{
	type[blockNbr] = BLOCK_SLOW;
	menu[BLOCK_SLOW]->CopyOntoBlock(blocks[blockNbr]->getFrame(), 24, 24);
}