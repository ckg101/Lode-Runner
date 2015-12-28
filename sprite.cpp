#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include <math.h>
#include "sprite.h"

SPRITE::SPRITE(IDirect3DDevice9* d, int nbr_of_frames, int screen_width, int screen_height)
{
	d3ddev = d;
	nbrOfFrames = nbr_of_frames;
	frames = NULL;
	frames = (IMAGE*)malloc(sizeof(IMAGE)*nbr_of_frames);
	for(int index = 0; index < nbr_of_frames; index++)
	{
		frames[index].image = NULL;
	}
	frameState = 0;
	x_pos = 0;
	y_pos = 0;
	screenWidth = screen_width;
	screenHeight = screen_height;
	animationType = ANIMATION_AUTOMATIC_LOOP;
}

SPRITE::~SPRITE()
{
	if(frames)
	{
		for(int index = 0; index < nbrOfFrames; index++)
		{
			if(frames[index].image)
				free(frames[index].image);
		}
		free(frames);
	}
}

int SPRITE::loadBitmaps(wchar_t* name)
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
	int index = 0;
	while(index < nbrOfFrames)
	{
		
		counter = 0;
		counter2 = 0;
		frames[index].image = NULL;
		swprintf(fileName, L"%s%d.bmp", name,index);
		fp = _wfopen(fileName, L"rb");
		if (fp == NULL)
		{
			MessageBox(NULL, L"file not exist", fileName, MB_OK);
			return 0;
		}
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

		// allocate memory for image
		frames[index].image = (unsigned long*)malloc(bih.biHeight * bih.biWidth * 4);
		temp_image = (unsigned char*)malloc(bfh.bfSize);
		if(frames[index].image == NULL)
		{
			MessageBox(NULL, L"Malloc image Error", L"error", MB_OK);
			return 0;
		}

		fread((unsigned char*)temp_image, bfh.bfSize, 1, fp);
		fclose(fp);
		
		frames[index].parameters.top = 0;
		frames[index].parameters.bottom = bih.biHeight;
		frames[index].parameters.left = 0;
		frames[index].parameters.right = bih.biWidth;

		lBits = frames[index].image;
	

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
	return 1;
}

int SPRITE::copyBitmaps(IMAGE* image, int frame_nbr)
{
	frames[frame_nbr].image = NULL;

	frames[frame_nbr].image = (unsigned long*)malloc(image->parameters.right * image->parameters.bottom * 4);

	if(frames[frame_nbr].image == NULL)
	{
		MessageBox(NULL, L"Malloc image Error", L"error", MB_OK);
		return 0;
	}

	// copy memory for image
	memcpy((unsigned long*)frames[frame_nbr].image, (unsigned long*)image->image, image->parameters.right * image->parameters.bottom * 4);

	
		
	frames[frame_nbr].parameters.top = 0;
	frames[frame_nbr].parameters.bottom = image->parameters.bottom;
	frames[frame_nbr].parameters.left = 0;
	frames[frame_nbr].parameters.right = image->parameters.right;

	return 1;
}

int SPRITE::setAnimationType(int a)
{
	switch(a)
	{
		case ANIMATION_AUTOMATIC_LOOP:
			break;
		case ANIMATION_TRIGGERED_SEQ:
			break;
		default:
			return 0;
	}
	animationType = a;
	return 1;
}

int SPRITE::renderSprite(IDirect3DSurface9* &backbuffer)
{
		HRESULT hr;
		unsigned char* pBits;
		unsigned long* lBits;
		unsigned long* lFrame;
		D3DLOCKED_RECT locked_rect;
		int counter = 0;
		int counter2 = 0;
		
		hr = backbuffer->LockRect(&locked_rect, NULL, 0);
		if(FAILED(hr))
		{
			MessageBoxW(NULL, L"Failed to lock surface", L"Error", MB_OK);
			if(hr == D3DERR_INVALIDCALL)
			{
				MessageBoxW(NULL, L"Invalid Call", L"Error", MB_OK);
				return 0;
			}
			if(hr == D3DERR_WASSTILLDRAWING)
			{
				MessageBoxW(NULL, L"WAS STILL DRAWING", L"Error", MB_OK);
				return 0;
			}
		}
		pBits = (unsigned char*) locked_rect.pBits;
		//pBits -= locked_rect.Pitch;
		lBits = (unsigned long*) pBits;
		//pFrame = frames[frameState].image;
		lFrame = frames[frameState].image;
		//counter = bih.biWidth * bih.biHeight;
		
		for(int y = 0; y < frames[frameState].parameters.bottom; y++)
		{
			//counter = frames[frameState].parameters.right * y;
			
			for(int x = 0; x < frames[frameState].parameters.right; x++)
			{
				if(lFrame[counter2] == transparencyColor)
				{
					// do nothing
				}
				else
					lBits[counter] = lFrame[counter2];
				
				counter++;
				counter2++;
			}
			counter = screenWidth * (y+y_pos)+x_pos;
		}
		backbuffer->UnlockRect();
		if(animationType == ANIMATION_AUTOMATIC_LOOP)
		{
			frameState++;
		}
		if(animationType == ANIMATION_TRIGGERED_SEQ)
		{
			// do not advance frameState
		}
		if(frameState == nbrOfFrames)
				frameState = 0;
		return 1;
}

int SPRITE::CopyOntoBlock(IMAGE* frame)
{
	HRESULT hr;
		
		unsigned long* lBits;
		unsigned long* lFrame;
		int counter = 0;
		int counter2 = 0;
		lBits = frame->image;
		lFrame = frames[frameState].image;
		//counter = bih.biWidth * bih.biHeight;

		
		for(int y = 0; y < 48; y++)
		{
			//counter = frames[frameState].parameters.right * y;
			
			for(int x = 0; x < 48; x++)
			{
				if(lFrame[counter2] == transparencyColor)
				{
					// do nothing
				}
				else
					lBits[counter] = lFrame[counter2];
				
				counter++;
				counter2++;
			}
			//counter = screenWidth * (y+y_pos)+x_pos;
		}
		
		return 1;
}

void SPRITE::nextFrame(void)
{
	frameState++;
}

void SPRITE::setTransparencyColor(unsigned long color)
{
	transparencyColor = color;
}

IMAGE* SPRITE::getFrame(void)
{
	return &frames[frameState];
}


////////////////////////////////////////////////////////////////////////////////////////////
// PLATFORM CLASS
////////////////////////////////////////////////////////////////////////////////////////////

PLATFORM::PLATFORM(IDirect3DDevice9* d, int screen_width, int screen_height)
{
	d3ddev = d;
	screenWidth = screen_width;
	screenHeight = screen_height;
	nbrOfBlocks = 0;
	blocks = NULL;
	isOccupied = NULL;
	isSelected = NULL;
	sheet = NULL;
	menu = NULL;
	type = NULL;
	temp_sprite = NULL;
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
}

// nbr_of_blocks MUST BE SQUARE
int PLATFORM::initialize(unsigned int nbr_of_blocks, unsigned int nbr_of_types)
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
	memset((unsigned int*)type, (unsigned int)BLOCK_EMPTY, sizeof(unsigned int) * nbr_of_blocks);

	// set each block as false to represent an empty block
	isOccupied = (unsigned char*)malloc(sizeof(unsigned char) * nbr_of_blocks);
	if(isOccupied == NULL)
		return -1;
	memset((unsigned char*)isOccupied, (unsigned char)IS_NOT_OCCUPIED, sizeof(unsigned char) * nbr_of_blocks);
	nbrOfBlocks = nbr_of_blocks;

	nbrOfTypes = nbr_of_types;

	for(unsigned int index = 0; index < nbrOfBlocks; index++)
	{
		blocks[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
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
			x+=48;
			counter++;
		}
		y+=48;
		x = 0;
	}

	// load sprite sheet
	sheet = (IMAGE*)malloc(sizeof(IMAGE) * nbrOfTypes);
	if(sheet == NULL)
		return -3;

	menu = (SPRITE**)malloc(sizeof(SPRITE*) * (nbrOfTypes+1));
	if(menu == NULL)
		return -4;
	
	memset(menu, 0, sizeof(SPRITE*)*(nbrOfTypes+1));

	isSelected = (bool*)malloc(sizeof(bool) * nbr_of_types);
	if(isSelected == NULL)
		return -5;
	memset((bool*)isSelected, (bool)false, sizeof(bool) * nbr_of_types);
	
	for(unsigned int index = 0; index < nbrOfTypes; index++)
	{
		sheet[index].image = NULL;
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
	for(index = 0; index < nbrOfTypes+1; index++)
	{
		if(menu[index])
		{
			delete menu[index];
			menu[index] = NULL;
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
			MessageBox(NULL, L"file not exist", L"error", MB_OK);
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
		}
		menu[index] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
		if(menu[index] == NULL)
			return -1;
		menu[index]->copyBitmaps(&sheet[index], 0);
	}
	
	menu[nbrOfTypes] = new SPRITE(d3ddev, 1, screenWidth, screenHeight);
	if(menu[nbrOfTypes] == NULL)
		return -1;

	menu[nbrOfTypes]->loadBitmaps(L"Graphics\\block34_");
	menu[nbrOfTypes]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));

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
			x+=48;
			counter++;
			if(counter == nbrOfTypes)
				break;
		}
		y+=48;
		x=768;
	}
	return 1;
}

int PLATFORM::addPlatform(unsigned int blockNbr, unsigned int _type)
{
	if(blockNbr >= nbrOfBlocks)
		return 0;
	if(_type >= nbrOfTypes)
		return -1;
	if(isOccupied[blockNbr] != IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER)
	{
		blocks[blockNbr]->copyBitmaps(&sheet[_type], 0);
		type[blockNbr] = _type;
	}
	if(isOccupied[blockNbr] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER)
	{
		// filter out which types can be added as a second layers
		if(_type == BLOCK_GOLD_COIN || _type == BLOCK_LADDER || _type == BLOCK_LADDER_END || _type == BLOCK_BOMB_SMALL)
		{
			temp_sprite->copyBitmaps(&sheet[_type], 0);
			temp_sprite->x_pos = blocks[blockNbr]->x_pos;
			temp_sprite->y_pos = blocks[blockNbr]->y_pos;
			temp_sprite->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
			temp_sprite->CopyOntoBlock(blocks[blockNbr]->getFrame());
			//if(type[blockNbr] != 0)
				type[blockNbr] = type[blockNbr] + 200 + _type;
		}
	}
		
	return 1;
}

void PLATFORM::setBlock(unsigned int blockNbr)
{
	if(blockNbr >= 0 && blockNbr < 256)
	{
		if(isOccupied[blockNbr] == 3)
			isOccupied[blockNbr]= 4;
		else if(isOccupied[blockNbr] == 0)
			isOccupied[blockNbr] = 1;
		if(type[blockNbr] == 0)
			isOccupied[blockNbr] = 3;
		blocks[blockNbr]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	}
	else if(blockNbr >= 256 && blockNbr < 256+nbrOfTypes)
	{
		memset((bool*)isSelected, (bool)false, sizeof(bool)*nbrOfTypes);
		isSelected[blockNbr-256] = true;
		menu[blockNbr-256]->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
		menu[nbrOfTypes]->x_pos = menu[blockNbr-256]->x_pos;
		menu[nbrOfTypes]->y_pos = menu[blockNbr-256]->y_pos;
	}
}

void PLATFORM::renderPlatform(IDirect3DSurface9* &buf)
{
	unsigned int index;
	for(index = 0; index < nbrOfBlocks; index++)
	{	
		if(isOccupied[index] == IS_OCCUPIED || isOccupied[index] == IS_OCCUPIED_WITH_EMPTY_SECOND_LAYER 
			|| isOccupied[index]== IS_OCCUPIED_FULL)
			blocks[index]->renderSprite(buf);
	}

	for(index = 0; index < nbrOfTypes; index++)
	{
		menu[index]->renderSprite(buf);
		if(isSelected[index])
		{
			menu[nbrOfTypes]->renderSprite(buf);
		}
	}
}

void PLATFORM::GetBlockCoordinates(unsigned int blockNbr, int &x, int &y)
{
	if(blockNbr >= 0 && blockNbr < nbrOfBlocks)
	{
		x = blocks[blockNbr]->x_pos;
		y = blocks[blockNbr]->y_pos;
	}
	else if(blockNbr >= 256 && blockNbr < 256+nbrOfTypes)
	{
		x = menu[blockNbr-256]->x_pos;
		y = menu[blockNbr-256]->y_pos;
	}
}

unsigned int PLATFORM::getBlockNbr(int x, int y)
{
	for(int index = 0; index < nbrOfBlocks; index++)
	{
		if(x >= blocks[index]->x_pos && x <= blocks[index]->x_pos+47)
			if(y >= blocks[index]->y_pos && y <= blocks[index]->y_pos+47)
				return index;
	}

	for(int index = 0; index < nbrOfTypes; index++)
	{
		if(x >= menu[index]->x_pos && x <= menu[index]->x_pos+47)
			if(y >= menu[index]->y_pos && y <= menu[index]->y_pos+47)
				return index+256;
	}
	return 0;
}

unsigned char PLATFORM::getIsOccupied(unsigned int blockNbr)
{
	if(blockNbr >= nbrOfBlocks)
		return 2;
	return isOccupied[blockNbr];
}

bool PLATFORM::getIsSelected(unsigned int blockNbr)
{
	return isSelected[blockNbr-256];
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


///////////////////////////////////////////////////////////////////////////////
// CURSOR CLASS
//////////////////////////////////////////////////////////////////////////////

CURSOR::CURSOR(IDirect3DDevice9* &d, unsigned int type_nbr, int screen_width, int screen_height)
{
	cursorGraphic = NULL;
	selectionGraphic = NULL;
	cursorGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	cursorGraphic->loadBitmaps(L"Graphics\\block33_");
	cursorGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	cursorGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	selectionGraphic = new SPRITE(d, 1, screen_width, screen_height); 
	selectionGraphic->loadBitmaps(L"Graphics\\block34_");
	selectionGraphic->setTransparencyColor(D3DCOLOR_XRGB(0,0,0));
	selectionGraphic->setAnimationType(ANIMATION_TRIGGERED_SEQ);
	typeNbr = 0;
	blockCursor = 0;
	screenWidth = screen_width;
	screenHeight = screen_height;
}

CURSOR::~CURSOR()
{
	if(cursorGraphic)
		delete cursorGraphic;
	if(selectionGraphic)
		delete selectionGraphic;
}

int CURSOR::SetType(unsigned int type_nbr)
{
	typeNbr = type_nbr;
	return 1;
}

unsigned int CURSOR::GetType(void)
{
	return typeNbr;
}

void CURSOR::Render(IDirect3DSurface9* &backbuffer)
{
	selectionGraphic->renderSprite(backbuffer);
	cursorGraphic->renderSprite(backbuffer);
}

void CURSOR::SetSelectionX_Pos(int x)
{
	selectionGraphic->x_pos = x;
}

void CURSOR::SetSelectionY_Pos(int y)
{
	selectionGraphic->y_pos = y;
}

void CURSOR::MoveCursorX(int qty)
{
	if(cursorGraphic->x_pos+qty < screenWidth-48 && cursorGraphic->x_pos+qty >= 0)
		cursorGraphic->x_pos+=qty;
}

void CURSOR::MoveCursorY(int qty)
{
	if(cursorGraphic->y_pos+qty < screenHeight-47 && cursorGraphic->y_pos+qty >=0)
		cursorGraphic->y_pos+=qty;
}

POINT CURSOR::GetCursorPosition(void)
{
	POINT p;
	p.x = cursorGraphic->x_pos;
	p.y = cursorGraphic->y_pos;
	return p;
}

int CURSOR::GetBlockCursor(void)
{
	return blockCursor;
}

int CURSOR::SetBlockCursor(int c)
{
	if(blockCursor < 256 && blockCursor >= 0)
	{
		blockCursor = c;
		return 1;
	}
	else if(blockCursor >= 256 && blockCursor < 256+32)
	{
		blockCursor = c;
		return 2;
	}
	return 0;
}