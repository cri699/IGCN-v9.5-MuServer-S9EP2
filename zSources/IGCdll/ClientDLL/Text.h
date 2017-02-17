//////////////////////////////////////////////////////////////////////
// Text.h
#ifndef TEXT_H
#define TEXT_H

// not working defines
//004510CC   55               PUSH EBP

#define TEXT_GET_CONTEXT 0x004512A0 // S9
//00451363   55               PUSH EBP

#define TEXT_PRINT 0x00451537 // S9
#define TEXT_SET_FONT 0x0045150F //S9
#define TEXT_SET_COLOR 0x0045148E //S9
#define TEXT_SET_BK_COLOR 0x004514B6 //s9
//004512E2   55               PUSH EBP



class CMuText{
public:
	void SetFont(HFONT hFont);
	void SetColor(COLORREF color);
	void SetBack(COLORREF color);
	void Out(char* text, int x, int y);
};

//void __fastcall muSetText(char* text, int x, int y);
//void __fastcall muSetTextColor(char* text, int x, int y, COLORREF color);
//void __fastcall muSetTextFontColor(char* text, int x, int y, HFONT hFont, COLORREF color);
//int __fastcall muGetTextSize(char* text, HFONT hFont);

extern CMuText* g_Text;

enum PAINT_RESULT{
	PAINT_MORE_DATA,
	PAINT_FAIL,
	PAINT_SUCCESS,
};

class CGlFont{
public:
	HFONT m_Font;
	GLuint m_List;
	float   texCoords_[255-32][4];
	unsigned int width_;
	unsigned int height_;
	unsigned int spacing_;
	unsigned int texture_;
	unsigned int fontSize_;

	std::string name_;
	bool bold_;
	bool italic_;
	PAINT_RESULT PaintAlphabet(HDC hDC, bool bMeasureOnly);
	float RenderChar (BYTE ch, float x, float y) const;
	void GetTextSize(const char* pstr, float &xOut, float &yOut) const;
	void Print(int x, int y, TCHAR* text);
	void PrintNewui(int x, int y, TCHAR* text);
public:
	CGlFont(TCHAR* Name, int Size, int Weight, BOOL bItalic, BOOL bUnderline);
	~CGlFont();

	void Draw(int x, int y, TCHAR* text);
	void DrawColor(int x, int y, COLORREF color, TCHAR* text);
	void DrawFormat(int x, int y, TCHAR* format, ...);
	void DrawColorFormat(int x, int y, COLORREF color, TCHAR* format, ...);

	void GetTextSize(TCHAR* text, SIZE& size);
	int GetTextWidth(TCHAR* text);

	void DrawNewui(int x, int y, TCHAR* text);
	void DrawNewuiColor(int x, int y, COLORREF color, TCHAR* text);
	void DrawNewuiFormat(int x, int y, TCHAR* format, ...);
	void DrawNewuiColorFormat(int x, int y, COLORREF color, TCHAR* format, ...);

	void GetNewuiTextSize(TCHAR* text, SIZE& size);
	int GetNewuiTextWidth(TCHAR* text);
};

enum FONT_TYPE{
	ARIAL_8,
	ARIAL_10,
	ARIAL_12,
	ARIAL_14,
	ARIAL_15,
	ARIAL_16,

	ARIAL_BLACK_10,
	ARIAL_BLACK_12,
	ARIAL_BLACK_14,
	ARIAL_BLACK_15,
	ARIAL_BLACK_16,

	ARIAL_UNDERLINE_10,
	ARIAL_UNDERLINE_12,
	ARIAL_UNDERLINE_14,
	ARIAL_UNDERLINE_15,
	ARIAL_UNDERLINE_16,

	ARIAL_ITALIC_10,
	ARIAL_ITALIC_12,
	ARIAL_ITALIC_14,
	ARIAL_ITALIC_15,
	ARIAL_ITALIC_16,
};

extern CGlFont* g_Font[100];
void __fastcall muGetWindowSize(int* vec);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

