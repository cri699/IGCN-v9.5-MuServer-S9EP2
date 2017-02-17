//////////////////////////////////////////////////////////////////////
// Camera.cpp
#include "stdafx.h"
#include "Camera.h"
#include "HookManager.h"
#include "offsets.h"

DWORD* g_Map = (DWORD*)MU_MAP_ADDR;

#pragma optimize("g", on)
CCamera* g_Camera = new CCamera;

void CCamera::Init()
{
//	MakeJmpHook(0x00645FC0, (DWORD)SetFogAsm);
	HookManager.MakeJmpHook(CAMERA_LOOK_AT_HOOK, CCamera::SetLookAtAsm);
	HookManager.MakeJmpHook(CAMERA_ROT_CLIP_HOOK, CCamera::SetRotClip);
//	MakeJmpHook(0x005DAF9D, CCamera::SetClip);

	// Rotclip - 0x005CCB2C
	// ZRot - 0x8187FA8
	DWORD OldProtect;
	VirtualProtect((LPVOID)CAMERA_ROT_ADDR, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	this->m_Rot = (float*)CAMERA_ROT_ADDR;

	VirtualProtect((LPVOID)CAMERA_HEIGHT_ADDR, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	this->m_Height = (float*)CAMERA_HEIGHT_ADDR;

	VirtualProtect((LPVOID)CAMERA_ZOOM_ADDR, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	this->m_Zoom = (float*)CAMERA_ZOOM_ADDR;

	// 004D8DA3			-		new zoom

	/*
	VirtualProtect((LPVOID)(0xD2A828), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
//	*(float*)0xD2A828 = 1690.0f;

	VirtualProtect((LPVOID)(0xD236F0), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
//	*(float*)0xD236F0 = 840.0f;

	VirtualProtect((LPVOID)(0x00D2A874), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
//	*(float*)(0x00D2A874) = 3440.0f;

	VirtualProtect((LPVOID)(0xD2370C), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
//	*(float*)(0xD2370C) = 30000.0f;
	*/

	m_DestHeight = 150.0f;
	m_DestRot = -45.0f;
//	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, NULL, NULL, NULL);

	m_MapView = false;
//	MemSet(CAMERA_ENABLE_FOG, 1, 1);

	// FOG FIX
	//ChangeAddress(0x006452A7, 0x60684A74);
}

void CCamera::Reset()
{
	m_DestHeight = 150.0f;
	m_DestRot = -45.0f;
}

void CCamera::MouseMove(float MoveX, float MoveY)
{
	if(*g_Map == -1 || *g_Map == 0x4a || *g_Map == 0x49 || *g_Map == 0x4E || *g_Map == 34 || *g_Map == 33 ||
		*g_Map == 0x5D || *g_Map == 0x5E /*|| m_UseCam.m_Checked == false*/)
			return;

	//if(!m_InvertY.m_Checked)
	//	MoveY = -MoveY;
	if(MoveY < 0.0f && *this->m_Zoom > 400.0f)
	{
		//this->m_DestHeight -= MoveY;
		*this->m_Zoom += (MoveY*1.5f);
	}
	if(MoveY > 0.0f /*&& *this->m_Zoom < 1100.0f*/)
	{
		//this->m_DestHeight -= MoveY;
		*this->m_Zoom += (MoveY*1.5f);
	}
	
	this->m_DestRot += MoveX/7.0f;
}

float GetV(float x)
{
	return tanh(x/5.0f)*2.0f;
}

void CCamera::CamProc()
{
	if(*g_Map == -1 || *g_Map == 0x4a || *g_Map == 0x49 || *g_Map == 0x4E)
		return;

	if(*g_Map == 34 || *g_Map == 33)
	{
		*m_Height = 150.0f;
		*m_Rot = -45.0f;
		return;
	}

/*	if(m_Smooth.m_Checked)
	{
		x = this->m_DestRot - *this->m_Rot;
		v = GetV(x/2.0f)*1.5f;
		*this->m_Rot+=v;

		x = this->m_DestHeight - *this->m_Height;
		v = GetV(x/7.0f)*14.0f;
		*this->m_Height+=v;
	}*/
	else{
		*m_Height = m_DestHeight;
		*m_Rot = m_DestRot;
	}

#ifdef IGC_CAM_DEBUG
	if(m_MapView)
	{
		*m_Rot = 90.0f+180.0f;
	//	*m_Height = 25500.0f;
	}
#endif
	//DS:[008E01B4]=1690.000
	//DS:[008DF7A8]=840.0000
	//005CC756     C745 94 008054>MOV DWORD PTR SS:[EBP-6C],45548000
	
//	*(float*)0x008E01B4 = 3400.0f;
}

void CCamera::SetLookAt()
{

	//float CharPos[3];
	//GetCharracterPos(CharPos);
	float* CamPos = (float*)CAMERA_POSITION_ADDR;

//	DWORD MapPosX = *(DWORD*)0x07C0AAF8;
//	DWORD MapPosY = *(DWORD*)0x07C0AAFC;

//	MapPosX = (*g_Player)->m_PosX;
//	MapPosY = (*g_Player)->m_PosY;
//	float fMapPosX = (float)MapPosX*100.0f;
//	float fMapPosY = (float)MapPosY*100.0f;

	m_LookAtZ = (-(1/7)*(*m_Zoom))+99;

	//gluLookAt(CamPos[0], CamPos[1], CamPos[2], CharPos[0], CharPos[1], CharPos[2]+m_LookAtZ, 0.0f, 0.0f, 1.0f);
	//
	glDisable(GL_FOG);
	
//	g_Sky->Render();

	this->CamProc();

	
	//Cam zpos
	// 1159.000f
	// 335.000f

//	glEnable(GL_FOG);
	// 1600 - 2200
	// 900 - 1400
	float startFog = (1.177f*CamPos[2])+505.657f;
	float endFog = (1.03f*CamPos[2])+1054.95f;

	glEnable (GL_FOG);
	float FogCol[3]={0.8f,0.8f,0.8f}; // Define a nice light grey
	glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color

	glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
	glFogf(GL_FOG_START, startFog);
	glFogf(GL_FOG_END, endFog);
//	glFogf(GL_FOG_START, 1600.0f);
//	glFogf(GL_FOG_END, 2200.0f);
	glFogf(GL_FOG_DENSITY, 2.0f);
}
#pragma optimize("g", off)

void __declspec(naked) CCamera::SetLookAtAsm()
{
	if(*g_Map == -1 || *g_Map == 0x4a || *g_Map == 0x49 || *g_Map == 0x4E || *g_Map == 34 || *g_Map == 33 ||
		*g_Map == 0x5D || *g_Map == 0x5E)
	{
		if(*g_Map == 34 || *g_Map == 33)
		{
			*g_Camera->m_Height = 150.0f;
			*g_Camera->m_Rot = -45.0f;
		}
		__asm{
			PUSH ECX;
			FLDZ;
			FSTP DWORD PTR SS:[ESP];
			MOV EAX, CAMERA_LOOK_AT_LEAVE;
			JMP EAX;
		}
	}
	g_Camera->SetLookAt();
	__asm{
		MOV EAX, CAMERA_LOOK_AT_BACK_JMP;
		JMP EAX;
	}
}

float rotclip = 0.0f;
void __declspec(naked) CCamera::SetRotClip()
{
	rotclip = -(*g_Camera->m_Rot);
	__asm{
		FLD rotclip;
/*	//	FSUB rotsub;
		FCHS;
		FSTP DWORD PTR SS:[EBP-0xAC];*/
		MOV EAX, CAMERA_ROT_CLIP_BACK_JMP;
		JMP EAX;
	}
}

float viewX;
float viewY;

void CalcViewport()
{
	if(*g_Map == 0x4E)
	{
		viewY  = 30000;//2690 + (abs(*this->MainPos.PosZ - 150) * 4) + 3000;
		viewX  = 39000;// + (abs(*this->MainPos.PosZ - 150) * 2) + 1500;
	}
	else{
		viewY  = 30000;//2690 + (abs(*this->MainPos.PosZ - 150) * 4) + 3000;
		viewX  = 59000;// + (abs(*this->MainPos.PosZ - 150) * 2) + 1500;
	}
}

void __declspec(naked) CCamera::SetClip()
{
	__asm{
		FSTP DWORD PTR SS:[EBP-0xA4];

		CALL CalcViewport;

		MOV EAX, viewX;
		MOV DWORD PTR SS:[EBP-0x78], EAX;
		FLD DWORD PTR SS:[EBP-0x78];
		FMUL DWORD PTR DS:[0xD26868];
		FSTP DWORD PTR SS:[EBP-0xB4];
		FLD DWORD PTR SS:[EBP-0x78];
		FMUL DWORD PTR DS:[0xD2A8C0];
		FSTP DWORD PTR SS:[EBP-0x34];
		FLD viewY;
		MOV EDX, CAMERA_CLIP_BACK_JMP;
		JMP EDX;
	}
}

void __declspec(naked) GetCharracterPos(float* vec)
{
	__asm {
		PUSH EBP;
		MOV EBP, ESP;
		PUSH vec;
		MOV EDX, MU_GET_CHARR_POS1;
		CALL EDX;
		MOV ECX, EAX;
		MOV EDX, MU_GET_CHARR_POS2;
		CALL EDX;
		MOV ECX, EAX;
		MOV EDX, MU_GET_CHARR_POS3;
		CALL EDX;
		MOV EBP, ESP;
		POP EBP;
		RETN;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

