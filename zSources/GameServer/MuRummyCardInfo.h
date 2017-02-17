//////////////////////////////////////////////////////////////////////
// MuRummyCardInfo.h
#pragma once
class CMuRummyCardInfo
{
public:
	CMuRummyCardInfo()
	{
		this->m_iColor = 0;
		this->m_iNumber = 0;
		this->m_iState = 0;
		this->m_iSlotNum = -1;
	}

	virtual ~CMuRummyCardInfo() 
	{ 

	}

	void SetColor(int iColor) { this->m_iColor = iColor; }
	void SetNumber(int iNumber) { this->m_iNumber = iNumber; }
	void SetState(int iState) { this->m_iState = iState; }
	void SetSlotNum(int iSlotNum) { this->m_iSlotNum = iSlotNum; }
	int GetColor() { return this->m_iColor; }
	int GetNumber() { return this->m_iNumber; }
	int GetState() { return this->m_iState; }
	int GetSlotNum() { return this->m_iSlotNum; }

	int		m_iColor;
	int		m_iNumber;
	int		m_iState;
	int		m_iSlotNum;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

