//////////////////////////////////////////////////////////////////////
// SecureMemory.h
#pragma once

namespace SecuredMemory
{
	template<typename Type>
class ZTNumeric {
public:
	template<class T>
	ZTNumeric(T Value) {
		memcpy(m_Buffer, &Value, sizeof(Type));
	};

	template<class T>
	operator T() {
		Type Result;
		memcpy(&Result, m_Buffer, sizeof(T));
		return Result;
	}

private:
	BYTE m_Buffer[8];
};

	enum AssemblerOp
	{
		NONE	= 0xFF,
		EX		= 0x0F,
		SJMP	= 0xEB,
		JMP		= 0xE9,
		JE		= 0x74,
		JNE		= 0x75,
		JGE		= 0x7D,
		JG		= 0x8F,
		JA		= 0x87,
		NOP		= 0x90,
		CALL	= 0xE8,
	};

	static bool SetBuffer(LPVOID Offset, LPVOID Buffer, int Size)
	{
		DWORD OldProtect, BytesReturn;
		if( VirtualProtect(Offset, Size, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE )
		{
			return false;
		}
		memcpy(Offset, Buffer, Size);
		if( VirtualProtect(Offset, Size, OldProtect, &BytesReturn) == FALSE )
		{
			return false;
		}
		return true;
	}

	static bool GetBuffer(LPVOID Offset, LPVOID Buffer, int Size)
	{
		DWORD OldProtect, BytesReturn;
		if( VirtualProtect(Offset, Size, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE )
		{
			return false;
		}
		memcpy(Buffer, Offset, Size);
		if( VirtualProtect(Offset, Size, OldProtect, &BytesReturn) == FALSE )
		{
			return false;
		}
		return true;
	}

	static bool SetRange(LPVOID Offset, WORD Count, BYTE Value)
	{
		LPBYTE Buffer = new BYTE[Count];
		memset(Buffer, Value, Count);
		return SecuredMemory::SetBuffer(Offset, Buffer, Count);
	}

	template <class T>
	static bool SetByte(T Offset, BYTE Value)
	{
		return SetBuffer(reinterpret_cast<LPVOID>(Offset), (LPVOID)&Value, sizeof(BYTE));
	}

	static BYTE GetByte(LPVOID Offset)
	{
		BYTE Value;
		if( GetBuffer(Offset, (LPBYTE)&Value, sizeof(BYTE)) )
		{
			return Value;
		}
		return (BYTE)-1;
	}

	static bool SetWord(LPVOID Offset, WORD Value)
	{
		return SetBuffer(Offset, (LPVOID)&Value, sizeof(WORD));
	}

	static WORD GetWord(LPVOID Offset)
	{
		WORD Value;
		if( GetBuffer(Offset, (LPBYTE)&Value, sizeof(WORD)) )
		{
			return Value;
		}
		return (WORD)-1;
	}

	static bool SetDword(LPVOID Offset, DWORD Value)
	{
		return SetBuffer(Offset, (LPVOID)&Value, sizeof(DWORD));
	}
	
	static DWORD GetDword(LPVOID Offset)
	{
		DWORD Value;
		if( GetBuffer(Offset, (LPVOID)&Value, sizeof(DWORD)) )
		{
			return Value;
		}
		return (DWORD)-1;
	}

	static bool SetDword64(LPVOID Offset, DWORD64 Value)
	{
		return SetBuffer(Offset, (LPVOID)&Value, sizeof(DWORD64));
	}

	static DWORD64 GetDword64(LPVOID Offset)
	{
		DWORD64 Value;
		if( GetBuffer(Offset, (LPVOID)&Value, sizeof(DWORD64)) )
		{
			return Value;
		}
		return (DWORD64)-1;
	}

	static bool SetFloat(LPVOID Offset, float Value)
	{
		return SetBuffer(Offset, (LPVOID)&Value, sizeof(float));
	}

	static float GetFloat(LPVOID Offset)
	{
		float Value;
		if( GetBuffer(Offset, (LPVOID)&Value, sizeof(float)) )
		{
			return Value;
		}
		return (float)-1;
	}
	
	static bool SetDouble(LPVOID Offset, double Value)
	{
		return SetBuffer(Offset, (LPVOID)&Value, sizeof(double));
	}
	
	static double GetDouble(LPVOID Offset)
	{
		double Value;
		if( GetBuffer(Offset, (LPVOID)&Value, sizeof(double)) )
		{
			return Value;
		}
		return (double)-1;
	}

	template<typename Type, class TClass1, class TClass2>
	static ZTNumeric<Type> setNumeric(TClass1 Offset, TClass2 Value) {
		return ZTNumeric<bool>(SecuredMemory::SetBuffer(reinterpret_cast<LPVOID>(Offset), 
			reinterpret_cast<LPVOID>(&Value), sizeof(Type)));
	};

	template<typename Type, class TClass1, class TClass2>
	static ZTNumeric<Type> getNumeric(TClass1 Offset, TClass2 Value) {
		Type Value;
		if (GetBuffer(reinterpret_cast<LPVOID>(Offset), 
			reinterpret_cast<LPVOID>(&Value), sizeof(Type))) {
			return ZTNumeric<Type>(Value);
		}
		return ZTNumeric<Type>((Type)-1);
	};
	
	static bool SetOp(LPVOID Offset, LPVOID DestOffset, BYTE Command)
	{
		BYTE Buffer[5];
		if( Command == NONE )
		{
			return SetDword((LPVOID)((DWORD)Offset + 1),
				(DWORD)DestOffset - ((DWORD)Offset + sizeof(Buffer)));
		}
		DWORD Shift	= (ULONG_PTR)DestOffset - (ULONG_PTR)Offset - sizeof(Buffer);
		Buffer[0] = Command;
		memcpy((LPVOID)&Buffer[1], (LPVOID)&Shift, sizeof(ULONG_PTR));
		return SecuredMemory::SetBuffer(Offset, (LPVOID)Buffer, sizeof(Buffer));
	}

	static bool SetOpEx(LPVOID Offset, LPVOID DestOffset, BYTE Command)
	{
		BYTE Buffer[6];
		if( Command == NONE )
		{
			return SetDword((LPVOID)((DWORD)Offset + 2), 
				(DWORD)DestOffset - (DWORD)Offset + sizeof(Buffer));
		}
		DWORD Shift = (ULONG_PTR)DestOffset - (ULONG_PTR)Offset - sizeof(Buffer);
		Buffer[0]	= EX;
		Buffer[1]	= Command;
		memcpy((LPVOID)&Buffer[2], (LPVOID)&Shift, sizeof(ULONG_PTR));
		return SecuredMemory::SetBuffer(Offset, (LPVOID)Buffer, sizeof(Buffer));
	}
};

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

