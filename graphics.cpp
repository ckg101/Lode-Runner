#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <dinput.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xaudio2.h>
#include "graphics.h"

GRAPHICS::GRAPHICS(LPDIRECT3DDEVICE9 dev)
{
	d3ddev = dev;
}

int GRAPHICS::LoadBitmapImage(wchar_t* fileName, IDirect3DSurface9* &s, RECT* parameters)
// This function loads a bitmap from a file and copies it to a Direct3d surface
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	FILE* fp = NULL;
	unsigned char* image = NULL;
	D3DLOCKED_RECT locked_rect;
	HRESULT hresult;
	unsigned char* pBits;
	unsigned long* lBits;
	int counter = 0;
	int counter2 = 0;

	fp = _wfopen(fileName, L"rb");
	if (fp == NULL)
	{
		MessageBox(NULL, L"file not exist", L"error", MB_OK);
		return 0;
	}
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

	image = (unsigned char*)malloc(bfh.bfSize);
	if(image == NULL)
	{
		MessageBox(NULL, L"Malloc image Error", L"error", MB_OK);
		return 0;
	}

	fread((unsigned char*)image, bih.biSizeImage, 1, fp);
	fclose(fp);

	// create memory for image

	hresult = d3ddev->CreateOffscreenPlainSurface(bih.biWidth, bih.biHeight, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &s, NULL);
	if(hresult != D3D_OK)
	{
		MessageBox(NULL, L"create surface error", L"error", MB_OK);
		return 0;
	}
	parameters->top = 0;
	parameters->bottom = bih.biHeight;
	parameters->left = 0;
	parameters->right = bih.biWidth;

	s->LockRect(&locked_rect, NULL, 0);
	pBits = (unsigned char*) locked_rect.pBits;
	pBits -= locked_rect.Pitch;
	lBits = (unsigned long*) pBits;

	//counter = bih.biWidth * bih.biHeight;

	// Print line by line from the bottom to the top becuase BMP is stored
	// upside down
	for(int y = bih.biHeight; y > 0; y--)
	{
		counter = bih.biWidth * y;
		for(int x = 0; x < bih.biWidth; x++)
		{
			/*pBits[counter] = 255; //D3DCOLOR_XRGB(255, 0, 0);
			counter++;
			pBits[counter] = 0;
			counter++;
			pBits[counter] = 0;
			counter++;
			pBits[counter] = 0;
			counter++;*/
			lBits[counter] = D3DCOLOR_XRGB(image[counter2+2],image[counter2+1],image[counter2]);   // BMP is in BGR format so swap Red and Blue
			counter2+=3;
			counter++;
		}
	}
	s->UnlockRect();
	free(image);
	return 1;

}