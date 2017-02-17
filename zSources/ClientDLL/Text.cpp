//////////////////////////////////////////////////////////////////////
// Text.cpp
#include "stdafx.h"
#include "Text.h"
#include "Macros.h"
//#include "Interface.h"
#include "ServerInfo.h"
#include "offsets.h"
CMuText* g_Text = new CMuText;

void __fastcall muGetWindowSize(int* vec)
{
	vec[0] = *(DWORD*)MU_WINDOW_SIZE;
	vec[1] = *(DWORD*)(MU_WINDOW_SIZE+4);
}
void CMuText::Out(char* text, int x, int y)
{
	__asm {
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH text;
		PUSH y;
		PUSH x;

		MOV EDX, TEXT_GET_CONTEXT;
		CALL EDX;
		MOV ECX, EAX;
		MOV EDX, TEXT_PRINT;
		CALL EDX;
	}
}

void CMuText::SetFont(HFONT hFont)
{
	__asm {
		PUSH hFont;
		MOV EDX, TEXT_GET_CONTEXT;
		CALL EDX;
		MOV ECX,EAX;
		MOV EDX, TEXT_SET_FONT;
		CALL EDX;
	}
}

void CMuText::SetColor(COLORREF color)
{
	__asm{
		PUSH color;
		MOV EAX, TEXT_GET_CONTEXT;
		CALL EAX;
		MOV ECX,EAX;
		MOV EAX, TEXT_SET_COLOR;
		CALL EAX;
	}
}

void CMuText::SetBack(COLORREF color)
{
	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);
	BYTE a = GetAValue(color);
	__asm{
		PUSH a;
		PUSH b;
		PUSH g;
		PUSH r;
		MOV EDX, TEXT_GET_CONTEXT;
		CALL EDX;
		MOV ECX,EAX;
		MOV EDX, TEXT_SET_BK_COLOR;
		CALL EDX;
	}
}






CGlFont* g_Font[100];

CGlFont::CGlFont(TCHAR* Name, int Size, int Weight, BOOL bItalic, BOOL bUnderline)
{
	m_Font = CreateFont(Size, 0, 0, 0, Weight, bItalic, bUnderline, FALSE,
		g_ServerInfo->GetCharset(), OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH,
		Name);

	width_ = 128;
	height_ = 128;

	spacing_ = Size/3;
	fontSize_ = Size;

	if(!m_Font)
	{
		//g_Console->Out("Nie mozna utworzyc czcionki %s!");
		return;
	}

	HDC hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);

	HFONT hFontOld = (HFONT) SelectObject(hDC, m_Font);

	PAINT_RESULT p;
	while(PAINT_MORE_DATA == (p = PaintAlphabet(hDC, true)))
	{
		width_ *= 2;
		height_ *= 2;
	}

	DWORD*      pBitmapBits;
	BITMAPINFO bmi;
	ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  (int)width_;
	bmi.bmiHeader.biHeight      = -(int)height_;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;

	// Create a bitmap for the font
	HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
		(void**)&pBitmapBits, NULL, 0 );

	HGDIOBJ hbmOld = SelectObject( hDC, hbmBitmap );

	// Set text properties
	SetTextColor( hDC, RGB(255,255,255) );
	SetBkColor(   hDC, RGB(0,0,0) );
	SetTextAlign( hDC, TA_TOP );

	// Paint the alphabet onto the selected bitmap
	p = PaintAlphabet( hDC, false );// == PAINT_SUCCESS;
	if(p == PAINT_SUCCESS)
	{
		glGenTextures(1, &texture_);
		BITMAP  bitmap;
		GetObject(hbmBitmap,sizeof(BITMAP), &bitmap);
		glBindTexture(GL_TEXTURE_2D, texture_); // Bind Our Texture
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		// Its actually BGRA, but since we're rendering white...
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.bmBits);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.bmBits);
	}
	SelectObject( hDC, hbmOld );
	SelectObject( hDC, hFontOld );
	DeleteObject( hbmBitmap );
//	DeleteObject( m_Font );
	DeleteDC( hDC );
}

PAINT_RESULT CGlFont::PaintAlphabet(HDC hDC, bool bMeasureOnly)
{
	SIZE size;
	TCHAR str[2] = TEXT("x");

	// Calculate the spacing between characters based on line height
	if( 0 == GetTextExtentPoint32( hDC, str, 1, &size ) )
		return PAINT_FAIL;
	spacing_ = (int)ceil(size.cy * 0.3f);

	// Set the starting point for the drawing
	DWORD x = spacing_;
	DWORD y = 0;

	// For each character, draw text on the DC and advance the current position
	for( int i = 32; i < 255; i++ )
	{
		str[0] = i;
		if( 0 == GetTextExtentPoint32( hDC, str, 1, &size ) )
			return PAINT_FAIL;

		if( (DWORD)(x + size.cx + spacing_) > width_ )
		{
			x  = spacing_;
			y += size.cy + 1;
		}

		// Check to see if there's room to write the character here
		if( y + size.cy > height_ )
			return PAINT_MORE_DATA;

		if( !bMeasureOnly )
		{
			// Perform the actual drawing
			if( 0 == ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL ) )
				return PAINT_FAIL;

			texCoords_[i-32][0] = ((FLOAT)(x))/width_;
			texCoords_[i-32][1] = ((FLOAT)(y))/height_;
			texCoords_[i-32][2] = ((FLOAT)(x + size.cx))/width_;
			texCoords_[i-32][3] = ((FLOAT)(y + size.cy))/height_;
		}
		x += size.cx + (2 * spacing_);
	}

	return PAINT_SUCCESS;
}

float CGlFont::RenderChar(BYTE ch, float x, float y) const
{
	float tx1 = texCoords_[ch-32][0];
	float ty1 = texCoords_[ch-32][1];
	float tx2 = texCoords_[ch-32][2];
	float ty2 = texCoords_[ch-32][3];

	float w = (tx2-tx1) * width_ ;
	float h = (ty2-ty1) * height_;

	glTexCoord2f(tx1, ty2);
	glVertex2f(x, y);

	glTexCoord2f(tx2, ty2);
	glVertex2f(x+w, y);

	glTexCoord2f(tx2, ty1);
	glVertex2f(x+w, y+h);

	glTexCoord2f(tx1, ty1);
	glVertex2f(x, y+h);

	return w;
}

CGlFont::~CGlFont()
{
//	glDeleteLists(m_List, 255);
	glDeleteTextures(1, &texture_);
}

void CGlFont::Print( int xs, int ys, TCHAR* text )
{
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	gluOrtho2D(0.0f, 800.0f, 600.0f, 0.0f);

	int WindowSize[2];
	muGetWindowSize(WindowSize);
	float x = (float)xs*(float)WindowSize[0]/640.0f;
	float y = (float)WindowSize[1]-((float)WindowSize[1]*(float)ys/480.0f);
	y-=fontSize_;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBegin (GL_QUADS);
	
//	glColor4f(1.0f, 1.0f, 1.0, 0.7f);
	
//	glColor4f(0.0f, 0.0f, 0.0, 1.0f);
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			x += spacing_;
		}
		else if (*ptr == '\n')
		{
			y += fontSize_;
			x = (float)xs*(float)WindowSize[0]/640.0f;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
			x += RenderChar(*ptr, x, y); 
		}
		else
		{
			x += RenderChar(*ptr, x, y);            
		}
	}       
	glEnd();
//	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
}

void CGlFont::PrintNewui( int xs, int ys, TCHAR* text )
{
	int WindowSize[2];
	muGetWindowSize(WindowSize);
	float x = (float)xs;
	float y = (float)WindowSize[1]-(float)ys;
	y-=fontSize_;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBegin (GL_QUADS);

	//	glColor4f(1.0f, 1.0f, 1.0, 0.7f);

	//	glColor4f(0.0f, 0.0f, 0.0, 1.0f);
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			x += spacing_;
		}
		else if (*ptr == '\n')
		{
			y += fontSize_;
			x = (float)xs*(float)WindowSize[0]/640.0f;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
			x += RenderChar(*ptr, x, y);    
		}
		else
		{
			x += RenderChar(*ptr, x, y);            
		}
	}       
	glEnd();
	//	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

void CGlFont::Draw(int x, int y, TCHAR* text)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	Print(x, y, text);
}

void CGlFont::DrawNewui(int x, int y, TCHAR* text)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	PrintNewui(x, y, text);
}

void CGlFont::DrawColor(int x, int y, COLORREF color, TCHAR* text)
{
	float r = (float)GetRValue(color);
	float g = (float)GetGValue(color);
	float b = (float)GetBValue(color);
	glColor4f(r/255.0f, g/255.0f, b/255.0f, 1.0f);
	Print(x, y, text);
}

void CGlFont::DrawNewuiColor(int x, int y, COLORREF color, TCHAR* text)
{
	float r = (float)GetRValue(color);
	float g = (float)GetGValue(color);
	float b = (float)GetBValue(color);
	glColor4f(r/255.0f, g/255.0f, b/255.0f, 1.0f);
	PrintNewui(x, y, text);
}

void CGlFont::DrawFormat( int x, int y, TCHAR* format, ... )
{
	TCHAR TextBuff[1024] = {0};
	va_list va;
	va_start(va, format);
	tvsprintf(TextBuff, format, va);
	va_end(va);
	Draw(x, y, TextBuff);
}

void CGlFont::DrawNewuiFormat( int x, int y, TCHAR* format, ... )
{
	TCHAR TextBuff[1024] = {0};
	va_list va;
	va_start(va, format);
	tvsprintf(TextBuff, format, va);
	va_end(va);
	DrawNewui(x, y, TextBuff);
}

void CGlFont::DrawColorFormat( int x, int y, COLORREF color, TCHAR* format, ... )
{
	TCHAR TextBuff[1024] = {0};
	va_list va;
	va_start(va, format);
	tvsprintf(TextBuff, format, va);
	va_end(va);
	DrawColor(x, y, color, TextBuff);
}

void CGlFont::DrawNewuiColorFormat( int x, int y, COLORREF color, TCHAR* format, ... )
{
	TCHAR TextBuff[1024] = {0};
	va_list va;
	va_start(va, format);
	tvsprintf(TextBuff, format, va);
	va_end(va);
	DrawNewuiColor(x, y, color, TextBuff);
}

void CGlFont::GetTextSize( TCHAR* text, SIZE& tsize )
{
//	float Height = (float)g_Width;
//	float Width = (float)g_Height;
	int WindowSize[2];
	muGetWindowSize(WindowSize);
	float size = 0.0f;
	float maxH = 0.0f;
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			size += spacing_;
		}
		else if (*ptr == '\n')
		{
			break;
			//	y += fontSize_;
			//	x = (float)xs*(float)WindowSize[0]/640.0f;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
		}
		else
		{
			int ch = *ptr;
			float tx1 = texCoords_[ch-32][0];
			float ty1 = texCoords_[ch-32][1];
			float tx2 = texCoords_[ch-32][2];
			float ty2 = texCoords_[ch-32][3];

			float w = (tx2-tx1) * width_ ;
			float h = (ty2-ty1) * height_;
			size += w;
			if(maxH < h)
				maxH = h;
		}
	}
	tsize.cx = size*640.0f/WindowSize[0];
	tsize.cy = maxH*480.0f/WindowSize[1];
}

int CGlFont::GetTextWidth( TCHAR* text )
{
/*	int WindowSize[2];
	muGetWindowSize(WindowSize);

	HDC hDC = *(HDC*)MU_HDC_ADDRESS;//g_hDC;
	SelectObject(hDC, m_Font);
	SIZE tsize;
	GetTextExtentPoint(hDC, text, tstrlen(text), &tsize);

	return (tsize.cx*640.0f/(float)WindowSize[0]);*/
	int WindowSize[2];
	muGetWindowSize(WindowSize);
	float size = 0.0f;
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			size += spacing_;
		}
		else if (*ptr == '\n')
		{
			break;
		//	y += fontSize_;
		//	x = (float)xs*(float)WindowSize[0]/640.0f;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
		}
		else
		{
			int ch = *ptr;
			float tx1 = texCoords_[ch-32][0];
			float ty1 = texCoords_[ch-32][1];
			float tx2 = texCoords_[ch-32][2];
			float ty2 = texCoords_[ch-32][3];

			float w = (tx2-tx1) * width_ ;
			float h = (ty2-ty1) * height_;
			size += w;
		}
	}
	return size*640.0f/WindowSize[0];
}

void CGlFont::GetNewuiTextSize( TCHAR* text, SIZE& tsize )
{
	float size = 0.0f;
	float maxH = 0.0f;
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			size += spacing_;
		}
		else if (*ptr == '\n')
		{
			break;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
		}
		else
		{
			int ch = *ptr;
			float tx1 = texCoords_[ch-32][0];
			float ty1 = texCoords_[ch-32][1];
			float tx2 = texCoords_[ch-32][2];
			float ty2 = texCoords_[ch-32][3];

			float w = (tx2-tx1) * width_ ;
			float h = (ty2-ty1) * height_;
			size += w;
			if(maxH < h)
				maxH = h;
		}
	}
	tsize.cx = size;
	tsize.cy = maxH;
}

int CGlFont::GetNewuiTextWidth( TCHAR* text )
{
	float size = 0.0f;
	for (const TCHAR *ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			size += spacing_;
		}
		else if (*ptr == '\n')
		{
			break;
		}   
		else if( (*ptr-32) < 0 || (*ptr-32) >= 128-32 )
		{
		}
		else
		{
			int ch = *ptr;
			float tx1 = texCoords_[ch-32][0];
			float ty1 = texCoords_[ch-32][1];
			float tx2 = texCoords_[ch-32][2];
			float ty2 = texCoords_[ch-32][3];

			float w = (tx2-tx1) * width_ ;
			float h = (ty2-ty1) * height_;
			size += w;
		}
	}
	return size;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

