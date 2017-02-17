//////////////////////////////////////////////////////////////////////
// Camera.h
#ifndef CAMERA_H
#define CAMERA_H
//0062FA74   51               PUSH ECX

#define CAMERA_LOOK_AT_HOOK 0x0062FA74 //s8e3
#define CAMERA_LOOK_AT_LEAVE 0x0062FA80 //s8e3
#define CAMERA_LOOK_AT_BACK_JMP 0x0062FB0F //s8e3
#define CAMERA_ROT_CLIP_HOOK 0x005CF463 // s8e3
#define CAMERA_ROT_CLIP_BACK_JMP 0x005CF469 // s8e3
#define CAMERA_CLIP_BACK_JMP 0x005D9425 // s8e3

#define CAMERA_ROT_ADDR 0x8B37BF0 // s8e3
#define CAMERA_HEIGHT_ADDR 0x101CAD0 //s8e3
#define CAMERA_ZOOM_ADDR 0x101EBA8 // s8e3

#define CAMERA_POSITION_ADDR 0x8B37BF4 // s8e3

class CCamera {

public:
	bool m_MapView;

	float* m_Rot;
	float* m_Height;
	float* m_Zoom;

	float m_DestRot;
	float m_DestHeight;

	float* m_RotClip;
	float* m_XClip;
	float* m_YClip;

	float m_LookAtZ;

	void Init();
	void Reset();
	void CamThread();
	void MouseMove(float MoveX, float MoveY);
	void CamProc();
	void SetLookAt();
	void SetMapView();
	static void SetLookAtAsm();
	static void SetRotClip();
	static void SetClip();
};

void GetCharracterPos(float* vec);

extern DWORD* g_Map;
extern CCamera* g_Camera;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

