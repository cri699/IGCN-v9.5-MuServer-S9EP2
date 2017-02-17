//////////////////////////////////////////////////////////////////////
// CameraV2.h
#ifndef CAMERAV2_H
#define CAMERAV2_H

//0106CB58   0000             ADD BYTE PTR DS:[EAX],AL

//004DFC5C   E8 B12E0000      CALL main.004E2B12
//005B6CFD   55               PUSH EBP

#define	pCamThis				((LPVOID(__cdecl*)()) 0x004DFC5C) // Location 1:1 function format NOT 1:1
#define pGetCameraZoom			((float(__thiscall*)(LPVOID This)) 0x005B6CFD) // S9E2 Location not 1:1 Format 1:1
#define	pCursorX				*(int*)0x8B97684
#define	pCursorY				*(int*)0x8B97688

#define oCam_Zoom				0x1072DB0	// S9E2
#define oCam_PosZDef			0x106CB58	// S9E2 Restore PosZ 0106CB58
#define oCam_PosZ				0x106CB64	// S9E2 PosZ
#define oCam_RotZ				0x8B97650	// S9E2 8B97650
#define oCam_RotZDef			0x106ED00	// S9E2
#define oCam_RotY				0x106ED10	// S9E2
#define oCam_ClipX				0x1073C40	// S9E2
#define oCam_ClipY				0x106ECC8	// S9E2
#define oCam_ClipZ				0x1073C48	// S9E2
#define oCam_ClipXCS			0x1073C18	// S9E2
#define oCam_ClipYCS			0x1073C20	// S9E2
#define oCam_ClipX2				0x1073C38	// S9E2 -- #?
#define oCam_ClipY2				0x1073C40	// S9E2 -- #?
#define oCam_ClipZ2				0x1072DA4	// S9E2
#define oCam_ClipGL				0x106CCCC	// S9E2

#define ROTATION_SENS	5
#define ZOOM_MAX		60
#define ZOOM_MIN		15
#define ZOOM_INTER		1
#define CAMERA_DEBUG	1
#define PERCF(x, y)		((float)x / 100.0f * (float)y)
// ----------------------------------------------------------------------------------------------

typedef struct
{
	float	RotationY;
	float	RotationZ;
	float	PositionZ;
	double	ClipX;
	float	ClipY;
	double	ClipZ;
	double	ClipX2;
	float	ClipY2;
	float	ClipZ2;
	float	ClipGL;
} DefaultData, * lpDefData;
// ----------------------------------------------------------------------------------------------

class CameraV2
{
public:
				CameraV2();
				~CameraV2();
	// ----
	void		Load();
	void		Init();
	void		Switch();
	void		Run(MOUSEHOOKSTRUCTEX * Mouse, WPARAM wParam);
	void		Rotate();
	// ----
	static float __fastcall GetZoom(LPVOID This, LPVOID EDX);
	// ----
	bool		IsActive;
	float		ZoomPercent;
	float		Zoom;
	// ----
private:
	DefaultData Default;
	BYTE		RotateMode;
	bool		InMove;
	int			TempCursorX;
	int			TempCursorY;
	// ----
}; extern CameraV2 g_Camera;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

