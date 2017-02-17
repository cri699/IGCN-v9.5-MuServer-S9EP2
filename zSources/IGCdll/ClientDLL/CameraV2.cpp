//////////////////////////////////////////////////////////////////////
// CameraV2.cpp
#include "stdafx.h"
#include "CameraV2.h"
#include "SecureMemory.h"
#include "offsets.h"
using namespace SecuredMemory;

CameraV2	g_Camera;
// ----------------------------------------------------------------------------------------------

CameraV2::CameraV2()
{
	this->Default.RotationY	= *(float*)oCam_RotY;
	this->Default.RotationZ	= *(float*)oCam_RotZDef;
	this->Default.PositionZ	= *(float*)oCam_PosZ;
	this->Default.ClipX		= *(double*)oCam_ClipX;
	this->Default.ClipY		= *(float*)oCam_ClipY;
	this->Default.ClipZ		= *(double*)oCam_ClipZ;
	this->Default.ClipX2	= *(double*)oCam_ClipX2;
	this->Default.ClipY2	= *(float*)oCam_ClipY2;
	this->Default.ClipZ2	= *(float*)oCam_ClipZ2;
	this->Default.ClipGL	= *(float*)oCam_ClipGL;
	// ----
	this->InMove			= 0;
	this->IsActive			= 0;
	this->RotateMode		= 2;
	this->ZoomPercent		= 0.0;
	this->TempCursorX		= 0;
	this->TempCursorY		= 0;
	// ----
	this->Zoom				= 35.0f;
	this->ZoomPercent		= this->Zoom / ((float)ZOOM_MAX / 100.0f);
}
// ----------------------------------------------------------------------------------------------

CameraV2::~CameraV2()
{

}
// ----------------------------------------------------------------------------------------------

void CameraV2::Load()
{
	SetOp((LPVOID)0x004DF15A, (LPVOID)this->GetZoom, CALL);
}
// ----------------------------------------------------------------------------------------------

void CameraV2::Init()
{
	if( pPlayerState != GameProcess )
	{
		return;
	}
	// ----
	SetFloat((LPVOID)oCam_RotY,		this->Default.RotationY);
	SetFloat((LPVOID)oCam_RotZ,		this->Default.RotationZ);
	SetFloat((LPVOID)oCam_PosZ,		this->Default.PositionZ);
	SetDouble((LPVOID)oCam_ClipX,	this->Default.ClipX);
	SetFloat((LPVOID)oCam_ClipY,	this->Default.ClipY);
	SetDouble((LPVOID)oCam_ClipZ,	this->Default.ClipZ);
	SetDouble((LPVOID)oCam_ClipX2,	this->Default.ClipX2);
	SetFloat((LPVOID)oCam_ClipY2,	this->Default.ClipY2);
	SetFloat((LPVOID)oCam_ClipZ2,	this->Default.ClipZ2);
	SetFloat((LPVOID)oCam_ClipGL,	this->Default.ClipGL);
	// ----
	this->Zoom			= 35.0f;
	this->ZoomPercent	= this->Zoom / ((float)ZOOM_MAX / 100.0f);
}
// ----------------------------------------------------------------------------------------------

void CameraV2::Switch()
{
	if( pPlayerState != GameProcess )
	{
		return;
	}
	// ----
	this->IsActive = !this->IsActive;
}
// ----------------------------------------------------------------------------------------------

void CameraV2::Run(MOUSEHOOKSTRUCTEX * Mouse, WPARAM wParam)
{
	/*if( pMapNumber == 30 || pMapNumber == 31 )
	{
		this->IsActive = false;
		this->Init();
	}*/
	// ----
	switch(wParam)
	{
	case WM_MOUSEWHEEL:
		{
			if( !this->IsActive )
			{
				return;
			}
			// ----
			if( (int)Mouse->mouseData > 0 )
			{
				if( this->Zoom < ZOOM_MAX )
				{
					this->Zoom += ZOOM_INTER;
				}
				else
				{
					this->Zoom = ZOOM_MAX;
				}
			}
			else if( (int)Mouse->mouseData < 0 )
			{
				if( this->Zoom > ZOOM_MIN )
				{
					this->Zoom -= ZOOM_INTER;
				}
				else
				{
					this->Zoom = ZOOM_MIN;
				}
			}
			// ----
			this->ZoomPercent = this->Zoom / ((float)ZOOM_MAX / 100.0f);
			// ----
			SetDouble((LPVOID)oCam_ClipX, PERCF(PERCF(this->Default.ClipX, 500), this->ZoomPercent / 1.8f));
			SetFloat((LPVOID)oCam_ClipY, PERCF(PERCF(this->Default.ClipY, 285), this->ZoomPercent / 1.8f));
			SetDouble((LPVOID)oCam_ClipZ, PERCF(PERCF(this->Default.ClipZ, -395), this->ZoomPercent / 1.8f));
			SetDouble((LPVOID)oCam_ClipX2, PERCF(PERCF(this->Default.ClipX2, 235), this->ZoomPercent / 1.8f));
			SetFloat((LPVOID)oCam_ClipY2, PERCF(PERCF(this->Default.ClipY2, 195), this->ZoomPercent / 1.8f));
			SetFloat((LPVOID)oCam_ClipZ2, PERCF(PERCF(this->Default.ClipZ2, -4000), this->ZoomPercent / 1.8f));
			SetFloat((LPVOID)oCam_ClipGL, PERCF(PERCF(this->Default.ClipGL, 250), this->ZoomPercent / 1.8f));
		}
		break;
		// --
	case WM_MBUTTONDOWN:
		{
			this->InMove = true;
		}
		break;
		// --
	case WM_MBUTTONUP:
		{
			this->InMove = false;
		}
		break;
	}
}
// ----------------------------------------------------------------------------------------------

void CameraV2::Rotate()
{
	if( !this->IsActive )
	{
		return;
	}
	// ----
	if( this->RotateMode == 1 ) //-> Auto rotation
	{
		if( pCursorX <= 150 )
		{
			SetFloat((LPVOID)oCam_RotZ,	*(float*)oCam_RotZ - ROTATION_SENS);
		}
		else if( pCursorX >= 500 )
		{
			SetFloat((LPVOID)oCam_RotZ,	*(float*)oCam_RotZ + ROTATION_SENS);
		}
	}
	else if( this->RotateMode == 2 && this->InMove ) //-> Manual rotation
	{
		if( this->TempCursorX < pCursorX )
		{
			SetFloat((LPVOID)oCam_RotZ,	*(float*)oCam_RotZ + ROTATION_SENS);
		}
		else if( this->TempCursorX > pCursorX )
		{
			SetFloat((LPVOID)oCam_RotZ,	*(float*)oCam_RotZ - ROTATION_SENS);
		}
		// ----
		this->TempCursorX = pCursorX;
	}
}
// ----------------------------------------------------------------------------------------------

float CameraV2::GetZoom(LPVOID This, LPVOID EDX)
{
	if( pPlayerState != GameProcess )
	{
		return pGetCameraZoom(This);
	}
	// ----
	return g_Camera.Zoom;
	//*(float*)((int)pCamThis() + 0x20) = g_Camera.Zoom;
}
// -------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

