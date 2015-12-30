#ifndef TITLESCREEN_H
#define TITLESCREEN_H

class TITLESCREEN
{
	public:
		TITLESCREEN(IDirect3DDevice9* d, int screen_width, int screen_height);
		~TITLESCREEN(void);
	private:
		SPRITE* titleScreenImage;
public:
	// Copy image to backbuffer to be displayed
	void Render(IDirect3DSurface9* &backbuffer);
};

#endif