#ifndef CURSOR_H
#define CURSOR_H
#include "sprite.h"

class CURSOR
{
	public:
		CURSOR(IDirect3DDevice9* &d, unsigned int type_nbr, int screen_width, int screen_height);
		CURSOR(IDirect3DDevice9* &d, int screen_width, int screen_height);
		~CURSOR(); 
		int SetType(unsigned int type_nbr);
		unsigned int GetType(void);
		void Render(D3DLOCKED_RECT&backbuffer);
		void SetSelectionX_Pos(int x);
		void SetSelectionY_Pos(int y);
		void MoveCursorX(int qty);
		void MoveCursorY(int qty);
		int GetBlockCursor(void);
		int SetBlockCursor(int c);
		int SetButtonCursor(int c);
		POINT GetCursorPosition(void);
	private:
		int blockCursor;
		SPRITE* cursorGraphic;
		SPRITE* selectionGraphic;
		unsigned int typeNbr;			// ID for the image to be displayed as cursor from PLATFORM sheet
		int screenWidth;
		int screenHeight;
		bool displaySelectionGraphic;
};

#endif