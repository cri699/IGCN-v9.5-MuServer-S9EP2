//////////////////////////////////////////////////////////////////////
// spe.h
#ifndef SPE_H
#define SPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TLog.h"
#include "prodef.h"

// GS-N 0.99.60T - 0x00474A10 - Completed 


class CStreamPacketEngine_Server
{

public:

	CStreamPacketEngine_Server()	// Good
	{
		//this->m_twister.Install();
		this->Clear();
	};

	virtual ~CStreamPacketEngine_Server()	// Good
	{
		//this->m_twister.Finish();
		return;
	};

	void Clear()	// Good 
	{
		this->m_wSize = 0;
	};

protected:

	bool XorData(int iStart, int iEnd, int iDir)	// Good
	{
		if ( iStart < iEnd  )
		{
			g_Log.AddC(TColor::Red,  "CStreamPacketEngine XorData Error %d,%d", iStart, iEnd);
			return false;
		}

		unsigned char byXorFilter[32];

		byXorFilter[0] = 0xAB;
		byXorFilter[1] = 0x11;
		byXorFilter[2] = 0xCD;
		byXorFilter[3] = 0xFE;
		byXorFilter[4] = 0x18;
		byXorFilter[5] = 0x23;
		byXorFilter[6] = 0xC5;
		byXorFilter[7] = 0xA3;
		byXorFilter[8] = 0xCA;
		byXorFilter[9] = 0x33;
		byXorFilter[10] = 0xC1;
		byXorFilter[11] = 0xCC;
		byXorFilter[12] = 0x66;
		byXorFilter[13] = 0x67;
		byXorFilter[14] = 0x21;
		byXorFilter[15] = 0xF3;
		byXorFilter[16] = 0x32;
		byXorFilter[17] = 0x12;
		byXorFilter[18] = 0x15;
		byXorFilter[19] = 0x35;
		byXorFilter[20] = 0x29;
		byXorFilter[21] = 0xFF;
		byXorFilter[22] = 0xFE;
		byXorFilter[23] = 0x1D;
		byXorFilter[24] = 0x44;
		byXorFilter[25] = 0xEF;
		byXorFilter[26] = 0xCD;
		byXorFilter[27] = 0x41;
		byXorFilter[28] = 0x26;
		byXorFilter[29] = 0x3C;
		byXorFilter[30] = 0x4E;
		byXorFilter[31] = 0x4D;

		for ( int i = iStart ; i != iEnd ; i+=iDir )
		{
			this->m_byBuffer[i] ^= this->m_byBuffer[i-1] ^ byXorFilter[ i%32 ];
		}
		return true;
	}

public:

	BOOL AddData(void* pSrc, WORD wSize )	// Good 
	{
		if ( ((this->m_wSize + wSize) >=  2048) ||  (wSize == 0) )
		{
			g_Log.AddC(TColor::Red, "CStreamPacketEngine Adding Buffer Size Error %d", this->m_wSize + wSize);
			int iSize = 2048;
			return FALSE;
		}

		memcpy((void*)&this->m_byBuffer[this->m_wSize], pSrc, wSize);
		this->m_wSize += wSize;
		return TRUE;
	}

	BOOL ExtractPacket(void* pTar)	// Good
	{
		WORD wSize;
		BYTE byXorFilter[32];

		BYTE byTemp[2048];

		

		switch ( this->m_byBuffer[0] )
		{
			case 0xC1:
			case 0xC3:
				wSize = this->m_byBuffer[1];
				break;

			case 0xC2:
			case 0xC4:
				wSize = this->m_byBuffer[1] * 256 + this->m_byBuffer[2];
				break;

			default:
				return true;
				break;
		}

		if ( this->m_wSize < wSize )
		{
			return 2;
		}

		byXorFilter[0] = 0xAB;
		byXorFilter[1] = 0x11;
		byXorFilter[2] = 0xCD;
		byXorFilter[3] = 0xFE;
		byXorFilter[4] = 0x18;
		byXorFilter[5] = 0x23;
		byXorFilter[6] = 0xC5;
		byXorFilter[7] = 0xA3;
		byXorFilter[8] = 0xCA;
		byXorFilter[9] = 0x33;
		byXorFilter[10] = 0xC1;
		byXorFilter[11] = 0xCC;
		byXorFilter[12] = 0x66;
		byXorFilter[13] = 0x67;
		byXorFilter[14] = 0x21;
		byXorFilter[15] = 0xF3;
		byXorFilter[16] = 0x32;
		byXorFilter[17] = 0x12;
		byXorFilter[18] = 0x15;
		byXorFilter[19] = 0x35;
		byXorFilter[20] = 0x29;
		byXorFilter[21] = 0xFF;
		byXorFilter[22] = 0xFE;
		byXorFilter[23] = 0x1D;
		byXorFilter[24] = 0x44;
		byXorFilter[25] = 0xEF;
		byXorFilter[26] = 0xCD;
		byXorFilter[27] = 0x41;
		byXorFilter[28] = 0x26;
		byXorFilter[29] = 0x3C;
		byXorFilter[30] = 0x4E;
		byXorFilter[31] = 0x4D;

#ifndef EMU_NOCRYPT

		//this->Correct();

		if(this->m_byBuffer[0] == 0xC1 || this->m_byBuffer[0] == 0xC3)
		{
			if(!this->XorData(wSize-1, 2, -1))
			{
				return true;
			}

		}
		else if(this->m_byBuffer[0] == 0xC2 || this->m_byBuffer[0] == 0xC4)
		{
			if(!this->XorData(wSize-1, 3, -1))
			{
				return true;
			}
		}
#endif EMU_NOCRYPT

		memcpy(pTar, this->m_byBuffer, wSize);
		this->m_wSize-=wSize;
		memcpy(byTemp, &this->m_byBuffer[wSize], this->m_wSize);
		memcpy(this->m_byBuffer, byTemp, this->m_wSize);
		return FALSE;
	}

	void Correct()
	{
		if (this->m_byBuffer[0] != 0xC1 && this->m_byBuffer[0] != 0xC2)
		{
			return;
		}

		BYTE headCode;
		BYTE *data;
		WORD length;

		if (this->m_byBuffer[0] == 0xC1)
		{
			headCode = this->m_byBuffer[2];
			data = &this->m_byBuffer[3];
			length = this->m_byBuffer[1] - 3;
		}

		else
		{
			headCode = this->m_byBuffer[3];
			data = &this->m_byBuffer[4];
			length = MAKE_NUMBERW(this->m_byBuffer[1], this->m_byBuffer[2]) - 4;
		}

		//this->m_twister.Correct(headCode, data, length);
	}	

private:

	WORD m_wSize;	// 4
	BYTE m_byBuffer[2048];	// 6
	//PacketTwistRunner m_twister; // 1.01.04 GS
	
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

