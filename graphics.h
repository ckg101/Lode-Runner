#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
//#include <D3dx9core.h>

typedef struct tBACKGROUND
{
		IDirect3DSurface9* surface;
		RECT parameters;
} BACKGROUND;

typedef struct tIMAGE
{
	unsigned long* image;
	RECT parameters;
} IMAGE;

class GRAPHICS
{
	public:
		GRAPHICS(LPDIRECT3DDEVICE9 dev);
		int LoadBitmapImage(wchar_t* fileName, IDirect3DSurface9* &s, RECT* parameters);
	protected:
		LPDIRECT3DDEVICE9 d3ddev;

};

#endif