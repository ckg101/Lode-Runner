#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <D3dx9core.h>
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
			MessageBox(NULL, L"file not exist SPRITE::loadBitmaps(wchar_t* name)", fileName, MB_OK);
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
		//memset(lBits, D3DCOLOR_XRGB(255, 255, 255), sizeof(unsigned long) * bih.biHeight * bih.biWidth);

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

int SPRITE::renderSprite(D3DLOCKED_RECT &locked_rect)
{
		HRESULT hr;
		unsigned char* pBits;
		unsigned long* lBits;
		unsigned long* lFrame;
		//D3DLOCKED_RECT locked_rect;
		int counter = 0;
		int counter2 = 0;
		int x, y;
		
		/*hr = backbuffer->LockRect(&locked_rect, NULL, 0);
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
		}*/
		pBits = (unsigned char*) locked_rect.pBits;
		//pBits -= locked_rect.Pitch;
		lBits = (unsigned long*) pBits;
		//pFrame = frames[frameState].image;
		lFrame = frames[frameState].image;
		//counter = bih.biWidth * bih.biHeight;
		
		for(y = 0; y < frames[frameState].parameters.bottom; y++)
		{
			//counter = frames[frameState].parameters.right * y;
			
			for(x = 0; x < frames[frameState].parameters.right; x++)
			{
				if(lFrame[counter2] != transparencyColor)
				{
					lBits[counter] = lFrame[counter2];
				}			
				counter++;
				counter2++;
			}
			counter = screenWidth * (y+y_pos)+x_pos;
		}
		//backbuffer->UnlockRect();
		if(animationType == ANIMATION_AUTOMATIC_LOOP)
		{
			frameState++;
			if(frameState == nbrOfFrames)
				frameState = 0;
		}
		//if(animationType == ANIMATION_TRIGGERED_SEQ)
		//{
			// do not advance frameState
		//}
		return 1;
}

int SPRITE::CopyOntoBlock(IMAGE* frame, unsigned int width, unsigned int height)
{
	HRESULT hr;
		
		unsigned long* lBits;
		unsigned long* lFrame;
		int counter = 0;
		int counter2 = 0;
		lBits = frame->image;
		lFrame = frames[frameState].image;
		//counter = bih.biWidth * bih.biHeight;
	
		for(int y = 0; y < width; y++)
		{
			//counter = frames[frameState].parameters.right * y;
			
			for(int x = 0; x < height; x++)
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

void SPRITE::setTransparencyColor(unsigned long color)
{
	transparencyColor = color;
}

IMAGE* SPRITE::getFrame(void)
{
	return &frames[frameState];
}

IMAGE* SPRITE::getFrame(unsigned int frameNbr)
{
	return &frames[frameNbr];
}





