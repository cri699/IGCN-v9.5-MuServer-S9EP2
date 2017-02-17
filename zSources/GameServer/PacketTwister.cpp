//////////////////////////////////////////////////////////////////////
// PacketTwister.cpp
#include "stdafx.h"
#include "PacketTwister.h"
#include "TLog.h"

void PacketTwister_1::Twist(BYTE *data, int length)
{
	char temp;

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					temp = data[15];
					data[15] = data[8];
					data[8] = temp;
					temp = data[28] >> 1;
					data[28] <<= 7;
					data[28] |= temp;
					data[7] ^= 0xCBu;
					temp = data[17] >> 5;
					data[17] *= 8;
					data[17] |= temp;
					temp = data[31];
					data[31] = data[4];
					data[4] = temp;
					data[7] ^= 0x7Cu;
					data[13] ^= 0x53u;
					temp = (data[30] >> 3) & 1;
					if ((data[30] >> 2) & 1)
						data[30] |= 8u;
					else
						data[30] &= 0xF7u;
					if (temp)
						data[30] |= 4u;
					else
						data[30] &= 0xFBu;
					temp = data[2] >> 3;
					data[2] *= 32;
					data[2] |= temp;
					temp = (data[11] >> 3) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					if (temp)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					data[28] ^= 0x75u;
				}
				else
				{
					temp = (data[5] >> 7) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					if (temp)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					temp = data[10];
					data[10] = data[7];
					data[7] = temp;
					data[0] ^= 0x6Cu;
					temp = data[9];
					data[9] = data[5];
					data[5] = temp;
					temp = data[3] >> 4;
					data[3] *= 16;
					data[3] |= temp;
					temp = data[8];
					data[8] = data[9];
					data[9] = temp;
					data[2] ^= 0x26u;
					temp = (data[9] >> 5) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					if (temp)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
				}
			}
			else
			{
				temp = (data[7] >> 5) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x20u;
				else
					data[7] &= 0xDFu;
				if (temp)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				temp = data[7] >> 6;
				data[7] *= 4;
				data[7] |= temp;
				temp = data[0] >> 6;
				data[0] *= 4;
				data[0] |= temp;
				data[1] ^= 0x1Au;
				temp = (data[2] >> 6) & 1;
				if ((data[2] >> 3) & 1)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				if (temp)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				temp = data[2];
				data[2] = data[5];
				data[5] = temp;
				temp = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= temp;
				temp = data[6];
				data[6] = data[3];
				data[3] = temp;
				temp = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (temp)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				data[1] ^= 0x91u;
				temp = (data[1] >> 4) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				if (temp)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
			}
		}
		else
		{
			temp = (data[2] >> 7) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (temp)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			temp = (data[1] >> 2) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (temp)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			data[0] ^= 0xB1u;
			temp = data[0];
			data[0] = data[3];
			data[3] = temp;
			temp = (data[0] >> 3) & 1;
			if ((data[0] >> 7) & 1)
				data[0] |= 8u;
			else
				data[0] &= 0xF7u;
			if (temp)
				data[0] |= 0x80u;
			else
				data[0] &= 0x7Fu;
			temp = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= temp;
			data[0] ^= 0x9Cu;
			temp = data[1] >> 3;
			data[1] *= 32;
			data[1] |= temp;
			temp = (data[1] >> 3) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (temp)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
		}
	}
}

void PacketTwister_1::Correct(BYTE *data, int length)
{
	char temp;

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[28] ^= 0x75u;
					temp = (data[11] >> 3) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					if (temp)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					temp = data[2] >> 5;
					data[2] *= 8;
					data[2] |= temp;
					temp = (data[30] >> 3) & 1;
					if ((data[30] >> 2) & 1)
						data[30] |= 8u;
					else
						data[30] &= 0xF7u;
					if (temp)
						data[30] |= 4u;
					else
						data[30] &= 0xFBu;
					data[13] ^= 0x53u;
					data[7] ^= 0x7Cu;
					temp = data[31];
					data[31] = data[4];
					data[4] = temp;
					temp = data[17] >> 3;
					data[17] *= 32;
					data[17] |= temp;
					data[7] ^= 0xCBu;
					temp = data[28] >> 7;
					data[28] *= 2;
					data[28] |= temp;
					temp = data[15];
					data[15] = data[8];
					data[8] = temp;
				}
				else
				{
					temp = (data[9] >> 5) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					if (temp)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					data[2] ^= 0x26u;
					temp = data[8];
					data[8] = data[9];
					data[9] = temp;
					temp = data[3] >> 4;
					data[3] *= 16;
					data[3] |= temp;
					temp = data[9];
					data[9] = data[5];
					data[5] = temp;
					data[0] ^= 0x6Cu;
					temp = data[10];
					data[10] = data[7];
					data[7] = temp;
					temp = (data[5] >> 7) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					if (temp)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
				}
			}
			else
			{
				temp = (data[1] >> 4) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				if (temp)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				data[1] ^= 0x91u;
				temp = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (temp)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				temp = data[6];
				data[6] = data[3];
				data[3] = temp;
				temp = data[3] >> 6;
				data[3] *= 4;
				data[3] |= temp;
				temp = data[2];
				data[2] = data[5];
				data[5] = temp;
				temp = (data[2] >> 6) & 1;
				if ((data[2] >> 3) & 1)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				if (temp)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				data[1] ^= 0x1Au;
				temp = data[0] >> 2;
				data[0] <<= 6;
				data[0] |= temp;
				temp = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= temp;
				temp = (data[7] >> 5) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x20u;
				else
					data[7] &= 0xDFu;
				if (temp)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
			}
		}
		else
		{
			temp = (data[1] >> 3) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (temp)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			temp = data[1] >> 5;
			data[1] *= 8;
			data[1] |= temp;
			data[0] ^= 0x9Cu;
			temp = data[2] >> 6;
			data[2] *= 4;
			data[2] |= temp;
			temp = (data[0] >> 3) & 1;
			if ((data[0] >> 7) & 1)
				data[0] |= 8u;
			else
				data[0] &= 0xF7u;
			if (temp)
				data[0] |= 0x80u;
			else
				data[0] &= 0x7Fu;
			temp = data[0];
			data[0] = data[3];
			data[3] = temp;
			data[0] ^= 0xB1u;
			temp = (data[1] >> 2) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (temp)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			temp = (data[2] >> 7) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (temp)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
		}
	}
}

void PacketTwister_2::Twist(BYTE *data, int length)
{
	char temp;

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					temp = data[26];
					data[26] = data[1];
					data[1] = temp;
					temp = data[11];
					data[11] = data[11];
					data[11] = temp;
					temp = data[22];
					data[22] = data[6];
					data[6] = temp;
					temp = data[9];
					data[9] = data[24];
					data[24] = temp;
				}
				else
				{
					temp = (data[8] >> 2) & 1;
					if ((data[8] >> 7) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (temp)
						data[8] |= 0x80u;
					else
						data[8] &= 0x7Fu;
					temp = data[2] >> 6;
					data[2] *= 4;
					data[2] |= temp;
					temp = data[10] >> 4;
					data[10] *= 16;
					data[10] |= temp;
					temp = data[13] >> 6;
					data[13] *= 4;
					data[13] |= temp;
					data[8] ^= 0x37u;
					temp = (data[8] >> 6) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 0x40u;
					else
						data[8] &= 0xBFu;
					if (temp)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					temp = data[7] >> 5;
					data[7] *= 8;
					data[7] |= temp;
					data[4] ^= 0x79u;
					temp = data[15];
					data[15] = data[3];
					data[3] = temp;
					data[14] ^= 0x77u;
					temp = (data[15] >> 2) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (temp)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
				}
			}
			else
			{
				temp = *data >> 6;
				*data *= 4;
				*data |= temp;
				temp = data[7];
				data[7] = *data;
				*data = temp;
				temp = data[4];
				data[4] = data[5];
				data[5] = temp;
				temp = data[3] >> 6;
				data[3] *= 4;
				data[3] |= temp;
				data[5] ^= 0xD2u;
				temp = data[6] >> 2;
				data[6] <<= 6;
				data[6] |= temp;
				data[2] ^= 0xC4u;
				data[5] ^= 0xC4u;
				temp = data[7] >> 6;
				data[7] *= 4;
				data[7] |= temp;
				temp = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= temp;
				*data ^= 0xDAu;
				temp = data[5];
				data[5] = data[1];
				data[1] = temp;
				data[2] ^= 0x75u;
			}
		}
		else
		{
			data[1] ^= 0x51u;
			temp = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (temp)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			temp = *data >> 1;
			*data <<= 7;
			*data |= temp;
			data[1] ^= 0xC1u;
			temp = (data[2] >> 4) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (temp)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
		}
	}
}

void PacketTwister_2::Correct(BYTE *data, int length)
{
	char temp;

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					temp = data[9];
					data[9] = data[24];
					data[24] = temp;
					temp = data[22];
					data[22] = data[6];
					data[6] = temp;
					temp = data[11];
					data[11] = data[11];
					data[11] = temp;
					temp = data[26];
					data[26] = data[1];
					data[1] = temp;
				}
				else
				{
					temp = (data[15] >> 2) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (temp)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					data[14] ^= 0x77u;
					temp = data[15];
					data[15] = data[3];
					data[3] = temp;
					data[4] ^= 0x79u;
					temp = data[7] >> 3;
					data[7] *= 32;
					data[7] |= temp;
					temp = (data[8] >> 6) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 0x40u;
					else
						data[8] &= 0xBFu;
					if (temp)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					data[8] ^= 0x37u;
					temp = data[13] >> 2;
					data[13] <<= 6;
					data[13] |= temp;
					temp = data[10] >> 4;
					data[10] *= 16;
					data[10] |= temp;
					temp = data[2] >> 2;
					data[2] <<= 6;
					data[2] |= temp;
					temp = (data[8] >> 2) & 1;
					if ((data[8] >> 7) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (temp)
						data[8] |= 0x80u;
					else
						data[8] &= 0x7Fu;
				}
			}
			else
			{
				data[2] ^= 0x75u;
				temp = data[5];
				data[5] = data[1];
				data[1] = temp;
				*data ^= 0xDAu;
				temp = data[6] >> 7;
				data[6] *= 2;
				data[6] |= temp;
				temp = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= temp;
				data[5] ^= 0xC4u;
				data[2] ^= 0xC4u;
				temp = data[6] >> 6;
				data[6] *= 4;
				data[6] |= temp;
				data[5] ^= 0xD2u;
				temp = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= temp;
				temp = data[4];
				data[4] = data[5];
				data[5] = temp;
				temp = data[7];
				data[7] = *data;
				*data = temp;
				temp = *data >> 2;
				*data <<= 6;
				*data |= temp;
			}
		}
		else
		{
			temp = (data[2] >> 4) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (temp)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			data[1] ^= 0xC1u;
			temp = *data >> 7;
			*data *= 2;
			*data |= temp;
			temp = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (temp)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			data[1] ^= 0x51u;
		}
	}
}

void PacketTwister_3::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST18_1@17
	char v7; // ST17_1@17
	char v8; // ST16_1@17
	char v9; // ST0D_1@41
	char v10; // ST0A_1@49
	char v11; // ST09_1@49
	char v12; // ST08_1@49
	char v13; // ST05_1@55
	char v14; // ST04_1@56
	char v15; // [sp+6h] [bp-1Ah]@49
	char v16; // [sp+Bh] [bp-15h]@43
	char v17; // [sp+Eh] [bp-12h]@35
	char v18; // [sp+10h] [bp-10h]@29
	char v19; // [sp+12h] [bp-Eh]@23
	char v20; // [sp+14h] [bp-Ch]@17
	char v21; // [sp+19h] [bp-7h]@11
	char v2; // [sp+1Bh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[29] ^= 0xC0u;
					v14 = data[12] >> 4;
					data[12] *= 16;
					data[12] |= v14;
					data[14] ^= 0xAFu;
				}
				else
				{
					v16 = (data[1] >> 2) & 1;
					if ((data[1] >> 6) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v16)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
					v10 = data[2];
					data[2] = data[8];
					data[8] = v10;
					data[5] ^= 0x99u;
					data[1] ^= 0x94u;
					v11 = data[10];
					data[10] = data[7];
					data[7] = v11;
					v12 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v12;
					v15 = (data[2] >> 2) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v15)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					v13 = data[13] >> 6;
					data[13] *= 4;
					data[13] |= v13;
				}
			}
			else
			{
				v21 = (data[5] >> 2) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v21)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v6 = data[6];
				data[6] = data[7];
				data[7] = v6;
				v7 = *data;
				*data = data[5];
				data[5] = v7;
				data[3] ^= 0x6Eu;
				v8 = data[7];
				data[7] = data[7];
				data[7] = v8;
				data[7] ^= 0x2Au;
				v20 = (data[3] >> 7) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v20)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				data[7] ^= 0xE6u;
				v19 = (data[7] >> 1) & 1;
				if ((data[7] >> 2) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v19)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				v18 = (data[2] >> 4) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v18)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				data[5] ^= 0xC2u;
				v17 = (data[2] >> 3) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				if (v17)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v9 = data[5];
				data[5] = data[6];
				data[6] = v9;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v5 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v5;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
		}
	}

}

void PacketTwister_3::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@11
	char v7; // ST11_1@35
	char v8; // ST10_1@35
	char v9; // ST0F_1@35
	char v10; // ST0C_1@43
	char v11; // ST09_1@49
	char v12; // ST08_1@49
	char v13; // ST07_1@49
	char v14; // ST04_1@56
	char v15; // [sp+5h] [bp-1Bh]@49
	char v16; // [sp+Ah] [bp-16h]@43
	char v17; // [sp+Dh] [bp-13h]@35
	char v18; // [sp+12h] [bp-Eh]@29
	char v19; // [sp+14h] [bp-Ch]@23
	char v20; // [sp+16h] [bp-Ah]@17
	char v21; // [sp+18h] [bp-8h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[14] ^= 0xAFu;
					v14 = data[12] >> 4;
					data[12] *= 16;
					data[12] |= v14;
					data[29] ^= 0xC0u;
				}
				else
				{
					v10 = data[13] >> 2;
					data[13] <<= 6;
					data[13] |= v10;
					v16 = (data[2] >> 2) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v16)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					v11 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v11;
					v12 = data[10];
					data[10] = data[7];
					data[7] = v12;
					data[1] ^= 0x94u;
					data[5] ^= 0x99u;
					v13 = data[2];
					data[2] = data[8];
					data[8] = v13;
					v15 = (data[1] >> 2) & 1;
					if ((data[1] >> 6) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v15)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
				}
			}
			else
			{
				v6 = data[5];
				data[5] = data[6];
				data[6] = v6;
				v21 = (data[2] >> 3) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				if (v21)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				data[5] ^= 0xC2u;
				v20 = (data[2] >> 4) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v20)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v19 = (data[7] >> 1) & 1;
				if ((data[7] >> 2) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v19)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				data[7] ^= 0xE6u;
				v18 = (data[3] >> 7) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v18)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				data[7] ^= 0x2Au;
				v7 = data[7];
				data[7] = data[7];
				data[7] = v7;
				data[3] ^= 0x6Eu;
				v8 = *data;
				*data = data[5];
				data[5] = v8;
				v9 = data[6];
				data[6] = data[7];
				data[7] = v9;
				v17 = (data[5] >> 2) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v17)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v3 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v3;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
		}
	}
}

void PacketTwister_4::Twist(BYTE *data, int length)
{
	char v3; // ST33_1@3
	char v4; // ST32_1@3
	char v5; // ST31_1@3
	char v6; // ST2A_1@21
	char v7; // ST21_1@47
	char v8; // ST1E_1@53
	char v9; // ST1D_1@53
	char v10; // ST1C_1@53
	char v11; // ST1B_1@53
	char v12; // ST16_1@67
	char v13; // ST13_1@73
	char v14; // ST12_1@73
	char v15; // ST11_1@73
	char v16; // ST10_1@73
	char v17; // ST0D_1@80
	char v18; // ST0C_1@80
	char v19; // ST0B_1@80
	char v20; // [sp+7h] [bp-2Dh]@86
	char v21; // [sp+9h] [bp-2Bh]@80
	char v22; // [sp+Eh] [bp-26h]@73
	char v23; // [sp+14h] [bp-20h]@67
	char v24; // [sp+17h] [bp-1Dh]@59
	char v25; // [sp+19h] [bp-1Bh]@53
	char v26; // [sp+1Fh] [bp-15h]@47
	char v27; // [sp+22h] [bp-12h]@41
	char v28; // [sp+24h] [bp-10h]@35
	char v29; // [sp+26h] [bp-Eh]@27
	char v30; // [sp+28h] [bp-Ch]@21
	char v31; // [sp+2Bh] [bp-9h]@15
	char v32; // [sp+2Dh] [bp-7h]@9
	char v2; // [sp+2Fh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[13] ^= 0x26u;
					v17 = data[5];
					data[5] = data[3];
					data[3] = v17;
					data[5] ^= 0x91u;
					v18 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v18;
					data[29] ^= 0x96u;
					v19 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v19;
					data[15] ^= 0x61u;
					v21 = (data[3] >> 2) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v21)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					v20 = (data[16] >> 3) & 1;
					if ((data[16] >> 6) & 1)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					if (v20)
						data[16] |= 0x40u;
					else
						data[16] &= 0xBFu;
				}
				else
				{
					v12 = data[10] >> 4;
					data[10] *= 16;
					data[10] |= v12;
					data[7] ^= 0x5Au;
					v23 = (data[15] >> 2) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v23)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v13 = *data;
					*data = data[1];
					data[1] = v13;
					v14 = data[7];
					data[7] = *data;
					*data = v14;
					v15 = data[15];
					data[15] = data[4];
					data[4] = v15;
					data[2] ^= 0x9Eu;
					v16 = data[11] >> 1;
					data[11] <<= 7;
					data[11] |= v16;
					v22 = (data[4] >> 7) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v22)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
				}
			}
			else
			{
				v28 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v28)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				*data ^= 0x14u;
				v27 = (data[4] >> 4) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v27)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v7 = data[2];
				data[2] = data[1];
				data[1] = v7;
				v26 = (data[4] >> 4) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v26)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v8 = data[4];
				data[4] = data[4];
				data[4] = v8;
				v9 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v9;
				data[3] ^= 0xFCu;
				v10 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v10;
				v11 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v11;
				v25 = (data[5] >> 5) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v25)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v24 = (data[3] >> 4) & 1;
				if ((data[3] >> 3) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v24)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				data[6] ^= 0x79u;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v4 = data[2];
			data[2] = data[3];
			data[3] = v4;
			v5 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v5;
			v2 = (data[2] >> 3) & 1;
			if ((data[2] >> 2) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v2)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			v32 = (data[1] >> 5) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			if (v32)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			v31 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v31)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v30 = (data[2] >> 7) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v30)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v29 = (*data >> 2) & 1;
			if ((*data >> 5) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v29)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
		}
	}
}

void PacketTwister_4::Correct(BYTE *data, int length)
{
	char v3; // ST2F_1@15
	char v4; // ST28_1@33
	char v5; // ST27_1@33
	char v6; // ST26_1@33
	char v7; // ST21_1@47
	char v8; // ST20_1@47
	char v9; // ST1F_1@47
	char v10; // ST1E_1@47
	char v11; // ST1B_1@53
	char v12; // ST14_1@73
	char v13; // ST13_1@73
	char v14; // ST12_1@73
	char v15; // ST11_1@73
	char v16; // ST0E_1@79
	char v17; // ST09_1@92
	char v18; // ST08_1@92
	char v19; // ST07_1@92
	char v20; // [sp+Ah] [bp-2Ah]@86
	char v21; // [sp+Ch] [bp-28h]@80
	char v22; // [sp+Fh] [bp-25h]@73
	char v23; // [sp+15h] [bp-1Fh]@67
	char v24; // [sp+17h] [bp-1Dh]@59
	char v25; // [sp+19h] [bp-1Bh]@53
	char v26; // [sp+1Ch] [bp-18h]@47
	char v27; // [sp+22h] [bp-12h]@41
	char v28; // [sp+24h] [bp-10h]@35
	char v29; // [sp+29h] [bp-Bh]@27
	char v30; // [sp+2Bh] [bp-9h]@21
	char v31; // [sp+2Dh] [bp-7h]@15
	char v32; // [sp+30h] [bp-4h]@9
	char v2; // [sp+32h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = (data[16] >> 3) & 1;
					if ((data[16] >> 6) & 1)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					if (v21)
						data[16] |= 0x40u;
					else
						data[16] &= 0xBFu;
					v20 = (data[3] >> 2) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v20)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					data[15] ^= 0x61u;
					v17 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v17;
					data[29] ^= 0x96u;
					v18 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v18;
					data[5] ^= 0x91u;
					v19 = data[5];
					data[5] = data[3];
					data[3] = v19;
					data[13] ^= 0x26u;
				}
				else
				{
					v23 = (data[4] >> 7) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v23)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
					v12 = data[11] >> 7;
					data[11] *= 2;
					data[11] |= v12;
					data[2] ^= 0x9Eu;
					v13 = data[15];
					data[15] = data[4];
					data[4] = v13;
					v14 = data[7];
					data[7] = *data;
					*data = v14;
					v15 = *data;
					*data = data[1];
					data[1] = v15;
					v22 = (data[15] >> 2) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v22)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					data[7] ^= 0x5Au;
					v16 = data[10] >> 4;
					data[10] *= 16;
					data[10] |= v16;
				}
			}
			else
			{
				data[6] ^= 0x79u;
				v28 = (data[3] >> 4) & 1;
				if ((data[3] >> 3) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v28)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				v27 = (data[5] >> 5) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v27)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v7 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v7;
				v8 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v8;
				data[3] ^= 0xFCu;
				v9 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v9;
				v10 = data[4];
				data[4] = data[4];
				data[4] = v10;
				v26 = (data[4] >> 4) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v26)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v11 = data[2];
				data[2] = data[1];
				data[1] = v11;
				v25 = (data[4] >> 4) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v25)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				*data ^= 0x14u;
				v24 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v24)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
			}
		}
		else
		{
			v2 = (*data >> 2) & 1;
			if ((*data >> 5) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			v32 = (data[2] >> 7) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v32)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v31 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v31)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v30 = (data[1] >> 5) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			if (v30)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			v29 = (data[2] >> 3) & 1;
			if ((data[2] >> 2) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v29)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			v5 = data[2];
			data[2] = data[3];
			data[3] = v5;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
		}
	}
}

void PacketTwister_5::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@5
	char v6; // ST1A_1@11
	char v7; // ST19_1@11
	char v8; // ST18_1@11
	char v9; // ST15_1@17
	char v10; // ST12_1@25
	char v11; // ST11_1@25
	char v12; // ST10_1@26
	char v13; // ST0F_1@26
	char v14; // ST0E_1@26
	char v15; // ST09_1@38
	char v16; // ST06_1@44
	char v17; // ST05_1@44
	char v18; // ST04_1@44
	char v19; // [sp+7h] [bp-19h]@38
	char v20; // [sp+Ah] [bp-16h]@32
	char v21; // [sp+Ch] [bp-14h]@26
	char v22; // [sp+13h] [bp-Dh]@19
	char v23; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+1Bh] [bp-5h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[13] >> 7;
					data[13] *= 2;
					data[13] |= v12;
					v13 = data[8] >> 7;
					data[8] *= 2;
					data[8] |= v13;
					v14 = *data;
					*data = data[30];
					data[30] = v14;
					v21 = (data[12] >> 3) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v21)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					v20 = (data[24] >> 3) & 1;
					if ((data[24] >> 6) & 1)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					if (v20)
						data[24] |= 0x40u;
					else
						data[24] &= 0xBFu;
					v15 = data[20] >> 4;
					data[20] *= 16;
					data[20] |= v15;
					v19 = (data[22] >> 5) & 1;
					if ((data[22] >> 1) & 1)
						data[22] |= 0x20u;
					else
						data[22] &= 0xDFu;
					if (v19)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					v16 = data[22];
					data[22] = data[17];
					data[17] = v16;
					data[5] ^= 0xF9u;
					v17 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v17;
					v18 = data[29] >> 1;
					data[29] <<= 7;
					data[29] |= v18;
				}
				else
				{
					v22 = (data[13] >> 1) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					if (v22)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					v10 = data[14];
					data[14] = data[12];
					data[12] = v10;
					v11 = data[5] >> 2;
					data[5] <<= 6;
					data[5] |= v11;
				}
			}
			else
			{
				data[1] ^= 0xAu;
				data[4] ^= 0xA0u;
				v5 = data[2];
				data[2] = data[3];
				data[3] = v5;
				v2 = (data[2] >> 1) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				if (v2)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v6 = data[1];
				data[1] = data[7];
				data[7] = v6;
				v7 = data[1];
				data[1] = data[4];
				data[4] = v7;
				v8 = data[3] >> 7;
				data[3] *= 2;
				data[3] |= v8;
				v23 = (data[3] >> 2) & 1;
				if ((data[3] >> 2) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v23)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				v9 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v9;
			}
		}
		else
		{
			data[1] ^= 0x51u;
			v3 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v3;
			data[2] ^= 0xEDu;
			v4 = *data >> 4;
			*data *= 16;
			*data |= v4;
		}
	}
}

void PacketTwister_5::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@5
	char v6; // ST1A_1@11
	char v7; // ST19_1@11
	char v8; // ST18_1@11
	char v9; // ST15_1@17
	char v10; // ST14_1@19
	char v11; // ST13_1@19
	char v12; // ST10_1@26
	char v13; // ST0F_1@26
	char v14; // ST0E_1@26
	char v15; // ST0B_1@32
	char v16; // ST06_1@44
	char v17; // ST05_1@44
	char v18; // ST04_1@44
	char v19; // [sp+7h] [bp-19h]@38
	char v20; // [sp+9h] [bp-17h]@32
	char v21; // [sp+Ch] [bp-14h]@26
	char v22; // [sp+11h] [bp-Fh]@19
	char v23; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+1Bh] [bp-5h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[29] >> 7;
					data[29] *= 2;
					data[29] |= v12;
					v13 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v13;
					data[5] ^= 0xF9u;
					v14 = data[22];
					data[22] = data[17];
					data[17] = v14;
					v21 = (data[22] >> 5) & 1;
					if ((data[22] >> 1) & 1)
						data[22] |= 0x20u;
					else
						data[22] &= 0xDFu;
					if (v21)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					v15 = data[20] >> 4;
					data[20] *= 16;
					data[20] |= v15;
					v20 = (data[24] >> 3) & 1;
					if ((data[24] >> 6) & 1)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					if (v20)
						data[24] |= 0x40u;
					else
						data[24] &= 0xBFu;
					v19 = (data[12] >> 3) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v19)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					v16 = *data;
					*data = data[30];
					data[30] = v16;
					v17 = data[8] >> 1;
					data[8] <<= 7;
					data[8] |= v17;
					v18 = data[13] >> 1;
					data[13] <<= 7;
					data[13] |= v18;
				}
				else
				{
					v10 = data[5] >> 6;
					data[5] *= 4;
					data[5] |= v10;
					v11 = data[14];
					data[14] = data[12];
					data[12] = v11;
					v22 = (data[13] >> 1) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					if (v22)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
				}
			}
			else
			{
				v5 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v5;
				v2 = (data[3] >> 2) & 1;
				if ((data[3] >> 2) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v2)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				v6 = data[3] >> 1;
				data[3] <<= 7;
				data[3] |= v6;
				v7 = data[1];
				data[1] = data[4];
				data[4] = v7;
				v8 = data[1];
				data[1] = data[7];
				data[7] = v8;
				v23 = (data[2] >> 1) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				if (v23)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v9 = data[2];
				data[2] = data[3];
				data[3] = v9;
				data[4] ^= 0xA0u;
				data[1] ^= 0xAu;
			}
		}
		else
		{
			v3 = *data >> 4;
			*data *= 16;
			*data |= v3;
			data[2] ^= 0xEDu;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			data[1] ^= 0x51u;
		}
	}
}

void PacketTwister_6::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST26_1@15
	char v5; // ST23_1@21
	char v6; // ST20_1@29
	char v7; // ST1B_1@41
	char v8; // ST1A_1@41
	char v9; // ST17_1@47
	char v10; // ST16_1@49
	char v11; // ST15_1@49
	char v12; // ST14_1@49
	char v13; // ST13_1@49
	char v14; // ST10_1@55
	char v15; // ST0F_1@55
	char v16; // ST0E_1@55
	char v17; // ST0D_1@55
	char v18; // ST0C_1@56
	char v19; // ST0B_1@56
	char v20; // ST0A_1@56
	char v21; // ST07_1@62
	char v22; // ST06_1@62
	char v23; // ST05_1@62
	char v24; // ST04_1@62
	char v25; // [sp+8h] [bp-24h]@56
	char v26; // [sp+11h] [bp-1Bh]@49
	char v27; // [sp+18h] [bp-14h]@41
	char v28; // [sp+1Ch] [bp-10h]@35
	char v29; // [sp+1Eh] [bp-Eh]@29
	char v30; // [sp+21h] [bp-Bh]@23
	char v31; // [sp+24h] [bp-8h]@15
	char v32; // [sp+27h] [bp-5h]@9
	char v2; // [sp+29h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[27] >> 1;
					data[27] <<= 7;
					data[27] |= v18;
					v19 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v19;
					data[13] ^= 0x35u;
					v20 = data[30];
					data[30] = data[9];
					data[9] = v20;
					v25 = (data[1] >> 4) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					if (v25)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v21 = data[13] >> 5;
					data[13] *= 8;
					data[13] |= v21;
					v22 = data[15];
					data[15] = data[25];
					data[25] = v22;
					v23 = data[13];
					data[13] = data[18];
					data[18] = v23;
					data[27] ^= 0x11u;
					v24 = data[27] >> 2;
					data[27] <<= 6;
					data[27] |= v24;
				}
				else
				{
					data[3] ^= 0x32u;
					v10 = data[7];
					data[7] = data[7];
					data[7] = v10;
					v11 = data[14];
					data[14] = data[3];
					data[3] = v11;
					v12 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v12;
					v13 = *data >> 5;
					*data *= 8;
					*data |= v13;
					v26 = (data[5] >> 2) & 1;
					if ((data[5] >> 4) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v26)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					v14 = data[10] >> 1;
					data[10] <<= 7;
					data[10] |= v14;
					data[5] ^= 0x84u;
					v15 = *data;
					*data = data[9];
					data[9] = v15;
					data[6] = data[6];
					v16 = data[10];
					data[10] = data[2];
					data[2] = v16;
					v17 = data[2];
					data[2] = data[7];
					data[7] = v17;
				}
			}
			else
			{
				v30 = (data[5] >> 2) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v30)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				data[1] ^= 0xFBu;
				*data ^= 0xABu;
				data[4] ^= 0x78u;
				v6 = data[2] >> 3;
				data[2] *= 32;
				data[2] |= v6;
				v29 = (data[7] >> 7) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v29)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v28 = (data[2] >> 5) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v28)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v7 = data[6];
				data[6] = data[1];
				data[1] = v7;
				data[3] ^= 0xE7u;
				v8 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v8;
				v27 = (data[1] >> 1) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v27)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v9 = *data;
				*data = data[5];
				data[5] = v9;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			v2 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v32 = (data[2] >> 3) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v32)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v4 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v4;
			data[1] ^= 0x3Eu;
			v31 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v31)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v5 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v5;
		}
	}
}

void PacketTwister_6::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST28_1@9
	char v5; // ST23_1@21
	char v6; // ST22_1@23
	char v7; // ST1F_1@29
	char v8; // ST1E_1@29
	char v9; // ST19_1@41
	char v10; // ST16_1@49
	char v11; // ST15_1@49
	char v12; // ST14_1@49
	char v13; // ST13_1@49
	char v14; // ST10_1@55
	char v15; // ST0F_1@55
	char v16; // ST0E_1@55
	char v17; // ST0D_1@55
	char v18; // ST0C_1@56
	char v19; // ST0B_1@56
	char v20; // ST0A_1@56
	char v21; // ST09_1@56
	char v22; // ST06_1@62
	char v23; // ST05_1@62
	char v24; // ST04_1@62
	char v25; // [sp+7h] [bp-25h]@56
	char v26; // [sp+11h] [bp-1Bh]@49
	char v27; // [sp+17h] [bp-15h]@41
	char v28; // [sp+1Ah] [bp-12h]@35
	char v29; // [sp+1Ch] [bp-10h]@29
	char v30; // [sp+20h] [bp-Ch]@23
	char v31; // [sp+24h] [bp-8h]@15
	char v32; // [sp+26h] [bp-6h]@9
	char v2; // [sp+29h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[27] >> 6;
					data[27] *= 4;
					data[27] |= v18;
					data[27] ^= 0x11u;
					v19 = data[13];
					data[13] = data[18];
					data[18] = v19;
					v20 = data[15];
					data[15] = data[25];
					data[25] = v20;
					v21 = data[13] >> 3;
					data[13] *= 32;
					data[13] |= v21;
					v25 = (data[1] >> 4) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					if (v25)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v22 = data[30];
					data[30] = data[9];
					data[9] = v22;
					data[13] ^= 0x35u;
					v23 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v23;
					v24 = data[27] >> 7;
					data[27] *= 2;
					data[27] |= v24;
				}
				else
				{
					v10 = data[2];
					data[2] = data[7];
					data[7] = v10;
					v11 = data[10];
					data[10] = data[2];
					data[2] = v11;
					data[6] = data[6];
					v12 = *data;
					*data = data[9];
					data[9] = v12;
					data[5] ^= 0x84u;
					v13 = data[10] >> 7;
					data[10] *= 2;
					data[10] |= v13;
					v26 = (data[5] >> 2) & 1;
					if ((data[5] >> 4) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v26)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					v14 = *data >> 3;
					*data *= 32;
					*data |= v14;
					v15 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v15;
					v16 = data[14];
					data[14] = data[3];
					data[3] = v16;
					v17 = data[7];
					data[7] = data[7];
					data[7] = v17;
					data[3] ^= 0x32u;
				}
			}
			else
			{
				v6 = *data;
				*data = data[5];
				data[5] = v6;
				v30 = (data[1] >> 1) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v30)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v7 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v7;
				data[3] ^= 0xE7u;
				v8 = data[6];
				data[6] = data[1];
				data[1] = v8;
				v29 = (data[2] >> 5) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v29)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v28 = (data[7] >> 7) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v28)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v9 = data[2] >> 5;
				data[2] *= 8;
				data[2] |= v9;
				data[4] ^= 0x78u;
				*data ^= 0xABu;
				data[1] ^= 0xFBu;
				v27 = (data[5] >> 2) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v27)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
			}
		}
		else
		{
			v3 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v3;
			v2 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[1] ^= 0x3Eu;
			v4 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v4;
			v32 = (data[2] >> 3) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v32)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v31 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v31)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v5 = data[3];
			data[3] = data[2];
			data[2] = v5;
		}
	}
}

void PacketTwister_7::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1A_1@15
	char v5; // ST17_1@21
	char v6; // ST16_1@21
	char v7; // ST13_1@29
	char v8; // ST12_1@29
	char v9; // ST11_1@29
	char v10; // ST10_1@31
	char v11; // ST0F_1@31
	char v12; // ST0E_1@31
	char v13; // ST0D_1@31
	char v14; // ST0C_1@31
	char v15; // ST0B_1@32
	char v16; // ST08_1@38
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // [sp+9h] [bp-17h]@32
	char v20; // [sp+14h] [bp-Ch]@21
	char v21; // [sp+18h] [bp-8h]@15
	char v22; // [sp+1Bh] [bp-5h]@9
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[5] >> 6;
					data[5] *= 4;
					data[5] |= v15;
					v19 = (data[23] >> 1) & 1;
					if ((data[23] >> 4) & 1)
						data[23] |= 2u;
					else
						data[23] &= 0xFDu;
					if (v19)
						data[23] |= 0x10u;
					else
						data[23] &= 0xEFu;
					v16 = data[29] >> 6;
					data[29] *= 4;
					data[29] |= v16;
					v17 = data[24];
					data[24] = data[2];
					data[2] = v17;
					v18 = data[28] >> 1;
					data[28] <<= 7;
					data[28] |= v18;
				}
				else
				{
					data[1] ^= 0x53u;
					v10 = data[11] >> 2;
					data[11] <<= 6;
					data[11] |= v10;
					data[8] ^= 2u;
					data[7] ^= 0x21u;
					v11 = data[15];
					data[15] = *data;
					*data = v11;
					v12 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v12;
					v13 = data[12] >> 7;
					data[12] *= 2;
					data[12] |= v13;
					v14 = data[3] >> 6;
					data[3] *= 4;
					data[3] |= v14;
				}
			}
			else
			{
				v7 = data[1];
				data[1] = data[2];
				data[2] = v7;
				v8 = data[6];
				data[6] = data[3];
				data[3] = v8;
				v9 = data[4];
				data[4] = data[6];
				data[6] = v9;
			}
		}
		else
		{
			v3 = *data;
			*data = data[3];
			data[3] = v3;
			*data ^= 0xD7u;
			v2 = (data[1] >> 5) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v22 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v22)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v4 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v4;
			v21 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v21)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			data[1] ^= 0x2Du;
			v5 = *data >> 6;
			*data *= 4;
			*data |= v5;
			data[1] ^= 0x3Fu;
			v6 = data[3];
			data[3] = *data;
			*data = v6;
			v20 = (data[2] >> 5) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v20)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
		}
	}
}

void PacketTwister_7::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST19_1@15
	char v6; // ST14_1@27
	char v7; // ST13_1@29
	char v8; // ST12_1@29
	char v9; // ST11_1@29
	char v10; // ST10_1@31
	char v11; // ST0F_1@31
	char v12; // ST0E_1@31
	char v13; // ST0D_1@31
	char v14; // ST0C_1@31
	char v15; // ST0B_1@32
	char v16; // ST0A_1@32
	char v17; // ST09_1@32
	char v18; // ST06_1@38
	char v19; // [sp+7h] [bp-19h]@32
	char v20; // [sp+15h] [bp-Bh]@21
	char v21; // [sp+17h] [bp-9h]@15
	char v22; // [sp+1Ah] [bp-6h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[28] >> 7;
					data[28] *= 2;
					data[28] |= v15;
					v16 = data[24];
					data[24] = data[2];
					data[2] = v16;
					v17 = data[29] >> 2;
					data[29] <<= 6;
					data[29] |= v17;
					v19 = (data[23] >> 1) & 1;
					if ((data[23] >> 4) & 1)
						data[23] |= 2u;
					else
						data[23] &= 0xFDu;
					if (v19)
						data[23] |= 0x10u;
					else
						data[23] &= 0xEFu;
					v18 = data[5] >> 2;
					data[5] <<= 6;
					data[5] |= v18;
				}
				else
				{
					v10 = data[3] >> 2;
					data[3] <<= 6;
					data[3] |= v10;
					v11 = data[12] >> 1;
					data[12] <<= 7;
					data[12] |= v11;
					v12 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v12;
					v13 = data[15];
					data[15] = *data;
					*data = v13;
					data[7] ^= 0x21u;
					data[8] ^= 2u;
					v14 = data[11] >> 6;
					data[11] *= 4;
					data[11] |= v14;
					data[1] ^= 0x53u;
				}
			}
			else
			{
				v7 = data[4];
				data[4] = data[6];
				data[6] = v7;
				v8 = data[6];
				data[6] = data[3];
				data[3] = v8;
				v9 = data[1];
				data[1] = data[2];
				data[2] = v9;
			}
		}
		else
		{
			v2 = (data[2] >> 5) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v2)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			data[1] ^= 0x3Fu;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			data[1] ^= 0x2Du;
			v22 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v22)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			v21 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v21)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v20 = (data[1] >> 5) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			if (v20)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			*data ^= 0xD7u;
			v6 = *data;
			*data = data[3];
			data[3] = v6;
		}
	}
}

void PacketTwister_8::Twist(BYTE *data, int length)
{
	char v3; // ST2D_1@11
	char v4; // ST28_1@23
	char v5; // ST27_1@23
	char v6; // ST22_1@35
	char v7; // ST21_1@35
	char v8; // ST20_1@35
	char v9; // ST1D_1@43
	char v10; // ST1C_1@43
	char v11; // ST1B_1@43
	char v12; // ST18_1@49
	char v13; // ST15_1@55
	char v14; // ST14_1@55
	char v15; // ST0F_1@68
	char v16; // ST0E_1@68
	char v17; // ST0B_1@74
	char v18; // [sp+5h] [bp-2Bh]@86
	char v19; // [sp+7h] [bp-29h]@80
	char v20; // [sp+9h] [bp-27h]@74
	char v21; // [sp+Ch] [bp-24h]@68
	char v22; // [sp+10h] [bp-20h]@62
	char v23; // [sp+12h] [bp-1Eh]@56
	char v24; // [sp+16h] [bp-1Ah]@49
	char v25; // [sp+19h] [bp-17h]@43
	char v26; // [sp+1Eh] [bp-12h]@35
	char v27; // [sp+23h] [bp-Dh]@29
	char v28; // [sp+25h] [bp-Bh]@23
	char v29; // [sp+29h] [bp-7h]@17
	char v30; // [sp+2Bh] [bp-5h]@11
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[29] ^= 0x7Au;
					v23 = (data[18] >> 7) & 1;
					if ((data[18] >> 3) & 1)
						data[18] |= 0x80u;
					else
						data[18] &= 0x7Fu;
					if (v23)
						data[18] |= 8u;
					else
						data[18] &= 0xF7u;
					v22 = (data[4] >> 7) & 1;
					if ((data[4] >> 4) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v22)
						data[4] |= 0x10u;
					else
						data[4] &= 0xEFu;
					v15 = data[27];
					data[27] = data[2];
					data[2] = v15;
					v16 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v16;
					v21 = (data[19] >> 6) & 1;
					if ((data[19] >> 3) & 1)
						data[19] |= 0x40u;
					else
						data[19] &= 0xBFu;
					if (v21)
						data[19] |= 8u;
					else
						data[19] &= 0xF7u;
					v17 = data[11] >> 1;
					data[11] <<= 7;
					data[11] |= v17;
					v20 = (data[27] >> 6) & 1;
					if ((data[27] >> 5) & 1)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					if (v20)
						data[27] |= 0x20u;
					else
						data[27] &= 0xDFu;
					v19 = (data[24] >> 2) & 1;
					if ((data[24] >> 6) & 1)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					if (v19)
						data[24] |= 0x40u;
					else
						data[24] &= 0xBFu;
					v18 = (data[28] >> 6) & 1;
					if ((data[28] >> 5) & 1)
						data[28] |= 0x40u;
					else
						data[28] &= 0xBFu;
					if (v18)
						data[28] |= 0x20u;
					else
						data[28] &= 0xDFu;
				}
				else
				{
					v9 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v9;
					v10 = data[8];
					data[8] = data[1];
					data[1] = v10;
					v11 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v11;
					v25 = (data[14] >> 6) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v25)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					v12 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v12;
					data[8] ^= 0xCEu;
					v24 = (*data >> 3) & 1;
					if ((*data >> 1) & 1)
						*data |= 8u;
					else
						*data &= 0xF7u;
					if (v24)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v13 = data[9] >> 6;
					data[9] *= 4;
					data[9] |= v13;
					v14 = data[4] >> 3;
					data[4] *= 32;
					data[4] |= v14;
				}
			}
			else
			{
				v3 = data[7] >> 5;
				data[7] *= 8;
				data[7] |= v3;
				v30 = (data[1] >> 7) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v30)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v29 = (data[2] >> 5) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v29)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v4 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v4;
				v5 = *data >> 1;
				*data <<= 7;
				*data |= v5;
				v28 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v28)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v27 = (data[6] >> 7) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				if (v27)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				v6 = data[7] >> 5;
				data[7] *= 8;
				data[7] |= v6;
				v7 = *data;
				*data = data[2];
				data[2] = v7;
				v8 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v8;
				v26 = (data[4] >> 7) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v26)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
			}
		}
		else
		{
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			data[1] = data[1];
			data[1] ^= 0xB2u;
		}
	}
}

void PacketTwister_8::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@17
	char v4; // ST2A_1@17
	char v5; // ST29_1@17
	char v6; // ST24_1@29
	char v7; // ST23_1@29
	char v8; // ST1E_1@41
	char v9; // ST1D_1@43
	char v10; // ST1C_1@43
	char v11; // ST19_1@49
	char v12; // ST16_1@55
	char v13; // ST15_1@55
	char v14; // ST14_1@55
	char v15; // ST0D_1@74
	char v16; // ST0A_1@80
	char v17; // ST09_1@80
	char v18; // [sp+5h] [bp-2Bh]@86
	char v19; // [sp+7h] [bp-29h]@80
	char v20; // [sp+Bh] [bp-25h]@74
	char v21; // [sp+Eh] [bp-22h]@68
	char v22; // [sp+10h] [bp-20h]@62
	char v23; // [sp+12h] [bp-1Eh]@56
	char v24; // [sp+17h] [bp-19h]@49
	char v25; // [sp+1Ah] [bp-16h]@43
	char v26; // [sp+1Fh] [bp-11h]@35
	char v27; // [sp+21h] [bp-Fh]@29
	char v28; // [sp+25h] [bp-Bh]@23
	char v29; // [sp+27h] [bp-9h]@17
	char v30; // [sp+2Ch] [bp-4h]@11
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v23 = (data[28] >> 6) & 1;
					if ((data[28] >> 5) & 1)
						data[28] |= 0x40u;
					else
						data[28] &= 0xBFu;
					if (v23)
						data[28] |= 0x20u;
					else
						data[28] &= 0xDFu;
					v22 = (data[24] >> 2) & 1;
					if ((data[24] >> 6) & 1)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					if (v22)
						data[24] |= 0x40u;
					else
						data[24] &= 0xBFu;
					v21 = (data[27] >> 6) & 1;
					if ((data[27] >> 5) & 1)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					if (v21)
						data[27] |= 0x20u;
					else
						data[27] &= 0xDFu;
					v15 = data[11] >> 7;
					data[11] *= 2;
					data[11] |= v15;
					v20 = (data[19] >> 6) & 1;
					if ((data[19] >> 3) & 1)
						data[19] |= 0x40u;
					else
						data[19] &= 0xBFu;
					if (v20)
						data[19] |= 8u;
					else
						data[19] &= 0xF7u;
					v16 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v16;
					v17 = data[27];
					data[27] = data[2];
					data[2] = v17;
					v19 = (data[4] >> 7) & 1;
					if ((data[4] >> 4) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v19)
						data[4] |= 0x10u;
					else
						data[4] &= 0xEFu;
					v18 = (data[18] >> 7) & 1;
					if ((data[18] >> 3) & 1)
						data[18] |= 0x80u;
					else
						data[18] &= 0x7Fu;
					if (v18)
						data[18] |= 8u;
					else
						data[18] &= 0xF7u;
					data[29] ^= 0x7Au;
				}
				else
				{
					v9 = data[4] >> 5;
					data[4] *= 8;
					data[4] |= v9;
					v10 = data[9] >> 2;
					data[9] <<= 6;
					data[9] |= v10;
					v25 = (*data >> 3) & 1;
					if ((*data >> 1) & 1)
						*data |= 8u;
					else
						*data &= 0xF7u;
					if (v25)
						*data |= 2u;
					else
						*data &= 0xFDu;
					data[8] ^= 0xCEu;
					v11 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v11;
					v24 = (data[14] >> 6) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v24)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					v12 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v12;
					v13 = data[8];
					data[8] = data[1];
					data[1] = v13;
					v14 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v14;
				}
			}
			else
			{
				v30 = (data[4] >> 7) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v30)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				v3 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v3;
				v4 = *data;
				*data = data[2];
				data[2] = v4;
				v5 = data[7] >> 3;
				data[7] *= 32;
				data[7] |= v5;
				v29 = (data[6] >> 7) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				if (v29)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				v28 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v28)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v6 = *data >> 7;
				*data *= 2;
				*data |= v6;
				v7 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v7;
				v27 = (data[2] >> 5) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v27)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v26 = (data[1] >> 7) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v26)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v8 = data[7] >> 3;
				data[7] *= 32;
				data[7] |= v8;
			}
		}
		else
		{
			data[1] ^= 0xB2u;
			data[1] = data[1];
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
		}
	}
}

void PacketTwister_9::Twist(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST1E_1@27
	char v5; // ST1D_1@27
	char v6; // ST1C_1@27
	char v7; // ST19_1@35
	char v8; // ST18_1@35
	char v9; // ST15_1@41
	char v10; // ST14_1@41
	char v11; // ST11_1@47
	char v12; // ST10_1@49
	char v13; // ST0D_1@55
	char v14; // ST0C_1@55
	char v15; // ST0B_1@56
	char v16; // [sp+7h] [bp-21h]@62
	char v17; // [sp+9h] [bp-1Fh]@56
	char v18; // [sp+Eh] [bp-1Ah]@49
	char v19; // [sp+12h] [bp-16h]@41
	char v20; // [sp+16h] [bp-12h]@35
	char v21; // [sp+1Ah] [bp-Eh]@27
	char v22; // [sp+1Fh] [bp-9h]@21
	char v23; // [sp+21h] [bp-7h]@15
	char v24; // [sp+23h] [bp-5h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[16];
					data[16] = data[17];
					data[17] = v15;
					v17 = (data[10] >> 5) & 1;
					if ((data[10] >> 7) & 1)
						data[10] |= 0x20u;
					else
						data[10] &= 0xDFu;
					if (v17)
						data[10] |= 0x80u;
					else
						data[10] &= 0x7Fu;
					v16 = (data[5] >> 3) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v16)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
				}
				else
				{
					data[8] ^= 0xB5u;
					v12 = data[8] >> 6;
					data[8] *= 4;
					data[8] |= v12;
					v18 = (data[11] >> 2) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v18)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v13 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v13;
					v14 = data[8] >> 6;
					data[8] *= 4;
					data[8] |= v14;
				}
			}
			else
			{
				v7 = *data;
				*data = data[5];
				data[5] = v7;
				v8 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v8;
				v20 = (data[4] >> 4) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v20)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				v9 = data[1];
				data[1] = data[2];
				data[2] = v9;
				v10 = data[2] >> 5;
				data[2] *= 8;
				data[2] |= v10;
				v19 = (data[2] >> 1) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				if (v19)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v11 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v11;
				data[1] ^= 0x11u;
			}
		}
		else
		{
			v2 = (data[3] >> 1) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v24 = (*data >> 7) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			if (v24)
				*data |= 4u;
			else
				*data &= 0xFBu;
			v23 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v23)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v22 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v22)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v4 = *data;
			*data = *data;
			*data = v4;
			v5 = *data >> 6;
			*data *= 4;
			*data |= v5;
			data[3] ^= 0x10u;
			v6 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v6;
			v21 = (data[2] >> 6) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v21)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			*data ^= 0xABu;
		}
	}
}

void PacketTwister_9::Correct(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST1C_1@27
	char v7; // ST19_1@35
	char v8; // ST16_1@41
	char v9; // ST15_1@41
	char v10; // ST12_1@47
	char v11; // ST11_1@47
	char v12; // ST10_1@49
	char v13; // ST0F_1@49
	char v14; // ST0C_1@55
	char v15; // ST07_1@68
	char v16; // [sp+8h] [bp-20h]@62
	char v17; // [sp+Ah] [bp-1Eh]@56
	char v18; // [sp+Dh] [bp-1Bh]@49
	char v19; // [sp+13h] [bp-15h]@41
	char v20; // [sp+17h] [bp-11h]@35
	char v21; // [sp+1Ah] [bp-Eh]@27
	char v22; // [sp+1Dh] [bp-Bh]@21
	char v23; // [sp+1Fh] [bp-9h]@15
	char v24; // [sp+21h] [bp-7h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[5] >> 3) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v17)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					v16 = (data[10] >> 5) & 1;
					if ((data[10] >> 7) & 1)
						data[10] |= 0x20u;
					else
						data[10] &= 0xDFu;
					if (v16)
						data[10] |= 0x80u;
					else
						data[10] &= 0x7Fu;
					v15 = data[16];
					data[16] = data[17];
					data[17] = v15;
				}
				else
				{
					v12 = data[8] >> 2;
					data[8] <<= 6;
					data[8] |= v12;
					v13 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v13;
					v18 = (data[11] >> 2) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v18)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v14 = data[8] >> 2;
					data[8] <<= 6;
					data[8] |= v14;
					data[8] ^= 0xB5u;
				}
			}
			else
			{
				data[1] ^= 0x11u;
				v7 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v7;
				v20 = (data[2] >> 1) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				if (v20)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v8 = data[2] >> 3;
				data[2] *= 32;
				data[2] |= v8;
				v9 = data[1];
				data[1] = data[2];
				data[2] = v9;
				v19 = (data[4] >> 4) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v19)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				v10 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v10;
				v11 = *data;
				*data = data[5];
				data[5] = v11;
			}
		}
		else
		{
			*data ^= 0xABu;
			v2 = (data[2] >> 6) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v3 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v3;
			data[3] ^= 0x10u;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			v5 = *data;
			*data = *data;
			*data = v5;
			v24 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v24)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v23 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v23)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v22 = (*data >> 7) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			if (v22)
				*data |= 4u;
			else
				*data &= 0xFBu;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
			v21 = (data[3] >> 1) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			if (v21)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
		}
	}
}

void PacketTwister_10::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST25_1@15
	char v6; // ST24_1@15
	char v7; // ST23_1@15
	char v8; // ST1C_1@35
	char v9; // ST1B_1@35
	char v10; // ST16_1@47
	char v11; // ST13_1@53
	char v12; // ST12_1@53
	char v13; // ST0F_1@59
	char v14; // ST0E_1@61
	char v15; // ST0D_1@61
	char v16; // ST0C_1@61
	char v17; // ST0B_1@61
	char v18; // ST0A_1@61
	char v19; // ST09_1@62
	char v20; // ST08_1@62
	char v21; // ST07_1@62
	char v22; // ST06_1@62
	char v23; // ST05_1@62
	char v24; // [sp+10h] [bp-1Ch]@53
	char v25; // [sp+14h] [bp-18h]@47
	char v26; // [sp+17h] [bp-15h]@41
	char v27; // [sp+19h] [bp-13h]@35
	char v28; // [sp+1Dh] [bp-Fh]@27
	char v29; // [sp+1Fh] [bp-Dh]@21
	char v30; // [sp+21h] [bp-Bh]@15
	char v31; // [sp+26h] [bp-6h]@9
	char v2; // [sp+28h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v19 = data[29];
					data[29] = data[11];
					data[11] = v19;
					data[14] ^= 0x81u;
					v20 = data[4] >> 6;
					data[4] *= 4;
					data[4] |= v20;
					v21 = data[21];
					data[21] = data[29];
					data[29] = v21;
					v22 = data[20];
					data[20] = data[18];
					data[18] = v22;
					data[28] ^= 0x85u;
					data[11] ^= 0x53u;
					data[18] ^= 0xABu;
					v23 = *data;
					*data = data[4];
					data[4] = v23;
					data[17] ^= 0x5Bu;
				}
				else
				{
					v14 = data[6];
					data[6] = data[4];
					data[4] = v14;
					v15 = data[4] >> 4;
					data[4] *= 16;
					data[4] |= v15;
					data[4] ^= 0x59u;
					data[5] ^= 0xA0u;
					data[15] ^= 0x89u;
					v16 = data[11] >> 3;
					data[11] *= 32;
					data[11] |= v16;
					v17 = data[10];
					data[10] = data[10];
					data[10] = v17;
					v18 = data[10] >> 5;
					data[10] *= 8;
					data[10] |= v18;
				}
			}
			else
			{
				data[6] ^= 0x18u;
				data[2] ^= 8u;
				v8 = *data >> 2;
				*data <<= 6;
				*data |= v8;
				data[3] ^= 0x5Fu;
				v9 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v9;
				v27 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v27)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v26 = (data[1] >> 2) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v26)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v10 = *data >> 6;
				*data *= 4;
				*data |= v10;
				v25 = (*data >> 2) & 1;
				if ((*data >> 7) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v25)
					*data |= 0x80u;
				else
					*data &= 0x7Fu;
				v11 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v11;
				v12 = data[1];
				data[1] = data[4];
				data[4] = v12;
				v24 = (data[1] >> 1) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v24)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				v13 = data[6] >> 6;
				data[6] *= 4;
				data[6] |= v13;
			}
		}
		else
		{
			*data ^= 0xFAu;
			data[1] ^= 0x8Au;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = data[1];
			data[1] = *data;
			*data = v4;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v31 = (data[3] >> 4) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v31)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v5 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v5;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			data[2] ^= 0xECu;
			v7 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v7;
			v30 = (data[1] >> 2) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v30)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v29 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v29)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v28 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v28)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
		}
	}
}

void PacketTwister_10::Correct(BYTE *data, int length)
{
	char v3; // ST25_1@21
	char v4; // ST24_1@21
	char v5; // ST23_1@21
	char v6; // ST1E_1@33
	char v7; // ST1D_1@33
	char v8; // ST1C_1@35
	char v9; // ST19_1@41
	char v10; // ST18_1@41
	char v11; // ST15_1@47
	char v12; // ST10_1@59
	char v13; // ST0F_1@59
	char v14; // ST0E_1@61
	char v15; // ST0D_1@61
	char v16; // ST0C_1@61
	char v17; // ST0B_1@61
	char v18; // ST0A_1@61
	char v19; // ST09_1@62
	char v20; // ST08_1@62
	char v21; // ST07_1@62
	char v22; // ST06_1@62
	char v23; // ST05_1@62
	char v24; // [sp+11h] [bp-1Bh]@53
	char v25; // [sp+13h] [bp-19h]@47
	char v26; // [sp+16h] [bp-16h]@41
	char v27; // [sp+1Ah] [bp-12h]@35
	char v28; // [sp+1Fh] [bp-Dh]@27
	char v29; // [sp+21h] [bp-Bh]@21
	char v30; // [sp+26h] [bp-6h]@15
	char v31; // [sp+28h] [bp-4h]@9
	char v2; // [sp+2Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[17] ^= 0x5Bu;
					v19 = *data;
					*data = data[4];
					data[4] = v19;
					data[18] ^= 0xABu;
					data[11] ^= 0x53u;
					data[28] ^= 0x85u;
					v20 = data[20];
					data[20] = data[18];
					data[18] = v20;
					v21 = data[21];
					data[21] = data[29];
					data[29] = v21;
					v22 = data[4] >> 2;
					data[4] <<= 6;
					data[4] |= v22;
					data[14] ^= 0x81u;
					v23 = data[29];
					data[29] = data[11];
					data[11] = v23;
				}
				else
				{
					v14 = data[10] >> 3;
					data[10] *= 32;
					data[10] |= v14;
					v15 = data[10];
					data[10] = data[10];
					data[10] = v15;
					v16 = data[11] >> 5;
					data[11] *= 8;
					data[11] |= v16;
					data[15] ^= 0x89u;
					data[5] ^= 0xA0u;
					data[4] ^= 0x59u;
					v17 = data[4] >> 4;
					data[4] *= 16;
					data[4] |= v17;
					v18 = data[6];
					data[6] = data[4];
					data[4] = v18;
				}
			}
			else
			{
				v8 = data[6] >> 2;
				data[6] <<= 6;
				data[6] |= v8;
				v27 = (data[1] >> 1) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v27)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				v9 = data[1];
				data[1] = data[4];
				data[4] = v9;
				v10 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v10;
				v26 = (*data >> 2) & 1;
				if ((*data >> 7) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v26)
					*data |= 0x80u;
				else
					*data &= 0x7Fu;
				v11 = *data >> 2;
				*data <<= 6;
				*data |= v11;
				v25 = (data[1] >> 2) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v25)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v24 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v24)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v12 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v12;
				data[3] ^= 0x5Fu;
				v13 = *data >> 6;
				*data *= 4;
				*data |= v13;
				data[2] ^= 8u;
				data[6] ^= 0x18u;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v31 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v31)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v30 = (data[1] >> 2) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v30)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v3 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v3;
			data[2] ^= 0xECu;
			v4 = *data;
			*data = data[2];
			data[2] = v4;
			v5 = data[3] >> 7;
			data[3] *= 2;
			data[3] |= v5;
			v29 = (data[3] >> 4) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v29)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v28 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v28)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
			data[1] ^= 0x8Au;
			*data ^= 0xFAu;
		}
	}
}

void PacketTwister_11::Twist(BYTE *data, int length)
{
	char v3; // ST2D_1@9
	char v4; // ST2C_1@9
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST28_1@9
	char v9; // ST27_1@9
	char v10; // ST20_1@29
	char v11; // ST1F_1@29
	char v12; // ST1E_1@29
	char v13; // ST1D_1@29
	char v14; // ST1C_1@29
	char v15; // ST19_1@37
	char v16; // ST14_1@49
	char v17; // ST13_1@49
	char v18; // ST0C_1@68
	char v19; // ST0B_1@68
	char v20; // ST08_1@74
	char v21; // ST07_1@74
	char v22; // ST04_1@80
	char v23; // [sp+5h] [bp-2Bh]@74
	char v24; // [sp+9h] [bp-27h]@68
	char v25; // [sp+Dh] [bp-23h]@61
	char v26; // [sp+Fh] [bp-21h]@55
	char v27; // [sp+11h] [bp-1Fh]@49
	char v28; // [sp+15h] [bp-1Bh]@43
	char v29; // [sp+17h] [bp-19h]@37
	char v30; // [sp+1Ah] [bp-16h]@29
	char v31; // [sp+21h] [bp-Fh]@23
	char v32; // [sp+23h] [bp-Dh]@17
	char v33; // [sp+25h] [bp-Bh]@9
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[8] >> 7;
					data[8] *= 2;
					data[8] |= v18;
					v19 = data[1];
					data[1] = data[20];
					data[20] = v19;
					v24 = (data[28] >> 2) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 4u;
					else
						data[28] &= 0xFBu;
					if (v24)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
					data[21] ^= 0x28u;
					v20 = data[6];
					data[6] = data[27];
					data[27] = v20;
					v21 = data[14] >> 7;
					data[14] *= 2;
					data[14] |= v21;
					v23 = (data[8] >> 4) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					if (v23)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					data[4] ^= 0xBCu;
					data[22] ^= 0xB0u;
					v22 = data[22];
					data[22] = data[7];
					data[7] = v22;
				}
				else
				{
					v15 = data[10];
					data[10] = data[6];
					data[6] = v15;
					v29 = (*data >> 1) & 1;
					if ((*data >> 2) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v29)
						*data |= 4u;
					else
						*data &= 0xFBu;
					v28 = (data[12] >> 6) & 1;
					if ((data[12] >> 6) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v28)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					data[13] ^= 0x74u;
					v16 = data[12];
					data[12] = data[5];
					data[5] = v16;
					v17 = data[7] >> 7;
					data[7] *= 2;
					data[7] |= v17;
					v27 = (*data >> 4) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					if (v27)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v26 = (data[1] >> 1) & 1;
					if ((data[1] >> 2) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v26)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					v25 = (data[5] >> 6) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					if (v25)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
				}
			}
			else
			{
				v32 = (data[3] >> 2) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v32)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				data[7] ^= 0xC3u;
				v31 = (data[6] >> 2) & 1;
				if ((data[6] >> 4) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v31)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				v10 = data[7];
				data[7] = *data;
				*data = v10;
				data[5] ^= 0x43u;
				data[4] ^= 0x74u;
				v11 = data[1] >> 4;
				data[1] *= 16;
				data[1] |= v11;
				v12 = data[2];
				data[2] = *data;
				*data = v12;
				data[7] ^= 0xA4u;
				v13 = data[7];
				data[7] = data[7];
				data[7] = v13;
				v14 = data[7];
				data[7] = data[2];
				data[2] = v14;
				v30 = (data[7] >> 4) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				if (v30)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				data[4] ^= 0x9Au;
			}
		}
		else
		{
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			v3 = data[2];
			data[2] = data[2];
			data[2] = v3;
			data[1] ^= 0x2Fu;
			v4 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v4;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v6 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v6;
			data[3] ^= 0xC9u;
			v7 = data[3];
			data[3] = data[3];
			data[3] = v7;
			v8 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v8;
			*data ^= 0x3Eu;
			v9 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v9;
			v33 = (data[1] >> 3) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v33)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
		}
	}
}

void PacketTwister_11::Correct(BYTE *data, int length)
{
	char v3; // ST2D_1@9
	char v4; // ST2C_1@9
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST28_1@9
	char v9; // ST27_1@9
	char v10; // ST22_1@23
	char v11; // ST21_1@23
	char v12; // ST20_1@23
	char v13; // ST1F_1@23
	char v14; // ST1E_1@23
	char v15; // ST13_1@55
	char v16; // ST12_1@55
	char v17; // ST0D_1@67
	char v18; // ST0C_1@68
	char v19; // ST09_1@74
	char v20; // ST08_1@74
	char v21; // ST05_1@80
	char v22; // ST04_1@80
	char v23; // [sp+6h] [bp-2Ah]@74
	char v24; // [sp+Ah] [bp-26h]@68
	char v25; // [sp+Eh] [bp-22h]@61
	char v26; // [sp+10h] [bp-20h]@55
	char v27; // [sp+14h] [bp-1Ch]@49
	char v28; // [sp+16h] [bp-1Ah]@43
	char v29; // [sp+18h] [bp-18h]@37
	char v30; // [sp+1Ah] [bp-16h]@29
	char v31; // [sp+1Ch] [bp-14h]@23
	char v32; // [sp+23h] [bp-Dh]@17
	char v33; // [sp+25h] [bp-Bh]@9
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[22];
					data[22] = data[7];
					data[7] = v18;
					data[22] ^= 0xB0u;
					data[4] ^= 0xBCu;
					v24 = (data[8] >> 4) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					if (v24)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					v19 = data[14] >> 1;
					data[14] <<= 7;
					data[14] |= v19;
					v20 = data[6];
					data[6] = data[27];
					data[27] = v20;
					data[21] ^= 0x28u;
					v23 = (data[28] >> 2) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 4u;
					else
						data[28] &= 0xFBu;
					if (v23)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
					v21 = data[1];
					data[1] = data[20];
					data[20] = v21;
					v22 = data[8] >> 1;
					data[8] <<= 7;
					data[8] |= v22;
				}
				else
				{
					v29 = (data[5] >> 6) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					if (v29)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v28 = (data[1] >> 1) & 1;
					if ((data[1] >> 2) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v28)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					v27 = (*data >> 4) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					if (v27)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v15 = data[7] >> 1;
					data[7] <<= 7;
					data[7] |= v15;
					v16 = data[12];
					data[12] = data[5];
					data[5] = v16;
					data[13] ^= 0x74u;
					v26 = (data[12] >> 6) & 1;
					if ((data[12] >> 6) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v26)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					v25 = (*data >> 1) & 1;
					if ((*data >> 2) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v25)
						*data |= 4u;
					else
						*data &= 0xFBu;
					v17 = data[10];
					data[10] = data[6];
					data[6] = v17;
				}
			}
			else
			{
				data[4] ^= 0x9Au;
				v32 = (data[7] >> 4) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				if (v32)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v10 = data[7];
				data[7] = data[2];
				data[2] = v10;
				v11 = data[7];
				data[7] = data[7];
				data[7] = v11;
				data[7] ^= 0xA4u;
				v12 = data[2];
				data[2] = *data;
				*data = v12;
				v13 = data[1] >> 4;
				data[1] *= 16;
				data[1] |= v13;
				data[4] ^= 0x74u;
				data[5] ^= 0x43u;
				v14 = data[7];
				data[7] = *data;
				*data = v14;
				v31 = (data[6] >> 2) & 1;
				if ((data[6] >> 4) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v31)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				data[7] ^= 0xC3u;
				v30 = (data[3] >> 2) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v30)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
			}
		}
		else
		{
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v3 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v3;
			*data ^= 0x3Eu;
			v4 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v4;
			v5 = data[3];
			data[3] = data[3];
			data[3] = v5;
			data[3] ^= 0xC9u;
			v6 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v6;
			v7 = data[1];
			data[1] = data[2];
			data[2] = v7;
			v8 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v8;
			data[1] ^= 0x2Fu;
			v9 = data[2];
			data[2] = data[2];
			data[2] = v9;
			v33 = (data[1] >> 4) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v33)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
		}
	}
}

void PacketTwister_12::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@27
	char v4; // ST2A_1@27
	char v5; // ST27_1@33
	char v6; // ST26_1@33
	char v7; // ST25_1@33
	char v8; // ST24_1@35
	char v9; // ST23_1@35
	char v10; // ST22_1@35
	char v11; // ST1F_1@41
	char v12; // ST18_1@61
	char v13; // ST17_1@61
	char v14; // ST16_1@61
	char v15; // ST15_1@61
	char v16; // ST14_1@61
	char v17; // ST13_1@61
	char v18; // ST10_1@67
	char v19; // ST0F_1@67
	char v20; // ST0E_1@68
	char v21; // ST0B_1@74
	char v22; // ST0A_1@74
	char v23; // ST05_1@86
	char v24; // ST04_1@86
	char v25; // [sp+6h] [bp-2Eh]@80
	char v26; // [sp+8h] [bp-2Ch]@74
	char v27; // [sp+Ch] [bp-28h]@68
	char v28; // [sp+11h] [bp-23h]@61
	char v29; // [sp+19h] [bp-1Bh]@53
	char v30; // [sp+1Bh] [bp-19h]@47
	char v31; // [sp+1Dh] [bp-17h]@41
	char v32; // [sp+20h] [bp-14h]@35
	char v33; // [sp+28h] [bp-Ch]@27
	char v34; // [sp+2Ch] [bp-8h]@21
	char v35; // [sp+2Eh] [bp-6h]@15
	char v36; // [sp+30h] [bp-4h]@9
	char v2; // [sp+32h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[21] >> 6;
					data[21] *= 4;
					data[21] |= v20;
					v27 = (data[5] >> 6) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					if (v27)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					v21 = data[12];
					data[12] = data[12];
					data[12] = v21;
					v22 = data[3] >> 6;
					data[3] *= 4;
					data[3] |= v22;
					data[13] ^= 0x7Fu;
					v26 = (data[21] >> 1) & 1;
					if ((data[21] >> 7) & 1)
						data[21] |= 2u;
					else
						data[21] &= 0xFDu;
					if (v26)
						data[21] |= 0x80u;
					else
						data[21] &= 0x7Fu;
					data[21] ^= 0xF5u;
					v25 = (data[20] >> 1) & 1;
					if ((data[20] >> 6) & 1)
						data[20] |= 2u;
					else
						data[20] &= 0xFDu;
					if (v25)
						data[20] |= 0x40u;
					else
						data[20] &= 0xBFu;
					v23 = data[21] >> 5;
					data[21] *= 8;
					data[21] |= v23;
					v24 = data[13] >> 2;
					data[13] <<= 6;
					data[13] |= v24;
				}
				else
				{
					v12 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v12;
					data[10] ^= 0xFEu;
					v13 = data[15];
					data[15] = data[5];
					data[5] = v13;
					v14 = data[5];
					data[5] = data[6];
					data[6] = v14;
					v15 = data[14] >> 7;
					data[14] *= 2;
					data[14] |= v15;
					v16 = data[14] >> 6;
					data[14] *= 4;
					data[14] |= v16;
					v17 = data[10] >> 7;
					data[10] *= 2;
					data[10] |= v17;
					v28 = (data[3] >> 2) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v28)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					v18 = data[4];
					data[4] = data[4];
					data[4] = v18;
					v19 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v19;
				}
			}
			else
			{
				v8 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v8;
				data[5] ^= 0x7Du;
				data[7] ^= 0x7Eu;
				v9 = data[1] >> 5;
				data[1] *= 8;
				data[1] |= v9;
				v10 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v10;
				v32 = (data[1] >> 7) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v32)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v11 = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= v11;
				*data ^= 0xDAu;
				v31 = (*data >> 5) & 1;
				if ((*data >> 3) & 1)
					*data |= 0x20u;
				else
					*data &= 0xDFu;
				if (v31)
					*data |= 8u;
				else
					*data &= 0xF7u;
				v30 = (data[3] >> 4) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v30)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				v29 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v29)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
			}
		}
		else
		{
			data[3] ^= 0x4Bu;
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			*data ^= 0x32u;
			v36 = (data[2] >> 6) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v36)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v35 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v35)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v34 = (data[1] >> 4) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v34)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = *data;
			*data = v4;
			v33 = (data[3] >> 4) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v33)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v5 = *data >> 1;
			*data <<= 7;
			*data |= v5;
			v6 = *data >> 5;
			*data *= 8;
			*data |= v6;
			data[1] ^= 0x69u;
			v7 = data[3];
			data[3] = data[1];
			data[1] = v7;
		}
	}
}

void PacketTwister_12::Correct(BYTE *data, int length)
{
	char v3; // ST33_1@3
	char v4; // ST32_1@3
	char v5; // ST31_1@3
	char v6; // ST2E_1@9
	char v7; // ST2D_1@9
	char v8; // ST1E_1@53
	char v9; // ST1B_1@59
	char v10; // ST1A_1@59
	char v11; // ST19_1@59
	char v12; // ST18_1@61
	char v13; // ST17_1@61
	char v14; // ST14_1@67
	char v15; // ST13_1@67
	char v16; // ST12_1@67
	char v17; // ST11_1@67
	char v18; // ST10_1@67
	char v19; // ST0F_1@67
	char v20; // ST0E_1@68
	char v21; // ST0D_1@68
	char v22; // ST08_1@80
	char v23; // ST07_1@80
	char v24; // ST04_1@86
	char v25; // [sp+5h] [bp-2Fh]@80
	char v26; // [sp+9h] [bp-2Bh]@74
	char v27; // [sp+Bh] [bp-29h]@68
	char v28; // [sp+15h] [bp-1Fh]@61
	char v29; // [sp+1Ch] [bp-18h]@53
	char v30; // [sp+1Fh] [bp-15h]@47
	char v31; // [sp+21h] [bp-13h]@41
	char v32; // [sp+23h] [bp-11h]@35
	char v33; // [sp+25h] [bp-Fh]@27
	char v34; // [sp+27h] [bp-Dh]@21
	char v35; // [sp+29h] [bp-Bh]@15
	char v36; // [sp+2Bh] [bp-9h]@9
	char v2; // [sp+2Fh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[13] >> 6;
					data[13] *= 4;
					data[13] |= v20;
					v21 = data[21] >> 3;
					data[21] *= 32;
					data[21] |= v21;
					v27 = (data[20] >> 1) & 1;
					if ((data[20] >> 6) & 1)
						data[20] |= 2u;
					else
						data[20] &= 0xFDu;
					if (v27)
						data[20] |= 0x40u;
					else
						data[20] &= 0xBFu;
					data[21] ^= 0xF5u;
					v26 = (data[21] >> 1) & 1;
					if ((data[21] >> 7) & 1)
						data[21] |= 2u;
					else
						data[21] &= 0xFDu;
					if (v26)
						data[21] |= 0x80u;
					else
						data[21] &= 0x7Fu;
					data[13] ^= 0x7Fu;
					v22 = data[3] >> 2;
					data[3] <<= 6;
					data[3] |= v22;
					v23 = data[12];
					data[12] = data[12];
					data[12] = v23;
					v25 = (data[5] >> 6) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					if (v25)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					v24 = data[21] >> 2;
					data[21] <<= 6;
					data[21] |= v24;
				}
				else
				{
					v12 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v12;
					v13 = data[4];
					data[4] = data[4];
					data[4] = v13;
					v28 = (data[3] >> 2) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v28)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					v14 = data[10] >> 1;
					data[10] <<= 7;
					data[10] |= v14;
					v15 = data[14] >> 2;
					data[14] <<= 6;
					data[14] |= v15;
					v16 = data[14] >> 1;
					data[14] <<= 7;
					data[14] |= v16;
					v17 = data[5];
					data[5] = data[6];
					data[6] = v17;
					v18 = data[15];
					data[15] = data[5];
					data[5] = v18;
					data[10] ^= 0xFEu;
					v19 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v19;
				}
			}
			else
			{
				v32 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v32)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v31 = (data[3] >> 4) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v31)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				v30 = (*data >> 5) & 1;
				if ((*data >> 3) & 1)
					*data |= 0x20u;
				else
					*data &= 0xDFu;
				if (v30)
					*data |= 8u;
				else
					*data &= 0xF7u;
				*data ^= 0xDAu;
				v8 = data[7] >> 6;
				data[7] *= 4;
				data[7] |= v8;
				v29 = (data[1] >> 7) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v29)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v9 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v9;
				v10 = data[1] >> 3;
				data[1] *= 32;
				data[1] |= v10;
				data[7] ^= 0x7Eu;
				data[5] ^= 0x7Du;
				v11 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v11;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			data[1] ^= 0x69u;
			v4 = *data >> 3;
			*data *= 32;
			*data |= v4;
			v5 = *data >> 7;
			*data *= 2;
			*data |= v5;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = data[2];
			data[2] = data[1];
			data[1] = v7;
			v36 = (data[1] >> 4) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v36)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			v35 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v35)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v34 = (data[2] >> 6) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v34)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			*data ^= 0x32u;
			v33 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v33)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			data[3] ^= 0x4Bu;
		}
	}
}

void PacketTwister_13::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST29_1@3
	char v6; // ST22_1@21
	char v7; // ST21_1@21
	char v8; // ST20_1@21
	char v9; // ST1F_1@23
	char v10; // ST1E_1@23
	char v11; // ST1D_1@23
	char v12; // ST1C_1@23
	char v13; // ST1B_1@25
	char v14; // ST1A_1@25
	char v15; // ST15_1@37
	char v16; // ST14_1@37
	char v17; // ST13_1@37
	char v18; // ST12_1@37
	char v19; // ST11_1@37
	char v20; // ST10_1@38
	char v21; // ST0F_1@38
	char v22; // ST0E_1@38
	char v23; // ST0D_1@38
	char v24; // ST0C_1@38
	char v25; // ST0B_1@38
	char v26; // ST08_1@44
	char v27; // ST07_1@44
	char v28; // [sp+9h] [bp-23h]@38
	char v29; // [sp+16h] [bp-16h]@31
	char v30; // [sp+18h] [bp-14h]@25
	char v31; // [sp+23h] [bp-9h]@15
	char v32; // [sp+25h] [bp-7h]@9
	char v2; // [sp+27h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[19] >> 2;
					data[19] <<= 6;
					data[19] |= v20;
					data[22] ^= 0xAAu;
					v21 = data[11] >> 3;
					data[11] *= 32;
					data[11] |= v21;
					v22 = data[2];
					data[2] = data[9];
					data[9] = v22;
					v23 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v23;
					v24 = data[19] >> 5;
					data[19] *= 8;
					data[19] |= v24;
					data[5] ^= 0x5Du;
					data[1] ^= 0x6Fu;
					v25 = data[19];
					data[19] = data[30];
					data[30] = v25;
					v28 = (data[18] >> 2) & 1;
					if ((data[18] >> 2) & 1)
						data[18] |= 4u;
					else
						data[18] &= 0xFBu;
					if (v28)
						data[18] |= 4u;
					else
						data[18] &= 0xFBu;
					v26 = data[24];
					data[24] = data[22];
					data[22] = v26;
					v27 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v27;
				}
				else
				{
					v13 = data[2];
					data[2] = data[8];
					data[8] = v13;
					v14 = data[10] >> 4;
					data[10] *= 16;
					data[10] |= v14;
					v30 = (data[15] >> 2) & 1;
					if ((data[15] >> 5) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v30)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					data[12] ^= 0xF7u;
					v29 = (data[14] >> 7) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v29)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					v15 = data[13] >> 6;
					data[13] *= 4;
					data[13] |= v15;
					v16 = data[4];
					data[4] = data[14];
					data[14] = v16;
					data[10] ^= 0xD7u;
					v17 = data[7];
					data[7] = data[13];
					data[13] = v17;
					v18 = data[1] >> 1;
					data[1] <<= 7;
					data[1] |= v18;
					v19 = data[8];
					data[8] = data[9];
					data[9] = v19;
				}
			}
			else
			{
				v9 = data[1];
				data[1] = data[4];
				data[4] = v9;
				v10 = data[3];
				data[3] = *data;
				*data = v10;
				v11 = data[2];
				data[2] = data[1];
				data[1] = v11;
				v12 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v12;
			}
		}
		else
		{
			v3 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v3;
			v4 = data[1];
			data[1] = *data;
			*data = v4;
			v5 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v5;
			v2 = (data[3] >> 3) & 1;
			if ((data[3] >> 4) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v2)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			v32 = (data[1] >> 7) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v32)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v31 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v31)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			data[1] ^= 0xEu;
			v7 = *data >> 1;
			*data <<= 7;
			*data |= v7;
			v8 = data[1];
			data[1] = data[2];
			data[2] = v8;
		}
	}
}

void PacketTwister_13::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST29_1@3
	char v6; // ST22_1@21
	char v7; // ST21_1@21
	char v8; // ST20_1@21
	char v9; // ST1F_1@23
	char v10; // ST1E_1@23
	char v11; // ST1D_1@23
	char v12; // ST1C_1@23
	char v13; // ST1B_1@25
	char v14; // ST1A_1@25
	char v15; // ST19_1@25
	char v16; // ST18_1@25
	char v17; // ST17_1@25
	char v18; // ST12_1@37
	char v19; // ST11_1@37
	char v20; // ST10_1@38
	char v21; // ST0F_1@38
	char v22; // ST0C_1@44
	char v23; // ST0B_1@44
	char v24; // ST0A_1@44
	char v25; // ST09_1@44
	char v26; // ST08_1@44
	char v27; // ST07_1@44
	char v28; // [sp+Dh] [bp-1Fh]@38
	char v29; // [sp+13h] [bp-19h]@31
	char v30; // [sp+15h] [bp-17h]@25
	char v31; // [sp+23h] [bp-9h]@15
	char v32; // [sp+25h] [bp-7h]@9
	char v2; // [sp+27h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v20;
					v21 = data[24];
					data[24] = data[22];
					data[22] = v21;
					v28 = (data[18] >> 2) & 1;
					if ((data[18] >> 2) & 1)
						data[18] |= 4u;
					else
						data[18] &= 0xFBu;
					if (v28)
						data[18] |= 4u;
					else
						data[18] &= 0xFBu;
					v22 = data[19];
					data[19] = data[30];
					data[30] = v22;
					data[1] ^= 0x6Fu;
					data[5] ^= 0x5Du;
					v23 = data[19] >> 3;
					data[19] *= 32;
					data[19] |= v23;
					v24 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v24;
					v25 = data[2];
					data[2] = data[9];
					data[9] = v25;
					v26 = data[11] >> 5;
					data[11] *= 8;
					data[11] |= v26;
					data[22] ^= 0xAAu;
					v27 = data[19] >> 6;
					data[19] *= 4;
					data[19] |= v27;
				}
				else
				{
					v13 = data[8];
					data[8] = data[9];
					data[9] = v13;
					v14 = data[1] >> 7;
					data[1] *= 2;
					data[1] |= v14;
					v15 = data[7];
					data[7] = data[13];
					data[13] = v15;
					data[10] ^= 0xD7u;
					v16 = data[4];
					data[4] = data[14];
					data[14] = v16;
					v17 = data[13] >> 2;
					data[13] <<= 6;
					data[13] |= v17;
					v30 = (data[14] >> 7) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v30)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					data[12] ^= 0xF7u;
					v29 = (data[15] >> 2) & 1;
					if ((data[15] >> 5) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v29)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					v18 = data[10] >> 4;
					data[10] *= 16;
					data[10] |= v18;
					v19 = data[2];
					data[2] = data[8];
					data[8] = v19;
				}
			}
			else
			{
				v9 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v9;
				v10 = data[2];
				data[2] = data[1];
				data[1] = v10;
				v11 = data[3];
				data[3] = *data;
				*data = v11;
				v12 = data[1];
				data[1] = data[4];
				data[4] = v12;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = *data >> 7;
			*data *= 2;
			*data |= v4;
			data[1] ^= 0xEu;
			v5 = data[2];
			data[2] = *data;
			*data = v5;
			v2 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v32 = (data[1] >> 7) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v32)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v31 = (data[3] >> 3) & 1;
			if ((data[3] >> 4) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v31)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			v6 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v6;
			v7 = data[1];
			data[1] = *data;
			*data = v7;
			v8 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v8;
		}
	}
}

void PacketTwister_14::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST1D_1@23
	char v8; // ST1A_1@29
	char v9; // ST19_1@29
	char v10; // ST18_1@29
	char v11; // ST17_1@29
	char v12; // ST14_1@37
	char v13; // ST13_1@37
	char v14; // ST12_1@37
	char v15; // ST11_1@37
	char v16; // ST0E_1@43
	char v17; // ST0B_1@50
	char v18; // ST0A_1@50
	char v19; // ST09_1@50
	char v20; // ST08_1@50
	char v21; // [sp+4h] [bp-24h]@56
	char v22; // [sp+6h] [bp-22h]@50
	char v23; // [sp+Ch] [bp-1Ch]@43
	char v24; // [sp+Fh] [bp-19h]@37
	char v25; // [sp+15h] [bp-13h]@29
	char v26; // [sp+1Bh] [bp-Dh]@23
	char v27; // [sp+1Eh] [bp-Ah]@17
	char v28; // [sp+20h] [bp-8h]@9
	char v2; // [sp+25h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[30];
					data[30] = data[8];
					data[8] = v17;
					v18 = data[18];
					data[18] = data[18];
					data[18] = v18;
					v19 = data[13];
					data[13] = data[9];
					data[9] = v19;
					v20 = data[8];
					data[8] = data[29];
					data[29] = v20;
					v22 = (data[5] >> 3) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v22)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v21 = (data[10] >> 6) & 1;
					if ((data[10] >> 1) & 1)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					if (v21)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
				}
				else
				{
					v12 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v12;
					v13 = data[12] >> 6;
					data[12] *= 4;
					data[12] |= v13;
					data[15] ^= 0xE9u;
					data[9] ^= 0x9Eu;
					v14 = data[13] >> 3;
					data[13] *= 32;
					data[13] |= v14;
					v15 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v15;
					v24 = (*data >> 5) & 1;
					if ((*data >> 5) & 1)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					if (v24)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					v16 = data[4];
					data[4] = data[8];
					data[8] = v16;
					v23 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v23)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
				}
			}
			else
			{
				v27 = (data[1] >> 2) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v27)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v7 = data[3];
				data[3] = *data;
				*data = v7;
				*data ^= 0xFu;
				v26 = (data[3] >> 4) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v26)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				v8 = data[7];
				data[7] = data[4];
				data[4] = v8;
				data[2] ^= 0x50u;
				v9 = data[1];
				data[1] = data[7];
				data[7] = v9;
				v10 = data[4];
				data[4] = data[4];
				data[4] = v10;
				v11 = data[2];
				data[2] = *data;
				*data = v11;
				data[2] ^= 0xB8u;
				v25 = (data[3] >> 5) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v25)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
			data[3] ^= 0xDFu;
			v5 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v5;
			data[2] = data[2];
			v6 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v6;
			v28 = (*data >> 2) & 1;
			if ((*data >> 4) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v28)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
		}
	}
}

void PacketTwister_14::Correct(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST20_1@15
	char v7; // ST1D_1@23
	char v8; // ST1C_1@23
	char v9; // ST1B_1@23
	char v10; // ST1A_1@23
	char v11; // ST17_1@29
	char v12; // ST12_1@43
	char v13; // ST0F_1@49
	char v14; // ST0E_1@49
	char v15; // ST0D_1@49
	char v16; // ST0C_1@49
	char v17; // ST07_1@62
	char v18; // ST06_1@62
	char v19; // ST05_1@62
	char v20; // ST04_1@62
	char v21; // [sp+8h] [bp-20h]@56
	char v22; // [sp+Ah] [bp-1Eh]@50
	char v23; // [sp+10h] [bp-18h]@43
	char v24; // [sp+13h] [bp-15h]@37
	char v25; // [sp+15h] [bp-13h]@29
	char v26; // [sp+18h] [bp-10h]@23
	char v27; // [sp+1Eh] [bp-Ah]@17
	char v28; // [sp+21h] [bp-7h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v22 = (data[10] >> 6) & 1;
					if ((data[10] >> 1) & 1)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					if (v22)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					v21 = (data[5] >> 3) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v21)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v17 = data[8];
					data[8] = data[29];
					data[29] = v17;
					v18 = data[13];
					data[13] = data[9];
					data[9] = v18;
					v19 = data[18];
					data[18] = data[18];
					data[18] = v19;
					v20 = data[30];
					data[30] = data[8];
					data[8] = v20;
				}
				else
				{
					v24 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v24)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v12 = data[4];
					data[4] = data[8];
					data[8] = v12;
					v23 = (*data >> 5) & 1;
					if ((*data >> 5) & 1)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					if (v23)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					v13 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v13;
					v14 = data[13] >> 5;
					data[13] *= 8;
					data[13] |= v14;
					data[9] ^= 0x9Eu;
					data[15] ^= 0xE9u;
					v15 = data[12] >> 2;
					data[12] <<= 6;
					data[12] |= v15;
					v16 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v16;
				}
			}
			else
			{
				v27 = (data[3] >> 5) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v27)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				data[2] ^= 0xB8u;
				v7 = data[2];
				data[2] = *data;
				*data = v7;
				v8 = data[4];
				data[4] = data[4];
				data[4] = v8;
				v9 = data[1];
				data[1] = data[7];
				data[7] = v9;
				data[2] ^= 0x50u;
				v10 = data[7];
				data[7] = data[4];
				data[4] = v10;
				v26 = (data[3] >> 4) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v26)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				*data ^= 0xFu;
				v11 = data[3];
				data[3] = *data;
				*data = v11;
				v25 = (data[1] >> 2) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v25)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
			}
		}
		else
		{
			v2 = (*data >> 2) & 1;
			if ((*data >> 4) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			v3 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v3;
			data[2] = data[2];
			v4 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v4;
			data[3] ^= 0xDFu;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v28 = (data[3] >> 4) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v28)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v6 = data[1];
			data[1] = data[2];
			data[2] = v6;
		}
	}
}

void PacketTwister_15::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST19_1@11
	char v8; // ST18_1@11
	char v9; // ST15_1@17
	char v10; // ST14_1@17
	char v11; // ST11_1@23
	char v12; // ST10_1@25
	char v13; // ST0D_1@31
	char v14; // ST0C_1@31
	char v15; // ST0B_1@31
	char v16; // ST0A_1@32
	char v17; // ST09_1@32
	char v18; // [sp+7h] [bp-19h]@32
	char v19; // [sp+Eh] [bp-12h]@25
	char v20; // [sp+12h] [bp-Eh]@17
	char v21; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+1Ah] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[12];
					data[12] = data[27];
					data[27] = v16;
					v17 = data[25];
					data[25] = data[29];
					data[29] = v17;
					v18 = (data[20] >> 2) & 1;
					if ((data[20] >> 2) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v18)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					data[4] ^= 0x50u;
				}
				else
				{
					v12 = data[8];
					data[8] = data[14];
					data[14] = v12;
					data[5] ^= 0xE7u;
					data[11] ^= 0x5Eu;
					v19 = (data[12] >> 3) & 1;
					if ((data[12] >> 4) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v19)
						data[12] |= 0x10u;
					else
						data[12] &= 0xEFu;
					v13 = data[10];
					data[10] = data[1];
					data[1] = v13;
					v14 = data[2] >> 2;
					data[2] <<= 6;
					data[2] |= v14;
					v15 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v15;
				}
			}
			else
			{
				data[5] ^= 4u;
				v7 = data[7];
				data[7] = *data;
				*data = v7;
				data[4] ^= 0x13u;
				v8 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v8;
				data[1] ^= 0x6Bu;
				v21 = (data[5] >> 2) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v21)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				data[5] ^= 0xDFu;
				v9 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v9;
				v10 = *data >> 6;
				*data *= 4;
				*data |= v10;
				v20 = (data[3] >> 6) & 1;
				if ((data[3] >> 3) & 1)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				if (v20)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				v11 = data[4] >> 3;
				data[4] *= 32;
				data[4] |= v11;
			}
		}
		else
		{
			v3 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v3;
			v4 = data[3];
			data[3] = *data;
			*data = v4;
			*data ^= 0xE3u;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v6 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v6;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
		}
	}
}

void PacketTwister_15::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@11
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST12_1@23
	char v11; // ST11_1@23
	char v12; // ST10_1@25
	char v13; // ST0F_1@25
	char v14; // ST0E_1@25
	char v15; // ST0B_1@31
	char v16; // ST08_1@38
	char v17; // ST07_1@38
	char v18; // [sp+9h] [bp-17h]@32
	char v19; // [sp+Ch] [bp-14h]@25
	char v20; // [sp+13h] [bp-Dh]@17
	char v21; // [sp+17h] [bp-9h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[4] ^= 0x50u;
					v18 = (data[20] >> 2) & 1;
					if ((data[20] >> 2) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v18)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					v16 = data[25];
					data[25] = data[29];
					data[29] = v16;
					v17 = data[12];
					data[12] = data[27];
					data[27] = v17;
				}
				else
				{
					v12 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v12;
					v13 = data[2] >> 6;
					data[2] *= 4;
					data[2] |= v13;
					v14 = data[10];
					data[10] = data[1];
					data[1] = v14;
					v19 = (data[12] >> 3) & 1;
					if ((data[12] >> 4) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v19)
						data[12] |= 0x10u;
					else
						data[12] &= 0xEFu;
					data[11] ^= 0x5Eu;
					data[5] ^= 0xE7u;
					v15 = data[8];
					data[8] = data[14];
					data[14] = v15;
				}
			}
			else
			{
				v7 = data[4] >> 5;
				data[4] *= 8;
				data[4] |= v7;
				v21 = (data[3] >> 6) & 1;
				if ((data[3] >> 3) & 1)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				if (v21)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				v8 = *data >> 2;
				*data <<= 6;
				*data |= v8;
				v9 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v9;
				data[5] ^= 0xDFu;
				v20 = (data[5] >> 2) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v20)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				data[1] ^= 0x6Bu;
				v10 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v10;
				data[4] ^= 0x13u;
				v11 = data[7];
				data[7] = *data;
				*data = v11;
				data[5] ^= 4u;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v3 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v3;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
			*data ^= 0xE3u;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			v6 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v6;
		}
	}
}

void PacketTwister_16::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST1F_1@9
	char v6; // ST1C_1@17
	char v7; // ST1B_1@17
	char v8; // ST18_1@23
	char v9; // ST17_1@23
	char v10; // ST16_1@25
	char v11; // ST15_1@25
	char v12; // ST12_1@31
	char v13; // ST11_1@31
	char v14; // ST10_1@31
	char v15; // ST0F_1@31
	char v16; // ST0E_1@31
	char v17; // ST07_1@50
	char v18; // ST06_1@50
	char v19; // ST05_1@50
	char v20; // ST04_1@50
	char v21; // [sp+8h] [bp-1Ch]@44
	char v22; // [sp+Ah] [bp-1Ah]@38
	char v23; // [sp+Ch] [bp-18h]@31
	char v24; // [sp+13h] [bp-11h]@25
	char v25; // [sp+19h] [bp-Bh]@17
	char v26; // [sp+1Dh] [bp-7h]@11
	char v2; // [sp+20h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[14] ^= 0x60u;
					v22 = (data[23] >> 3) & 1;
					if ((data[23] >> 2) & 1)
						data[23] |= 8u;
					else
						data[23] &= 0xF7u;
					if (v22)
						data[23] |= 4u;
					else
						data[23] &= 0xFBu;
					v21 = (data[20] >> 4) & 1;
					if ((data[20] >> 3) & 1)
						data[20] |= 0x10u;
					else
						data[20] &= 0xEFu;
					if (v21)
						data[20] |= 8u;
					else
						data[20] &= 0xF7u;
					v17 = data[25] >> 1;
					data[25] <<= 7;
					data[25] |= v17;
					data[17] ^= 0xC9u;
					*data ^= 0x98u;
					v18 = data[15];
					data[15] = data[24];
					data[24] = v18;
					v19 = data[18] >> 3;
					data[18] *= 32;
					data[18] |= v19;
					v20 = data[26];
					data[26] = data[2];
					data[2] = v20;
				}
				else
				{
					v10 = data[15];
					data[15] = data[8];
					data[8] = v10;
					v11 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v11;
					data[1] ^= 0x3Au;
					v24 = (data[3] >> 4) & 1;
					if ((data[3] >> 2) & 1)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					if (v24)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					v12 = *data >> 2;
					*data <<= 6;
					*data |= v12;
					v13 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v13;
					v14 = *data >> 6;
					*data *= 4;
					*data |= v14;
					v15 = data[15] >> 3;
					data[15] *= 32;
					data[15] |= v15;
					v16 = data[9];
					data[9] = data[2];
					data[2] = v16;
					v23 = (data[3] >> 7) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v23)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					data[2] ^= 0x95u;
				}
			}
			else
			{
				data[1] ^= 0x60u;
				data[5] ^= 0xB4u;
				data[1] ^= 0x76u;
				v26 = (data[4] >> 5) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v26)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				data[5] ^= 0x9Bu;
				v6 = data[6];
				data[6] = data[6];
				data[6] = v6;
				v7 = data[1];
				data[1] = data[6];
				data[6] = v7;
				v25 = (data[1] >> 1) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v25)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				v8 = *data;
				*data = data[1];
				data[1] = v8;
				*data ^= 0xD8u;
				v9 = data[2];
				data[2] = data[6];
				data[6] = v9;
			}
		}
		else
		{
			v3 = *data >> 2;
			*data <<= 6;
			*data |= v3;
			data[2] ^= 0xA7u;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v2 = (data[2] >> 3) & 1;
			if ((data[2] >> 3) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v2)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			v5 = data[3];
			data[3] = data[3];
			data[3] = v5;
		}
	}
}

void PacketTwister_16::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1E_1@11
	char v7; // ST1D_1@11
	char v8; // ST1A_1@17
	char v9; // ST19_1@17
	char v10; // ST14_1@31
	char v11; // ST13_1@31
	char v12; // ST12_1@31
	char v13; // ST11_1@31
	char v14; // ST10_1@31
	char v15; // ST0D_1@37
	char v16; // ST0C_1@37
	char v17; // ST0B_1@38
	char v18; // ST0A_1@38
	char v19; // ST09_1@38
	char v20; // ST08_1@38
	char v21; // [sp+4h] [bp-20h]@44
	char v22; // [sp+6h] [bp-1Eh]@38
	char v23; // [sp+Eh] [bp-16h]@31
	char v24; // [sp+15h] [bp-Fh]@25
	char v25; // [sp+17h] [bp-Dh]@17
	char v26; // [sp+1Bh] [bp-9h]@11
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[26];
					data[26] = data[2];
					data[2] = v17;
					v18 = data[18] >> 5;
					data[18] *= 8;
					data[18] |= v18;
					v19 = data[15];
					data[15] = data[24];
					data[24] = v19;
					*data ^= 0x98u;
					data[17] ^= 0xC9u;
					v20 = data[25] >> 7;
					data[25] *= 2;
					data[25] |= v20;
					v22 = (data[20] >> 4) & 1;
					if ((data[20] >> 3) & 1)
						data[20] |= 0x10u;
					else
						data[20] &= 0xEFu;
					if (v22)
						data[20] |= 8u;
					else
						data[20] &= 0xF7u;
					v21 = (data[23] >> 3) & 1;
					if ((data[23] >> 2) & 1)
						data[23] |= 8u;
					else
						data[23] &= 0xF7u;
					if (v21)
						data[23] |= 4u;
					else
						data[23] &= 0xFBu;
					data[14] ^= 0x60u;
				}
				else
				{
					data[2] ^= 0x95u;
					v24 = (data[3] >> 7) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v24)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v10 = data[9];
					data[9] = data[2];
					data[2] = v10;
					v11 = data[15] >> 5;
					data[15] *= 8;
					data[15] |= v11;
					v12 = *data >> 2;
					*data <<= 6;
					*data |= v12;
					v13 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v13;
					v14 = *data >> 6;
					*data *= 4;
					*data |= v14;
					v23 = (data[3] >> 4) & 1;
					if ((data[3] >> 2) & 1)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					if (v23)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					data[1] ^= 0x3Au;
					v15 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v15;
					v16 = data[15];
					data[15] = data[8];
					data[8] = v16;
				}
			}
			else
			{
				v6 = data[2];
				data[2] = data[6];
				data[6] = v6;
				*data ^= 0xD8u;
				v7 = *data;
				*data = data[1];
				data[1] = v7;
				v26 = (data[1] >> 1) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v26)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				v8 = data[1];
				data[1] = data[6];
				data[6] = v8;
				v9 = data[6];
				data[6] = data[6];
				data[6] = v9;
				data[5] ^= 0x9Bu;
				v25 = (data[4] >> 5) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v25)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				data[1] ^= 0x76u;
				data[5] ^= 0xB4u;
				data[1] ^= 0x60u;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[3];
			data[3] = v3;
			v2 = (data[2] >> 3) & 1;
			if ((data[2] >> 3) & 1)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			if (v2)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			data[2] ^= 0xA7u;
			v5 = *data >> 6;
			*data *= 4;
			*data |= v5;
		}
	}
}

void PacketTwister_17::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST16_1@15
	char v9; // ST15_1@15
	char v10; // ST14_1@17
	char v11; // ST13_1@17
	char v12; // ST12_1@19
	char v13; // ST0F_1@25
	char v14; // ST0E_1@25
	char v15; // ST0D_1@25
	char v16; // ST0A_1@31
	char v17; // ST09_1@32
	char v18; // ST08_1@32
	char v19; // ST07_1@32
	char v20; // ST06_1@32
	char v21; // [sp+4h] [bp-1Ch]@32
	char v22; // [sp+Bh] [bp-15h]@25
	char v23; // [sp+10h] [bp-10h]@19
	char v24; // [sp+17h] [bp-9h]@9
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[10];
					data[10] = data[4];
					data[4] = v17;
					v18 = data[16] >> 1;
					data[16] <<= 7;
					data[16] |= v18;
					data[15] ^= 0x6Bu;
					data[23] ^= 0x1Cu;
					v19 = data[14];
					data[14] = data[7];
					data[7] = v19;
					v20 = data[20];
					data[20] = data[14];
					data[14] = v20;
					v21 = (data[6] >> 2) & 1;
					if ((data[6] >> 2) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v21)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					data[12] ^= 0xF9u;
				}
				else
				{
					v12 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v12;
					data[3] ^= 0x90u;
					v23 = (data[13] >> 5) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v23)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v13 = data[10];
					data[10] = data[9];
					data[9] = v13;
					v14 = data[12];
					data[12] = data[12];
					data[12] = v14;
					v15 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v15;
					*data ^= 0x58u;
					v22 = (data[5] >> 2) & 1;
					if ((data[5] >> 4) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v22)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					v16 = data[3];
					data[3] = data[2];
					data[2] = v16;
				}
			}
			else
			{
				v10 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v10;
				data[2] ^= 0x67u;
				v11 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v11;
				data[3] ^= 0xAu;
				data[1] ^= 0xCDu;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			v2 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			v5 = *data;
			*data = data[3];
			data[3] = v5;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			v7 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v7;
			v24 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v24)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			data[1] ^= 0x16u;
			v8 = *data >> 2;
			*data <<= 6;
			*data |= v8;
			data[3] ^= 0x23u;
			data[2] ^= 0x6Fu;
			v9 = *data >> 5;
			*data *= 8;
			*data |= v9;
		}
	}
}

void PacketTwister_17::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST18_1@9
	char v9; // ST15_1@15
	char v10; // ST14_1@17
	char v11; // ST13_1@17
	char v12; // ST12_1@19
	char v13; // ST0F_1@25
	char v14; // ST0E_1@25
	char v15; // ST0D_1@25
	char v16; // ST0A_1@31
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // ST05_1@38
	char v20; // ST04_1@38
	char v21; // [sp+8h] [bp-18h]@32
	char v22; // [sp+Bh] [bp-15h]@25
	char v23; // [sp+10h] [bp-10h]@19
	char v24; // [sp+16h] [bp-Ah]@9
	char v2; // [sp+1Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[12] ^= 0xF9u;
					v21 = (data[6] >> 2) & 1;
					if ((data[6] >> 2) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v21)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					v17 = data[20];
					data[20] = data[14];
					data[14] = v17;
					v18 = data[14];
					data[14] = data[7];
					data[7] = v18;
					data[23] ^= 0x1Cu;
					data[15] ^= 0x6Bu;
					v19 = data[16] >> 7;
					data[16] *= 2;
					data[16] |= v19;
					v20 = data[10];
					data[10] = data[4];
					data[4] = v20;
				}
				else
				{
					v12 = data[3];
					data[3] = data[2];
					data[2] = v12;
					v23 = (data[5] >> 2) & 1;
					if ((data[5] >> 4) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v23)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					*data ^= 0x58u;
					v13 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v13;
					v14 = data[12];
					data[12] = data[12];
					data[12] = v14;
					v15 = data[10];
					data[10] = data[9];
					data[9] = v15;
					v22 = (data[13] >> 5) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v22)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					data[3] ^= 0x90u;
					v16 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v16;
				}
			}
			else
			{
				data[1] ^= 0xCDu;
				data[3] ^= 0xAu;
				v10 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v10;
				data[2] ^= 0x67u;
				v11 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v11;
			}
		}
		else
		{
			v3 = *data >> 3;
			*data *= 32;
			*data |= v3;
			data[2] ^= 0x6Fu;
			data[3] ^= 0x23u;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			data[1] ^= 0x16u;
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			v5 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v5;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			v7 = *data;
			*data = data[3];
			data[3] = v7;
			v8 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v8;
			v24 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v24)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v9 = data[3];
			data[3] = data[1];
			data[1] = v9;
		}
	}
}

void PacketTwister_18::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2E_1@3
	char v5; // ST2D_1@3
	char v6; // ST2C_1@3
	char v7; // ST2B_1@3
	char v8; // ST28_1@9
	char v9; // ST27_1@11
	char v10; // ST24_1@17
	char v11; // ST21_1@23
	char v12; // ST1E_1@31
	char v13; // ST19_1@43
	char v14; // ST18_1@43
	char v15; // ST11_1@61
	char v16; // ST10_1@62
	char v17; // ST0D_1@68
	char v18; // ST0A_1@74
	char v19; // ST07_1@80
	char v20; // ST04_1@86
	char v21; // [sp+5h] [bp-2Bh]@80
	char v22; // [sp+8h] [bp-28h]@74
	char v23; // [sp+Bh] [bp-25h]@68
	char v24; // [sp+Eh] [bp-22h]@62
	char v25; // [sp+12h] [bp-1Eh]@55
	char v26; // [sp+14h] [bp-1Ch]@49
	char v27; // [sp+16h] [bp-1Ah]@43
	char v28; // [sp+1Ah] [bp-16h]@37
	char v29; // [sp+1Ch] [bp-14h]@31
	char v30; // [sp+1Fh] [bp-11h]@25
	char v31; // [sp+22h] [bp-Eh]@17
	char v32; // [sp+25h] [bp-Bh]@11
	char v2; // [sp+29h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[21] ^= 0xAFu;
					v16 = data[20];
					data[20] = data[2];
					data[2] = v16;
					v24 = (data[27] >> 7) & 1;
					if ((data[27] >> 2) & 1)
						data[27] |= 0x80u;
					else
						data[27] &= 0x7Fu;
					if (v24)
						data[27] |= 4u;
					else
						data[27] &= 0xFBu;
					v17 = data[22] >> 5;
					data[22] *= 8;
					data[22] |= v17;
					data[9] ^= 0x12u;
					v23 = (data[22] >> 6) & 1;
					if ((data[22] >> 3) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v23)
						data[22] |= 8u;
					else
						data[22] &= 0xF7u;
					v18 = data[20] >> 4;
					data[20] *= 16;
					data[20] |= v18;
					v22 = (data[10] >> 4) & 1;
					if ((data[10] >> 5) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v22)
						data[10] |= 0x20u;
					else
						data[10] &= 0xDFu;
					v19 = data[13];
					data[13] = data[25];
					data[25] = v19;
					v21 = (data[26] >> 3) & 1;
					if ((data[26] >> 1) & 1)
						data[26] |= 8u;
					else
						data[26] &= 0xF7u;
					if (v21)
						data[26] |= 2u;
					else
						data[26] &= 0xFDu;
					data[26] ^= 0x92u;
					data[28] ^= 0xBAu;
					v20 = data[16] >> 2;
					data[16] <<= 6;
					data[16] |= v20;
				}
				else
				{
					v30 = (data[10] >> 1) & 1;
					if ((data[10] >> 3) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v30)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
					v12 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v12;
					data[3] ^= 0x57u;
					data[14] ^= 1u;
					v29 = (data[15] >> 6) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 0x40u;
					else
						data[15] &= 0xBFu;
					if (v29)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					data[13] ^= 0x84u;
					v28 = (data[1] >> 2) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v28)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v13 = data[3] >> 3;
					data[3] *= 32;
					data[3] |= v13;
					v14 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v14;
					v27 = (data[7] >> 3) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v27)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					v26 = (data[6] >> 4) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v26)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					v25 = (data[3] >> 6) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					if (v25)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					v15 = data[2];
					data[2] = data[4];
					data[4] = v15;
				}
			}
			else
			{
				v9 = data[7];
				data[7] = data[5];
				data[5] = v9;
				v32 = (data[4] >> 2) & 1;
				if ((data[4] >> 6) & 1)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				if (v32)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				v10 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v10;
				v31 = (data[4] >> 7) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v31)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				*data ^= 0xD0u;
				v11 = data[6];
				data[6] = data[3];
				data[3] = v11;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			data[1] ^= 0xCEu;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			v5 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v5;
			*data ^= 0x68u;
			v6 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v6;
			v7 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v7;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v8 = *data;
			*data = data[1];
			data[1] = v8;
		}
	}
}

void PacketTwister_18::Correct(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2C_1@9
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST28_1@9
	char v9; // ST27_1@11
	char v10; // ST24_1@17
	char v11; // ST21_1@23
	char v12; // ST20_1@25
	char v13; // ST19_1@43
	char v14; // ST18_1@43
	char v15; // ST13_1@55
	char v16; // ST10_1@62
	char v17; // ST0D_1@68
	char v18; // ST0A_1@74
	char v19; // ST07_1@80
	char v20; // ST04_1@86
	char v21; // [sp+5h] [bp-2Bh]@80
	char v22; // [sp+8h] [bp-28h]@74
	char v23; // [sp+Bh] [bp-25h]@68
	char v24; // [sp+Eh] [bp-22h]@62
	char v25; // [sp+11h] [bp-1Fh]@55
	char v26; // [sp+14h] [bp-1Ch]@49
	char v27; // [sp+16h] [bp-1Ah]@43
	char v28; // [sp+1Ah] [bp-16h]@37
	char v29; // [sp+1Ch] [bp-14h]@31
	char v30; // [sp+1Eh] [bp-12h]@25
	char v31; // [sp+22h] [bp-Eh]@17
	char v32; // [sp+25h] [bp-Bh]@11
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[16] >> 6;
					data[16] *= 4;
					data[16] |= v16;
					data[28] ^= 0xBAu;
					data[26] ^= 0x92u;
					v24 = (data[26] >> 3) & 1;
					if ((data[26] >> 1) & 1)
						data[26] |= 8u;
					else
						data[26] &= 0xF7u;
					if (v24)
						data[26] |= 2u;
					else
						data[26] &= 0xFDu;
					v17 = data[13];
					data[13] = data[25];
					data[25] = v17;
					v23 = (data[10] >> 4) & 1;
					if ((data[10] >> 5) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v23)
						data[10] |= 0x20u;
					else
						data[10] &= 0xDFu;
					v18 = data[20] >> 4;
					data[20] *= 16;
					data[20] |= v18;
					v22 = (data[22] >> 6) & 1;
					if ((data[22] >> 3) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v22)
						data[22] |= 8u;
					else
						data[22] &= 0xF7u;
					data[9] ^= 0x12u;
					v19 = data[22] >> 3;
					data[22] *= 32;
					data[22] |= v19;
					v21 = (data[27] >> 7) & 1;
					if ((data[27] >> 2) & 1)
						data[27] |= 0x80u;
					else
						data[27] &= 0x7Fu;
					if (v21)
						data[27] |= 4u;
					else
						data[27] &= 0xFBu;
					v20 = data[20];
					data[20] = data[2];
					data[2] = v20;
					data[21] ^= 0xAFu;
				}
				else
				{
					v12 = data[2];
					data[2] = data[4];
					data[4] = v12;
					v30 = (data[3] >> 6) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					if (v30)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					v29 = (data[6] >> 4) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v29)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					v28 = (data[7] >> 3) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v28)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					v13 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v13;
					v14 = data[3] >> 5;
					data[3] *= 8;
					data[3] |= v14;
					v27 = (data[1] >> 2) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v27)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					data[13] ^= 0x84u;
					v26 = (data[15] >> 6) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 0x40u;
					else
						data[15] &= 0xBFu;
					if (v26)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					data[14] ^= 1u;
					data[3] ^= 0x57u;
					v15 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v15;
					v25 = (data[10] >> 1) & 1;
					if ((data[10] >> 3) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v25)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
				}
			}
			else
			{
				v9 = data[6];
				data[6] = data[3];
				data[3] = v9;
				*data ^= 0xD0u;
				v32 = (data[4] >> 7) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v32)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				v10 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v10;
				v31 = (data[4] >> 2) & 1;
				if ((data[4] >> 6) & 1)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				if (v31)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				v11 = data[7];
				data[7] = data[5];
				data[5] = v11;
			}
		}
		else
		{
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v4 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v4;
			v5 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v5;
			*data ^= 0x68u;
			v6 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v6;
			v7 = *data >> 2;
			*data <<= 6;
			*data |= v7;
			data[1] ^= 0xCEu;
			v8 = data[1];
			data[1] = *data;
			*data = v8;
		}
	}
}

void PacketTwister_19::Twist(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST18_1@11
	char v5; // ST17_1@11
	char v6; // ST16_1@11
	char v7; // ST15_1@11
	char v8; // ST12_1@19
	char v9; // ST11_1@19
	char v10; // ST10_1@19
	char v11; // ST0D_1@25
	char v12; // ST08_1@38
	char v13; // ST05_1@44
	char v14; // [sp+6h] [bp-16h]@38
	char v15; // [sp+9h] [bp-13h]@32
	char v16; // [sp+Bh] [bp-11h]@26
	char v17; // [sp+Eh] [bp-Eh]@19
	char v18; // [sp+13h] [bp-9h]@11
	char v2; // [sp+19h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = (data[11] >> 1) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v16)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					data[13] ^= 0x35u;
					v15 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v15)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v12 = data[31] >> 1;
					data[31] <<= 7;
					data[31] |= v12;
					v14 = (data[2] >> 4) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x10u;
					else
						data[2] &= 0xEFu;
					if (v14)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					v13 = data[20];
					data[20] = data[19];
					data[19] = v13;
				}
				else
				{
					v8 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v8;
					v9 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v9;
					data[4] ^= 0xA5u;
					v10 = data[5] >> 3;
					data[5] *= 32;
					data[5] |= v10;
					v17 = (data[9] >> 1) & 1;
					if ((data[9] >> 2) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v17)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					v11 = data[9];
					data[9] = data[9];
					data[9] = v11;
				}
			}
			else
			{
				v4 = data[3];
				data[3] = data[1];
				data[1] = v4;
				data[7] ^= 0x4Du;
				v5 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v5;
				v6 = data[4];
				data[4] = data[1];
				data[1] = v6;
				data[3] ^= 0xCAu;
				v7 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v7;
				data[6] ^= 0x98u;
				v18 = (data[7] >> 6) & 1;
				if ((data[7] >> 6) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v18)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
			}
		}
		else
		{
			v3 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v3;
			data[3] ^= 0x48u;
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
		}
	}
}

void PacketTwister_19::Correct(BYTE *data, int length)
{
	char v3; // ST19_1@9
	char v4; // ST16_1@17
	char v5; // ST15_1@17
	char v6; // ST14_1@17
	char v7; // ST13_1@17
	char v8; // ST12_1@19
	char v9; // ST0F_1@25
	char v10; // ST0E_1@25
	char v11; // ST0D_1@25
	char v12; // ST0C_1@26
	char v13; // ST09_1@32
	char v14; // [sp+5h] [bp-17h]@38
	char v15; // [sp+7h] [bp-15h]@32
	char v16; // [sp+Ah] [bp-12h]@26
	char v17; // [sp+10h] [bp-Ch]@19
	char v18; // [sp+17h] [bp-5h]@11
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[20];
					data[20] = data[19];
					data[19] = v12;
					v16 = (data[2] >> 4) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x10u;
					else
						data[2] &= 0xEFu;
					if (v16)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					v13 = data[31] >> 7;
					data[31] *= 2;
					data[31] |= v13;
					v15 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v15)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					data[13] ^= 0x35u;
					v14 = (data[11] >> 1) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v14)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
				}
				else
				{
					v8 = data[9];
					data[9] = data[9];
					data[9] = v8;
					v17 = (data[9] >> 1) & 1;
					if ((data[9] >> 2) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v17)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					v9 = data[5] >> 5;
					data[5] *= 8;
					data[5] |= v9;
					data[4] ^= 0xA5u;
					v10 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v10;
					v11 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v11;
				}
			}
			else
			{
				v18 = (data[7] >> 6) & 1;
				if ((data[7] >> 6) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v18)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				data[6] ^= 0x98u;
				v4 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v4;
				data[3] ^= 0xCAu;
				v5 = data[4];
				data[4] = data[1];
				data[1] = v5;
				v6 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v6;
				data[7] ^= 0x4Du;
				v7 = data[3];
				data[3] = data[1];
				data[1] = v7;
			}
		}
		else
		{
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			data[3] ^= 0x48u;
			v3 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v3;
		}
	}
}

void PacketTwister_20::Twist(BYTE *data, int length)
{
	char v3; // ST29_1@9
	char v4; // ST28_1@9
	char v5; // ST27_1@9
	char v6; // ST26_1@9
	char v7; // ST25_1@9
	char v8; // ST24_1@11
	char v9; // ST23_1@11
	char v10; // ST22_1@11
	char v11; // ST21_1@11
	char v12; // ST20_1@11
	char v13; // ST1B_1@25
	char v14; // ST1A_1@25
	char v15; // ST19_1@25
	char v16; // ST18_1@25
	char v17; // ST17_1@25
	char v18; // ST16_1@25
	char v19; // ST13_1@31
	char v20; // ST12_1@31
	char v21; // ST11_1@31
	char v22; // ST0A_1@50
	char v23; // ST09_1@50
	char v24; // ST08_1@50
	char v25; // ST07_1@50
	char v26; // [sp+Bh] [bp-21h]@44
	char v27; // [sp+Dh] [bp-1Fh]@38
	char v28; // [sp+Fh] [bp-1Dh]@32
	char v29; // [sp+14h] [bp-18h]@25
	char v30; // [sp+1Ch] [bp-10h]@17
	char v31; // [sp+1Eh] [bp-Eh]@11
	char v2; // [sp+2Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v28 = (data[10] >> 4) & 1;
					if ((data[10] >> 3) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v28)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
					v27 = (data[30] >> 4) & 1;
					if ((data[30] >> 4) & 1)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					if (v27)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					v26 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v26)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v22 = data[15];
					data[15] = data[4];
					data[4] = v22;
					data[1] ^= 0x43u;
					v23 = data[21] >> 5;
					data[21] *= 8;
					data[21] |= v23;
					v24 = data[14];
					data[14] = *data;
					*data = v24;
					data[20] ^= 0xF1u;
					v25 = data[26];
					data[26] = data[1];
					data[1] = v25;
				}
				else
				{
					v13 = data[6];
					data[6] = data[10];
					data[10] = v13;
					v14 = data[10] >> 5;
					data[10] *= 8;
					data[10] |= v14;
					v15 = data[4];
					data[4] = data[3];
					data[3] = v15;
					data[11] ^= 0x7Du;
					v16 = data[5];
					data[5] = data[7];
					data[7] = v16;
					data[13] ^= 0x5Au;
					v17 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v17;
					v18 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v18;
					v29 = (*data >> 7) & 1;
					if ((*data >> 5) & 1)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					if (v29)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					data[2] ^= 0x15u;
					v19 = data[1] >> 7;
					data[1] *= 2;
					data[1] |= v19;
					v20 = data[4] >> 6;
					data[4] *= 4;
					data[4] |= v20;
					v21 = data[3] >> 6;
					data[3] *= 4;
					data[3] |= v21;
				}
			}
			else
			{
				data[3] ^= 0x60u;
				v8 = data[4];
				data[4] = data[1];
				data[1] = v8;
				v9 = data[2];
				data[2] = *data;
				*data = v9;
				v10 = *data;
				*data = data[3];
				data[3] = v10;
				v11 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v11;
				v12 = data[4] >> 1;
				data[4] <<= 7;
				data[4] |= v12;
				v31 = (*data >> 6) & 1;
				if ((*data >> 7) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v31)
					*data |= 0x80u;
				else
					*data &= 0x7Fu;
				data[4] ^= 0xE5u;
				v30 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v30)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
			}
		}
		else
		{
			*data ^= 0xCCu;
			v2 = (data[1] >> 7) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			v5 = data[1];
			data[1] = data[1];
			data[1] = v5;
			v6 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
		}
	}
}

void PacketTwister_20::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST29_1@3
	char v6; // ST28_1@3
	char v7; // ST27_1@3
	char v8; // ST20_1@23
	char v9; // ST1F_1@23
	char v10; // ST1E_1@23
	char v11; // ST1D_1@23
	char v12; // ST1C_1@23
	char v13; // ST1B_1@25
	char v14; // ST1A_1@25
	char v15; // ST19_1@25
	char v16; // ST16_1@31
	char v17; // ST15_1@31
	char v18; // ST14_1@31
	char v19; // ST13_1@31
	char v20; // ST12_1@31
	char v21; // ST11_1@31
	char v22; // ST10_1@32
	char v23; // ST0F_1@32
	char v24; // ST0E_1@32
	char v25; // ST0D_1@32
	char v26; // [sp+7h] [bp-25h]@44
	char v27; // [sp+9h] [bp-23h]@38
	char v28; // [sp+Bh] [bp-21h]@32
	char v29; // [sp+17h] [bp-15h]@25
	char v30; // [sp+21h] [bp-Bh]@17
	char v31; // [sp+23h] [bp-9h]@11
	char v2; // [sp+25h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v22 = data[26];
					data[26] = data[1];
					data[1] = v22;
					data[20] ^= 0xF1u;
					v23 = data[14];
					data[14] = *data;
					*data = v23;
					v24 = data[21] >> 3;
					data[21] *= 32;
					data[21] |= v24;
					data[1] ^= 0x43u;
					v25 = data[15];
					data[15] = data[4];
					data[4] = v25;
					v28 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v28)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v27 = (data[30] >> 4) & 1;
					if ((data[30] >> 4) & 1)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					if (v27)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					v26 = (data[10] >> 4) & 1;
					if ((data[10] >> 3) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v26)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
				}
				else
				{
					v13 = data[3] >> 2;
					data[3] <<= 6;
					data[3] |= v13;
					v14 = data[4] >> 2;
					data[4] <<= 6;
					data[4] |= v14;
					v15 = data[1] >> 1;
					data[1] <<= 7;
					data[1] |= v15;
					data[2] ^= 0x15u;
					v29 = (*data >> 7) & 1;
					if ((*data >> 5) & 1)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					if (v29)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					v16 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v16;
					v17 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v17;
					data[13] ^= 0x5Au;
					v18 = data[5];
					data[5] = data[7];
					data[7] = v18;
					data[11] ^= 0x7Du;
					v19 = data[4];
					data[4] = data[3];
					data[3] = v19;
					v20 = data[10] >> 3;
					data[10] *= 32;
					data[10] |= v20;
					v21 = data[6];
					data[6] = data[10];
					data[10] = v21;
				}
			}
			else
			{
				v31 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v31)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				data[4] ^= 0xE5u;
				v30 = (*data >> 6) & 1;
				if ((*data >> 7) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v30)
					*data |= 0x80u;
				else
					*data &= 0x7Fu;
				v8 = data[4] >> 7;
				data[4] *= 2;
				data[4] |= v8;
				v9 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v9;
				v10 = *data;
				*data = data[3];
				data[3] = v10;
				v11 = data[2];
				data[2] = *data;
				*data = v11;
				v12 = data[4];
				data[4] = data[1];
				data[1] = v12;
				data[3] ^= 0x60u;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v4 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v4;
			v5 = data[1];
			data[1] = data[1];
			data[1] = v5;
			v6 = *data >> 2;
			*data <<= 6;
			*data |= v6;
			v7 = data[1];
			data[1] = data[2];
			data[2] = v7;
			v2 = (data[1] >> 7) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			*data ^= 0xCCu;
		}
	}
}

void PacketTwister_21::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST20_1@3
	char v7; // ST1D_1@9
	char v8; // ST1C_1@9
	char v9; // ST1B_1@9
	char v10; // ST18_1@15
	char v11; // ST15_1@23
	char v12; // ST14_1@25
	char v13; // ST0D_1@44
	char v14; // ST0C_1@44
	char v15; // ST0B_1@44
	char v16; // ST0A_1@44
	char v17; // ST09_1@44
	char v18; // [sp+5h] [bp-1Fh]@50
	char v19; // [sp+7h] [bp-1Dh]@44
	char v20; // [sp+Eh] [bp-16h]@38
	char v21; // [sp+10h] [bp-14h]@32
	char v22; // [sp+12h] [bp-12h]@26
	char v23; // [sp+16h] [bp-Eh]@17
	char v24; // [sp+19h] [bp-Bh]@9
	char v2; // [sp+1Eh] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[26] ^= 0xE4u;
					v22 = (data[9] >> 2) & 1;
					if ((data[9] >> 3) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v22)
						data[9] |= 8u;
					else
						data[9] &= 0xF7u;
					v21 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v21)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v20 = (data[9] >> 5) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					if (v20)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v13 = data[30] >> 1;
					data[30] <<= 7;
					data[30] |= v13;
					v14 = data[27] >> 3;
					data[27] *= 32;
					data[27] |= v14;
					v15 = data[30];
					data[30] = data[29];
					data[29] = v15;
					v16 = data[28];
					data[28] = data[18];
					data[18] = v16;
					v17 = data[21] >> 3;
					data[21] *= 32;
					data[21] |= v17;
					v19 = (data[9] >> 6) & 1;
					if ((data[9] >> 7) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v19)
						data[9] |= 0x80u;
					else
						data[9] &= 0x7Fu;
					v18 = (data[11] >> 4) & 1;
					if ((data[11] >> 5) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v18)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
				}
				else
				{
					data[13] ^= 0xD5u;
					data[8] ^= 0x30u;
					v12 = data[12] >> 4;
					data[12] *= 16;
					data[12] |= v12;
				}
			}
			else
			{
				data[1] ^= 0xB9u;
				data[6] ^= 0xEEu;
				v23 = (*data >> 2) & 1;
				if ((*data >> 2) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v23)
					*data |= 4u;
				else
					*data &= 0xFBu;
				data[5] ^= 0xC4u;
				v11 = data[6];
				data[6] = data[5];
				data[5] = v11;
				data[7] ^= 0x60u;
			}
		}
		else
		{
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = *data;
			*data = v4;
			v5 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v5;
			v6 = data[1] >> 5;
			data[1] *= 8;
			data[1] |= v6;
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			v7 = *data;
			*data = data[2];
			data[2] = v7;
			v8 = *data;
			*data = data[2];
			data[2] = v8;
			data[2] ^= 3u;
			v9 = data[1];
			data[1] = *data;
			*data = v9;
			v24 = (data[1] >> 6) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v24)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v10 = *data >> 4;
			*data *= 16;
			*data |= v10;
		}
	}
}

void PacketTwister_21::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1E_1@9
	char v7; // ST1B_1@15
	char v8; // ST1A_1@15
	char v9; // ST19_1@15
	char v10; // ST18_1@15
	char v11; // ST17_1@17
	char v12; // ST14_1@25
	char v13; // ST0F_1@38
	char v14; // ST0E_1@38
	char v15; // ST0D_1@38
	char v16; // ST0C_1@38
	char v17; // ST0B_1@38
	char v18; // [sp+5h] [bp-1Fh]@50
	char v19; // [sp+7h] [bp-1Dh]@44
	char v20; // [sp+9h] [bp-1Bh]@38
	char v21; // [sp+10h] [bp-14h]@32
	char v22; // [sp+12h] [bp-12h]@26
	char v23; // [sp+15h] [bp-Fh]@17
	char v24; // [sp+1Ch] [bp-8h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v22 = (data[11] >> 4) & 1;
					if ((data[11] >> 5) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v22)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
					v21 = (data[9] >> 6) & 1;
					if ((data[9] >> 7) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v21)
						data[9] |= 0x80u;
					else
						data[9] &= 0x7Fu;
					v13 = data[21] >> 5;
					data[21] *= 8;
					data[21] |= v13;
					v14 = data[28];
					data[28] = data[18];
					data[18] = v14;
					v15 = data[30];
					data[30] = data[29];
					data[29] = v15;
					v16 = data[27] >> 5;
					data[27] *= 8;
					data[27] |= v16;
					v17 = data[30] >> 7;
					data[30] *= 2;
					data[30] |= v17;
					v20 = (data[9] >> 5) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					if (v20)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v19 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v19)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v18 = (data[9] >> 2) & 1;
					if ((data[9] >> 3) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v18)
						data[9] |= 8u;
					else
						data[9] &= 0xF7u;
					data[26] ^= 0xE4u;
				}
				else
				{
					v12 = data[12] >> 4;
					data[12] *= 16;
					data[12] |= v12;
					data[8] ^= 0x30u;
					data[13] ^= 0xD5u;
				}
			}
			else
			{
				data[7] ^= 0x60u;
				v11 = data[6];
				data[6] = data[5];
				data[5] = v11;
				data[5] ^= 0xC4u;
				v23 = (*data >> 2) & 1;
				if ((*data >> 2) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v23)
					*data |= 4u;
				else
					*data &= 0xFBu;
				data[6] ^= 0xEEu;
				data[1] ^= 0xB9u;
			}
		}
		else
		{
			v3 = *data >> 4;
			*data *= 16;
			*data |= v3;
			v2 = (data[1] >> 6) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v4 = data[1];
			data[1] = *data;
			*data = v4;
			data[2] ^= 3u;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			v24 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v24)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			v7 = data[1] >> 3;
			data[1] *= 32;
			data[1] |= v7;
			v8 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v8;
			v9 = data[2];
			data[2] = *data;
			*data = v9;
			v10 = *data;
			*data = data[1];
			data[1] = v10;
		}
	}
}

void PacketTwister_22::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2E_1@3
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST22_1@27
	char v9; // ST1F_1@35
	char v10; // ST1E_1@35
	char v11; // ST1D_1@37
	char v12; // ST1C_1@37
	char v13; // ST17_1@49
	char v14; // ST14_1@55
	char v15; // ST0F_1@68
	char v16; // ST0E_1@68
	char v17; // ST0D_1@68
	char v18; // ST0C_1@68
	char v19; // ST0B_1@68
	char v20; // ST0A_1@68
	char v21; // ST09_1@68
	char v22; // ST06_1@74
	char v23; // ST05_1@74
	char v24; // ST04_1@74
	char v25; // [sp+7h] [bp-29h]@68
	char v26; // [sp+10h] [bp-20h]@61
	char v27; // [sp+12h] [bp-1Eh]@55
	char v28; // [sp+15h] [bp-1Bh]@49
	char v29; // [sp+18h] [bp-18h]@43
	char v30; // [sp+1Ah] [bp-16h]@37
	char v31; // [sp+20h] [bp-10h]@29
	char v32; // [sp+23h] [bp-Dh]@21
	char v33; // [sp+25h] [bp-Bh]@15
	char v34; // [sp+27h] [bp-9h]@9
	char v2; // [sp+2Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[18] >> 5;
					data[18] *= 8;
					data[18] |= v15;
					v16 = *data >> 6;
					*data *= 4;
					*data |= v16;
					v17 = data[2];
					data[2] = data[25];
					data[25] = v17;
					v18 = data[19] >> 4;
					data[19] *= 16;
					data[19] |= v18;
					v19 = data[19] >> 6;
					data[19] *= 4;
					data[19] |= v19;
					v20 = data[31];
					data[31] = data[24];
					data[24] = v20;
					v21 = data[9] >> 6;
					data[9] *= 4;
					data[9] |= v21;
					v25 = (data[22] >> 1) & 1;
					if ((data[22] >> 6) & 1)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					if (v25)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					v22 = *data;
					*data = data[28];
					data[28] = v22;
					v23 = data[19];
					data[19] = data[25];
					data[25] = v23;
					data[23] ^= 0xA2u;
					v24 = data[8];
					data[8] = data[12];
					data[12] = v24;
				}
				else
				{
					*data ^= 0x31u;
					v11 = data[12] >> 1;
					data[12] <<= 7;
					data[12] |= v11;
					*data ^= 0xE8u;
					v12 = data[1];
					data[1] = data[15];
					data[15] = v12;
					v30 = (data[8] >> 7) & 1;
					if ((data[8] >> 2) & 1)
						data[8] |= 0x80u;
					else
						data[8] &= 0x7Fu;
					if (v30)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					data[4] ^= 0x93u;
					v29 = (data[10] >> 1) & 1;
					if ((data[10] >> 6) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v29)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					v13 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v13;
					v28 = (data[9] >> 6) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v28)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v14 = data[5] >> 5;
					data[5] *= 8;
					data[5] |= v14;
					v27 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v27)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v26 = (*data >> 1) & 1;
					if ((*data >> 3) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v26)
						*data |= 8u;
					else
						*data &= 0xF7u;
				}
			}
			else
			{
				v31 = (*data >> 6) & 1;
				if ((*data >> 2) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v31)
					*data |= 4u;
				else
					*data &= 0xFBu;
				data[3] ^= 0xFAu;
				v9 = data[4] >> 7;
				data[4] *= 2;
				data[4] |= v9;
				v10 = data[2];
				data[2] = data[3];
				data[3] = v10;
			}
		}
		else
		{
			v3 = *data;
			*data = *data;
			*data = v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v2 = (data[1] >> 6) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v2)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v5 = data[2];
			data[2] = data[1];
			data[1] = v5;
			v6 = data[3];
			data[3] = data[3];
			data[3] = v6;
			v7 = data[3];
			data[3] = data[3];
			data[3] = v7;
			data[3] ^= 0xFu;
			v34 = (data[2] >> 2) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v34)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			*data ^= 0xB1u;
			v33 = (data[2] >> 2) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v33)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			data[2] ^= 0x5Cu;
			v32 = (*data >> 1) & 1;
			if ((*data >> 6) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v32)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			v8 = data[3];
			data[3] = data[2];
			data[2] = v8;
		}
	}
}

void PacketTwister_22::Correct(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST28_1@21
	char v5; // ST27_1@21
	char v6; // ST26_1@21
	char v7; // ST23_1@27
	char v8; // ST22_1@27
	char v9; // ST21_1@29
	char v10; // ST20_1@29
	char v11; // ST19_1@49
	char v12; // ST16_1@55
	char v13; // ST11_1@67
	char v14; // ST10_1@67
	char v15; // ST0F_1@68
	char v16; // ST0E_1@68
	char v17; // ST0D_1@68
	char v18; // ST0A_1@74
	char v19; // ST09_1@74
	char v20; // ST08_1@74
	char v21; // ST07_1@74
	char v22; // ST06_1@74
	char v23; // ST05_1@74
	char v24; // ST04_1@74
	char v25; // [sp+Bh] [bp-25h]@68
	char v26; // [sp+12h] [bp-1Eh]@61
	char v27; // [sp+14h] [bp-1Ch]@55
	char v28; // [sp+17h] [bp-19h]@49
	char v29; // [sp+1Ah] [bp-16h]@43
	char v30; // [sp+1Ch] [bp-14h]@37
	char v31; // [sp+1Eh] [bp-12h]@29
	char v32; // [sp+24h] [bp-Ch]@21
	char v33; // [sp+29h] [bp-7h]@15
	char v34; // [sp+2Bh] [bp-5h]@9
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[8];
					data[8] = data[12];
					data[12] = v15;
					data[23] ^= 0xA2u;
					v16 = data[19];
					data[19] = data[25];
					data[25] = v16;
					v17 = *data;
					*data = data[28];
					data[28] = v17;
					v25 = (data[22] >> 1) & 1;
					if ((data[22] >> 6) & 1)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					if (v25)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					v18 = data[9] >> 2;
					data[9] <<= 6;
					data[9] |= v18;
					v19 = data[31];
					data[31] = data[24];
					data[24] = v19;
					v20 = data[19] >> 2;
					data[19] <<= 6;
					data[19] |= v20;
					v21 = data[19] >> 4;
					data[19] *= 16;
					data[19] |= v21;
					v22 = data[2];
					data[2] = data[25];
					data[25] = v22;
					v23 = *data >> 2;
					*data <<= 6;
					*data |= v23;
					v24 = data[18] >> 3;
					data[18] *= 32;
					data[18] |= v24;
				}
				else
				{
					v30 = (*data >> 1) & 1;
					if ((*data >> 3) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v30)
						*data |= 8u;
					else
						*data &= 0xF7u;
					v29 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v29)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v11 = data[5] >> 3;
					data[5] *= 32;
					data[5] |= v11;
					v28 = (data[9] >> 6) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v28)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v12 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v12;
					v27 = (data[10] >> 1) & 1;
					if ((data[10] >> 6) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v27)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					data[4] ^= 0x93u;
					v26 = (data[8] >> 7) & 1;
					if ((data[8] >> 2) & 1)
						data[8] |= 0x80u;
					else
						data[8] &= 0x7Fu;
					if (v26)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					v13 = data[1];
					data[1] = data[15];
					data[15] = v13;
					*data ^= 0xE8u;
					v14 = data[12] >> 7;
					data[12] *= 2;
					data[12] |= v14;
					*data ^= 0x31u;
				}
			}
			else
			{
				v9 = data[2];
				data[2] = data[3];
				data[3] = v9;
				v10 = data[4] >> 1;
				data[4] <<= 7;
				data[4] |= v10;
				data[3] ^= 0xFAu;
				v31 = (*data >> 6) & 1;
				if ((*data >> 2) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v31)
					*data |= 4u;
				else
					*data &= 0xFBu;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			v2 = (*data >> 1) & 1;
			if ((*data >> 6) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			data[2] ^= 0x5Cu;
			v34 = (data[2] >> 2) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v34)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			*data ^= 0xB1u;
			v33 = (data[2] >> 2) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v33)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			data[3] ^= 0xFu;
			v4 = data[3];
			data[3] = data[3];
			data[3] = v4;
			v5 = data[3];
			data[3] = data[3];
			data[3] = v5;
			v6 = data[2];
			data[2] = data[1];
			data[1] = v6;
			v32 = (data[1] >> 6) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v32)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v7 = data[1];
			data[1] = data[3];
			data[3] = v7;
			v8 = *data;
			*data = *data;
			*data = v8;
		}
	}
}

void PacketTwister_23::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST28_1@9
	char v5; // ST25_1@15
	char v6; // ST24_1@17
	char v7; // ST21_1@23
	char v8; // ST18_1@49
	char v9; // ST13_1@61
	char v10; // ST0E_1@73
	char v11; // ST0D_1@73
	char v12; // ST0C_1@73
	char v13; // ST0B_1@73
	char v14; // ST0A_1@74
	char v15; // ST09_1@74
	char v16; // ST08_1@74
	char v17; // [sp+6h] [bp-26h]@74
	char v18; // [sp+Fh] [bp-1Dh]@67
	char v19; // [sp+11h] [bp-1Bh]@61
	char v20; // [sp+14h] [bp-18h]@55
	char v21; // [sp+16h] [bp-16h]@49
	char v22; // [sp+19h] [bp-13h]@43
	char v23; // [sp+1Bh] [bp-11h]@35
	char v24; // [sp+1Dh] [bp-Fh]@29
	char v25; // [sp+1Fh] [bp-Dh]@23
	char v26; // [sp+22h] [bp-Ah]@17
	char v27; // [sp+26h] [bp-6h]@9
	char v2; // [sp+29h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[4] >> 5;
					data[4] *= 8;
					data[4] |= v14;
					v15 = data[31];
					data[31] = data[9];
					data[9] = v15;
					v16 = data[11];
					data[11] = data[3];
					data[3] = v16;
					v17 = (data[28] >> 2) & 1;
					if ((data[28] >> 4) & 1)
						data[28] |= 4u;
					else
						data[28] &= 0xFBu;
					if (v17)
						data[28] |= 0x10u;
					else
						data[28] &= 0xEFu;
				}
				else
				{
					v22 = (data[2] >> 4) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x10u;
					else
						data[2] &= 0xEFu;
					if (v22)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					v8 = data[13] >> 1;
					data[13] <<= 7;
					data[13] |= v8;
					v21 = (data[14] >> 6) & 1;
					if ((data[14] >> 3) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v21)
						data[14] |= 8u;
					else
						data[14] &= 0xF7u;
					v20 = (data[6] >> 2) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v20)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v9 = data[1] >> 4;
					data[1] *= 16;
					data[1] |= v9;
					v19 = (data[15] >> 1) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					if (v19)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					v18 = (data[4] >> 2) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					if (v18)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
					v10 = data[3];
					data[3] = data[10];
					data[10] = v10;
					v11 = data[4];
					data[4] = data[7];
					data[7] = v11;
					v12 = data[15];
					data[15] = *data;
					*data = v12;
					v13 = data[3];
					data[3] = data[5];
					data[5] = v13;
					data[11] ^= 0xD2u;
				}
			}
			else
			{
				v6 = data[4] >> 7;
				data[4] *= 2;
				data[4] |= v6;
				v26 = (data[3] >> 7) & 1;
				if ((data[3] >> 2) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v26)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				v7 = *data >> 7;
				*data *= 2;
				*data |= v7;
				data[2] ^= 9u;
				v25 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v25)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v24 = (data[7] >> 6) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v24)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				data[7] ^= 0x86u;
				data[4] ^= 0xB6u;
				data[7] ^= 0x32u;
				v23 = (data[6] >> 6) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				if (v23)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				*data ^= 0xD1u;
				data[2] ^= 0x33u;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			v4 = *data >> 5;
			*data *= 8;
			*data |= v4;
			v27 = (data[3] >> 4) & 1;
			if ((data[3] >> 5) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v27)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
		}
	}
}

void PacketTwister_23::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST28_1@9
	char v5; // ST25_1@15
	char v6; // ST1E_1@35
	char v7; // ST1B_1@41
	char v8; // ST1A_1@43
	char v9; // ST19_1@43
	char v10; // ST18_1@43
	char v11; // ST17_1@43
	char v12; // ST12_1@55
	char v13; // ST0D_1@67
	char v14; // ST08_1@80
	char v15; // ST07_1@80
	char v16; // ST06_1@80
	char v17; // [sp+9h] [bp-23h]@74
	char v18; // [sp+Bh] [bp-21h]@67
	char v19; // [sp+Eh] [bp-1Eh]@61
	char v20; // [sp+10h] [bp-1Ch]@55
	char v21; // [sp+13h] [bp-19h]@49
	char v22; // [sp+15h] [bp-17h]@43
	char v23; // [sp+1Ch] [bp-10h]@35
	char v24; // [sp+1Fh] [bp-Dh]@29
	char v25; // [sp+21h] [bp-Bh]@23
	char v26; // [sp+23h] [bp-9h]@17
	char v27; // [sp+26h] [bp-6h]@9
	char v2; // [sp+29h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[28] >> 2) & 1;
					if ((data[28] >> 4) & 1)
						data[28] |= 4u;
					else
						data[28] &= 0xFBu;
					if (v17)
						data[28] |= 0x10u;
					else
						data[28] &= 0xEFu;
					v14 = data[11];
					data[11] = data[3];
					data[3] = v14;
					v15 = data[31];
					data[31] = data[9];
					data[9] = v15;
					v16 = data[4] >> 3;
					data[4] *= 32;
					data[4] |= v16;
				}
				else
				{
					data[11] ^= 0xD2u;
					v8 = data[3];
					data[3] = data[5];
					data[5] = v8;
					v9 = data[15];
					data[15] = *data;
					*data = v9;
					v10 = data[4];
					data[4] = data[7];
					data[7] = v10;
					v11 = data[3];
					data[3] = data[10];
					data[10] = v11;
					v22 = (data[4] >> 2) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					if (v22)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
					v21 = (data[15] >> 1) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					if (v21)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					v12 = data[1] >> 4;
					data[1] *= 16;
					data[1] |= v12;
					v20 = (data[6] >> 2) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v20)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v19 = (data[14] >> 6) & 1;
					if ((data[14] >> 3) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v19)
						data[14] |= 8u;
					else
						data[14] &= 0xF7u;
					v13 = data[13] >> 7;
					data[13] *= 2;
					data[13] |= v13;
					v18 = (data[2] >> 4) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x10u;
					else
						data[2] &= 0xEFu;
					if (v18)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
				}
			}
			else
			{
				data[2] ^= 0x33u;
				*data ^= 0xD1u;
				v26 = (data[6] >> 6) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				if (v26)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				data[7] ^= 0x32u;
				data[4] ^= 0xB6u;
				data[7] ^= 0x86u;
				v25 = (data[7] >> 6) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v25)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v24 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v24)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				data[2] ^= 9u;
				v6 = *data >> 1;
				*data <<= 7;
				*data |= v6;
				v23 = (data[3] >> 7) & 1;
				if ((data[3] >> 2) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v23)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				v7 = data[4] >> 1;
				data[4] <<= 7;
				data[4] |= v7;
			}
		}
		else
		{
			v3 = *data;
			*data = data[2];
			data[2] = v3;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 5) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			v4 = *data >> 3;
			*data *= 32;
			*data |= v4;
			v27 = (data[1] >> 2) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v27)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
		}
	}
}

void PacketTwister_24::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST22_1@11
	char v7; // ST21_1@11
	char v8; // ST20_1@11
	char v9; // ST1F_1@11
	char v10; // ST1E_1@13
	char v11; // ST1D_1@13
	char v12; // ST1C_1@13
	char v13; // ST17_1@25
	char v14; // ST14_1@32
	char v15; // ST11_1@38
	char v16; // ST10_1@38
	char v17; // ST0D_1@44
	char v18; // ST0C_1@44
	char v19; // ST0B_1@44
	char v20; // ST08_1@50
	char v21; // ST05_1@56
	char v22; // [sp+6h] [bp-22h]@50
	char v23; // [sp+9h] [bp-1Fh]@44
	char v24; // [sp+Eh] [bp-1Ah]@38
	char v25; // [sp+12h] [bp-16h]@32
	char v26; // [sp+15h] [bp-13h]@25
	char v27; // [sp+18h] [bp-10h]@19
	char v28; // [sp+1Ah] [bp-Eh]@13
	char v2; // [sp+23h] [bp-5h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[31] >> 6;
					data[31] *= 4;
					data[31] |= v14;
					v25 = (*data >> 2) & 1;
					if ((*data >> 3) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v25)
						*data |= 8u;
					else
						*data &= 0xF7u;
					v15 = data[28] >> 1;
					data[28] <<= 7;
					data[28] |= v15;
					v16 = data[28];
					data[28] = data[13];
					data[13] = v16;
					v24 = (data[19] >> 5) & 1;
					if ((data[19] >> 4) & 1)
						data[19] |= 0x20u;
					else
						data[19] &= 0xDFu;
					if (v24)
						data[19] |= 0x10u;
					else
						data[19] &= 0xEFu;
					v17 = data[8];
					data[8] = data[31];
					data[31] = v17;
					v18 = data[9] >> 6;
					data[9] *= 4;
					data[9] |= v18;
					v19 = data[10];
					data[10] = data[19];
					data[19] = v19;
					v23 = (data[13] >> 4) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 0x10u;
					else
						data[13] &= 0xEFu;
					if (v23)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					v20 = data[3] >> 3;
					data[3] *= 32;
					data[3] |= v20;
					v22 = (data[9] >> 3) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 8u;
					else
						data[9] &= 0xF7u;
					if (v22)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v21 = data[22];
					data[22] = data[9];
					data[9] = v21;
				}
				else
				{
					v10 = data[15];
					data[15] = data[9];
					data[9] = v10;
					v11 = data[5] >> 7;
					data[5] *= 2;
					data[5] |= v11;
					data[8] ^= 0xAEu;
					v12 = data[12];
					data[12] = data[6];
					data[6] = v12;
					v28 = (data[8] >> 5) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 0x20u;
					else
						data[8] &= 0xDFu;
					if (v28)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					v27 = (data[3] >> 2) & 1;
					if ((data[3] >> 4) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v27)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					data[15] ^= 0x57u;
					v13 = data[11];
					data[11] = *data;
					*data = v13;
					v26 = (data[3] >> 2) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v26)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					data[13] ^= 0xB2u;
				}
			}
			else
			{
				v2 = (*data >> 2) & 1;
				if ((*data >> 1) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v2)
					*data |= 2u;
				else
					*data &= 0xFDu;
				v6 = data[4];
				data[4] = data[6];
				data[6] = v6;
				v7 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v7;
				v8 = data[7];
				data[7] = data[6];
				data[6] = v8;
				v9 = data[4];
				data[4] = *data;
				*data = v9;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[2];
			data[2] = v3;
			v4 = data[3];
			data[3] = data[3];
			data[3] = v4;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			data[1] ^= 0x5Au;
		}
	}
}

void PacketTwister_24::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@5
	char v7; // ST23_1@5
	char v8; // ST22_1@5
	char v9; // ST21_1@5
	char v10; // ST1C_1@19
	char v11; // ST17_1@31
	char v12; // ST16_1@31
	char v13; // ST15_1@31
	char v14; // ST14_1@32
	char v15; // ST11_1@38
	char v16; // ST0E_1@44
	char v17; // ST0D_1@44
	char v18; // ST0C_1@44
	char v19; // ST09_1@50
	char v20; // ST08_1@50
	char v21; // ST05_1@56
	char v22; // [sp+6h] [bp-22h]@50
	char v23; // [sp+Ah] [bp-1Eh]@44
	char v24; // [sp+Fh] [bp-19h]@38
	char v25; // [sp+12h] [bp-16h]@32
	char v26; // [sp+18h] [bp-10h]@25
	char v27; // [sp+1Ah] [bp-Eh]@19
	char v28; // [sp+1Dh] [bp-Bh]@13
	char v2; // [sp+1Fh] [bp-9h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[22];
					data[22] = data[9];
					data[9] = v14;
					v25 = (data[9] >> 3) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 8u;
					else
						data[9] &= 0xF7u;
					if (v25)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v15 = data[3] >> 5;
					data[3] *= 8;
					data[3] |= v15;
					v24 = (data[13] >> 4) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 0x10u;
					else
						data[13] &= 0xEFu;
					if (v24)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					v16 = data[10];
					data[10] = data[19];
					data[19] = v16;
					v17 = data[9] >> 2;
					data[9] <<= 6;
					data[9] |= v17;
					v18 = data[8];
					data[8] = data[31];
					data[31] = v18;
					v23 = (data[19] >> 5) & 1;
					if ((data[19] >> 4) & 1)
						data[19] |= 0x20u;
					else
						data[19] &= 0xDFu;
					if (v23)
						data[19] |= 0x10u;
					else
						data[19] &= 0xEFu;
					v19 = data[28];
					data[28] = data[13];
					data[13] = v19;
					v20 = data[28] >> 7;
					data[28] *= 2;
					data[28] |= v20;
					v22 = (*data >> 2) & 1;
					if ((*data >> 3) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v22)
						*data |= 8u;
					else
						*data &= 0xF7u;
					v21 = data[31] >> 2;
					data[31] <<= 6;
					data[31] |= v21;
				}
				else
				{
					data[13] ^= 0xB2u;
					v28 = (data[3] >> 2) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v28)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					v10 = data[11];
					data[11] = *data;
					*data = v10;
					data[15] ^= 0x57u;
					v27 = (data[3] >> 2) & 1;
					if ((data[3] >> 4) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v27)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					v26 = (data[8] >> 5) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 0x20u;
					else
						data[8] &= 0xDFu;
					if (v26)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					v11 = data[12];
					data[12] = data[6];
					data[6] = v11;
					data[8] ^= 0xAEu;
					v12 = data[5] >> 1;
					data[5] <<= 7;
					data[5] |= v12;
					v13 = data[15];
					data[15] = data[9];
					data[9] = v13;
				}
			}
			else
			{
				v6 = data[4];
				data[4] = *data;
				*data = v6;
				v7 = data[7];
				data[7] = data[6];
				data[6] = v7;
				v8 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v8;
				v9 = data[4];
				data[4] = data[6];
				data[6] = v9;
				v2 = (*data >> 2) & 1;
				if ((*data >> 1) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v2)
					*data |= 2u;
				else
					*data &= 0xFDu;
			}
		}
		else
		{
			data[1] ^= 0x5Au;
			v3 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v3;
			v4 = data[3];
			data[3] = data[3];
			data[3] = v4;
			v5 = data[2];
			data[2] = data[2];
			data[2] = v5;
		}
	}
}

void PacketTwister_25::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST23_1@5
	char v8; // ST22_1@5
	char v9; // ST1F_1@11
	char v10; // ST1E_1@11
	char v11; // ST1B_1@17
	char v12; // ST1A_1@17
	char v13; // ST19_1@19
	char v14; // ST18_1@19
	char v15; // ST17_1@19
	char v16; // ST16_1@19
	char v17; // ST13_1@25
	char v18; // ST12_1@25
	char v19; // ST11_1@25
	char v20; // ST10_1@25
	char v21; // ST0F_1@26
	char v22; // ST0E_1@26
	char v23; // ST0B_1@32
	char v24; // ST0A_1@32
	char v25; // ST07_1@38
	char v26; // ST06_1@38
	char v27; // ST05_1@38
	char v28; // [sp+8h] [bp-20h]@32
	char v29; // [sp+Ch] [bp-1Ch]@26
	char v30; // [sp+14h] [bp-14h]@19
	char v31; // [sp+1Ch] [bp-Ch]@11
	char v2; // [sp+20h] [bp-8h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = data[4] >> 1;
					data[4] <<= 7;
					data[4] |= v21;
					v22 = data[20];
					data[20] = data[5];
					data[5] = v22;
					v29 = (data[1] >> 1) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v29)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v23 = data[28];
					data[28] = data[26];
					data[26] = v23;
					v24 = data[16] >> 6;
					data[16] *= 4;
					data[16] |= v24;
					v28 = (data[16] >> 5) & 1;
					if ((data[16] >> 7) & 1)
						data[16] |= 0x20u;
					else
						data[16] &= 0xDFu;
					if (v28)
						data[16] |= 0x80u;
					else
						data[16] &= 0x7Fu;
					data[18] ^= 0x6Cu;
					v25 = data[14];
					data[14] = data[12];
					data[12] = v25;
					v26 = data[20] >> 1;
					data[20] <<= 7;
					data[20] |= v26;
					v27 = data[16] >> 4;
					data[16] *= 16;
					data[16] |= v27;
				}
				else
				{
					v13 = data[12];
					data[12] = *data;
					*data = v13;
					v14 = data[2];
					data[2] = *data;
					*data = v14;
					v15 = *data >> 6;
					*data *= 4;
					*data |= v15;
					v16 = data[12];
					data[12] = data[6];
					data[6] = v16;
					data[4] ^= 0xCu;
					v30 = (*data >> 1) & 1;
					if ((*data >> 7) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v30)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					v17 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v17;
					data[9] ^= 0x28u;
					v18 = data[12];
					data[12] = data[14];
					data[14] = v18;
					v19 = data[1];
					data[1] = data[15];
					data[15] = v19;
					v20 = data[11];
					data[11] = data[14];
					data[14] = v20;
				}
			}
			else
			{
				v7 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v7;
				v8 = data[6];
				data[6] = data[3];
				data[3] = v8;
				v2 = (data[5] >> 1) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v2)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v9 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v9;
				data[1] ^= 0x5Du;
				data[2] ^= 0xB3u;
				v10 = data[3];
				data[3] = *data;
				*data = v10;
				v31 = (data[7] >> 2) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v31)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v11 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v11;
				v12 = data[4] >> 7;
				data[4] *= 2;
				data[4] |= v12;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			*data ^= 0xC4u;
			v5 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
		}
	}
}

void PacketTwister_25::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST23_1@5
	char v8; // ST22_1@5
	char v9; // ST1F_1@11
	char v10; // ST1E_1@11
	char v11; // ST1B_1@17
	char v12; // ST1A_1@17
	char v13; // ST19_1@19
	char v14; // ST18_1@19
	char v15; // ST17_1@19
	char v16; // ST16_1@19
	char v17; // ST13_1@25
	char v18; // ST12_1@25
	char v19; // ST11_1@25
	char v20; // ST10_1@25
	char v21; // ST0F_1@26
	char v22; // ST0E_1@26
	char v23; // ST0D_1@26
	char v24; // ST0A_1@32
	char v25; // ST09_1@32
	char v26; // ST06_1@38
	char v27; // ST05_1@38
	char v28; // [sp+7h] [bp-21h]@32
	char v29; // [sp+Bh] [bp-1Dh]@26
	char v30; // [sp+14h] [bp-14h]@19
	char v31; // [sp+1Ch] [bp-Ch]@11
	char v2; // [sp+20h] [bp-8h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = data[16] >> 4;
					data[16] *= 16;
					data[16] |= v21;
					v22 = data[20] >> 7;
					data[20] *= 2;
					data[20] |= v22;
					v23 = data[14];
					data[14] = data[12];
					data[12] = v23;
					data[18] ^= 0x6Cu;
					v29 = (data[16] >> 5) & 1;
					if ((data[16] >> 7) & 1)
						data[16] |= 0x20u;
					else
						data[16] &= 0xDFu;
					if (v29)
						data[16] |= 0x80u;
					else
						data[16] &= 0x7Fu;
					v24 = data[16] >> 2;
					data[16] <<= 6;
					data[16] |= v24;
					v25 = data[28];
					data[28] = data[26];
					data[26] = v25;
					v28 = (data[1] >> 1) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v28)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v26 = data[20];
					data[20] = data[5];
					data[5] = v26;
					v27 = data[4] >> 7;
					data[4] *= 2;
					data[4] |= v27;
				}
				else
				{
					v13 = data[11];
					data[11] = data[14];
					data[14] = v13;
					v14 = data[1];
					data[1] = data[15];
					data[15] = v14;
					v15 = data[12];
					data[12] = data[14];
					data[14] = v15;
					data[9] ^= 0x28u;
					v16 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v16;
					v30 = (*data >> 1) & 1;
					if ((*data >> 7) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v30)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					data[4] ^= 0xCu;
					v17 = data[12];
					data[12] = data[6];
					data[6] = v17;
					v18 = *data >> 2;
					*data <<= 6;
					*data |= v18;
					v19 = data[2];
					data[2] = *data;
					*data = v19;
					v20 = data[12];
					data[12] = *data;
					*data = v20;
				}
			}
			else
			{
				v7 = data[4] >> 1;
				data[4] <<= 7;
				data[4] |= v7;
				v8 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v8;
				v2 = (data[7] >> 2) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v2)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v9 = data[3];
				data[3] = *data;
				*data = v9;
				data[2] ^= 0xB3u;
				data[1] ^= 0x5Du;
				v10 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v10;
				v31 = (data[5] >> 1) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v31)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v11 = data[6];
				data[6] = data[3];
				data[3] = v11;
				v12 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v12;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v4 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v4;
			*data ^= 0xC4u;
			v5 = *data >> 6;
			*data *= 4;
			*data |= v5;
			v6 = data[3];
			data[3] = data[1];
			data[1] = v6;
		}
	}
}

void PacketTwister_26::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1C_1@9
	char v5; // ST19_1@15
	char v6; // ST18_1@15
	char v7; // ST17_1@15
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST14_1@17
	char v11; // ST13_1@17
	char v12; // ST0E_1@31
	char v13; // ST0B_1@37
	char v14; // ST0A_1@38
	char v15; // ST09_1@38
	char v16; // [sp+7h] [bp-19h]@38
	char v17; // [sp+Ch] [bp-14h]@31
	char v18; // [sp+Fh] [bp-11h]@25
	char v19; // [sp+11h] [bp-Fh]@19
	char v20; // [sp+1Ah] [bp-6h]@9
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[2];
					data[2] = data[13];
					data[13] = v14;
					data[30] ^= 0x59u;
					v15 = data[28];
					data[28] = data[20];
					data[20] = v15;
					v16 = (data[4] >> 7) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v16)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
				}
				else
				{
					v19 = (data[14] >> 7) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v19)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					data[9] ^= 0x1Fu;
					data[5] ^= 0xE8u;
					v18 = (data[2] >> 7) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v18)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					v12 = data[10] >> 7;
					data[10] *= 2;
					data[10] |= v12;
					data[8] ^= 0x8Du;
					v17 = (data[14] >> 4) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v17)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					v13 = data[2] >> 1;
					data[2] <<= 7;
					data[2] |= v13;
				}
			}
			else
			{
				v8 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v8;
				v9 = data[5];
				data[5] = data[6];
				data[6] = v9;
				v10 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v10;
				v11 = *data >> 6;
				*data *= 4;
				*data |= v11;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[3];
			data[3] = v3;
			v2 = (data[2] >> 5) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v4 = data[2];
			data[2] = data[1];
			data[1] = v4;
			data[2] ^= 0x53u;
			data[3] ^= 0xA5u;
			v20 = (*data >> 5) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v20)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[3] ^= 0x7Cu;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = *data;
			*data = data[1];
			data[1] = v7;
		}
	}
}

void PacketTwister_26::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1A_1@9
	char v7; // ST17_1@15
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST14_1@17
	char v11; // ST13_1@17
	char v12; // ST12_1@19
	char v13; // ST0F_1@25
	char v14; // ST08_1@44
	char v15; // ST07_1@44
	char v16; // [sp+9h] [bp-17h]@38
	char v17; // [sp+Bh] [bp-15h]@31
	char v18; // [sp+Dh] [bp-13h]@25
	char v19; // [sp+10h] [bp-10h]@19
	char v20; // [sp+18h] [bp-8h]@9
	char v2; // [sp+1Bh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = (data[4] >> 7) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v16)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					v14 = data[28];
					data[28] = data[20];
					data[20] = v14;
					data[30] ^= 0x59u;
					v15 = data[2];
					data[2] = data[13];
					data[13] = v15;
				}
				else
				{
					v12 = data[2] >> 7;
					data[2] *= 2;
					data[2] |= v12;
					v19 = (data[14] >> 4) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v19)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					data[8] ^= 0x8Du;
					v13 = data[10] >> 1;
					data[10] <<= 7;
					data[10] |= v13;
					v18 = (data[2] >> 7) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v18)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					data[5] ^= 0xE8u;
					data[9] ^= 0x1Fu;
					v17 = (data[14] >> 7) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v17)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
				}
			}
			else
			{
				v8 = *data >> 2;
				*data <<= 6;
				*data |= v8;
				v9 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v9;
				v10 = data[5];
				data[5] = data[6];
				data[6] = v10;
				v11 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v11;
			}
		}
		else
		{
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = *data;
			*data = v4;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			data[3] ^= 0x7Cu;
			v2 = (*data >> 5) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[3] ^= 0xA5u;
			data[2] ^= 0x53u;
			v6 = data[2];
			data[2] = data[1];
			data[1] = v6;
			v20 = (data[2] >> 5) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v20)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v7 = data[3];
			data[3] = data[3];
			data[3] = v7;
		}
	}
}

void PacketTwister_27::Twist(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@11
	char v5; // ST19_1@17
	char v6; // ST18_1@17
	char v7; // ST17_1@17
	char v8; // ST16_1@17
	char v9; // ST15_1@19
	char v10; // ST0E_1@37
	char v11; // ST0B_1@43
	char v12; // ST0A_1@43
	char v13; // ST09_1@44
	char v14; // ST06_1@50
	char v15; // ST05_1@50
	char v16; // [sp+7h] [bp-19h]@44
	char v17; // [sp+Ch] [bp-14h]@37
	char v18; // [sp+Fh] [bp-11h]@31
	char v19; // [sp+11h] [bp-Fh]@25
	char v20; // [sp+13h] [bp-Dh]@19
	char v21; // [sp+1Ah] [bp-6h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[9];
					data[9] = data[28];
					data[28] = v13;
					v16 = (data[15] >> 7) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					if (v16)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					data[22] ^= 0x10u;
					v14 = data[19] >> 2;
					data[19] <<= 6;
					data[19] |= v14;
					v15 = data[9];
					data[9] = data[17];
					data[17] = v15;
					data[9] ^= 0xC2u;
				}
				else
				{
					v9 = data[6];
					data[6] = data[9];
					data[9] = v9;
					*data ^= 0x78u;
					data[5] ^= 0x65u;
					data[2] ^= 0x64u;
					v20 = (*data >> 1) & 1;
					if ((*data >> 1) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v20)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v19 = (data[1] >> 5) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					if (v19)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v18 = (data[13] >> 4) & 1;
					if ((data[13] >> 1) & 1)
						data[13] |= 0x10u;
					else
						data[13] &= 0xEFu;
					if (v18)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					v10 = data[8];
					data[8] = data[10];
					data[10] = v10;
					v17 = (data[5] >> 2) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v17)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v11 = data[10];
					data[10] = data[6];
					data[6] = v11;
					v12 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v12;
				}
			}
			else
			{
				v4 = data[3];
				data[3] = data[3];
				data[3] = v4;
				data[4] ^= 0x32u;
				v21 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v21)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v5 = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= v5;
				v6 = data[6] >> 6;
				data[6] *= 4;
				data[6] |= v6;
				v7 = *data >> 6;
				*data *= 4;
				*data |= v7;
				v8 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v8;
			}
		}
		else
		{
			v2 = (*data >> 2) & 1;
			if ((*data >> 5) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			*data ^= 0x65u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			data[2] ^= 0x31u;
		}
	}
}

void PacketTwister_27::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1C_1@11
	char v5; // ST1B_1@11
	char v6; // ST1A_1@11
	char v7; // ST19_1@11
	char v8; // ST16_1@17
	char v9; // ST15_1@19
	char v10; // ST14_1@19
	char v11; // ST11_1@25
	char v12; // ST0A_1@43
	char v13; // ST09_1@44
	char v14; // ST08_1@44
	char v15; // ST05_1@50
	char v16; // [sp+6h] [bp-1Ah]@44
	char v17; // [sp+Bh] [bp-15h]@37
	char v18; // [sp+Dh] [bp-13h]@31
	char v19; // [sp+Fh] [bp-11h]@25
	char v20; // [sp+12h] [bp-Eh]@19
	char v21; // [sp+17h] [bp-9h]@11
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[9] ^= 0xC2u;
					v13 = data[9];
					data[9] = data[17];
					data[17] = v13;
					v14 = data[19] >> 6;
					data[19] *= 4;
					data[19] |= v14;
					data[22] ^= 0x10u;
					v16 = (data[15] >> 7) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					if (v16)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v15 = data[9];
					data[9] = data[28];
					data[28] = v15;
				}
				else
				{
					v9 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v9;
					v10 = data[10];
					data[10] = data[6];
					data[6] = v10;
					v20 = (data[5] >> 2) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v20)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v11 = data[8];
					data[8] = data[10];
					data[10] = v11;
					v19 = (data[13] >> 4) & 1;
					if ((data[13] >> 1) & 1)
						data[13] |= 0x10u;
					else
						data[13] &= 0xEFu;
					if (v19)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					v18 = (data[1] >> 5) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					if (v18)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v17 = (*data >> 1) & 1;
					if ((*data >> 1) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v17)
						*data |= 2u;
					else
						*data &= 0xFDu;
					data[2] ^= 0x64u;
					data[5] ^= 0x65u;
					*data ^= 0x78u;
					v12 = data[6];
					data[6] = data[9];
					data[9] = v12;
				}
			}
			else
			{
				v4 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v4;
				v5 = *data >> 2;
				*data <<= 6;
				*data |= v5;
				v6 = data[6] >> 2;
				data[6] <<= 6;
				data[6] |= v6;
				v7 = data[7] >> 6;
				data[7] *= 4;
				data[7] |= v7;
				v21 = (data[2] >> 2) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v21)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				data[4] ^= 0x32u;
				v8 = data[3];
				data[3] = data[3];
				data[3] = v8;
			}
		}
		else
		{
			data[2] ^= 0x31u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			*data ^= 0x65u;
			v2 = (*data >> 2) & 1;
			if ((*data >> 5) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
		}
	}
}

void PacketTwister_28::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST23_1@3
	char v8; // ST1E_1@15
	char v9; // ST1D_1@15
	char v10; // ST1C_1@15
	char v11; // ST15_1@35
	char v12; // ST14_1@35
	char v13; // ST11_1@43
	char v14; // ST10_1@43
	char v15; // ST0F_1@43
	char v16; // ST0E_1@43
	char v17; // ST09_1@56
	char v18; // ST08_1@56
	char v19; // ST07_1@56
	char v20; // [sp+5h] [bp-23h]@56
	char v21; // [sp+Ah] [bp-1Eh]@49
	char v22; // [sp+Ch] [bp-1Ch]@43
	char v23; // [sp+12h] [bp-16h]@35
	char v24; // [sp+16h] [bp-12h]@29
	char v25; // [sp+18h] [bp-10h]@21
	char v26; // [sp+1Ah] [bp-Eh]@15
	char v27; // [sp+1Fh] [bp-9h]@9
	char v2; // [sp+21h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[24] >> 5;
					data[24] *= 8;
					data[24] |= v17;
					v18 = data[23];
					data[23] = data[14];
					data[14] = v18;
					v19 = data[16];
					data[16] = data[3];
					data[3] = v19;
					data[7] ^= 0x47u;
					v20 = (data[28] >> 3) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 8u;
					else
						data[28] &= 0xF7u;
					if (v20)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
				}
				else
				{
					data[14] ^= 0xA0u;
					v13 = data[3];
					data[3] = data[10];
					data[10] = v13;
					data[15] ^= 0x2Cu;
					v14 = data[3];
					data[3] = data[3];
					data[3] = v14;
					v15 = data[10] >> 3;
					data[10] *= 32;
					data[10] |= v15;
					v16 = data[3];
					data[3] = data[3];
					data[3] = v16;
					v22 = (data[4] >> 6) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v22)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v21 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v21)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
				}
			}
			else
			{
				data[3] ^= 0xD9u;
				v24 = (data[7] >> 7) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v24)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				data[7] ^= 0x7Bu;
				v11 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v11;
				v12 = data[6];
				data[6] = data[1];
				data[1] = v12;
				v23 = (data[5] >> 7) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x80u;
				else
					data[5] &= 0x7Fu;
				if (v23)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v5 = data[3];
			data[3] = data[2];
			data[2] = v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = data[3] >> 7;
			data[3] *= 2;
			data[3] |= v7;
			v2 = (data[2] >> 5) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v27 = (data[3] >> 5) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v27)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v8 = data[3];
			data[3] = *data;
			*data = v8;
			v9 = data[2];
			data[2] = data[2];
			data[2] = v9;
			v10 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v10;
			v26 = (data[3] >> 5) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v26)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v25 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v25)
				*data |= 2u;
			else
				*data &= 0xFDu;
		}
	}
}

void PacketTwister_28::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@15
	char v4; // ST22_1@15
	char v5; // ST21_1@15
	char v6; // ST1C_1@27
	char v7; // ST1B_1@27
	char v8; // ST1A_1@27
	char v9; // ST19_1@27
	char v10; // ST18_1@27
	char v11; // ST15_1@35
	char v12; // ST14_1@35
	char v13; // ST0D_1@55
	char v14; // ST0C_1@55
	char v15; // ST0B_1@55
	char v16; // ST0A_1@55
	char v17; // ST07_1@62
	char v18; // ST06_1@62
	char v19; // ST05_1@62
	char v20; // [sp+8h] [bp-20h]@56
	char v21; // [sp+Eh] [bp-1Ah]@49
	char v22; // [sp+10h] [bp-18h]@43
	char v23; // [sp+12h] [bp-16h]@35
	char v24; // [sp+16h] [bp-12h]@29
	char v25; // [sp+1Dh] [bp-Bh]@21
	char v26; // [sp+1Fh] [bp-9h]@15
	char v27; // [sp+24h] [bp-4h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = (data[28] >> 3) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 8u;
					else
						data[28] &= 0xF7u;
					if (v20)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
					data[7] ^= 0x47u;
					v17 = data[16];
					data[16] = data[3];
					data[3] = v17;
					v18 = data[23];
					data[23] = data[14];
					data[14] = v18;
					v19 = data[24] >> 3;
					data[24] *= 32;
					data[24] |= v19;
				}
				else
				{
					v22 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v22)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v21 = (data[4] >> 6) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v21)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v13 = data[3];
					data[3] = data[3];
					data[3] = v13;
					v14 = data[10] >> 5;
					data[10] *= 8;
					data[10] |= v14;
					v15 = data[3];
					data[3] = data[3];
					data[3] = v15;
					data[15] ^= 0x2Cu;
					v16 = data[3];
					data[3] = data[10];
					data[10] = v16;
					data[14] ^= 0xA0u;
				}
			}
			else
			{
				v24 = (data[5] >> 7) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x80u;
				else
					data[5] &= 0x7Fu;
				if (v24)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v11 = data[6];
				data[6] = data[1];
				data[1] = v11;
				v12 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v12;
				data[7] ^= 0x7Bu;
				v23 = (data[7] >> 7) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v23)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				data[3] ^= 0xD9u;
			}
		}
		else
		{
			v2 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v27 = (data[3] >> 5) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v27)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v3 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v3;
			v4 = data[2];
			data[2] = data[2];
			data[2] = v4;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			v26 = (data[3] >> 5) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v26)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v25 = (data[2] >> 5) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v25)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v6 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
			v8 = data[3];
			data[3] = data[2];
			data[2] = v8;
			v9 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v9;
			v10 = data[1];
			data[1] = *data;
			*data = v10;
		}
	}
}

void PacketTwister_29::Twist(BYTE *data, int length)
{
	char v3; // ST19_1@9
	char v4; // ST18_1@9
	char v5; // ST13_1@23
	char v6; // ST12_1@23
	char v7; // ST0F_1@29
	char v8; // ST0C_1@37
	char v9; // ST09_1@43
	char v10; // ST06_1@50
	char v11; // [sp+4h] [bp-18h]@50
	char v12; // [sp+7h] [bp-15h]@44
	char v13; // [sp+Ah] [bp-12h]@37
	char v14; // [sp+Dh] [bp-Fh]@31
	char v15; // [sp+10h] [bp-Ch]@23
	char v16; // [sp+14h] [bp-8h]@15
	char v17; // [sp+16h] [bp-6h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = (data[1] >> 6) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
					if (v12)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v10 = data[6];
					data[6] = data[28];
					data[28] = v10;
					v11 = (data[31] >> 2) & 1;
					if ((data[31] >> 2) & 1)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
					if (v11)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
				}
				else
				{
					v14 = (*data >> 2) & 1;
					if ((*data >> 5) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v14)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					data[14] ^= 0x46u;
					v8 = data[4] >> 3;
					data[4] *= 32;
					data[4] |= v8;
					v13 = (data[13] >> 7) & 1;
					if ((data[13] >> 5) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v13)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					v9 = data[10] >> 3;
					data[10] *= 32;
					data[10] |= v9;
					data[1] ^= 0xEFu;
					data[12] ^= 0x9Eu;
				}
			}
			else
			{
				v5 = data[6];
				data[6] = data[7];
				data[7] = v5;
				v6 = data[3];
				data[3] = data[4];
				data[4] = v6;
				data[7] ^= 0x85u;
				v15 = (data[7] >> 1) & 1;
				if ((data[7] >> 2) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v15)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				v7 = data[2];
				data[2] = data[1];
				data[1] = v7;
				data[4] ^= 0x1Fu;
			}
		}
		else
		{
			data[3] ^= 0x49u;
			data[1] ^= 0xD5u;
			v2 = (*data >> 1) & 1;
			if ((*data >> 1) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v3 = *data >> 1;
			*data <<= 7;
			*data |= v3;
			v4 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v4;
			v17 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v17)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v16 = (*data >> 6) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			if (v16)
				*data |= 4u;
			else
				*data &= 0xFBu;
			data[3] ^= 0xC0u;
		}
	}
}

void PacketTwister_29::Correct(BYTE *data, int length)
{
	char v3; // ST17_1@15
	char v4; // ST16_1@15
	char v5; // ST13_1@23
	char v6; // ST10_1@29
	char v7; // ST0F_1@29
	char v8; // ST0E_1@31
	char v9; // ST0B_1@37
	char v10; // ST06_1@50
	char v11; // [sp+4h] [bp-18h]@50
	char v12; // [sp+7h] [bp-15h]@44
	char v13; // [sp+9h] [bp-13h]@37
	char v14; // [sp+Ch] [bp-10h]@31
	char v15; // [sp+11h] [bp-Bh]@23
	char v16; // [sp+14h] [bp-8h]@15
	char v17; // [sp+18h] [bp-4h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = (data[31] >> 2) & 1;
					if ((data[31] >> 2) & 1)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
					if (v12)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
					v10 = data[6];
					data[6] = data[28];
					data[28] = v10;
					v11 = (data[1] >> 6) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
					if (v11)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
				}
				else
				{
					data[12] ^= 0x9Eu;
					data[1] ^= 0xEFu;
					v8 = data[10] >> 5;
					data[10] *= 8;
					data[10] |= v8;
					v14 = (data[13] >> 7) & 1;
					if ((data[13] >> 5) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v14)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					v9 = data[4] >> 5;
					data[4] *= 8;
					data[4] |= v9;
					data[14] ^= 0x46u;
					v13 = (*data >> 2) & 1;
					if ((*data >> 5) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v13)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
				}
			}
			else
			{
				data[4] ^= 0x1Fu;
				v5 = data[2];
				data[2] = data[1];
				data[1] = v5;
				v15 = (data[7] >> 1) & 1;
				if ((data[7] >> 2) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v15)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				data[7] ^= 0x85u;
				v6 = data[3];
				data[3] = data[4];
				data[4] = v6;
				v7 = data[6];
				data[6] = data[7];
				data[7] = v7;
			}
		}
		else
		{
			data[3] ^= 0xC0u;
			v2 = (*data >> 6) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			if (v2)
				*data |= 4u;
			else
				*data &= 0xFBu;
			v17 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v17)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v3 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v3;
			v4 = *data >> 7;
			*data *= 2;
			*data |= v4;
			v16 = (*data >> 1) & 1;
			if ((*data >> 1) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v16)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[1] ^= 0xD5u;
			data[3] ^= 0x49u;
		}
	}
}

void PacketTwister_30::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1E_1@9
	char v7; // ST1D_1@11
	char v8; // ST1C_1@11
	char v9; // ST19_1@17
	char v10; // ST18_1@17
	char v11; // ST15_1@23
	char v12; // ST14_1@23
	char v13; // ST11_1@31
	char v14; // ST10_1@31
	char v15; // ST0B_1@43
	char v16; // ST0A_1@44
	char v17; // ST09_1@44
	char v18; // ST06_1@50
	char v19; // ST05_1@50
	char v20; // ST04_1@50
	char v21; // [sp+7h] [bp-1Dh]@44
	char v22; // [sp+Ch] [bp-18h]@37
	char v23; // [sp+Eh] [bp-16h]@31
	char v24; // [sp+12h] [bp-12h]@23
	char v25; // [sp+16h] [bp-Eh]@17
	char v26; // [sp+1Ah] [bp-Ah]@11
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[16];
					data[16] = data[9];
					data[9] = v16;
					data[18] ^= 0x8Fu;
					v17 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v17;
					v21 = (data[22] >> 1) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					if (v21)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v18 = data[23];
					data[23] = data[24];
					data[24] = v18;
					v19 = data[18];
					data[18] = data[21];
					data[21] = v19;
					v20 = data[8];
					data[8] = data[14];
					data[14] = v20;
				}
				else
				{
					v13 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v13;
					v14 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v14;
					data[1] ^= 0xA3u;
					v23 = (data[15] >> 2) & 1;
					if ((data[15] >> 7) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v23)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					v22 = (data[8] >> 2) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v22)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					v15 = data[2];
					data[2] = data[8];
					data[8] = v15;
				}
			}
			else
			{
				v7 = *data;
				*data = data[4];
				data[4] = v7;
				v8 = data[5] >> 5;
				data[5] *= 8;
				data[5] |= v8;
				v26 = (data[1] >> 3) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v26)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v9 = *data >> 6;
				*data *= 4;
				*data |= v9;
				v10 = data[3];
				data[3] = data[5];
				data[5] = v10;
				v25 = (data[2] >> 4) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v25)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v11 = data[6];
				data[6] = data[3];
				data[3] = v11;
				v12 = *data >> 7;
				*data *= 2;
				*data |= v12;
				v24 = (data[5] >> 1) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v24)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			*data ^= 0x18u;
			v4 = *data >> 7;
			*data *= 2;
			*data |= v4;
			*data ^= 0x8Eu;
			*data ^= 0xE7u;
			v5 = data[1] >> 3;
			data[1] *= 32;
			data[1] |= v5;
			v6 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v6;
		}
	}
}

void PacketTwister_30::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST1E_1@9
	char v7; // ST1B_1@17
	char v8; // ST1A_1@17
	char v9; // ST17_1@23
	char v10; // ST16_1@23
	char v11; // ST13_1@29
	char v12; // ST12_1@29
	char v13; // ST11_1@31
	char v14; // ST0C_1@43
	char v15; // ST0B_1@43
	char v16; // ST0A_1@44
	char v17; // ST09_1@44
	char v18; // ST08_1@44
	char v19; // ST05_1@50
	char v20; // ST04_1@50
	char v21; // [sp+6h] [bp-1Eh]@44
	char v22; // [sp+Dh] [bp-17h]@37
	char v23; // [sp+Fh] [bp-15h]@31
	char v24; // [sp+14h] [bp-10h]@23
	char v25; // [sp+18h] [bp-Ch]@17
	char v26; // [sp+1Ch] [bp-8h]@11
	char v2; // [sp+1Fh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[8];
					data[8] = data[14];
					data[14] = v16;
					v17 = data[18];
					data[18] = data[21];
					data[21] = v17;
					v18 = data[23];
					data[23] = data[24];
					data[24] = v18;
					v21 = (data[22] >> 1) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 2u;
					else
						data[22] &= 0xFDu;
					if (v21)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v19 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v19;
					data[18] ^= 0x8Fu;
					v20 = data[16];
					data[16] = data[9];
					data[9] = v20;
				}
				else
				{
					v13 = data[2];
					data[2] = data[8];
					data[8] = v13;
					v23 = (data[8] >> 2) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v23)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					v22 = (data[15] >> 2) & 1;
					if ((data[15] >> 7) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v22)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					data[1] ^= 0xA3u;
					v14 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v14;
					v15 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v15;
				}
			}
			else
			{
				v26 = (data[5] >> 1) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v26)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				v7 = *data >> 1;
				*data <<= 7;
				*data |= v7;
				v8 = data[6];
				data[6] = data[3];
				data[3] = v8;
				v25 = (data[2] >> 4) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v25)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v9 = data[3];
				data[3] = data[5];
				data[5] = v9;
				v10 = *data >> 2;
				*data <<= 6;
				*data |= v10;
				v24 = (data[1] >> 3) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v24)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v11 = data[5] >> 3;
				data[5] *= 32;
				data[5] |= v11;
				v12 = *data;
				*data = data[4];
				data[4] = v12;
			}
		}
		else
		{
			v3 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v3;
			v4 = data[1] >> 5;
			data[1] *= 8;
			data[1] |= v4;
			*data ^= 0xE7u;
			*data ^= 0x8Eu;
			v5 = *data >> 1;
			*data <<= 7;
			*data |= v5;
			*data ^= 0x18u;
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 7) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			v6 = data[3];
			data[3] = *data;
			*data = v6;
		}
	}
}

void PacketTwister_31::Twist(BYTE *data, int length)
{
	char v3; // ST17_1@15
	char v4; // ST16_1@15
	char v5; // ST15_1@15
	char v6; // ST14_1@17
	char v7; // ST0F_1@29
	char v8; // ST0E_1@29
	char v9; // ST0D_1@29
	char v10; // ST0C_1@29
	char v11; // ST0B_1@29
	char v12; // ST08_1@35
	char v13; // ST07_1@37
	char v14; // ST06_1@38
	char v15; // [sp+4h] [bp-18h]@38
	char v16; // [sp+9h] [bp-13h]@29
	char v17; // [sp+10h] [bp-Ch]@23
	char v18; // [sp+12h] [bp-Ah]@17
	char v19; // [sp+18h] [bp-4h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[26];
					data[26] = data[22];
					data[22] = v14;
					v15 = (data[15] >> 2) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v15)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					data[11] ^= 0x6Au;
				}
				else
				{
					v13 = data[10];
					data[10] = data[12];
					data[12] = v13;
					data[9] ^= 0x46u;
					data[10] ^= 0xC3u;
				}
			}
			else
			{
				v6 = data[3];
				data[3] = data[5];
				data[5] = v6;
				*data ^= 0x2Bu;
				v18 = (data[6] >> 1) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				if (v18)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				v17 = (*data >> 1) & 1;
				if ((*data >> 2) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v17)
					*data |= 4u;
				else
					*data &= 0xFBu;
				v7 = data[6] >> 6;
				data[6] *= 4;
				data[6] |= v7;
				v8 = data[5];
				data[5] = *data;
				*data = v8;
				v9 = *data >> 6;
				*data *= 4;
				*data |= v9;
				v10 = data[3];
				data[3] = data[4];
				data[4] = v10;
				v11 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v11;
				v16 = (data[7] >> 4) & 1;
				if ((data[7] >> 4) & 1)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				if (v16)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				v12 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v12;
			}
		}
		else
		{
			data[3] ^= 0x15u;
			*data ^= 0xDBu;
			v2 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v19 = (*data >> 1) & 1;
			if ((*data >> 1) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v19)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			*data ^= 0x6Au;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			data[2] ^= 0xC1u;
			v5 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v5;
		}
	}
}

void PacketTwister_31::Correct(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST19_1@3
	char v6; // ST14_1@17
	char v7; // ST11_1@23
	char v8; // ST10_1@23
	char v9; // ST0F_1@23
	char v10; // ST0E_1@23
	char v11; // ST0D_1@23
	char v12; // ST08_1@35
	char v13; // ST07_1@37
	char v14; // ST04_1@44
	char v15; // [sp+5h] [bp-17h]@38
	char v16; // [sp+9h] [bp-13h]@29
	char v17; // [sp+Bh] [bp-11h]@23
	char v18; // [sp+12h] [bp-Ah]@17
	char v19; // [sp+15h] [bp-7h]@9
	char v2; // [sp+17h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[11] ^= 0x6Au;
					v15 = (data[15] >> 2) & 1;
					if ((data[15] >> 4) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v15)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					v14 = data[26];
					data[26] = data[22];
					data[22] = v14;
				}
				else
				{
					data[10] ^= 0xC3u;
					data[9] ^= 0x46u;
					v13 = data[10];
					data[10] = data[12];
					data[12] = v13;
				}
			}
			else
			{
				v6 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v6;
				v18 = (data[7] >> 4) & 1;
				if ((data[7] >> 4) & 1)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				if (v18)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				v7 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v7;
				v8 = data[3];
				data[3] = data[4];
				data[4] = v8;
				v9 = *data >> 2;
				*data <<= 6;
				*data |= v9;
				v10 = data[5];
				data[5] = *data;
				*data = v10;
				v11 = data[6] >> 2;
				data[6] <<= 6;
				data[6] |= v11;
				v17 = (*data >> 1) & 1;
				if ((*data >> 2) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v17)
					*data |= 4u;
				else
					*data &= 0xFBu;
				v16 = (data[6] >> 1) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				if (v16)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				*data ^= 0x2Bu;
				v12 = data[3];
				data[3] = data[5];
				data[5] = v12;
			}
		}
		else
		{
			v3 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v3;
			data[2] ^= 0xC1u;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			*data ^= 0x6Au;
			v5 = data[3];
			data[3] = data[1];
			data[1] = v5;
			v2 = (*data >> 1) & 1;
			if ((*data >> 1) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v19 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v19)
				*data |= 2u;
			else
				*data &= 0xFDu;
			*data ^= 0xDBu;
			data[3] ^= 0x15u;
		}
	}
}

void PacketTwister_32::Twist(BYTE *data, int length)
{
	char v3; // ST17_1@3
	char v4; // ST16_1@5
	char v5; // ST15_1@5
	char v6; // ST12_1@13
	char v7; // ST11_1@13
	char v8; // ST10_1@13
	char v9; // ST0F_1@13
	char v10; // ST0E_1@14
	char v11; // ST0D_1@14
	char v12; // ST0C_1@14
	char v13; // ST0B_1@14
	char v14; // ST0A_1@14
	char v15; // ST09_1@14
	char v16; // ST08_1@14
	char v17; // ST07_1@14
	char v2; // [sp+13h] [bp-5h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[20] ^= 0x29u;
					v10 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v10;
					v11 = data[25] >> 3;
					data[25] *= 32;
					data[25] |= v11;
					v12 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v12;
					v13 = data[13];
					data[13] = data[29];
					data[29] = v13;
					v14 = data[23];
					data[23] = data[23];
					data[23] = v14;
					v15 = data[25] >> 5;
					data[25] *= 8;
					data[25] |= v15;
					v16 = data[18] >> 6;
					data[18] *= 4;
					data[18] |= v16;
					v17 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v17;
					data[10] ^= 0x75u;
					data[15] ^= 0xEAu;
				}
				else
				{
					v6 = data[5];
					data[5] = data[9];
					data[9] = v6;
					v7 = data[13] >> 1;
					data[13] <<= 7;
					data[13] |= v7;
					v8 = data[10];
					data[10] = *data;
					*data = v8;
					v9 = data[3];
					data[3] = data[11];
					data[11] = v9;
				}
			}
			else
			{
				v4 = data[2] >> 1;
				data[2] <<= 7;
				data[2] |= v4;
				data[4] ^= 0xEBu;
				v5 = data[3];
				data[3] = data[2];
				data[2] = v5;
				v2 = (data[1] >> 1) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v2)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
			}
		}
		else
		{
			data[3] ^= 0xDEu;
			data[3] ^= 0x9Au;
			v3 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v3;
		}
	}
}

void PacketTwister_32::Correct(BYTE *data, int length)
{
	char v3; // ST17_1@3
	char v4; // ST14_1@11
	char v5; // ST13_1@11
	char v6; // ST12_1@13
	char v7; // ST11_1@13
	char v8; // ST10_1@13
	char v9; // ST0F_1@13
	char v10; // ST0E_1@14
	char v11; // ST0D_1@14
	char v12; // ST0C_1@14
	char v13; // ST0B_1@14
	char v14; // ST0A_1@14
	char v15; // ST09_1@14
	char v16; // ST08_1@14
	char v17; // ST07_1@14
	char v2; // [sp+15h] [bp-3h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[15] ^= 0xEAu;
					data[10] ^= 0x75u;
					v10 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v10;
					v11 = data[18] >> 2;
					data[18] <<= 6;
					data[18] |= v11;
					v12 = data[25] >> 3;
					data[25] *= 32;
					data[25] |= v12;
					v13 = data[23];
					data[23] = data[23];
					data[23] = v13;
					v14 = data[13];
					data[13] = data[29];
					data[29] = v14;
					v15 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v15;
					v16 = data[25] >> 5;
					data[25] *= 8;
					data[25] |= v16;
					v17 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v17;
					data[20] ^= 0x29u;
				}
				else
				{
					v6 = data[3];
					data[3] = data[11];
					data[11] = v6;
					v7 = data[10];
					data[10] = *data;
					*data = v7;
					v8 = data[13] >> 7;
					data[13] *= 2;
					data[13] |= v8;
					v9 = data[5];
					data[5] = data[9];
					data[9] = v9;
				}
			}
			else
			{
				v2 = (data[1] >> 1) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v2)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				v4 = data[3];
				data[3] = data[2];
				data[2] = v4;
				data[4] ^= 0xEBu;
				v5 = data[2] >> 7;
				data[2] *= 2;
				data[2] |= v5;
			}
		}
		else
		{
			v3 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v3;
			data[3] ^= 0x9Au;
			data[3] ^= 0xDEu;
		}
	}
}

void PacketTwister_33::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST19_1@9
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST10_1@31
	char v11; // ST0D_1@37
	char v12; // ST0C_1@38
	char v13; // ST0B_1@38
	char v14; // ST0A_1@38
	char v15; // ST09_1@38
	char v16; // ST08_1@38
	char v17; // ST07_1@38
	char v18; // [sp+Eh] [bp-12h]@31
	char v19; // [sp+11h] [bp-Fh]@25
	char v20; // [sp+13h] [bp-Dh]@17
	char v21; // [sp+17h] [bp-9h]@11
	char v2; // [sp+1Ah] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v12;
					v13 = data[22] >> 3;
					data[22] *= 32;
					data[22] |= v13;
					data[25] ^= 0xC3u;
					v14 = data[3] >> 3;
					data[3] *= 32;
					data[3] |= v14;
					v15 = data[17];
					data[17] = data[26];
					data[26] = v15;
					data[20] ^= 0xD9u;
					v16 = data[7];
					data[7] = data[3];
					data[3] = v16;
					v17 = data[11] >> 1;
					data[11] <<= 7;
					data[11] |= v17;
				}
				else
				{
					v19 = (data[6] >> 2) & 1;
					if ((data[6] >> 6) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v19)
						data[6] |= 0x40u;
					else
						data[6] &= 0xBFu;
					v10 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v10;
					v18 = (data[6] >> 5) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 0x20u;
					else
						data[6] &= 0xDFu;
					if (v18)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v11 = data[10] >> 7;
					data[10] *= 2;
					data[10] |= v11;
					data[3] ^= 0xF4u;
				}
			}
			else
			{
				data[1] ^= 0xD8u;
				v21 = (data[4] >> 1) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v21)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				v8 = data[6];
				data[6] = data[5];
				data[5] = v8;
				data[5] ^= 0x19u;
				data[6] ^= 0x51u;
				*data ^= 0x5Bu;
				*data ^= 0x65u;
				v9 = data[1];
				data[1] = data[2];
				data[2] = v9;
				v20 = (data[6] >> 6) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				if (v20)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				data[6] ^= 0xB4u;
			}
		}
		else
		{
			*data ^= 0xA9u;
			v3 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v3;
			data[1] ^= 0x4Du;
			v4 = data[1];
			data[1] = data[1];
			data[1] = v4;
			*data ^= 0x48u;
			*data ^= 0x50u;
			data[3] ^= 0x32u;
			v5 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v5;
			v6 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v6;
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			v7 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v7;
		}
	}
}

void PacketTwister_33::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST12_1@25
	char v11; // ST0F_1@31
	char v12; // ST0C_1@38
	char v13; // ST0B_1@38
	char v14; // ST0A_1@38
	char v15; // ST09_1@38
	char v16; // ST08_1@38
	char v17; // ST07_1@38
	char v18; // [sp+Dh] [bp-13h]@31
	char v19; // [sp+10h] [bp-10h]@25
	char v20; // [sp+13h] [bp-Dh]@17
	char v21; // [sp+17h] [bp-9h]@11
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[11] >> 7;
					data[11] *= 2;
					data[11] |= v12;
					v13 = data[7];
					data[7] = data[3];
					data[3] = v13;
					data[20] ^= 0xD9u;
					v14 = data[17];
					data[17] = data[26];
					data[26] = v14;
					v15 = data[3] >> 5;
					data[3] *= 8;
					data[3] |= v15;
					data[25] ^= 0xC3u;
					v16 = data[22] >> 5;
					data[22] *= 8;
					data[22] |= v16;
					v17 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v17;
				}
				else
				{
					data[3] ^= 0xF4u;
					v10 = data[10] >> 1;
					data[10] <<= 7;
					data[10] |= v10;
					v19 = (data[6] >> 5) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 0x20u;
					else
						data[6] &= 0xDFu;
					if (v19)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v11 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v11;
					v18 = (data[6] >> 2) & 1;
					if ((data[6] >> 6) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v18)
						data[6] |= 0x40u;
					else
						data[6] &= 0xBFu;
				}
			}
			else
			{
				data[6] ^= 0xB4u;
				v21 = (data[6] >> 6) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				if (v21)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v8 = data[1];
				data[1] = data[2];
				data[2] = v8;
				*data ^= 0x65u;
				*data ^= 0x5Bu;
				data[6] ^= 0x51u;
				data[5] ^= 0x19u;
				v9 = data[6];
				data[6] = data[5];
				data[5] = v9;
				v20 = (data[4] >> 1) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v20)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				data[1] ^= 0xD8u;
			}
		}
		else
		{
			v3 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v3;
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			v5 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v5;
			data[3] ^= 0x32u;
			*data ^= 0x50u;
			*data ^= 0x48u;
			v6 = data[1];
			data[1] = data[1];
			data[1] = v6;
			data[1] ^= 0x4Du;
			v7 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v7;
			*data ^= 0xA9u;
		}
	}
}

void PacketTwister_34::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@5
	char v7; // ST23_1@5
	char v8; // ST20_1@11
	char v9; // ST1F_1@11
	char v10; // ST18_1@29
	char v11; // ST17_1@29
	char v12; // ST16_1@31
	char v13; // ST15_1@31
	char v14; // ST14_1@31
	char v15; // ST13_1@31
	char v16; // ST10_1@37
	char v17; // ST0F_1@37
	char v18; // ST0C_1@44
	char v19; // ST09_1@50
	char v20; // ST08_1@50
	char v21; // ST05_1@56
	char v22; // [sp+6h] [bp-22h]@50
	char v23; // [sp+Ah] [bp-1Eh]@44
	char v24; // [sp+Dh] [bp-1Bh]@38
	char v25; // [sp+11h] [bp-17h]@31
	char v26; // [sp+19h] [bp-Fh]@23
	char v27; // [sp+1Bh] [bp-Dh]@17
	char v28; // [sp+1Dh] [bp-Bh]@11
	char v2; // [sp+21h] [bp-7h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[16] ^= 0xD9u;
					v24 = (data[28] >> 7) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 0x80u;
					else
						data[28] &= 0x7Fu;
					if (v24)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
					v18 = data[2];
					data[2] = data[4];
					data[4] = v18;
					v23 = (data[5] >> 3) & 1;
					if ((data[5] >> 6) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v23)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					v19 = data[18] >> 6;
					data[18] *= 4;
					data[18] |= v19;
					v20 = data[16];
					data[16] = *data;
					*data = v20;
					data[10] ^= 0x2Bu;
					v22 = (data[24] >> 2) & 1;
					if ((data[24] >> 2) & 1)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					if (v22)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					v21 = data[27] >> 1;
					data[27] <<= 7;
					data[27] |= v21;
				}
				else
				{
					v12 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v12;
					v13 = data[6];
					data[6] = data[7];
					data[7] = v13;
					v14 = data[9];
					data[9] = data[14];
					data[14] = v14;
					v15 = data[1];
					data[1] = data[12];
					data[12] = v15;
					v25 = (*data >> 7) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					if (v25)
						*data |= 2u;
					else
						*data &= 0xFDu;
					data[6] ^= 0x8Du;
					data[12] ^= 0x85u;
					data[11] ^= 0xCCu;
					v16 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v16;
					v17 = data[5];
					data[5] = data[3];
					data[3] = v17;
				}
			}
			else
			{
				v6 = data[6];
				data[6] = data[6];
				data[6] = v6;
				v7 = *data;
				*data = data[4];
				data[4] = v7;
				data[5] ^= 0xACu;
				v2 = (data[5] >> 2) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v2)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				v8 = data[7];
				data[7] = *data;
				*data = v8;
				data[4] ^= 0xE5u;
				v9 = data[6];
				data[6] = *data;
				*data = v9;
				data[5] ^= 0xD9u;
				v28 = (data[6] >> 4) & 1;
				if ((data[6] >> 4) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v28)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				v27 = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v27)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v26 = (data[4] >> 5) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v26)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v10 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v10;
				v11 = data[7];
				data[7] = data[2];
				data[2] = v11;
			}
		}
		else
		{
			v3 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v3;
			data[2] ^= 0x71u;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			v5 = data[1];
			data[1] = data[3];
			data[3] = v5;
		}
	}
}

void PacketTwister_34::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@5
	char v7; // ST23_1@5
	char v8; // ST1C_1@23
	char v9; // ST1B_1@23
	char v10; // ST18_1@29
	char v11; // ST17_1@29
	char v12; // ST16_1@31
	char v13; // ST15_1@31
	char v14; // ST12_1@37
	char v15; // ST11_1@37
	char v16; // ST10_1@37
	char v17; // ST0F_1@37
	char v18; // ST0E_1@38
	char v19; // ST0B_1@44
	char v20; // ST0A_1@44
	char v21; // ST07_1@50
	char v22; // [sp+5h] [bp-23h]@50
	char v23; // [sp+8h] [bp-20h]@44
	char v24; // [sp+Ch] [bp-1Ch]@38
	char v25; // [sp+13h] [bp-15h]@31
	char v26; // [sp+19h] [bp-Fh]@23
	char v27; // [sp+1Dh] [bp-Bh]@17
	char v28; // [sp+1Fh] [bp-9h]@11
	char v2; // [sp+21h] [bp-7h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[27] >> 7;
					data[27] *= 2;
					data[27] |= v18;
					v24 = (data[24] >> 2) & 1;
					if ((data[24] >> 2) & 1)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					if (v24)
						data[24] |= 4u;
					else
						data[24] &= 0xFBu;
					data[10] ^= 0x2Bu;
					v19 = data[16];
					data[16] = *data;
					*data = v19;
					v20 = data[18] >> 2;
					data[18] <<= 6;
					data[18] |= v20;
					v23 = (data[5] >> 3) & 1;
					if ((data[5] >> 6) & 1)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					if (v23)
						data[5] |= 0x40u;
					else
						data[5] &= 0xBFu;
					v21 = data[2];
					data[2] = data[4];
					data[4] = v21;
					v22 = (data[28] >> 7) & 1;
					if ((data[28] >> 1) & 1)
						data[28] |= 0x80u;
					else
						data[28] &= 0x7Fu;
					if (v22)
						data[28] |= 2u;
					else
						data[28] &= 0xFDu;
					data[16] ^= 0xD9u;
				}
				else
				{
					v12 = data[5];
					data[5] = data[3];
					data[3] = v12;
					v13 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v13;
					data[11] ^= 0xCCu;
					data[12] ^= 0x85u;
					data[6] ^= 0x8Du;
					v25 = (*data >> 7) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x80u;
					else
						*data &= 0x7Fu;
					if (v25)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v14 = data[1];
					data[1] = data[12];
					data[12] = v14;
					v15 = data[9];
					data[9] = data[14];
					data[14] = v15;
					v16 = data[6];
					data[6] = data[7];
					data[7] = v16;
					v17 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v17;
				}
			}
			else
			{
				v6 = data[7];
				data[7] = data[2];
				data[2] = v6;
				v7 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v7;
				v2 = (data[4] >> 5) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v2)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v28 = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v28)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v27 = (data[6] >> 4) & 1;
				if ((data[6] >> 4) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v27)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				data[5] ^= 0xD9u;
				v8 = data[6];
				data[6] = *data;
				*data = v8;
				data[4] ^= 0xE5u;
				v9 = data[7];
				data[7] = *data;
				*data = v9;
				v26 = (data[5] >> 2) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v26)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				data[5] ^= 0xACu;
				v10 = *data;
				*data = data[4];
				data[4] = v10;
				v11 = data[6];
				data[6] = data[6];
				data[6] = v11;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[3];
			data[3] = v3;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			data[2] ^= 0x71u;
			v5 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v5;
		}
	}
}

void PacketTwister_35::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST19_1@11
	char v8; // ST18_1@11
	char v9; // ST13_1@25
	char v10; // ST12_1@25
	char v11; // ST11_1@26
	char v12; // ST10_1@26
	char v13; // ST0D_1@32
	char v14; // ST08_1@44
	char v15; // [sp+6h] [bp-1Ah]@44
	char v16; // [sp+9h] [bp-17h]@38
	char v17; // [sp+Bh] [bp-15h]@32
	char v18; // [sp+Eh] [bp-12h]@26
	char v19; // [sp+14h] [bp-Ch]@19
	char v20; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+1Ah] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v11 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v11;
					v12 = data[6];
					data[6] = data[4];
					data[4] = v12;
					v18 = (data[21] >> 2) & 1;
					if ((data[21] >> 4) & 1)
						data[21] |= 4u;
					else
						data[21] &= 0xFBu;
					if (v18)
						data[21] |= 0x10u;
					else
						data[21] &= 0xEFu;
					v13 = data[22];
					data[22] = data[17];
					data[17] = v13;
					data[23] ^= 0xB4u;
					v17 = (data[19] >> 7) & 1;
					if ((data[19] >> 7) & 1)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					if (v17)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					v16 = (data[17] >> 7) & 1;
					if ((data[17] >> 5) & 1)
						data[17] |= 0x80u;
					else
						data[17] &= 0x7Fu;
					if (v16)
						data[17] |= 0x20u;
					else
						data[17] &= 0xDFu;
					data[5] ^= 0x40u;
					v14 = data[23];
					data[23] = data[12];
					data[12] = v14;
					v15 = (data[27] >> 1) & 1;
					if ((data[27] >> 2) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v15)
						data[27] |= 4u;
					else
						data[27] &= 0xFBu;
				}
				else
				{
					data[3] ^= 0x2Fu;
					data[4] ^= 0x17u;
					data[15] ^= 0x5Bu;
					data[7] ^= 0x53u;
					v19 = (data[11] >> 7) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					if (v19)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					data[3] ^= 0x89u;
					data[14] ^= 0x6Du;
					v9 = data[13];
					data[13] = data[9];
					data[9] = v9;
					data[12] ^= 0xFCu;
					data[4] ^= 0xF7u;
					v10 = data[5];
					data[5] = data[4];
					data[4] = v10;
				}
			}
			else
			{
				v7 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v7;
				v8 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v8;
				data[1] ^= 0x98u;
				v20 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v20)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				data[3] ^= 0xDDu;
				data[1] ^= 6u;
				data[4] ^= 0x7Eu;
			}
		}
		else
		{
			data[2] ^= 0xCFu;
			*data ^= 0xD6u;
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			data[2] ^= 0x72u;
			*data ^= 0xE2u;
			v4 = data[2];
			data[2] = data[3];
			data[3] = v4;
			v5 = *data;
			*data = data[3];
			data[3] = v5;
			v6 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v6;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
		}
	}
}

void PacketTwister_35::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST19_1@11
	char v8; // ST18_1@11
	char v9; // ST13_1@25
	char v10; // ST12_1@25
	char v11; // ST11_1@26
	char v12; // ST10_1@26
	char v13; // ST0D_1@32
	char v14; // ST08_1@44
	char v15; // [sp+6h] [bp-1Ah]@44
	char v16; // [sp+9h] [bp-17h]@38
	char v17; // [sp+Bh] [bp-15h]@32
	char v18; // [sp+Eh] [bp-12h]@26
	char v19; // [sp+14h] [bp-Ch]@19
	char v20; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+1Ah] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v11 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v11;
					v12 = data[6];
					data[6] = data[4];
					data[4] = v12;
					v18 = (data[21] >> 2) & 1;
					if ((data[21] >> 4) & 1)
						data[21] |= 4u;
					else
						data[21] &= 0xFBu;
					if (v18)
						data[21] |= 0x10u;
					else
						data[21] &= 0xEFu;
					v13 = data[22];
					data[22] = data[17];
					data[17] = v13;
					data[23] ^= 0xB4u;
					v17 = (data[19] >> 7) & 1;
					if ((data[19] >> 7) & 1)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					if (v17)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					v16 = (data[17] >> 7) & 1;
					if ((data[17] >> 5) & 1)
						data[17] |= 0x80u;
					else
						data[17] &= 0x7Fu;
					if (v16)
						data[17] |= 0x20u;
					else
						data[17] &= 0xDFu;
					data[5] ^= 0x40u;
					v14 = data[23];
					data[23] = data[12];
					data[12] = v14;
					v15 = (data[27] >> 1) & 1;
					if ((data[27] >> 2) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v15)
						data[27] |= 4u;
					else
						data[27] &= 0xFBu;
				}
				else
				{
					data[3] ^= 0x2Fu;
					data[4] ^= 0x17u;
					data[15] ^= 0x5Bu;
					data[7] ^= 0x53u;
					v19 = (data[11] >> 7) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					if (v19)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					data[3] ^= 0x89u;
					data[14] ^= 0x6Du;
					v9 = data[13];
					data[13] = data[9];
					data[9] = v9;
					data[12] ^= 0xFCu;
					data[4] ^= 0xF7u;
					v10 = data[5];
					data[5] = data[4];
					data[4] = v10;
				}
			}
			else
			{
				v7 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v7;
				v8 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v8;
				data[1] ^= 0x98u;
				v20 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v20)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				data[3] ^= 0xDDu;
				data[1] ^= 6u;
				data[4] ^= 0x7Eu;
			}
		}
		else
		{
			data[2] ^= 0xCFu;
			*data ^= 0xD6u;
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			data[2] ^= 0x72u;
			*data ^= 0xE2u;
			v4 = data[2];
			data[2] = data[3];
			data[3] = v4;
			v5 = *data;
			*data = data[3];
			data[3] = v5;
			v6 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v6;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
		}
	}
}

void PacketTwister_36::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST12_1@29
	char v9; // ST0F_1@35
	char v10; // ST0E_1@37
	char v11; // ST0B_1@43
	char v12; // ST0A_1@43
	char v13; // ST09_1@44
	char v14; // ST06_1@50
	char v15; // ST05_1@50
	char v16; // [sp+7h] [bp-19h]@44
	char v17; // [sp+Ch] [bp-14h]@37
	char v18; // [sp+10h] [bp-10h]@29
	char v19; // [sp+13h] [bp-Dh]@23
	char v20; // [sp+15h] [bp-Bh]@17
	char v21; // [sp+17h] [bp-9h]@9
	char v2; // [sp+1Bh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v13;
					v16 = (data[9] >> 2) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v16)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v14 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v14;
					v15 = data[26] >> 3;
					data[26] *= 32;
					data[26] |= v15;
				}
				else
				{
					v10 = data[9];
					data[9] = *data;
					*data = v10;
					v17 = (data[14] >> 3) & 1;
					if ((data[14] >> 6) & 1)
						data[14] |= 8u;
					else
						data[14] &= 0xF7u;
					if (v17)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					v11 = data[11];
					data[11] = data[14];
					data[14] = v11;
					v12 = data[13];
					data[13] = data[11];
					data[11] = v12;
					data[6] ^= 0xDCu;
					data[12] ^= 0x59u;
				}
			}
			else
			{
				v20 = (*data >> 3) & 1;
				if ((*data >> 1) & 1)
					*data |= 8u;
				else
					*data &= 0xF7u;
				if (v20)
					*data |= 2u;
				else
					*data &= 0xFDu;
				data[3] ^= 0xFBu;
				v19 = (data[7] >> 2) & 1;
				if ((data[7] >> 3) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v19)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				data[7] ^= 0xABu;
				v8 = data[1];
				data[1] = data[6];
				data[6] = v8;
				v18 = (data[2] >> 4) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v18)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v9 = data[4];
				data[4] = data[3];
				data[3] = v9;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[1];
			data[1] = v3;
			v4 = data[3];
			data[3] = *data;
			*data = v4;
			v5 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v5;
			*data ^= 0xD6u;
			v2 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v6 = *data >> 3;
			*data *= 32;
			*data |= v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
			data[2] ^= 0x63u;
			data[1] ^= 0x9Au;
			v21 = (*data >> 2) & 1;
			if ((*data >> 4) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v21)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			data[2] = data[2];
		}
	}
}

void PacketTwister_36::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST19_1@15
	char v6; // ST18_1@15
	char v7; // ST17_1@15
	char v8; // ST16_1@17
	char v9; // ST13_1@23
	char v10; // ST0E_1@37
	char v11; // ST0D_1@37
	char v12; // ST0A_1@43
	char v13; // ST09_1@44
	char v14; // ST08_1@44
	char v15; // ST05_1@50
	char v16; // [sp+6h] [bp-1Ah]@44
	char v17; // [sp+Bh] [bp-15h]@37
	char v18; // [sp+Fh] [bp-11h]@29
	char v19; // [sp+11h] [bp-Fh]@23
	char v20; // [sp+14h] [bp-Ch]@17
	char v21; // [sp+1Ah] [bp-6h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[26] >> 5;
					data[26] *= 8;
					data[26] |= v13;
					v14 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v14;
					v16 = (data[9] >> 2) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v16)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v15 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v15;
				}
				else
				{
					data[12] ^= 0x59u;
					data[6] ^= 0xDCu;
					v10 = data[13];
					data[13] = data[11];
					data[11] = v10;
					v11 = data[11];
					data[11] = data[14];
					data[14] = v11;
					v17 = (data[14] >> 3) & 1;
					if ((data[14] >> 6) & 1)
						data[14] |= 8u;
					else
						data[14] &= 0xF7u;
					if (v17)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					v12 = data[9];
					data[9] = *data;
					*data = v12;
				}
			}
			else
			{
				v8 = data[4];
				data[4] = data[3];
				data[3] = v8;
				v20 = (data[2] >> 4) & 1;
				if ((data[2] >> 1) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v20)
					data[2] |= 2u;
				else
					data[2] &= 0xFDu;
				v9 = data[1];
				data[1] = data[6];
				data[6] = v9;
				data[7] ^= 0xABu;
				v19 = (data[7] >> 2) & 1;
				if ((data[7] >> 3) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v19)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				data[3] ^= 0xFBu;
				v18 = (*data >> 3) & 1;
				if ((*data >> 1) & 1)
					*data |= 8u;
				else
					*data &= 0xF7u;
				if (v18)
					*data |= 2u;
				else
					*data &= 0xFDu;
			}
		}
		else
		{
			data[2] = data[2];
			v2 = (*data >> 2) & 1;
			if ((*data >> 4) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			data[1] ^= 0x9Au;
			data[2] ^= 0x63u;
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v4 = *data >> 5;
			*data *= 8;
			*data |= v4;
			v21 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v21)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			*data ^= 0xD6u;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			v6 = data[3];
			data[3] = *data;
			*data = v6;
			v7 = data[1];
			data[1] = data[1];
			data[1] = v7;
		}
	}
}

void PacketTwister_37::Twist(BYTE *data, int length)
{
	char v3; // ST21_1@9
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1C_1@17
	char v7; // ST19_1@23
	char v8; // ST16_1@29
	char v9; // ST15_1@31
	char v10; // ST14_1@31
	char v11; // ST11_1@37
	char v12; // ST10_1@37
	char v13; // ST0D_1@43
	char v14; // ST0A_1@50
	char v15; // ST09_1@50
	char v16; // ST06_1@56
	char v17; // ST05_1@56
	char v18; // [sp+7h] [bp-1Dh]@50
	char v19; // [sp+Bh] [bp-19h]@44
	char v20; // [sp+Eh] [bp-16h]@37
	char v21; // [sp+12h] [bp-12h]@31
	char v22; // [sp+17h] [bp-Dh]@23
	char v23; // [sp+1Ah] [bp-Ah]@17
	char v24; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+22h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v19 = (data[31] >> 4) & 1;
					if ((data[31] >> 4) & 1)
						data[31] |= 0x10u;
					else
						data[31] &= 0xEFu;
					if (v19)
						data[31] |= 0x10u;
					else
						data[31] &= 0xEFu;
					v14 = data[29];
					data[29] = data[7];
					data[7] = v14;
					data[17] ^= 0xE6u;
					v15 = data[9] >> 6;
					data[9] *= 4;
					data[9] |= v15;
					v18 = (data[24] >> 1) & 1;
					if ((data[24] >> 3) & 1)
						data[24] |= 2u;
					else
						data[24] &= 0xFDu;
					if (v18)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					v16 = data[1] >> 4;
					data[1] *= 16;
					data[1] |= v16;
					data[10] ^= 0x97u;
					v17 = data[16] >> 5;
					data[16] *= 8;
					data[16] |= v17;
				}
				else
				{
					v9 = data[7];
					data[7] = data[3];
					data[3] = v9;
					v10 = data[10];
					data[10] = data[8];
					data[8] = v10;
					v21 = (data[9] >> 2) & 1;
					if ((data[9] >> 6) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v21)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					data[5] ^= 0x7Eu;
					data[6] ^= 0xDFu;
					v11 = data[10];
					data[10] = data[8];
					data[8] = v11;
					v12 = data[13];
					data[13] = data[7];
					data[7] = v12;
					v20 = (data[11] >> 6) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 0x40u;
					else
						data[11] &= 0xBFu;
					if (v20)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v13 = data[13];
					data[13] = data[3];
					data[3] = v13;
				}
			}
			else
			{
				v6 = data[1];
				data[1] = data[1];
				data[1] = v6;
				v23 = (data[5] >> 2) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v23)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				data[5] ^= 0x83u;
				v7 = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= v7;
				data[5] ^= 0x5Du;
				v22 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v22)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				v8 = data[1];
				data[1] = *data;
				*data = v8;
				data[4] ^= 0x98u;
			}
		}
		else
		{
			data[2] ^= 0x63u;
			v2 = (data[3] >> 3) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v5 = *data;
			*data = data[3];
			data[3] = v5;
			v24 = (*data >> 6) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			if (v24)
				*data |= 2u;
			else
				*data &= 0xFDu;
		}
	}
}

void PacketTwister_37::Correct(BYTE *data, int length)
{
	char v3; // ST21_1@9
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1C_1@17
	char v7; // ST19_1@23
	char v8; // ST16_1@29
	char v9; // ST15_1@31
	char v10; // ST12_1@37
	char v11; // ST11_1@37
	char v12; // ST0E_1@43
	char v13; // ST0D_1@43
	char v14; // ST0C_1@44
	char v15; // ST0B_1@44
	char v16; // ST08_1@50
	char v17; // ST07_1@50
	char v18; // [sp+5h] [bp-1Fh]@50
	char v19; // [sp+9h] [bp-1Bh]@44
	char v20; // [sp+Fh] [bp-15h]@37
	char v21; // [sp+13h] [bp-11h]@31
	char v22; // [sp+17h] [bp-Dh]@23
	char v23; // [sp+1Ah] [bp-Ah]@17
	char v24; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+22h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[16] >> 3;
					data[16] *= 32;
					data[16] |= v14;
					data[10] ^= 0x97u;
					v15 = data[1] >> 4;
					data[1] *= 16;
					data[1] |= v15;
					v19 = (data[24] >> 1) & 1;
					if ((data[24] >> 3) & 1)
						data[24] |= 2u;
					else
						data[24] &= 0xFDu;
					if (v19)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					v16 = data[9] >> 2;
					data[9] <<= 6;
					data[9] |= v16;
					data[17] ^= 0xE6u;
					v17 = data[29];
					data[29] = data[7];
					data[7] = v17;
					v18 = (data[31] >> 4) & 1;
					if ((data[31] >> 4) & 1)
						data[31] |= 0x10u;
					else
						data[31] &= 0xEFu;
					if (v18)
						data[31] |= 0x10u;
					else
						data[31] &= 0xEFu;
				}
				else
				{
					v9 = data[13];
					data[13] = data[3];
					data[3] = v9;
					v21 = (data[11] >> 6) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 0x40u;
					else
						data[11] &= 0xBFu;
					if (v21)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v10 = data[13];
					data[13] = data[7];
					data[7] = v10;
					v11 = data[10];
					data[10] = data[8];
					data[8] = v11;
					data[6] ^= 0xDFu;
					data[5] ^= 0x7Eu;
					v20 = (data[9] >> 2) & 1;
					if ((data[9] >> 6) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v20)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					v12 = data[10];
					data[10] = data[8];
					data[8] = v12;
					v13 = data[7];
					data[7] = data[3];
					data[3] = v13;
				}
			}
			else
			{
				data[4] ^= 0x98u;
				v6 = data[1];
				data[1] = *data;
				*data = v6;
				v23 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v23)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				data[5] ^= 0x5Du;
				v7 = data[7] >> 6;
				data[7] *= 4;
				data[7] |= v7;
				data[5] ^= 0x83u;
				v22 = (data[5] >> 2) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				if (v22)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v8 = data[1];
				data[1] = data[1];
				data[1] = v8;
			}
		}
		else
		{
			v2 = (*data >> 6) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x40u;
			else
				*data &= 0xBFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v3 = *data;
			*data = data[3];
			data[3] = v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			v24 = (data[3] >> 3) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v24)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			data[2] ^= 0x63u;
		}
	}
}

void PacketTwister_38::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST21_1@9
	char v8; // ST20_1@11
	char v9; // ST1F_1@11
	char v10; // ST1E_1@11
	char v11; // ST1D_1@11
	char v12; // ST1C_1@11
	char v13; // ST1B_1@11
	char v14; // ST18_1@17
	char v15; // ST17_1@17
	char v16; // ST14_1@25
	char v17; // ST13_1@25
	char v18; // ST12_1@25
	char v19; // ST11_1@25
	char v20; // ST10_1@25
	char v21; // ST0B_1@38
	char v22; // ST0A_1@38
	char v23; // ST09_1@38
	char v24; // ST08_1@38
	char v25; // ST07_1@38
	char v26; // [sp+Ch] [bp-1Ch]@31
	char v27; // [sp+Eh] [bp-1Ah]@25
	char v28; // [sp+15h] [bp-13h]@19
	char v29; // [sp+19h] [bp-Fh]@11
	char v2; // [sp+22h] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = data[7] >> 1;
					data[7] <<= 7;
					data[7] |= v21;
					v22 = data[30] >> 6;
					data[30] *= 4;
					data[30] |= v22;
					v23 = data[18];
					data[18] = data[5];
					data[5] = v23;
					v24 = data[20];
					data[20] = data[13];
					data[13] = v24;
					v25 = data[4] >> 1;
					data[4] <<= 7;
					data[4] |= v25;
					data[18] ^= 0x91u;
				}
				else
				{
					v28 = (data[1] >> 2) & 1;
					if ((data[1] >> 5) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v28)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					v16 = data[5];
					data[5] = data[6];
					data[6] = v16;
					data[11] ^= 0xBCu;
					v17 = *data;
					*data = *data;
					*data = v17;
					v18 = *data;
					*data = data[6];
					data[6] = v18;
					v19 = *data;
					*data = data[12];
					data[12] = v19;
					v20 = data[15] >> 7;
					data[15] *= 2;
					data[15] |= v20;
					v27 = (data[15] >> 3) & 1;
					if ((data[15] >> 5) & 1)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					if (v27)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					v26 = (data[13] >> 3) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					if (v26)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
				}
			}
			else
			{
				v8 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v8;
				v9 = data[5];
				data[5] = *data;
				*data = v9;
				v10 = *data >> 5;
				*data *= 8;
				*data |= v10;
				v11 = data[3];
				data[3] = data[4];
				data[4] = v11;
				v12 = data[7];
				data[7] = data[1];
				data[1] = v12;
				v13 = *data;
				*data = data[1];
				data[1] = v13;
				data[3] ^= 0x47u;
				*data ^= 0xAFu;
				v29 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v29)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				data[1] ^= 0x18u;
				v14 = data[3];
				data[3] = data[6];
				data[6] = v14;
				v15 = data[5] >> 5;
				data[5] *= 8;
				data[5] |= v15;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[3];
			data[3] = v3;
			v4 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v4;
			v5 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v5;
			v6 = data[3];
			data[3] = data[2];
			data[2] = v6;
			data[1] ^= 0xD8u;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v7 = data[1];
			data[1] = data[2];
			data[2] = v7;
		}
	}
}

void PacketTwister_38::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST20_1@11
	char v9; // ST1F_1@11
	char v10; // ST1C_1@17
	char v11; // ST1B_1@17
	char v12; // ST1A_1@17
	char v13; // ST19_1@17
	char v14; // ST18_1@17
	char v15; // ST17_1@17
	char v16; // ST12_1@31
	char v17; // ST11_1@31
	char v18; // ST10_1@31
	char v19; // ST0F_1@31
	char v20; // ST0E_1@31
	char v21; // ST0B_1@38
	char v22; // ST0A_1@38
	char v23; // ST09_1@38
	char v24; // ST08_1@38
	char v25; // ST07_1@38
	char v26; // [sp+Ch] [bp-1Ch]@31
	char v27; // [sp+13h] [bp-15h]@25
	char v28; // [sp+15h] [bp-13h]@19
	char v29; // [sp+1Dh] [bp-Bh]@11
	char v2; // [sp+25h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[18] ^= 0x91u;
					v21 = data[4] >> 7;
					data[4] *= 2;
					data[4] |= v21;
					v22 = data[20];
					data[20] = data[13];
					data[13] = v22;
					v23 = data[18];
					data[18] = data[5];
					data[5] = v23;
					v24 = data[30] >> 2;
					data[30] <<= 6;
					data[30] |= v24;
					v25 = data[7] >> 7;
					data[7] *= 2;
					data[7] |= v25;
				}
				else
				{
					v28 = (data[13] >> 3) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					if (v28)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v27 = (data[15] >> 3) & 1;
					if ((data[15] >> 5) & 1)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					if (v27)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					v16 = data[15] >> 1;
					data[15] <<= 7;
					data[15] |= v16;
					v17 = *data;
					*data = data[12];
					data[12] = v17;
					v18 = *data;
					*data = data[6];
					data[6] = v18;
					v19 = *data;
					*data = *data;
					*data = v19;
					data[11] ^= 0xBCu;
					v20 = data[5];
					data[5] = data[6];
					data[6] = v20;
					v26 = (data[1] >> 2) & 1;
					if ((data[1] >> 5) & 1)
						data[1] |= 4u;
					else
						data[1] &= 0xFBu;
					if (v26)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
				}
			}
			else
			{
				v8 = data[5] >> 3;
				data[5] *= 32;
				data[5] |= v8;
				v9 = data[3];
				data[3] = data[6];
				data[6] = v9;
				data[1] ^= 0x18u;
				v29 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v29)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				*data ^= 0xAFu;
				data[3] ^= 0x47u;
				v10 = *data;
				*data = data[1];
				data[1] = v10;
				v11 = data[7];
				data[7] = data[1];
				data[1] = v11;
				v12 = data[3];
				data[3] = data[4];
				data[4] = v12;
				v13 = *data >> 3;
				*data *= 32;
				*data |= v13;
				v14 = data[5];
				data[5] = *data;
				*data = v14;
				v15 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v15;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			data[1] ^= 0xD8u;
			v4 = data[3];
			data[3] = data[2];
			data[2] = v4;
			v5 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v5;
			v6 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v6;
			v7 = data[3];
			data[3] = data[3];
			data[3] = v7;
		}
	}
}

void PacketTwister_39::Twist(BYTE *data, int length)
{
	char v3; // ST17_1@29
	char v4; // ST16_1@29
	char v5; // ST15_1@29
	char v6; // ST0E_1@49
	char v7; // ST0D_1@49
	char v8; // ST0C_1@49
	char v9; // ST09_1@55
	char v10; // ST08_1@56
	char v11; // ST07_1@56
	char v12; // ST06_1@56
	char v13; // ST05_1@56
	char v14; // ST04_1@56
	char v15; // [sp+Ah] [bp-16h]@49
	char v16; // [sp+Fh] [bp-11h]@43
	char v17; // [sp+11h] [bp-Fh]@37
	char v18; // [sp+13h] [bp-Dh]@29
	char v19; // [sp+18h] [bp-8h]@23
	char v20; // [sp+1Ah] [bp-6h]@17
	char v21; // [sp+1Ch] [bp-4h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v10 = data[27];
					data[27] = data[7];
					data[7] = v10;
					v11 = data[31] >> 6;
					data[31] *= 4;
					data[31] |= v11;
					v12 = *data >> 4;
					*data *= 16;
					*data |= v12;
					data[29] ^= 0x39u;
					data[13] ^= 0xCAu;
					v13 = data[9];
					data[9] = data[17];
					data[17] = v13;
					data[10] ^= 0xC2u;
					v14 = data[6];
					data[6] = data[11];
					data[11] = v14;
				}
				else
				{
					v17 = (data[2] >> 3) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					if (v17)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					v16 = (data[11] >> 2) & 1;
					if ((data[11] >> 7) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v16)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					data[12] ^= 0x2Au;
					v6 = data[6];
					data[6] = data[3];
					data[3] = v6;
					v7 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v7;
					v8 = data[9];
					data[9] = data[10];
					data[10] = v8;
					v15 = (data[10] >> 3) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
					if (v15)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v9 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v9;
					data[11] ^= 0xA0u;
				}
			}
			else
			{
				data[5] ^= 0x35u;
				v20 = (data[1] >> 1) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v20)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v19 = (data[3] >> 2) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v19)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v3 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v3;
				data[7] ^= 0x95u;
				v4 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v4;
				v5 = data[1];
				data[1] = data[6];
				data[6] = v5;
				v18 = (data[3] >> 4) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v18)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
			}
		}
		else
		{
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			data[3] ^= 0x7Fu;
			v21 = (data[2] >> 5) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v21)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
		}
	}
}

void PacketTwister_39::Correct(BYTE *data, int length)
{
	char v3; // ST19_1@23
	char v4; // ST18_1@23
	char v5; // ST17_1@23
	char v6; // ST12_1@37
	char v7; // ST0F_1@43
	char v8; // ST0E_1@43
	char v9; // ST0D_1@43
	char v10; // ST08_1@56
	char v11; // ST07_1@56
	char v12; // ST06_1@56
	char v13; // ST05_1@56
	char v14; // ST04_1@56
	char v15; // [sp+9h] [bp-17h]@49
	char v16; // [sp+Bh] [bp-15h]@43
	char v17; // [sp+10h] [bp-10h]@37
	char v18; // [sp+13h] [bp-Dh]@29
	char v19; // [sp+15h] [bp-Bh]@23
	char v20; // [sp+1Ah] [bp-6h]@17
	char v21; // [sp+1Ch] [bp-4h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v10 = data[6];
					data[6] = data[11];
					data[11] = v10;
					data[10] ^= 0xC2u;
					v11 = data[9];
					data[9] = data[17];
					data[17] = v11;
					data[13] ^= 0xCAu;
					data[29] ^= 0x39u;
					v12 = *data >> 4;
					*data *= 16;
					*data |= v12;
					v13 = data[31] >> 2;
					data[31] <<= 6;
					data[31] |= v13;
					v14 = data[27];
					data[27] = data[7];
					data[7] = v14;
				}
				else
				{
					data[11] ^= 0xA0u;
					v6 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v6;
					v17 = (data[10] >> 3) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 8u;
					else
						data[10] &= 0xF7u;
					if (v17)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v7 = data[9];
					data[9] = data[10];
					data[10] = v7;
					v8 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v8;
					v9 = data[6];
					data[6] = data[3];
					data[3] = v9;
					data[12] ^= 0x2Au;
					v16 = (data[11] >> 2) & 1;
					if ((data[11] >> 7) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v16)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					v15 = (data[2] >> 3) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					if (v15)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
				}
			}
			else
			{
				v20 = (data[3] >> 4) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v20)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v3 = data[1];
				data[1] = data[6];
				data[6] = v3;
				v4 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v4;
				data[7] ^= 0x95u;
				v5 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v5;
				v19 = (data[3] >> 2) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 4u;
				else
					data[3] &= 0xFBu;
				if (v19)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v18 = (data[1] >> 1) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v18)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				data[5] ^= 0x35u;
			}
		}
		else
		{
			v2 = (data[2] >> 5) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v2)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			data[3] ^= 0x7Fu;
			v21 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v21)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
		}
	}
}

void PacketTwister_40::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2A_1@15
	char v5; // ST27_1@23
	char v6; // ST24_1@29
	char v7; // ST23_1@29
	char v8; // ST22_1@29
	char v9; // ST21_1@29
	char v10; // ST20_1@29
	char v11; // ST1F_1@29
	char v12; // ST1C_1@37
	char v13; // ST15_1@55
	char v14; // ST0C_1@80
	char v15; // [sp+6h] [bp-2Ah]@92
	char v16; // [sp+8h] [bp-28h]@86
	char v17; // [sp+Ah] [bp-26h]@80
	char v18; // [sp+Dh] [bp-23h]@74
	char v19; // [sp+Fh] [bp-21h]@68
	char v20; // [sp+11h] [bp-1Fh]@61
	char v21; // [sp+13h] [bp-1Dh]@55
	char v22; // [sp+16h] [bp-1Ah]@49
	char v23; // [sp+18h] [bp-18h]@43
	char v24; // [sp+1Ah] [bp-16h]@37
	char v25; // [sp+1Dh] [bp-13h]@29
	char v26; // [sp+25h] [bp-Bh]@23
	char v27; // [sp+28h] [bp-8h]@17
	char v28; // [sp+2Bh] [bp-5h]@9
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v19 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v19)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v18 = (data[16] >> 2) & 1;
					if ((data[16] >> 7) & 1)
						data[16] |= 4u;
					else
						data[16] &= 0xFBu;
					if (v18)
						data[16] |= 0x80u;
					else
						data[16] &= 0x7Fu;
					v14 = data[19] >> 3;
					data[19] *= 32;
					data[19] |= v14;
					data[22] ^= 0xD4u;
					v17 = (data[11] >> 1) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v17)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					v16 = (data[7] >> 7) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 0x80u;
					else
						data[7] &= 0x7Fu;
					if (v16)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					v15 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v15)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
				}
				else
				{
					v12 = data[3] >> 3;
					data[3] *= 32;
					data[3] |= v12;
					*data ^= 0x62u;
					v24 = (*data >> 2) & 1;
					if ((*data >> 4) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v24)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					v23 = (data[5] >> 5) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v23)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					v22 = (data[9] >> 2) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v22)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v13 = data[7];
					data[7] = data[5];
					data[5] = v13;
					v21 = (data[4] >> 6) & 1;
					if ((data[4] >> 2) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v21)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					v20 = (data[12] >> 6) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v20)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
				}
			}
			else
			{
				data[4] ^= 0x3Bu;
				v27 = (data[5] >> 6) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
				if (v27)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				data[4] ^= 0x1Du;
				v5 = data[3];
				data[3] = data[5];
				data[5] = v5;
				v26 = (data[5] >> 5) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v26)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v6 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v6;
				v7 = data[6];
				data[6] = data[1];
				data[1] = v7;
				*data ^= 0x7Cu;
				v8 = data[5] >> 5;
				data[5] *= 8;
				data[5] |= v8;
				v9 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v9;
				v10 = data[1];
				data[1] = data[6];
				data[6] = v10;
				v11 = data[3];
				data[3] = data[1];
				data[1] = v11;
				v25 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v25)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
			}
		}
		else
		{
			v3 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v3;
			data[3] ^= 0xCAu;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v28 = (data[3] >> 5) & 1;
			if ((data[3] >> 5) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v28)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			v4 = *data >> 3;
			*data *= 32;
			*data |= v4;
			data[1] ^= 0x1Au;
			data[2] ^= 0x20u;
			data[3] ^= 0x3Cu;
		}
	}
}

void PacketTwister_40::Correct(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2A_1@15
	char v5; // ST27_1@23
	char v6; // ST26_1@23
	char v7; // ST25_1@23
	char v8; // ST24_1@23
	char v9; // ST23_1@23
	char v10; // ST22_1@23
	char v11; // ST1F_1@29
	char v12; // ST18_1@49
	char v13; // ST11_1@67
	char v14; // ST0A_1@86
	char v15; // [sp+6h] [bp-2Ah]@92
	char v16; // [sp+8h] [bp-28h]@86
	char v17; // [sp+Bh] [bp-25h]@80
	char v18; // [sp+Dh] [bp-23h]@74
	char v19; // [sp+Fh] [bp-21h]@68
	char v20; // [sp+12h] [bp-1Eh]@61
	char v21; // [sp+14h] [bp-1Ch]@55
	char v22; // [sp+16h] [bp-1Ah]@49
	char v23; // [sp+19h] [bp-17h]@43
	char v24; // [sp+1Bh] [bp-15h]@37
	char v25; // [sp+1Dh] [bp-13h]@29
	char v26; // [sp+20h] [bp-10h]@23
	char v27; // [sp+28h] [bp-8h]@17
	char v28; // [sp+2Bh] [bp-5h]@9
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v19 = (data[22] >> 6) & 1;
					if ((data[22] >> 4) & 1)
						data[22] |= 0x40u;
					else
						data[22] &= 0xBFu;
					if (v19)
						data[22] |= 0x10u;
					else
						data[22] &= 0xEFu;
					v18 = (data[7] >> 7) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 0x80u;
					else
						data[7] &= 0x7Fu;
					if (v18)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					v17 = (data[11] >> 1) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v17)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					data[22] ^= 0xD4u;
					v14 = data[19] >> 5;
					data[19] *= 8;
					data[19] |= v14;
					v16 = (data[16] >> 2) & 1;
					if ((data[16] >> 7) & 1)
						data[16] |= 4u;
					else
						data[16] &= 0xFBu;
					if (v16)
						data[16] |= 0x80u;
					else
						data[16] &= 0x7Fu;
					v15 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v15)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
				}
				else
				{
					v24 = (data[12] >> 6) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v24)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					v23 = (data[4] >> 6) & 1;
					if ((data[4] >> 2) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v23)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					v12 = data[7];
					data[7] = data[5];
					data[5] = v12;
					v22 = (data[9] >> 2) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 4u;
					else
						data[9] &= 0xFBu;
					if (v22)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v21 = (data[5] >> 5) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v21)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					v20 = (*data >> 2) & 1;
					if ((*data >> 4) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v20)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					*data ^= 0x62u;
					v13 = data[3] >> 5;
					data[3] *= 8;
					data[3] |= v13;
				}
			}
			else
			{
				v27 = (data[6] >> 2) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v27)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				v5 = data[3];
				data[3] = data[1];
				data[1] = v5;
				v6 = data[1];
				data[1] = data[6];
				data[6] = v6;
				v7 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v7;
				v8 = data[5] >> 3;
				data[5] *= 32;
				data[5] |= v8;
				*data ^= 0x7Cu;
				v9 = data[6];
				data[6] = data[1];
				data[1] = v9;
				v10 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v10;
				v26 = (data[5] >> 5) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v26)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v11 = data[3];
				data[3] = data[5];
				data[5] = v11;
				data[4] ^= 0x1Du;
				v25 = (data[5] >> 6) & 1;
				if ((data[5] >> 5) & 1)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
				if (v25)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				data[4] ^= 0x3Bu;
			}
		}
		else
		{
			data[3] ^= 0x3Cu;
			data[2] ^= 0x20u;
			data[1] ^= 0x1Au;
			v3 = *data >> 5;
			*data *= 8;
			*data |= v3;
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 5) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			v28 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v28)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			data[3] ^= 0xCAu;
			v4 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v4;
		}
	}
}

void PacketTwister_41::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1C_1@11
	char v5; // ST1B_1@11
	char v6; // ST18_1@17
	char v7; // ST17_1@17
	char v8; // ST14_1@23
	char v9; // ST0F_1@37
	char v10; // ST0A_1@49
	char v11; // ST09_1@50
	char v12; // ST08_1@50
	char v13; // ST05_1@56
	char v14; // [sp+6h] [bp-1Ah]@50
	char v15; // [sp+Bh] [bp-15h]@43
	char v16; // [sp+Dh] [bp-13h]@37
	char v17; // [sp+10h] [bp-10h]@31
	char v18; // [sp+12h] [bp-Eh]@25
	char v19; // [sp+15h] [bp-Bh]@17
	char v20; // [sp+19h] [bp-7h]@11
	char v2; // [sp+1Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[10] ^= 0x1Bu;
					v11 = data[14];
					data[14] = data[22];
					data[22] = v11;
					v12 = data[14] >> 1;
					data[14] <<= 7;
					data[14] |= v12;
					v14 = (data[19] >> 7) & 1;
					if ((data[19] >> 5) & 1)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					if (v14)
						data[19] |= 0x20u;
					else
						data[19] &= 0xDFu;
					v13 = data[27];
					data[27] = data[16];
					data[16] = v13;
				}
				else
				{
					v18 = (data[15] >> 5) & 1;
					if ((data[15] >> 7) & 1)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					if (v18)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					data[5] ^= 0x2Cu;
					v17 = (data[7] >> 2) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 4u;
					else
						data[7] &= 0xFBu;
					if (v17)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					v9 = data[3];
					data[3] = data[12];
					data[12] = v9;
					v16 = (data[13] >> 7) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v16)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v15 = (data[3] >> 7) & 1;
					if ((data[3] >> 4) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v15)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					v10 = *data >> 5;
					*data *= 8;
					*data |= v10;
				}
			}
			else
			{
				v4 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v4;
				v5 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v5;
				v20 = (data[1] >> 2) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v20)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v6 = *data;
				*data = *data;
				*data = v6;
				v7 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v7;
				data[7] ^= 0x9Bu;
				data[1] ^= 0x68u;
				v19 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v19)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v8 = data[2] >> 5;
				data[2] *= 8;
				data[2] |= v8;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			data[1] ^= 0xEDu;
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
		}
	}
}

void PacketTwister_41::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@11
	char v5; // ST19_1@17
	char v6; // ST18_1@17
	char v7; // ST15_1@23
	char v8; // ST14_1@23
	char v9; // ST13_1@25
	char v10; // ST0E_1@37
	char v11; // ST09_1@50
	char v12; // ST06_1@56
	char v13; // ST05_1@56
	char v14; // [sp+7h] [bp-19h]@50
	char v15; // [sp+Ah] [bp-16h]@43
	char v16; // [sp+Ch] [bp-14h]@37
	char v17; // [sp+Fh] [bp-11h]@31
	char v18; // [sp+11h] [bp-Fh]@25
	char v19; // [sp+16h] [bp-Ah]@17
	char v20; // [sp+1Ah] [bp-6h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v11 = data[27];
					data[27] = data[16];
					data[16] = v11;
					v14 = (data[19] >> 7) & 1;
					if ((data[19] >> 5) & 1)
						data[19] |= 0x80u;
					else
						data[19] &= 0x7Fu;
					if (v14)
						data[19] |= 0x20u;
					else
						data[19] &= 0xDFu;
					v12 = data[14] >> 7;
					data[14] *= 2;
					data[14] |= v12;
					v13 = data[14];
					data[14] = data[22];
					data[22] = v13;
					data[10] ^= 0x1Bu;
				}
				else
				{
					v9 = *data >> 3;
					*data *= 32;
					*data |= v9;
					v18 = (data[3] >> 7) & 1;
					if ((data[3] >> 4) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v18)
						data[3] |= 0x10u;
					else
						data[3] &= 0xEFu;
					v17 = (data[13] >> 7) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v17)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v10 = data[3];
					data[3] = data[12];
					data[12] = v10;
					v16 = (data[7] >> 2) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 4u;
					else
						data[7] &= 0xFBu;
					if (v16)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					data[5] ^= 0x2Cu;
					v15 = (data[15] >> 5) & 1;
					if ((data[15] >> 7) & 1)
						data[15] |= 0x20u;
					else
						data[15] &= 0xDFu;
					if (v15)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
				}
			}
			else
			{
				v4 = data[2] >> 3;
				data[2] *= 32;
				data[2] |= v4;
				v20 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v20)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				data[1] ^= 0x68u;
				data[7] ^= 0x9Bu;
				v5 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v5;
				v6 = *data;
				*data = *data;
				*data = v6;
				v19 = (data[1] >> 2) & 1;
				if ((data[1] >> 5) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v19)
					data[1] |= 0x20u;
				else
					data[1] &= 0xDFu;
				v7 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v7;
				v8 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v8;
			}
		}
		else
		{
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			data[1] ^= 0xEDu;
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
		}
	}
}

void PacketTwister_42::Twist(BYTE *data, int length)
{
	char v3; // ST0F_1@3
	char v4; // ST0E_1@5
	char v5; // ST0D_1@5
	char v6; // ST0C_1@5
	char v7; // ST07_1@19
	char v8; // ST06_1@20
	char v9; // ST05_1@20
	char v10; // [sp+8h] [bp-8h]@13
	char v2; // [sp+Ah] [bp-6h]@7

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v8 = data[8] >> 7;
					data[8] *= 2;
					data[8] |= v8;
					v9 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v9;
					data[28] ^= 0xAEu;
				}
				else
				{
					v2 = (data[14] >> 2) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v2)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					data[4] ^= 0x40u;
					v10 = (data[3] >> 2) & 1;
					if ((data[3] >> 3) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v10)
						data[3] |= 8u;
					else
						data[3] &= 0xF7u;
					*data ^= 0xC7u;
					v7 = data[4] >> 4;
					data[4] *= 16;
					data[4] |= v7;
				}
			}
			else
			{
				v4 = data[4];
				data[4] = data[1];
				data[1] = v4;
				data[1] ^= 0x64u;
				v5 = data[4];
				data[4] = data[1];
				data[1] = v5;
				v6 = data[1] >> 5;
				data[1] *= 8;
				data[1] |= v6;
			}
		}
		else
		{
			data[1] ^= 0x3Cu;
			data[2] ^= 0x96u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
		}
	}
}

void PacketTwister_42::Correct(BYTE *data, int length)
{
	char v3; // ST0F_1@3
	char v4; // ST0E_1@5
	char v5; // ST0D_1@5
	char v6; // ST0C_1@5
	char v7; // ST0B_1@7
	char v8; // ST06_1@20
	char v9; // ST05_1@20
	char v10; // [sp+7h] [bp-9h]@13
	char v2; // [sp+9h] [bp-7h]@7

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[28] ^= 0xAEu;
					v8 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v8;
					v9 = data[8] >> 1;
					data[8] <<= 7;
					data[8] |= v9;
				}
				else
				{
					v7 = data[4] >> 4;
					data[4] *= 16;
					data[4] |= v7;
					*data ^= 0xC7u;
					v2 = (data[3] >> 2) & 1;
					if ((data[3] >> 3) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v2)
						data[3] |= 8u;
					else
						data[3] &= 0xF7u;
					data[4] ^= 0x40u;
					v10 = (data[14] >> 2) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v10)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
				}
			}
			else
			{
				v4 = data[1] >> 3;
				data[1] *= 32;
				data[1] |= v4;
				v5 = data[4];
				data[4] = data[1];
				data[1] = v5;
				data[1] ^= 0x64u;
				v6 = data[4];
				data[4] = data[1];
				data[1] = v6;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			data[2] ^= 0x96u;
			data[1] ^= 0x3Cu;
		}
	}
}

void PacketTwister_43::Twist(BYTE *data, int length)
{
	char v3; // ST19_1@9
	char v4; // ST18_1@9
	char v5; // ST15_1@15
	char v6; // ST14_1@17
	char v7; // ST13_1@17
	char v8; // ST10_1@25
	char v9; // ST0F_1@25
	char v10; // ST0E_1@25
	char v11; // ST0D_1@25
	char v12; // ST0C_1@25
	char v13; // ST0B_1@26
	char v14; // ST0A_1@26
	char v15; // ST09_1@26
	char v16; // ST08_1@26
	char v17; // ST07_1@26
	char v18; // [sp+11h] [bp-Bh]@19
	char v19; // [sp+16h] [bp-6h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[13];
					data[13] = data[11];
					data[11] = v13;
					v14 = data[18];
					data[18] = data[11];
					data[11] = v14;
					v15 = data[16] >> 5;
					data[16] *= 8;
					data[16] |= v15;
					data[28] ^= 0x11u;
					v16 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v16;
					data[5] ^= 0xE7u;
					data[4] ^= 0x55u;
					v17 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v17;
				}
				else
				{
					v18 = (*data >> 3) & 1;
					if ((*data >> 3) & 1)
						*data |= 8u;
					else
						*data &= 0xF7u;
					if (v18)
						*data |= 8u;
					else
						*data &= 0xF7u;
					v8 = data[5] >> 1;
					data[5] <<= 7;
					data[5] |= v8;
					data[1] ^= 0x4Bu;
					data[2] ^= 0x81u;
					v9 = data[4];
					data[4] = data[7];
					data[7] = v9;
					v10 = data[4] >> 7;
					data[4] *= 2;
					data[4] |= v10;
					v11 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v11;
					v12 = data[13] >> 3;
					data[13] *= 32;
					data[13] |= v12;
					data[11] ^= 0xACu;
				}
			}
			else
			{
				v6 = data[4] >> 5;
				data[4] *= 8;
				data[4] |= v6;
				v7 = *data;
				*data = data[6];
				data[6] = v7;
				*data ^= 0xEAu;
				data[6] ^= 2u;
			}
		}
		else
		{
			v2 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			data[1] ^= 0xEBu;
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			v4 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v4;
			v19 = (data[3] >> 6) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			if (v19)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			data[3] ^= 0xF6u;
			v5 = *data >> 4;
			*data *= 16;
			*data |= v5;
		}
	}
}

void PacketTwister_43::Correct(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST18_1@9
	char v5; // ST17_1@9
	char v6; // ST14_1@17
	char v7; // ST13_1@17
	char v8; // ST12_1@19
	char v9; // ST11_1@19
	char v10; // ST10_1@19
	char v11; // ST0F_1@19
	char v12; // ST0E_1@19
	char v13; // ST0B_1@26
	char v14; // ST0A_1@26
	char v15; // ST09_1@26
	char v16; // ST08_1@26
	char v17; // ST07_1@26
	char v18; // [sp+Ch] [bp-10h]@19
	char v19; // [sp+15h] [bp-7h]@9
	char v2; // [sp+19h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v13;
					data[4] ^= 0x55u;
					data[5] ^= 0xE7u;
					v14 = data[6] >> 4;
					data[6] *= 16;
					data[6] |= v14;
					data[28] ^= 0x11u;
					v15 = data[16] >> 3;
					data[16] *= 32;
					data[16] |= v15;
					v16 = data[18];
					data[18] = data[11];
					data[11] = v16;
					v17 = data[13];
					data[13] = data[11];
					data[11] = v17;
				}
				else
				{
					data[11] ^= 0xACu;
					v8 = data[13] >> 5;
					data[13] *= 8;
					data[13] |= v8;
					v9 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v9;
					v10 = data[4] >> 1;
					data[4] <<= 7;
					data[4] |= v10;
					v11 = data[4];
					data[4] = data[7];
					data[7] = v11;
					data[2] ^= 0x81u;
					data[1] ^= 0x4Bu;
					v12 = data[5] >> 7;
					data[5] *= 2;
					data[5] |= v12;
					v18 = (*data >> 3) & 1;
					if ((*data >> 3) & 1)
						*data |= 8u;
					else
						*data &= 0xF7u;
					if (v18)
						*data |= 8u;
					else
						*data &= 0xF7u;
				}
			}
			else
			{
				data[6] ^= 2u;
				*data ^= 0xEAu;
				v6 = *data;
				*data = data[6];
				data[6] = v6;
				v7 = data[4] >> 3;
				data[4] *= 32;
				data[4] |= v7;
			}
		}
		else
		{
			v3 = *data >> 4;
			*data *= 16;
			*data |= v3;
			data[3] ^= 0xF6u;
			v2 = (data[3] >> 6) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v4 = data[3] >> 7;
			data[3] *= 2;
			data[3] |= v4;
			v5 = data[3];
			data[3] = data[1];
			data[1] = v5;
			data[1] ^= 0xEBu;
			v19 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v19)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
		}
	}
}

void PacketTwister_44::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2C_1@9
	char v5; // ST29_1@15
	char v6; // ST24_1@29
	char v7; // ST1B_1@53
	char v8; // ST1A_1@53
	char v9; // ST17_1@61
	char v10; // ST16_1@61
	char v11; // ST15_1@61
	char v12; // ST14_1@61
	char v13; // ST13_1@61
	char v14; // ST12_1@61
	char v15; // ST11_1@61
	char v16; // ST10_1@62
	char v17; // ST0F_1@62
	char v18; // [sp+7h] [bp-29h]@80
	char v19; // [sp+9h] [bp-27h]@74
	char v20; // [sp+Bh] [bp-25h]@68
	char v21; // [sp+Dh] [bp-23h]@62
	char v22; // [sp+18h] [bp-18h]@53
	char v23; // [sp+1Ch] [bp-14h]@47
	char v24; // [sp+1Eh] [bp-12h]@41
	char v25; // [sp+20h] [bp-10h]@35
	char v26; // [sp+22h] [bp-Eh]@29
	char v27; // [sp+25h] [bp-Bh]@21
	char v28; // [sp+27h] [bp-9h]@15
	char v29; // [sp+2Ah] [bp-6h]@9
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[24] ^= 0x16u;
					data[27] ^= 0x19u;
					v16 = data[11] >> 4;
					data[11] *= 16;
					data[11] |= v16;
					data[24] ^= 0x4Fu;
					v17 = *data >> 2;
					*data <<= 6;
					*data |= v17;
					v21 = (data[25] >> 4) & 1;
					if ((data[25] >> 5) & 1)
						data[25] |= 0x10u;
					else
						data[25] &= 0xEFu;
					if (v21)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					v20 = (data[30] >> 1) & 1;
					if ((data[30] >> 4) & 1)
						data[30] |= 2u;
					else
						data[30] &= 0xFDu;
					if (v20)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					data[3] ^= 0x5Cu;
					v19 = (data[22] >> 2) & 1;
					if ((data[22] >> 5) & 1)
						data[22] |= 4u;
					else
						data[22] &= 0xFBu;
					if (v19)
						data[22] |= 0x20u;
					else
						data[22] &= 0xDFu;
					v18 = (data[23] >> 7) & 1;
					if ((data[23] >> 6) & 1)
						data[23] |= 0x80u;
					else
						data[23] &= 0x7Fu;
					if (v18)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					data[13] ^= 0x8Cu;
				}
				else
				{
					v9 = data[7];
					data[7] = data[13];
					data[13] = v9;
					v10 = data[14] >> 6;
					data[14] *= 4;
					data[14] |= v10;
					v11 = data[4];
					data[4] = data[9];
					data[9] = v11;
					*data ^= 0x9Bu;
					data[11] ^= 0xCAu;
					data[6] ^= 0x8Fu;
					v12 = data[2] >> 7;
					data[2] *= 2;
					data[2] |= v12;
					v13 = data[11];
					data[11] = data[6];
					data[6] = v13;
					v14 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v14;
					v15 = data[11] >> 5;
					data[11] *= 8;
					data[11] |= v15;
				}
			}
			else
			{
				v6 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v6;
				data[1] ^= 0x9Du;
				v26 = (*data >> 6) & 1;
				if ((*data >> 1) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v26)
					*data |= 2u;
				else
					*data &= 0xFDu;
				v25 = (data[1] >> 7) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v25)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v24 = (data[1] >> 6) & 1;
				if ((data[1] >> 2) & 1)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				if (v24)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				v23 = (data[5] >> 4) & 1;
				if ((data[5] >> 4) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v23)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				v7 = data[2] >> 4;
				data[2] *= 16;
				data[2] |= v7;
				data[7] ^= 0xDu;
				v8 = data[2] >> 5;
				data[2] *= 8;
				data[2] |= v8;
				v22 = (data[5] >> 3) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				if (v22)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
			}
		}
		else
		{
			v3 = *data >> 6;
			*data *= 4;
			*data |= v3;
			*data ^= 0xC5u;
			data[3] ^= 0xFAu;
			v2 = (data[2] >> 7) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v2)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v29 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v29)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			data[3] ^= 0xF7u;
			v5 = data[1] >> 3;
			data[1] *= 32;
			data[1] |= v5;
			v28 = (*data >> 3) & 1;
			if ((*data >> 3) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v28)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v27 = (data[3] >> 3) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v27)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
		}
	}
}

void PacketTwister_44::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@15
	char v4; // ST28_1@21
	char v5; // ST25_1@27
	char v6; // ST22_1@35
	char v7; // ST21_1@35
	char v8; // ST18_1@59
	char v9; // ST17_1@61
	char v10; // ST16_1@61
	char v11; // ST15_1@61
	char v12; // ST14_1@61
	char v13; // ST13_1@61
	char v14; // ST12_1@61
	char v15; // ST11_1@61
	char v16; // ST08_1@86
	char v17; // ST07_1@86
	char v18; // [sp+9h] [bp-27h]@80
	char v19; // [sp+Bh] [bp-25h]@74
	char v20; // [sp+Dh] [bp-23h]@68
	char v21; // [sp+Fh] [bp-21h]@62
	char v22; // [sp+19h] [bp-17h]@53
	char v23; // [sp+1Bh] [bp-15h]@47
	char v24; // [sp+1Dh] [bp-13h]@41
	char v25; // [sp+1Fh] [bp-11h]@35
	char v26; // [sp+23h] [bp-Dh]@29
	char v27; // [sp+26h] [bp-Ah]@21
	char v28; // [sp+29h] [bp-7h]@15
	char v29; // [sp+2Ch] [bp-4h]@9
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[13] ^= 0x8Cu;
					v21 = (data[23] >> 7) & 1;
					if ((data[23] >> 6) & 1)
						data[23] |= 0x80u;
					else
						data[23] &= 0x7Fu;
					if (v21)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					v20 = (data[22] >> 2) & 1;
					if ((data[22] >> 5) & 1)
						data[22] |= 4u;
					else
						data[22] &= 0xFBu;
					if (v20)
						data[22] |= 0x20u;
					else
						data[22] &= 0xDFu;
					data[3] ^= 0x5Cu;
					v19 = (data[30] >> 1) & 1;
					if ((data[30] >> 4) & 1)
						data[30] |= 2u;
					else
						data[30] &= 0xFDu;
					if (v19)
						data[30] |= 0x10u;
					else
						data[30] &= 0xEFu;
					v18 = (data[25] >> 4) & 1;
					if ((data[25] >> 5) & 1)
						data[25] |= 0x10u;
					else
						data[25] &= 0xEFu;
					if (v18)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					v16 = *data >> 6;
					*data *= 4;
					*data |= v16;
					data[24] ^= 0x4Fu;
					v17 = data[11] >> 4;
					data[11] *= 16;
					data[11] |= v17;
					data[27] ^= 0x19u;
					data[24] ^= 0x16u;
				}
				else
				{
					v9 = data[11] >> 3;
					data[11] *= 32;
					data[11] |= v9;
					v10 = data[13] >> 4;
					data[13] *= 16;
					data[13] |= v10;
					v11 = data[11];
					data[11] = data[6];
					data[6] = v11;
					v12 = data[2] >> 1;
					data[2] <<= 7;
					data[2] |= v12;
					data[6] ^= 0x8Fu;
					data[11] ^= 0xCAu;
					*data ^= 0x9Bu;
					v13 = data[4];
					data[4] = data[9];
					data[9] = v13;
					v14 = data[14] >> 2;
					data[14] <<= 6;
					data[14] |= v14;
					v15 = data[7];
					data[7] = data[13];
					data[13] = v15;
				}
			}
			else
			{
				v26 = (data[5] >> 3) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				if (v26)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v6 = data[2] >> 3;
				data[2] *= 32;
				data[2] |= v6;
				data[7] ^= 0xDu;
				v7 = data[2] >> 4;
				data[2] *= 16;
				data[2] |= v7;
				v25 = (data[5] >> 4) & 1;
				if ((data[5] >> 4) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v25)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				v24 = (data[1] >> 6) & 1;
				if ((data[1] >> 2) & 1)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				if (v24)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				v23 = (data[1] >> 7) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				if (v23)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v22 = (*data >> 6) & 1;
				if ((*data >> 1) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v22)
					*data |= 2u;
				else
					*data &= 0xFDu;
				data[1] ^= 0x9Du;
				v8 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v8;
			}
		}
		else
		{
			v2 = (data[3] >> 3) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v29 = (*data >> 3) & 1;
			if ((*data >> 3) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v29)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v3 = data[1] >> 5;
			data[1] *= 8;
			data[1] |= v3;
			data[3] ^= 0xF7u;
			v28 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v28)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v4 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v4;
			v27 = (data[2] >> 7) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v27)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			data[3] ^= 0xFAu;
			*data ^= 0xC5u;
			v5 = *data >> 2;
			*data <<= 6;
			*data |= v5;
		}
	}
}

void PacketTwister_45::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST23_1@3
	char v8; // ST20_1@9
	char v9; // ST1F_1@9
	char v10; // ST1C_1@17
	char v11; // ST1B_1@17
	char v12; // ST1A_1@17
	char v13; // ST17_1@23
	char v14; // ST16_1@25
	char v15; // ST15_1@25
	char v16; // ST14_1@25
	char v17; // ST11_1@31
	char v18; // ST10_1@31
	char v19; // ST0D_1@38
	char v20; // ST0C_1@38
	char v21; // ST0B_1@38
	char v22; // ST0A_1@38
	char v23; // ST09_1@38
	char v24; // ST08_1@38
	char v25; // ST07_1@38
	char v26; // [sp+5h] [bp-23h]@38
	char v27; // [sp+Eh] [bp-1Ah]@31
	char v28; // [sp+12h] [bp-16h]@25
	char v29; // [sp+18h] [bp-10h]@17
	char v30; // [sp+1Dh] [bp-Bh]@11
	char v2; // [sp+21h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v19 = data[27];
					data[27] = data[12];
					data[12] = v19;
					data[5] ^= 0x75u;
					data[9] ^= 0xEBu;
					v20 = data[26] >> 4;
					data[26] *= 16;
					data[26] |= v20;
					v21 = data[17] >> 3;
					data[17] *= 32;
					data[17] |= v21;
					v22 = data[27] >> 5;
					data[27] *= 8;
					data[27] |= v22;
					data[3] ^= 0xEu;
					v23 = data[15] >> 3;
					data[15] *= 32;
					data[15] |= v23;
					v24 = data[3] >> 3;
					data[3] *= 32;
					data[3] |= v24;
					v25 = data[30];
					data[30] = data[7];
					data[7] = v25;
					v26 = (data[5] >> 5) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v26)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
				}
				else
				{
					data[12] ^= 0x31u;
					v14 = data[2];
					data[2] = data[10];
					data[10] = v14;
					v15 = data[15] >> 7;
					data[15] *= 2;
					data[15] |= v15;
					v16 = data[15];
					data[15] = data[5];
					data[5] = v16;
					v28 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v28)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v17 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v17;
					v18 = data[3];
					data[3] = *data;
					*data = v18;
					v27 = (data[14] >> 4) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v27)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
				}
			}
			else
			{
				v30 = (data[4] >> 6) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				if (v30)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				v10 = data[2];
				data[2] = data[3];
				data[3] = v10;
				v11 = *data;
				*data = data[1];
				data[1] = v11;
				v12 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v12;
				data[7] ^= 0x42u;
				data[1] ^= 0xEFu;
				data[6] ^= 0x54u;
				v29 = (data[5] >> 4) & 1;
				if ((data[5] >> 4) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v29)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				v13 = data[1];
				data[1] = data[3];
				data[3] = v13;
			}
		}
		else
		{
			v3 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v3;
			v4 = data[2];
			data[2] = data[2];
			data[2] = v4;
			data[3] ^= 8u;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
			v6 = data[3];
			data[3] = data[2];
			data[2] = v6;
			v7 = data[2];
			data[2] = data[3];
			data[3] = v7;
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 2) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			v8 = *data >> 6;
			*data *= 4;
			*data |= v8;
			v9 = data[1];
			data[1] = data[3];
			data[3] = v9;
		}
	}
}

void PacketTwister_45::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST20_1@9
	char v9; // ST1F_1@9
	char v10; // ST1E_1@11
	char v11; // ST1B_1@17
	char v12; // ST1A_1@17
	char v13; // ST19_1@17
	char v14; // ST14_1@31
	char v15; // ST13_1@31
	char v16; // ST10_1@37
	char v17; // ST0F_1@37
	char v18; // ST0E_1@37
	char v19; // ST0B_1@44
	char v20; // ST0A_1@44
	char v21; // ST09_1@44
	char v22; // ST08_1@44
	char v23; // ST07_1@44
	char v24; // ST06_1@44
	char v25; // ST05_1@44
	char v26; // [sp+Ch] [bp-1Ch]@38
	char v27; // [sp+11h] [bp-17h]@31
	char v28; // [sp+15h] [bp-13h]@25
	char v29; // [sp+17h] [bp-11h]@17
	char v30; // [sp+1Ch] [bp-Ch]@11
	char v2; // [sp+24h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v26 = (data[5] >> 5) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v26)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					v19 = data[30];
					data[30] = data[7];
					data[7] = v19;
					v20 = data[3] >> 5;
					data[3] *= 8;
					data[3] |= v20;
					v21 = data[15] >> 5;
					data[15] *= 8;
					data[15] |= v21;
					data[3] ^= 0xEu;
					v22 = data[27] >> 3;
					data[27] *= 32;
					data[27] |= v22;
					v23 = data[17] >> 5;
					data[17] *= 8;
					data[17] |= v23;
					v24 = data[26] >> 4;
					data[26] *= 16;
					data[26] |= v24;
					data[9] ^= 0xEBu;
					data[5] ^= 0x75u;
					v25 = data[27];
					data[27] = data[12];
					data[12] = v25;
				}
				else
				{
					v28 = (data[14] >> 4) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v28)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					v14 = data[3];
					data[3] = *data;
					*data = v14;
					v15 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v15;
					v27 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v27)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v16 = data[15];
					data[15] = data[5];
					data[5] = v16;
					v17 = data[15] >> 1;
					data[15] <<= 7;
					data[15] |= v17;
					v18 = data[2];
					data[2] = data[10];
					data[10] = v18;
					data[12] ^= 0x31u;
				}
			}
			else
			{
				v10 = data[1];
				data[1] = data[3];
				data[3] = v10;
				v30 = (data[5] >> 4) & 1;
				if ((data[5] >> 4) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v30)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				data[6] ^= 0x54u;
				data[1] ^= 0xEFu;
				data[7] ^= 0x42u;
				v11 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v11;
				v12 = *data;
				*data = data[1];
				data[1] = v12;
				v13 = data[2];
				data[2] = data[3];
				data[3] = v13;
				v29 = (data[4] >> 6) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				if (v29)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[3];
			data[3] = v3;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			v2 = (data[2] >> 1) & 1;
			if ((data[2] >> 2) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v2)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			v5 = data[2];
			data[2] = data[3];
			data[3] = v5;
			v6 = data[3];
			data[3] = data[2];
			data[2] = v6;
			v7 = data[1];
			data[1] = *data;
			*data = v7;
			data[3] ^= 8u;
			v8 = data[2];
			data[2] = data[2];
			data[2] = v8;
			v9 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v9;
		}
	}
}

void PacketTwister_46::Twist(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST20_1@9
	char v9; // ST1D_1@15
	char v10; // ST18_1@29
	char v11; // ST15_1@37
	char v12; // ST14_1@37
	char v13; // ST13_1@37
	char v14; // ST12_1@37
	char v15; // ST11_1@37
	char v16; // ST10_1@37
	char v17; // ST0F_1@37
	char v18; // ST0E_1@37
	char v19; // ST0D_1@37
	char v20; // ST0C_1@38
	char v21; // ST0B_1@38
	char v22; // ST08_1@44
	char v23; // ST05_1@50
	char v24; // ST04_1@50
	char v25; // [sp+6h] [bp-22h]@44
	char v26; // [sp+9h] [bp-1Fh]@38
	char v27; // [sp+16h] [bp-12h]@29
	char v28; // [sp+19h] [bp-Fh]@23
	char v29; // [sp+1Bh] [bp-Dh]@17
	char v30; // [sp+1Eh] [bp-Ah]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[8] ^= 0x15u;
					v20 = data[1];
					data[1] = data[17];
					data[17] = v20;
					data[2] ^= 0x27u;
					v21 = data[3];
					data[3] = data[20];
					data[20] = v21;
					v26 = (data[30] >> 3) & 1;
					if ((data[30] >> 7) & 1)
						data[30] |= 8u;
					else
						data[30] &= 0xF7u;
					if (v26)
						data[30] |= 0x80u;
					else
						data[30] &= 0x7Fu;
					v22 = data[2] >> 2;
					data[2] <<= 6;
					data[2] |= v22;
					v25 = (data[2] >> 2) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v25)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v23 = data[16];
					data[16] = data[16];
					data[16] = v23;
					data[31] ^= 0x97u;
					v24 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v24;
				}
				else
				{
					v11 = data[14];
					data[14] = data[2];
					data[2] = v11;
					v12 = data[5] >> 6;
					data[5] *= 4;
					data[5] |= v12;
					v13 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v13;
					data[10] ^= 0x94u;
					v14 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v14;
					v15 = *data;
					*data = data[5];
					data[5] = v15;
					v16 = data[1];
					data[1] = data[15];
					data[15] = v16;
					v17 = data[15];
					data[15] = data[15];
					data[15] = v17;
					v18 = data[13];
					data[13] = data[1];
					data[1] = v18;
					v19 = data[5];
					data[5] = data[12];
					data[12] = v19;
				}
			}
			else
			{
				data[4] ^= 0xAFu;
				data[2] ^= 0x77u;
				*data ^= 0xA7u;
				v29 = (data[3] >> 3) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				if (v29)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				v28 = (data[1] >> 2) & 1;
				if ((data[1] >> 2) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v28)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				v10 = data[7] >> 6;
				data[7] *= 4;
				data[7] |= v10;
				v27 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v27)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				data[7] ^= 0xE1u;
				data[2] ^= 5u;
				data[2] ^= 0x73u;
			}
		}
		else
		{
			v2 = (data[3] >> 1) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v3 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v3;
			data[2] ^= 0xEDu;
			data[3] ^= 0x52u;
			v4 = *data >> 7;
			*data *= 2;
			*data |= v4;
			v5 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = data[3];
			data[3] = data[3];
			data[3] = v7;
			v8 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v8;
			data[1] ^= 0x67u;
			v30 = (data[1] >> 2) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v30)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			v9 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v9;
		}
	}
}

void PacketTwister_46::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST20_1@9
	char v9; // ST1F_1@9
	char v10; // ST1A_1@23
	char v11; // ST15_1@37
	char v12; // ST14_1@37
	char v13; // ST13_1@37
	char v14; // ST12_1@37
	char v15; // ST11_1@37
	char v16; // ST10_1@37
	char v17; // ST0F_1@37
	char v18; // ST0E_1@37
	char v19; // ST0D_1@37
	char v20; // ST0C_1@38
	char v21; // ST0B_1@38
	char v22; // ST08_1@44
	char v23; // ST05_1@50
	char v24; // ST04_1@50
	char v25; // [sp+6h] [bp-22h]@44
	char v26; // [sp+9h] [bp-1Fh]@38
	char v27; // [sp+16h] [bp-12h]@29
	char v28; // [sp+18h] [bp-10h]@23
	char v29; // [sp+1Bh] [bp-Dh]@17
	char v30; // [sp+1Dh] [bp-Bh]@9
	char v2; // [sp+25h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v20;
					data[31] ^= 0x97u;
					v21 = data[16];
					data[16] = data[16];
					data[16] = v21;
					v26 = (data[2] >> 2) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v26)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v22 = data[2] >> 6;
					data[2] *= 4;
					data[2] |= v22;
					v25 = (data[30] >> 3) & 1;
					if ((data[30] >> 7) & 1)
						data[30] |= 8u;
					else
						data[30] &= 0xF7u;
					if (v25)
						data[30] |= 0x80u;
					else
						data[30] &= 0x7Fu;
					v23 = data[3];
					data[3] = data[20];
					data[20] = v23;
					data[2] ^= 0x27u;
					v24 = data[1];
					data[1] = data[17];
					data[17] = v24;
					data[8] ^= 0x15u;
				}
				else
				{
					v11 = data[5];
					data[5] = data[12];
					data[12] = v11;
					v12 = data[13];
					data[13] = data[1];
					data[1] = v12;
					v13 = data[15];
					data[15] = data[15];
					data[15] = v13;
					v14 = data[1];
					data[1] = data[15];
					data[15] = v14;
					v15 = *data;
					*data = data[5];
					data[5] = v15;
					v16 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v16;
					data[10] ^= 0x94u;
					v17 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v17;
					v18 = data[5] >> 2;
					data[5] <<= 6;
					data[5] |= v18;
					v19 = data[14];
					data[14] = data[2];
					data[2] = v19;
				}
			}
			else
			{
				data[2] ^= 0x73u;
				data[2] ^= 5u;
				data[7] ^= 0xE1u;
				v29 = (data[2] >> 2) & 1;
				if ((data[2] >> 5) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v29)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				v10 = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= v10;
				v28 = (data[1] >> 2) & 1;
				if ((data[1] >> 2) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v28)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				v27 = (data[3] >> 3) & 1;
				if ((data[3] >> 6) & 1)
					data[3] |= 8u;
				else
					data[3] &= 0xF7u;
				if (v27)
					data[3] |= 0x40u;
				else
					data[3] &= 0xBFu;
				*data ^= 0xA7u;
				data[2] ^= 0x77u;
				data[4] ^= 0xAFu;
			}
		}
		else
		{
			v3 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v3;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			data[1] ^= 0x67u;
			v4 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v4;
			v5 = data[3];
			data[3] = data[3];
			data[3] = v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v7;
			v8 = *data >> 1;
			*data <<= 7;
			*data |= v8;
			data[3] ^= 0x52u;
			data[2] ^= 0xEDu;
			v9 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v9;
			v30 = (data[3] >> 1) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			if (v30)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
		}
	}
}

void PacketTwister_47::Twist(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST19_1@15
	char v6; // ST18_1@15
	char v7; // ST17_1@15
	char v8; // ST10_1@37
	char v9; // ST0F_1@37
	char v10; // ST0E_1@37
	char v11; // ST0B_1@44
	char v12; // ST0A_1@44
	char v13; // ST09_1@44
	char v14; // ST08_1@44
	char v15; // [sp+4h] [bp-1Ch]@50
	char v16; // [sp+6h] [bp-1Ah]@44
	char v17; // [sp+Ch] [bp-14h]@38
	char v18; // [sp+11h] [bp-Fh]@29
	char v19; // [sp+13h] [bp-Dh]@23
	char v20; // [sp+15h] [bp-Bh]@15
	char v21; // [sp+1Ah] [bp-6h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[14] >> 2) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v17)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					v11 = data[9];
					data[9] = data[20];
					data[20] = v11;
					v12 = data[24];
					data[24] = data[29];
					data[29] = v12;
					v13 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v13;
					v14 = data[2] >> 2;
					data[2] <<= 6;
					data[2] |= v14;
					data[17] ^= 0x2Au;
					v16 = (data[16] >> 2) & 1;
					if ((data[16] >> 3) & 1)
						data[16] |= 4u;
					else
						data[16] &= 0xFBu;
					if (v16)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					v15 = (data[6] >> 4) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v15)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
				}
				else
				{
					v8 = data[10];
					data[10] = data[1];
					data[1] = v8;
					v9 = data[10] >> 1;
					data[10] <<= 7;
					data[10] |= v9;
					v10 = data[6];
					data[6] = data[5];
					data[5] = v10;
				}
			}
			else
			{
				v19 = (data[7] >> 1) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v19)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v18 = (data[5] >> 3) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				if (v18)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				data[7] ^= 0xFAu;
			}
		}
		else
		{
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v3 = data[1] >> 5;
			data[1] *= 8;
			data[1] |= v3;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v21 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v21)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v5 = *data;
			*data = *data;
			*data = v5;
			v6 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v6;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
			data[2] ^= 0x20u;
			v20 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v20)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
		}
	}
}

void PacketTwister_47::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST18_1@15
	char v7; // ST17_1@15
	char v8; // ST10_1@37
	char v9; // ST0F_1@37
	char v10; // ST0E_1@37
	char v11; // ST09_1@50
	char v12; // ST08_1@50
	char v13; // ST07_1@50
	char v14; // ST06_1@50
	char v15; // [sp+4h] [bp-1Ch]@50
	char v16; // [sp+Ah] [bp-16h]@44
	char v17; // [sp+Ch] [bp-14h]@38
	char v18; // [sp+11h] [bp-Fh]@29
	char v19; // [sp+13h] [bp-Dh]@23
	char v20; // [sp+15h] [bp-Bh]@15
	char v21; // [sp+19h] [bp-7h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[6] >> 4) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v17)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					v16 = (data[16] >> 2) & 1;
					if ((data[16] >> 3) & 1)
						data[16] |= 4u;
					else
						data[16] &= 0xFBu;
					if (v16)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					data[17] ^= 0x2Au;
					v11 = data[2] >> 6;
					data[2] *= 4;
					data[2] |= v11;
					v12 = data[17] >> 2;
					data[17] <<= 6;
					data[17] |= v12;
					v13 = data[24];
					data[24] = data[29];
					data[29] = v13;
					v14 = data[9];
					data[9] = data[20];
					data[20] = v14;
					v15 = (data[14] >> 2) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v15)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
				}
				else
				{
					v8 = data[6];
					data[6] = data[5];
					data[5] = v8;
					v9 = data[10] >> 7;
					data[10] *= 2;
					data[10] |= v9;
					v10 = data[10];
					data[10] = data[1];
					data[1] = v10;
				}
			}
			else
			{
				data[7] ^= 0xFAu;
				v19 = (data[5] >> 3) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				if (v19)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				v18 = (data[7] >> 1) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v18)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
			}
		}
		else
		{
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 7) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			data[2] ^= 0x20u;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v4;
			v5 = *data;
			*data = *data;
			*data = v5;
			v21 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v21)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v6 = *data;
			*data = data[1];
			data[1] = v6;
			v7 = data[1] >> 3;
			data[1] *= 32;
			data[1] |= v7;
			v20 = (data[3] >> 5) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v20)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
		}
	}
}

void PacketTwister_48::Twist(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST19_1@3
	char v6; // ST18_1@3
	char v7; // ST11_1@23
	char v8; // ST10_1@25
	char v9; // ST0F_1@25
	char v10; // ST0C_1@31
	char v11; // ST09_1@38
	char v12; // [sp+7h] [bp-15h]@38
	char v13; // [sp+Ah] [bp-12h]@32
	char v14; // [sp+Dh] [bp-Fh]@25
	char v15; // [sp+12h] [bp-Ah]@15
	char v16; // [sp+14h] [bp-8h]@9
	char v2; // [sp+16h] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[27] = data[27];
					v13 = (data[13] >> 7) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v13)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					data[11] ^= 0xD3u;
					v11 = data[1] >> 1;
					data[1] <<= 7;
					data[1] |= v11;
					v12 = (data[2] >> 6) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v12)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
				}
				else
				{
					v8 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v8;
					data[7] ^= 0x3Cu;
					v9 = data[3];
					data[3] = data[11];
					data[11] = v9;
					data[12] ^= 0xF9u;
					v14 = (data[2] >> 6) & 1;
					if ((data[2] >> 2) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v14)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					v10 = data[15];
					data[15] = data[7];
					data[7] = v10;
					data[11] ^= 0xBu;
					*data ^= 0x2Au;
				}
			}
			else
			{
				data[5] ^= 0xC1u;
				v7 = *data;
				*data = data[2];
				data[2] = v7;
				data[7] ^= 0x34u;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[3];
			data[3] = v3;
			data[2] ^= 0x95u;
			*data ^= 0xDAu;
			v4 = *data;
			*data = *data;
			*data = v4;
			v5 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v5;
			v6 = data[3];
			data[3] = data[3];
			data[3] = v6;
			v2 = (*data >> 5) & 1;
			if ((*data >> 7) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v2)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			data[1] ^= 0x89u;
			v16 = (data[3] >> 2) & 1;
			if ((data[3] >> 4) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v16)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			v15 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v15)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
		}
	}
}

void PacketTwister_48::Correct(BYTE *data, int length)
{
	char v3; // ST15_1@21
	char v4; // ST14_1@21
	char v5; // ST13_1@21
	char v6; // ST12_1@21
	char v7; // ST11_1@23
	char v8; // ST10_1@25
	char v9; // ST0D_1@31
	char v10; // ST0C_1@31
	char v11; // ST09_1@38
	char v12; // [sp+7h] [bp-15h]@38
	char v13; // [sp+Ah] [bp-12h]@32
	char v14; // [sp+Eh] [bp-Eh]@25
	char v15; // [sp+16h] [bp-6h]@15
	char v16; // [sp+18h] [bp-4h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = (data[2] >> 6) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v13)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v11 = data[1] >> 7;
					data[1] *= 2;
					data[1] |= v11;
					data[11] ^= 0xD3u;
					v12 = (data[13] >> 7) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					if (v12)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					data[27] = data[27];
				}
				else
				{
					*data ^= 0x2Au;
					data[11] ^= 0xBu;
					v8 = data[15];
					data[15] = data[7];
					data[7] = v8;
					v14 = (data[2] >> 6) & 1;
					if ((data[2] >> 2) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v14)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					data[12] ^= 0xF9u;
					v9 = data[3];
					data[3] = data[11];
					data[11] = v9;
					data[7] ^= 0x3Cu;
					v10 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v10;
				}
			}
			else
			{
				data[7] ^= 0x34u;
				v7 = *data;
				*data = data[2];
				data[2] = v7;
				data[5] ^= 0xC1u;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v16 = (data[3] >> 2) & 1;
			if ((data[3] >> 4) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v16)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			data[1] ^= 0x89u;
			v15 = (*data >> 5) & 1;
			if ((*data >> 7) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v15)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v3 = data[3];
			data[3] = data[3];
			data[3] = v3;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v5 = *data;
			*data = *data;
			*data = v5;
			*data ^= 0xDAu;
			data[2] ^= 0x95u;
			v6 = data[2];
			data[2] = data[3];
			data[3] = v6;
		}
	}
}

void PacketTwister_49::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST1B_1@3
	char v8; // ST18_1@11
	char v9; // ST15_1@17
	char v10; // ST14_1@17
	char v11; // ST13_1@17
	char v12; // ST12_1@17
	char v13; // ST11_1@17
	char v14; // ST0E_1@25
	char v15; // ST0D_1@25
	char v16; // ST0C_1@26
	char v17; // ST0B_1@26
	char v18; // ST08_1@32
	char v19; // [sp+6h] [bp-1Ah]@32
	char v20; // [sp+9h] [bp-17h]@26
	char v21; // [sp+Fh] [bp-11h]@19
	char v22; // [sp+16h] [bp-Ah]@11
	char v2; // [sp+19h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[1];
					data[1] = data[4];
					data[4] = v16;
					data[1] ^= 0xA3u;
					v17 = data[4];
					data[4] = data[28];
					data[28] = v17;
					v20 = (data[13] >> 2) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v20)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					data[17] ^= 0xB5u;
					v18 = data[18] >> 2;
					data[18] <<= 6;
					data[18] |= v18;
					v19 = (data[6] >> 2) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v19)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
				}
				else
				{
					v21 = (data[8] >> 1) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					if (v21)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					v14 = data[11];
					data[11] = data[6];
					data[6] = v14;
					v15 = data[1];
					data[1] = data[3];
					data[3] = v15;
				}
			}
			else
			{
				v8 = data[7];
				data[7] = data[4];
				data[4] = v8;
				v22 = (data[6] >> 3) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				if (v22)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
				v9 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v9;
				data[2] ^= 0xCu;
				v10 = *data >> 6;
				*data *= 4;
				*data |= v10;
				v11 = *data >> 6;
				*data *= 4;
				*data |= v11;
				v12 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v12;
				v13 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v13;
				data[6] ^= 6u;
				*data ^= 0x70u;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v4 = data[1];
			data[1] = data[1];
			data[1] = v4;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
			v6 = *data;
			*data = data[1];
			data[1] = v6;
			data[2] ^= 0x9Eu;
			v7 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v7;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			data[3] ^= 0xFDu;
		}
	}
}

void PacketTwister_49::Correct(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST18_1@11
	char v9; // ST17_1@11
	char v10; // ST16_1@11
	char v11; // ST15_1@11
	char v12; // ST14_1@11
	char v13; // ST11_1@17
	char v14; // ST10_1@19
	char v15; // ST0F_1@19
	char v16; // ST0A_1@32
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // [sp+8h] [bp-18h]@32
	char v20; // [sp+Bh] [bp-15h]@26
	char v21; // [sp+Dh] [bp-13h]@19
	char v22; // [sp+12h] [bp-Eh]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = (data[6] >> 2) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					if (v20)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					v16 = data[18] >> 6;
					data[18] *= 4;
					data[18] |= v16;
					data[17] ^= 0xB5u;
					v19 = (data[13] >> 2) & 1;
					if ((data[13] >> 3) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v19)
						data[13] |= 8u;
					else
						data[13] &= 0xF7u;
					v17 = data[4];
					data[4] = data[28];
					data[28] = v17;
					data[1] ^= 0xA3u;
					v18 = data[1];
					data[1] = data[4];
					data[4] = v18;
				}
				else
				{
					v14 = data[1];
					data[1] = data[3];
					data[3] = v14;
					v15 = data[11];
					data[11] = data[6];
					data[6] = v15;
					v21 = (data[8] >> 1) & 1;
					if ((data[8] >> 1) & 1)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
					if (v21)
						data[8] |= 2u;
					else
						data[8] &= 0xFDu;
				}
			}
			else
			{
				*data ^= 0x70u;
				data[6] ^= 6u;
				v8 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v8;
				v9 = data[5] >> 4;
				data[5] *= 16;
				data[5] |= v9;
				v10 = *data >> 2;
				*data <<= 6;
				*data |= v10;
				v11 = *data >> 2;
				*data <<= 6;
				*data |= v11;
				data[2] ^= 0xCu;
				v12 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v12;
				v22 = (data[6] >> 3) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				if (v22)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
				v13 = data[7];
				data[7] = data[4];
				data[4] = v13;
			}
		}
		else
		{
			data[3] ^= 0xFDu;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v3 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v3;
			data[2] ^= 0x9Eu;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
			v6 = data[1];
			data[1] = data[1];
			data[1] = v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
		}
	}
}

void PacketTwister_50::Twist(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST19_1@3
	char v6; // ST12_1@23
	char v7; // ST11_1@25
	char v8; // ST10_1@25
	char v9; // ST0F_1@25
	char v10; // ST0E_1@25
	char v11; // ST0D_1@25
	char v12; // ST0A_1@32
	char v13; // ST09_1@32
	char v14; // ST08_1@32
	char v15; // ST07_1@32
	char v16; // [sp+Bh] [bp-11h]@26
	char v17; // [sp+13h] [bp-9h]@17
	char v18; // [sp+15h] [bp-7h]@11
	char v2; // [sp+17h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = (data[6] >> 7) & 1;
					if ((data[6] >> 4) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v16)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					v12 = data[26] >> 2;
					data[26] <<= 6;
					data[26] |= v12;
					v13 = data[1] >> 3;
					data[1] *= 32;
					data[1] |= v13;
					v14 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v14;
					data[13] ^= 0x26u;
					data[10] ^= 0xC8u;
					data[10] ^= 0xD7u;
					v15 = data[27] >> 6;
					data[27] *= 4;
					data[27] |= v15;
				}
				else
				{
					v7 = data[5] >> 3;
					data[5] *= 32;
					data[5] |= v7;
					data[14] ^= 0x24u;
					v8 = data[2] >> 2;
					data[2] <<= 6;
					data[2] |= v8;
					data[1] ^= 0x55u;
					v9 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v9;
					v10 = data[13] >> 6;
					data[13] *= 4;
					data[13] |= v10;
					v11 = data[3];
					data[3] = data[1];
					data[1] = v11;
				}
			}
			else
			{
				v18 = (*data >> 1) & 1;
				if ((*data >> 6) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v18)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				data[2] ^= 0xE4u;
				v17 = (data[2] >> 5) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v17)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				v6 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v6;
			}
		}
		else
		{
			*data ^= 0x3Cu;
			v3 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v3;
			*data ^= 0xEEu;
			v4 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v4;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v2 = (*data >> 2) & 1;
			if ((*data >> 7) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
		}
	}
}

void PacketTwister_50::Correct(BYTE *data, int length)
{
	char v3; // ST19_1@9
	char v4; // ST18_1@9
	char v5; // ST17_1@9
	char v6; // ST16_1@11
	char v7; // ST11_1@25
	char v8; // ST10_1@25
	char v9; // ST0F_1@25
	char v10; // ST0E_1@25
	char v11; // ST0D_1@25
	char v12; // ST0C_1@26
	char v13; // ST0B_1@26
	char v14; // ST0A_1@26
	char v15; // ST09_1@26
	char v16; // [sp+7h] [bp-15h]@26
	char v17; // [sp+12h] [bp-Ah]@17
	char v18; // [sp+14h] [bp-8h]@11
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[27] >> 2;
					data[27] <<= 6;
					data[27] |= v12;
					data[10] ^= 0xD7u;
					data[10] ^= 0xC8u;
					data[13] ^= 0x26u;
					v13 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v13;
					v14 = data[1] >> 5;
					data[1] *= 8;
					data[1] |= v14;
					v15 = data[26] >> 6;
					data[26] *= 4;
					data[26] |= v15;
					v16 = (data[6] >> 7) & 1;
					if ((data[6] >> 4) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v16)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
				}
				else
				{
					v7 = data[3];
					data[3] = data[1];
					data[1] = v7;
					v8 = data[13] >> 2;
					data[13] <<= 6;
					data[13] |= v8;
					v9 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v9;
					data[1] ^= 0x55u;
					v10 = data[2] >> 6;
					data[2] *= 4;
					data[2] |= v10;
					data[14] ^= 0x24u;
					v11 = data[5] >> 5;
					data[5] *= 8;
					data[5] |= v11;
				}
			}
			else
			{
				v6 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v6;
				v18 = (data[2] >> 5) & 1;
				if ((data[2] >> 6) & 1)
					data[2] |= 0x20u;
				else
					data[2] &= 0xDFu;
				if (v18)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				data[2] ^= 0xE4u;
				v17 = (*data >> 1) & 1;
				if ((*data >> 6) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v17)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
			}
		}
		else
		{
			v2 = (*data >> 2) & 1;
			if ((*data >> 7) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			*data ^= 0xEEu;
			v5 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v5;
			*data ^= 0x3Cu;
		}
	}
}

void PacketTwister_51::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2E_1@3
	char v5; // ST2B_1@11
	char v6; // ST2A_1@11
	char v7; // ST25_1@23
	char v8; // ST24_1@23
	char v9; // ST1B_1@49
	char v10; // ST1A_1@49
	char v11; // ST17_1@55
	char v12; // ST14_1@62
	char v13; // ST11_1@68
	char v14; // ST10_1@68
	char v15; // ST0F_1@68
	char v16; // ST0C_1@74
	char v17; // ST07_1@86
	char v18; // [sp+8h] [bp-28h]@80
	char v19; // [sp+Ah] [bp-26h]@74
	char v20; // [sp+Dh] [bp-23h]@68
	char v21; // [sp+12h] [bp-1Eh]@62
	char v22; // [sp+15h] [bp-1Bh]@55
	char v23; // [sp+18h] [bp-18h]@49
	char v24; // [sp+1Ch] [bp-14h]@43
	char v25; // [sp+1Eh] [bp-12h]@35
	char v26; // [sp+20h] [bp-10h]@29
	char v27; // [sp+22h] [bp-Eh]@23
	char v28; // [sp+26h] [bp-Ah]@17
	char v29; // [sp+28h] [bp-8h]@11
	char v2; // [sp+2Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[3];
					data[3] = data[15];
					data[15] = v12;
					v21 = (data[20] >> 2) & 1;
					if ((data[20] >> 1) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v21)
						data[20] |= 2u;
					else
						data[20] &= 0xFDu;
					v13 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v13;
					data[5] ^= 0x1Eu;
					v14 = data[30] >> 4;
					data[30] *= 16;
					data[30] |= v14;
					v15 = data[25];
					data[25] = data[31];
					data[31] = v15;
					data[21] ^= 0xAAu;
					v20 = (data[16] >> 1) & 1;
					if ((data[16] >> 3) & 1)
						data[16] |= 2u;
					else
						data[16] &= 0xFDu;
					if (v20)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					data[30] ^= 0x5Bu;
					v16 = data[5];
					data[5] = data[7];
					data[7] = v16;
					v19 = (*data >> 4) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					if (v19)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v18 = (data[9] >> 4) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					if (v18)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					v17 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v17;
				}
				else
				{
					v24 = (data[11] >> 4) & 1;
					if ((data[11] >> 7) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v24)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					v9 = data[10];
					data[10] = *data;
					*data = v9;
					data[15] ^= 0xD2u;
					v10 = data[15];
					data[15] = data[9];
					data[9] = v10;
					data[6] ^= 0xD8u;
					v23 = (data[4] >> 3) & 1;
					if ((data[4] >> 3) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v23)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					v11 = data[1];
					data[1] = data[3];
					data[3] = v11;
					data[13] ^= 0x6Du;
					data[11] ^= 0xD4u;
					v22 = (data[2] >> 6) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v22)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					data[11] ^= 0xD2u;
				}
			}
			else
			{
				data[6] ^= 0xEAu;
				data[5] ^= 0x42u;
				v5 = data[1] >> 5;
				data[1] *= 8;
				data[1] |= v5;
				v6 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v6;
				v29 = (data[4] >> 1) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v29)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				data[1] ^= 0xDAu;
				v28 = (data[4] >> 5) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v28)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				v7 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v7;
				v8 = data[4];
				data[4] = *data;
				*data = v8;
				v27 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v27)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v26 = (*data >> 2) & 1;
				if ((*data >> 1) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v26)
					*data |= 2u;
				else
					*data &= 0xFDu;
				v25 = (data[4] >> 2) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				if (v25)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
			}
		}
		else
		{
			data[2] ^= 0x79u;
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = data[1];
			data[1] = v4;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
		}
	}
}

void PacketTwister_51::Correct(BYTE *data, int length)
{
	char v3; // ST2D_1@9
	char v4; // ST2C_1@9
	char v5; // ST25_1@29
	char v6; // ST24_1@29
	char v7; // ST1F_1@41
	char v8; // ST1E_1@41
	char v9; // ST1B_1@49
	char v10; // ST18_1@55
	char v11; // ST17_1@55
	char v12; // ST14_1@62
	char v13; // ST0F_1@74
	char v14; // ST0C_1@80
	char v15; // ST0B_1@80
	char v16; // ST0A_1@80
	char v17; // ST07_1@86
	char v18; // [sp+8h] [bp-28h]@80
	char v19; // [sp+Dh] [bp-23h]@74
	char v20; // [sp+10h] [bp-20h]@68
	char v21; // [sp+12h] [bp-1Eh]@62
	char v22; // [sp+15h] [bp-1Bh]@55
	char v23; // [sp+19h] [bp-17h]@49
	char v24; // [sp+1Ch] [bp-14h]@43
	char v25; // [sp+20h] [bp-10h]@35
	char v26; // [sp+22h] [bp-Eh]@29
	char v27; // [sp+26h] [bp-Ah]@23
	char v28; // [sp+28h] [bp-8h]@17
	char v29; // [sp+2Ah] [bp-6h]@11
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[7] >> 4;
					data[7] *= 16;
					data[7] |= v12;
					v21 = (data[9] >> 4) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					if (v21)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					v20 = (*data >> 4) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x10u;
					else
						*data &= 0xEFu;
					if (v20)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v13 = data[5];
					data[5] = data[7];
					data[7] = v13;
					data[30] ^= 0x5Bu;
					v19 = (data[16] >> 1) & 1;
					if ((data[16] >> 3) & 1)
						data[16] |= 2u;
					else
						data[16] &= 0xFDu;
					if (v19)
						data[16] |= 8u;
					else
						data[16] &= 0xF7u;
					data[21] ^= 0xAAu;
					v14 = data[25];
					data[25] = data[31];
					data[31] = v14;
					v15 = data[30] >> 4;
					data[30] *= 16;
					data[30] |= v15;
					data[5] ^= 0x1Eu;
					v16 = data[17] >> 2;
					data[17] <<= 6;
					data[17] |= v16;
					v18 = (data[20] >> 2) & 1;
					if ((data[20] >> 1) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v18)
						data[20] |= 2u;
					else
						data[20] &= 0xFDu;
					v17 = data[3];
					data[3] = data[15];
					data[15] = v17;
				}
				else
				{
					data[11] ^= 0xD2u;
					v24 = (data[2] >> 6) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v24)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					data[11] ^= 0xD4u;
					data[13] ^= 0x6Du;
					v9 = data[1];
					data[1] = data[3];
					data[3] = v9;
					v23 = (data[4] >> 3) & 1;
					if ((data[4] >> 3) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v23)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					data[6] ^= 0xD8u;
					v10 = data[15];
					data[15] = data[9];
					data[9] = v10;
					data[15] ^= 0xD2u;
					v11 = data[10];
					data[10] = *data;
					*data = v11;
					v22 = (data[11] >> 4) & 1;
					if ((data[11] >> 7) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v22)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
				}
			}
			else
			{
				v29 = (data[4] >> 2) & 1;
				if ((data[4] >> 7) & 1)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				if (v29)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				v28 = (*data >> 2) & 1;
				if ((*data >> 1) & 1)
					*data |= 4u;
				else
					*data &= 0xFBu;
				if (v28)
					*data |= 2u;
				else
					*data &= 0xFDu;
				v27 = (data[6] >> 2) & 1;
				if ((data[6] >> 1) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v27)
					data[6] |= 2u;
				else
					data[6] &= 0xFDu;
				v5 = data[4];
				data[4] = *data;
				*data = v5;
				v6 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v6;
				v26 = (data[4] >> 5) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v26)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				data[1] ^= 0xDAu;
				v25 = (data[4] >> 1) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v25)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v7 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v7;
				v8 = data[1] >> 3;
				data[1] *= 32;
				data[1] |= v8;
				data[5] ^= 0x42u;
				data[6] ^= 0xEAu;
			}
		}
		else
		{
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = data[1];
			data[1] = v4;
			data[2] ^= 0x79u;
		}
	}
}

void PacketTwister_52::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST1F_1@21
	char v6; // ST1E_1@21
	char v7; // ST1D_1@23
	char v8; // ST1C_1@23
	char v9; // ST19_1@29
	char v10; // ST18_1@29
	char v11; // ST15_1@37
	char v12; // ST14_1@37
	char v13; // ST0D_1@55
	char v14; // ST0C_1@55
	char v15; // ST0B_1@55
	char v16; // ST0A_1@55
	char v17; // ST09_1@56
	char v18; // ST08_1@56
	char v19; // ST07_1@56
	char v20; // [sp+Eh] [bp-1Ah]@49
	char v21; // [sp+10h] [bp-18h]@43
	char v22; // [sp+12h] [bp-16h]@37
	char v23; // [sp+16h] [bp-12h]@29
	char v24; // [sp+1Ah] [bp-Eh]@23
	char v25; // [sp+20h] [bp-8h]@15
	char v26; // [sp+22h] [bp-6h]@9
	char v2; // [sp+24h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[20] ^= 0x92u;
					data[16] ^= 0x82u;
					data[2] ^= 0xF0u;
					v17 = data[29] >> 3;
					data[29] *= 32;
					data[29] |= v17;
					v18 = data[29];
					data[29] = data[13];
					data[13] = v18;
					v19 = data[29] >> 3;
					data[29] *= 32;
					data[29] |= v19;
				}
				else
				{
					v11 = data[6];
					data[6] = data[14];
					data[14] = v11;
					v12 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v12;
					v22 = (data[5] >> 7) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					if (v22)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					v21 = (data[14] >> 4) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v21)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					v20 = (data[6] >> 1) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					if (v20)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					data[14] ^= 0xA9u;
					v13 = data[8] >> 2;
					data[8] <<= 6;
					data[8] |= v13;
					data[15] ^= 0xA2u;
					v14 = data[4];
					data[4] = data[2];
					data[2] = v14;
					data[11] ^= 0x61u;
					v15 = data[8];
					data[8] = data[2];
					data[2] = v15;
					v16 = data[8];
					data[8] = data[11];
					data[11] = v16;
					data[15] ^= 0x3Cu;
				}
			}
			else
			{
				v7 = data[1];
				data[1] = data[6];
				data[6] = v7;
				v8 = data[3];
				data[3] = data[6];
				data[6] = v8;
				v24 = (data[7] >> 2) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v24)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				data[7] ^= 0x73u;
				v9 = *data >> 7;
				*data *= 2;
				*data |= v9;
				v10 = *data;
				*data = data[5];
				data[5] = v10;
				v23 = (*data >> 4) & 1;
				if ((*data >> 3) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v23)
					*data |= 8u;
				else
					*data &= 0xF7u;
			}
		}
		else
		{
			data[2] ^= 0x54u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v4;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v26 = (data[2] >> 1) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v26)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v25 = (data[1] >> 2) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v25)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v5 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v5;
			v6 = data[2];
			data[2] = data[3];
			data[3] = v6;
		}
	}
}

void PacketTwister_52::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST1F_1@21
	char v6; // ST1E_1@21
	char v7; // ST1B_1@29
	char v8; // ST1A_1@29
	char v9; // ST17_1@35
	char v10; // ST16_1@35
	char v11; // ST15_1@37
	char v12; // ST14_1@37
	char v13; // ST13_1@37
	char v14; // ST12_1@37
	char v15; // ST0B_1@55
	char v16; // ST0A_1@55
	char v17; // ST09_1@56
	char v18; // ST08_1@56
	char v19; // ST07_1@56
	char v20; // [sp+Ch] [bp-1Ch]@49
	char v21; // [sp+Eh] [bp-1Ah]@43
	char v22; // [sp+10h] [bp-18h]@37
	char v23; // [sp+18h] [bp-10h]@29
	char v24; // [sp+1Ch] [bp-Ch]@23
	char v25; // [sp+20h] [bp-8h]@15
	char v26; // [sp+22h] [bp-6h]@9
	char v2; // [sp+24h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[29] >> 5;
					data[29] *= 8;
					data[29] |= v17;
					v18 = data[29];
					data[29] = data[13];
					data[13] = v18;
					v19 = data[29] >> 5;
					data[29] *= 8;
					data[29] |= v19;
					data[2] ^= 0xF0u;
					data[16] ^= 0x82u;
					data[20] ^= 0x92u;
				}
				else
				{
					data[15] ^= 0x3Cu;
					v11 = data[8];
					data[8] = data[11];
					data[11] = v11;
					v12 = data[8];
					data[8] = data[2];
					data[2] = v12;
					data[11] ^= 0x61u;
					v13 = data[4];
					data[4] = data[2];
					data[2] = v13;
					data[15] ^= 0xA2u;
					v14 = data[8] >> 6;
					data[8] *= 4;
					data[8] |= v14;
					data[14] ^= 0xA9u;
					v22 = (data[6] >> 1) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					if (v22)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v21 = (data[14] >> 4) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 0x10u;
					else
						data[14] &= 0xEFu;
					if (v21)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					v20 = (data[5] >> 7) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					if (v20)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					v15 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v15;
					v16 = data[6];
					data[6] = data[14];
					data[14] = v16;
				}
			}
			else
			{
				v24 = (*data >> 4) & 1;
				if ((*data >> 3) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v24)
					*data |= 8u;
				else
					*data &= 0xF7u;
				v7 = *data;
				*data = data[5];
				data[5] = v7;
				v8 = *data >> 1;
				*data <<= 7;
				*data |= v8;
				data[7] ^= 0x73u;
				v23 = (data[7] >> 2) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v23)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v9 = data[3];
				data[3] = data[6];
				data[6] = v9;
				v10 = data[1];
				data[1] = data[6];
				data[6] = v10;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[3];
			data[3] = v3;
			v4 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v4;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v26 = (data[2] >> 1) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			if (v26)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v25 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v25)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			v6 = data[1];
			data[1] = data[2];
			data[2] = v6;
			data[2] ^= 0x54u;
		}
	}
}

void PacketTwister_53::Twist(BYTE *data, int length)
{
	char v3; // ST13_1@15
	char v4; // ST12_1@17
	char v5; // ST11_1@17
	char v6; // ST10_1@17
	char v7; // ST0F_1@17
	char v8; // ST0E_1@19
	char v9; // ST0D_1@19
	char v10; // ST0C_1@20
	char v11; // ST09_1@26
	char v12; // ST08_1@26
	char v13; // ST07_1@26
	char v14; // ST06_1@26
	char v15; // [sp+Ah] [bp-Eh]@20
	char v16; // [sp+14h] [bp-4h]@9
	char v2; // [sp+16h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v10 = data[12];
					data[12] = data[10];
					data[10] = v10;
					v15 = (data[3] >> 7) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v15)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					v11 = data[27];
					data[27] = data[27];
					data[27] = v11;
					data[31] ^= 0xB4u;
					v12 = data[3];
					data[3] = data[2];
					data[2] = v12;
					v13 = data[19];
					data[19] = data[22];
					data[22] = v13;
					data[12] ^= 0x8Fu;
					v14 = data[9];
					data[9] = data[7];
					data[7] = v14;
					data[26] ^= 0xA2u;
				}
				else
				{
					data[10] ^= 0xD1u;
					v8 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v8;
					v9 = *data;
					*data = data[6];
					data[6] = v9;
				}
			}
			else
			{
				v4 = data[2];
				data[2] = data[7];
				data[7] = v4;
				v5 = data[1];
				data[1] = data[2];
				data[2] = v5;
				v6 = data[7];
				data[7] = data[4];
				data[4] = v6;
				v7 = data[3];
				data[3] = data[5];
				data[5] = v7;
				data[2] ^= 0xCBu;
			}
		}
		else
		{
			v2 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v16 = (data[2] >> 5) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v16)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			data[3] ^= 0x14u;
			v3 = *data >> 5;
			*data *= 8;
			*data |= v3;
			data[1] ^= 0xD2u;
		}
	}
}

void PacketTwister_53::Correct(BYTE *data, int length)
{
	char v3; // ST17_1@3
	char v4; // ST12_1@17
	char v5; // ST11_1@17
	char v6; // ST10_1@17
	char v7; // ST0F_1@17
	char v8; // ST0E_1@19
	char v9; // ST0D_1@19
	char v10; // ST0C_1@20
	char v11; // ST0B_1@20
	char v12; // ST0A_1@20
	char v13; // ST09_1@20
	char v14; // ST06_1@26
	char v15; // [sp+7h] [bp-11h]@20
	char v16; // [sp+13h] [bp-5h]@9
	char v2; // [sp+15h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[26] ^= 0xA2u;
					v10 = data[9];
					data[9] = data[7];
					data[7] = v10;
					data[12] ^= 0x8Fu;
					v11 = data[19];
					data[19] = data[22];
					data[22] = v11;
					v12 = data[3];
					data[3] = data[2];
					data[2] = v12;
					data[31] ^= 0xB4u;
					v13 = data[27];
					data[27] = data[27];
					data[27] = v13;
					v15 = (data[3] >> 7) & 1;
					if ((data[3] >> 6) & 1)
						data[3] |= 0x80u;
					else
						data[3] &= 0x7Fu;
					if (v15)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					v14 = data[12];
					data[12] = data[10];
					data[10] = v14;
				}
				else
				{
					v8 = *data;
					*data = data[6];
					data[6] = v8;
					v9 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v9;
					data[10] ^= 0xD1u;
				}
			}
			else
			{
				data[2] ^= 0xCBu;
				v4 = data[3];
				data[3] = data[5];
				data[5] = v4;
				v5 = data[7];
				data[7] = data[4];
				data[4] = v5;
				v6 = data[1];
				data[1] = data[2];
				data[2] = v6;
				v7 = data[2];
				data[2] = data[7];
				data[7] = v7;
			}
		}
		else
		{
			data[1] ^= 0xD2u;
			v3 = *data >> 3;
			*data *= 32;
			*data |= v3;
			data[3] ^= 0x14u;
			v2 = (data[2] >> 5) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v2)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v16 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v16)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
		}
	}
}

void PacketTwister_54::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST1C_1@21
	char v5; // ST1B_1@21
	char v6; // ST1A_1@23
	char v7; // ST19_1@23
	char v8; // ST18_1@23
	char v9; // ST17_1@23
	char v10; // ST0C_1@55
	char v11; // ST0B_1@55
	char v12; // ST0A_1@55
	char v13; // ST09_1@56
	char v14; // ST08_1@56
	char v15; // ST07_1@56
	char v16; // ST06_1@56
	char v17; // ST05_1@56
	char v18; // ST04_1@56
	char v19; // [sp+Dh] [bp-17h]@49
	char v20; // [sp+Fh] [bp-15h]@43
	char v21; // [sp+11h] [bp-13h]@37
	char v22; // [sp+13h] [bp-11h]@31
	char v23; // [sp+15h] [bp-Fh]@23
	char v24; // [sp+1Dh] [bp-7h]@15
	char v25; // [sp+1Fh] [bp-5h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[13];
					data[13] = data[7];
					data[7] = v13;
					v14 = data[13];
					data[13] = data[23];
					data[23] = v14;
					v15 = data[15] >> 4;
					data[15] *= 16;
					data[15] |= v15;
					v16 = data[26];
					data[26] = data[6];
					data[6] = v16;
					v17 = data[27];
					data[27] = data[8];
					data[8] = v17;
					data[19] ^= 0xF0u;
					data[28] ^= 0x57u;
					v18 = data[16] >> 3;
					data[16] *= 32;
					data[16] |= v18;
					data[3] ^= 0x26u;
				}
				else
				{
					v22 = (data[12] >> 5) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 0x20u;
					else
						data[12] &= 0xDFu;
					if (v22)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					v21 = (data[6] >> 1) & 1;
					if ((data[6] >> 5) & 1)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					if (v21)
						data[6] |= 0x20u;
					else
						data[6] &= 0xDFu;
					data[9] ^= 0x6Du;
					v20 = (*data >> 2) & 1;
					if ((*data >> 6) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v20)
						*data |= 0x40u;
					else
						*data &= 0xBFu;
					v19 = (data[4] >> 3) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v19)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v10 = data[15] >> 5;
					data[15] *= 8;
					data[15] |= v10;
					v11 = data[7];
					data[7] = data[15];
					data[15] = v11;
					v12 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v12;
				}
			}
			else
			{
				v6 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v6;
				data[3] ^= 0xEDu;
				v7 = data[4];
				data[4] = data[6];
				data[6] = v7;
				v8 = data[5];
				data[5] = data[6];
				data[6] = v8;
				v9 = data[2] >> 1;
				data[2] <<= 7;
				data[2] |= v9;
				v23 = (data[7] >> 1) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v23)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				data[1] ^= 0xF3u;
			}
		}
		else
		{
			v3 = *data >> 3;
			*data *= 32;
			*data |= v3;
			v2 = (*data >> 2) & 1;
			if ((*data >> 3) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v25 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v25)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v24 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v24)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v5 = data[2];
			data[2] = data[3];
			data[3] = v5;
		}
	}
}

void PacketTwister_54::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST1B_1@21
	char v6; // ST18_1@29
	char v7; // ST17_1@29
	char v8; // ST16_1@29
	char v9; // ST15_1@29
	char v10; // ST14_1@31
	char v11; // ST13_1@31
	char v12; // ST12_1@31
	char v13; // ST09_1@56
	char v14; // ST08_1@56
	char v15; // ST07_1@56
	char v16; // ST06_1@56
	char v17; // ST05_1@56
	char v18; // ST04_1@56
	char v19; // [sp+Ah] [bp-1Ah]@49
	char v20; // [sp+Ch] [bp-18h]@43
	char v21; // [sp+Eh] [bp-16h]@37
	char v22; // [sp+10h] [bp-14h]@31
	char v23; // [sp+19h] [bp-Bh]@23
	char v24; // [sp+1Ch] [bp-8h]@15
	char v25; // [sp+1Eh] [bp-6h]@9
	char v2; // [sp+20h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[3] ^= 0x26u;
					v13 = data[16] >> 5;
					data[16] *= 8;
					data[16] |= v13;
					data[28] ^= 0x57u;
					data[19] ^= 0xF0u;
					v14 = data[27];
					data[27] = data[8];
					data[8] = v14;
					v15 = data[26];
					data[26] = data[6];
					data[6] = v15;
					v16 = data[15] >> 4;
					data[15] *= 16;
					data[15] |= v16;
					v17 = data[13];
					data[13] = data[23];
					data[23] = v17;
					v18 = data[13];
					data[13] = data[7];
					data[7] = v18;
				}
				else
				{
					v10 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v10;
					v11 = data[7];
					data[7] = data[15];
					data[15] = v11;
					v12 = data[15] >> 3;
					data[15] *= 32;
					data[15] |= v12;
					v22 = (data[4] >> 3) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v22)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v21 = (*data >> 2) & 1;
					if ((*data >> 6) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v21)
						*data |= 0x40u;
					else
						*data &= 0xBFu;
					data[9] ^= 0x6Du;
					v20 = (data[6] >> 1) & 1;
					if ((data[6] >> 5) & 1)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					if (v20)
						data[6] |= 0x20u;
					else
						data[6] &= 0xDFu;
					v19 = (data[12] >> 5) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 0x20u;
					else
						data[12] &= 0xDFu;
					if (v19)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
				}
			}
			else
			{
				data[1] ^= 0xF3u;
				v23 = (data[7] >> 1) & 1;
				if ((data[7] >> 1) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v23)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				v6 = data[2] >> 7;
				data[2] *= 2;
				data[2] |= v6;
				v7 = data[5];
				data[5] = data[6];
				data[6] = v7;
				v8 = data[4];
				data[4] = data[6];
				data[6] = v8;
				data[3] ^= 0xEDu;
				v9 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v9;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[3];
			data[3] = v3;
			v4 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v4;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v25 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v25)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v24 = (*data >> 2) & 1;
			if ((*data >> 3) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v24)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v5 = *data >> 5;
			*data *= 8;
			*data |= v5;
		}
	}
}

void PacketTwister_55::Twist(BYTE *data, int length)
{
	char v3; // ST21_1@9
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1C_1@15
	char v7; // ST1B_1@15
	char v8; // ST1A_1@15
	char v9; // ST17_1@21
	char v10; // ST12_1@35
	char v11; // ST11_1@35
	char v12; // ST10_1@35
	char v13; // ST0F_1@35
	char v14; // ST0E_1@37
	char v15; // ST0D_1@37
	char v16; // ST0C_1@37
	char v17; // ST09_1@44
	char v18; // ST08_1@44
	char v19; // ST07_1@44
	char v20; // [sp+5h] [bp-1Fh]@44
	char v21; // [sp+Ah] [bp-1Ah]@38
	char v22; // [sp+13h] [bp-11h]@29
	char v23; // [sp+15h] [bp-Fh]@23
	char v24; // [sp+18h] [bp-Ch]@15
	char v25; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+22h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = (data[21] >> 5) & 1;
					if ((data[21] >> 1) & 1)
						data[21] |= 0x20u;
					else
						data[21] &= 0xDFu;
					if (v21)
						data[21] |= 2u;
					else
						data[21] &= 0xFDu;
					v17 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v17;
					*data ^= 0x8Au;
					v18 = data[17] >> 3;
					data[17] *= 32;
					data[17] |= v18;
					v19 = data[7];
					data[7] = data[14];
					data[14] = v19;
					v20 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v20)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
				}
				else
				{
					v14 = *data;
					*data = data[3];
					data[3] = v14;
					*data ^= 0xC7u;
					v15 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v15;
					v16 = data[5];
					data[5] = data[10];
					data[10] = v16;
					data[13] ^= 0x34u;
				}
			}
			else
			{
				v23 = (data[7] >> 6) & 1;
				if ((data[7] >> 4) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v23)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
				v22 = (data[6] >> 4) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v22)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				v10 = data[2];
				data[2] = data[7];
				data[7] = v10;
				data[1] ^= 0x6Au;
				v11 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v11;
				v12 = data[5];
				data[5] = data[7];
				data[7] = v12;
				v13 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v13;
			}
		}
		else
		{
			v2 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v4;
			data[2] ^= 0x40u;
			v5 = data[1];
			data[1] = data[1];
			data[1] = v5;
			v25 = (data[1] >> 1) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v25)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v6 = *data >> 7;
			*data *= 2;
			*data |= v6;
			data[3] ^= 0x90u;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
			v8 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v8;
			v24 = (data[1] >> 1) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v24)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			*data ^= 0xE4u;
			v9 = data[3];
			data[3] = data[2];
			data[2] = v9;
		}
	}
}

void PacketTwister_55::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1E_1@9
	char v7; // ST1B_1@15
	char v8; // ST1A_1@15
	char v9; // ST19_1@15
	char v10; // ST16_1@23
	char v11; // ST15_1@23
	char v12; // ST14_1@23
	char v13; // ST13_1@23
	char v14; // ST0E_1@37
	char v15; // ST0D_1@37
	char v16; // ST0C_1@37
	char v17; // ST09_1@44
	char v18; // ST08_1@44
	char v19; // ST07_1@44
	char v20; // [sp+5h] [bp-1Fh]@44
	char v21; // [sp+Ah] [bp-1Ah]@38
	char v22; // [sp+Fh] [bp-15h]@29
	char v23; // [sp+11h] [bp-13h]@23
	char v24; // [sp+17h] [bp-Dh]@15
	char v25; // [sp+1Ch] [bp-8h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = (data[10] >> 4) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					if (v21)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v17 = data[7];
					data[7] = data[14];
					data[14] = v17;
					v18 = data[17] >> 5;
					data[17] *= 8;
					data[17] |= v18;
					*data ^= 0x8Au;
					v19 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v19;
					v20 = (data[21] >> 5) & 1;
					if ((data[21] >> 1) & 1)
						data[21] |= 0x20u;
					else
						data[21] &= 0xDFu;
					if (v20)
						data[21] |= 2u;
					else
						data[21] &= 0xFDu;
				}
				else
				{
					data[13] ^= 0x34u;
					v14 = data[5];
					data[5] = data[10];
					data[10] = v14;
					v15 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v15;
					*data ^= 0xC7u;
					v16 = *data;
					*data = data[3];
					data[3] = v16;
				}
			}
			else
			{
				v10 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v10;
				v11 = data[5];
				data[5] = data[7];
				data[7] = v11;
				v12 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v12;
				data[1] ^= 0x6Au;
				v13 = data[2];
				data[2] = data[7];
				data[7] = v13;
				v23 = (data[6] >> 4) & 1;
				if ((data[6] >> 6) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v23)
					data[6] |= 0x40u;
				else
					data[6] &= 0xBFu;
				v22 = (data[7] >> 6) & 1;
				if ((data[7] >> 4) & 1)
					data[7] |= 0x40u;
				else
					data[7] &= 0xBFu;
				if (v22)
					data[7] |= 0x10u;
				else
					data[7] &= 0xEFu;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			*data ^= 0xE4u;
			v2 = (data[1] >> 1) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v4 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v4;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
			data[3] ^= 0x90u;
			v6 = *data >> 1;
			*data <<= 7;
			*data |= v6;
			v25 = (data[1] >> 1) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v25)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v7 = data[1];
			data[1] = data[1];
			data[1] = v7;
			data[2] ^= 0x40u;
			v8 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v8;
			v9 = data[3];
			data[3] = *data;
			*data = v9;
			v24 = (data[1] >> 7) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 0x80u;
			else
				data[1] &= 0x7Fu;
			if (v24)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
		}
	}
}

void PacketTwister_56::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST29_1@3
	char v6; // ST26_1@9
	char v7; // ST25_1@9
	char v8; // ST20_1@21
	char v9; // ST1F_1@23
	char v10; // ST1C_1@29
	char v11; // ST19_1@35
	char v12; // ST18_1@35
	char v13; // ST17_1@35
	char v14; // ST16_1@37
	char v15; // ST15_1@37
	char v16; // ST12_1@43
	char v17; // ST11_1@44
	char v18; // ST10_1@44
	char v19; // ST0F_1@44
	char v20; // ST0E_1@44
	char v21; // ST0D_1@44
	char v22; // ST0C_1@44
	char v23; // ST0B_1@44
	char v24; // ST0A_1@44
	char v25; // ST09_1@44
	char v26; // ST08_1@44
	char v27; // ST07_1@44
	char v28; // [sp+13h] [bp-19h]@37
	char v29; // [sp+1Ah] [bp-12h]@29
	char v30; // [sp+1Dh] [bp-Fh]@23
	char v31; // [sp+21h] [bp-Bh]@15
	char v32; // [sp+23h] [bp-9h]@9
	char v2; // [sp+27h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[11];
					data[11] = data[16];
					data[16] = v17;
					v18 = data[29] >> 2;
					data[29] <<= 6;
					data[29] |= v18;
					v19 = data[31] >> 1;
					data[31] <<= 7;
					data[31] |= v19;
					v20 = data[3];
					data[3] = data[30];
					data[30] = v20;
					v21 = data[31] >> 5;
					data[31] *= 8;
					data[31] |= v21;
					v22 = data[23];
					data[23] = data[27];
					data[27] = v22;
					v23 = data[7];
					data[7] = data[5];
					data[5] = v23;
					v24 = data[1];
					data[1] = data[19];
					data[19] = v24;
					v25 = data[31];
					data[31] = data[27];
					data[27] = v25;
					v26 = data[5] >> 2;
					data[5] <<= 6;
					data[5] |= v26;
					data[29] ^= 0x2Du;
					v27 = data[28] >> 7;
					data[28] *= 2;
					data[28] |= v27;
				}
				else
				{
					v14 = data[3] >> 4;
					data[3] *= 16;
					data[3] |= v14;
					data[13] ^= 0x1Fu;
					v15 = data[12] >> 2;
					data[12] <<= 6;
					data[12] |= v15;
					data[11] ^= 0x88u;
					*data ^= 0x90u;
					v28 = (data[14] >> 2) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v28)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					v16 = data[8];
					data[8] = data[9];
					data[9] = v16;
					data[9] ^= 0x28u;
				}
			}
			else
			{
				v9 = data[3];
				data[3] = data[6];
				data[6] = v9;
				v30 = (data[4] >> 4) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v30)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v10 = data[1];
				data[1] = data[2];
				data[2] = v10;
				data[2] ^= 0x5Cu;
				v29 = (data[4] >> 7) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v29)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v11 = *data;
				*data = *data;
				*data = v11;
				data[6] ^= 1u;
				data[7] ^= 0x23u;
				v12 = data[3];
				data[3] = data[5];
				data[5] = v12;
				v13 = data[1];
				data[1] = data[6];
				data[6] = v13;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
			data[2] ^= 0x46u;
			v2 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v6 = *data >> 2;
			*data <<= 6;
			*data |= v6;
			v7 = *data >> 6;
			*data *= 4;
			*data |= v7;
			v32 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v32)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v31 = (data[1] >> 4) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v31)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v8 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v8;
		}
	}
}

void PacketTwister_56::Correct(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST26_1@15
	char v5; // ST25_1@15
	char v6; // ST22_1@21
	char v7; // ST21_1@21
	char v8; // ST20_1@21
	char v9; // ST1F_1@23
	char v10; // ST1E_1@23
	char v11; // ST1D_1@23
	char v12; // ST1A_1@29
	char v13; // ST17_1@35
	char v14; // ST16_1@37
	char v15; // ST13_1@43
	char v16; // ST12_1@43
	char v17; // ST11_1@44
	char v18; // ST10_1@44
	char v19; // ST0F_1@44
	char v20; // ST0E_1@44
	char v21; // ST0D_1@44
	char v22; // ST0C_1@44
	char v23; // ST0B_1@44
	char v24; // ST0A_1@44
	char v25; // ST09_1@44
	char v26; // ST08_1@44
	char v27; // ST07_1@44
	char v28; // [sp+14h] [bp-18h]@37
	char v29; // [sp+18h] [bp-14h]@29
	char v30; // [sp+1Bh] [bp-11h]@23
	char v31; // [sp+23h] [bp-9h]@15
	char v32; // [sp+27h] [bp-5h]@9
	char v2; // [sp+29h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[28] >> 1;
					data[28] <<= 7;
					data[28] |= v17;
					data[29] ^= 0x2Du;
					v18 = data[5] >> 6;
					data[5] *= 4;
					data[5] |= v18;
					v19 = data[31];
					data[31] = data[27];
					data[27] = v19;
					v20 = data[1];
					data[1] = data[19];
					data[19] = v20;
					v21 = data[7];
					data[7] = data[5];
					data[5] = v21;
					v22 = data[23];
					data[23] = data[27];
					data[27] = v22;
					v23 = data[31] >> 3;
					data[31] *= 32;
					data[31] |= v23;
					v24 = data[3];
					data[3] = data[30];
					data[30] = v24;
					v25 = data[31] >> 7;
					data[31] *= 2;
					data[31] |= v25;
					v26 = data[29] >> 6;
					data[29] *= 4;
					data[29] |= v26;
					v27 = data[11];
					data[11] = data[16];
					data[16] = v27;
				}
				else
				{
					data[9] ^= 0x28u;
					v14 = data[8];
					data[8] = data[9];
					data[9] = v14;
					v28 = (data[14] >> 2) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v28)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					*data ^= 0x90u;
					data[11] ^= 0x88u;
					v15 = data[12] >> 6;
					data[12] *= 4;
					data[12] |= v15;
					data[13] ^= 0x1Fu;
					v16 = data[3] >> 4;
					data[3] *= 16;
					data[3] |= v16;
				}
			}
			else
			{
				v9 = data[1];
				data[1] = data[6];
				data[6] = v9;
				v10 = data[3];
				data[3] = data[5];
				data[5] = v10;
				data[7] ^= 0x23u;
				data[6] ^= 1u;
				v11 = *data;
				*data = *data;
				*data = v11;
				v30 = (data[4] >> 7) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v30)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				data[2] ^= 0x5Cu;
				v12 = data[1];
				data[1] = data[2];
				data[2] = v12;
				v29 = (data[4] >> 4) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				if (v29)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v13 = data[3];
				data[3] = data[6];
				data[6] = v13;
			}
		}
		else
		{
			v3 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v3;
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v32 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v32)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v4 = *data >> 2;
			*data <<= 6;
			*data |= v4;
			v5 = *data >> 6;
			*data *= 4;
			*data |= v5;
			v31 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v31)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[2] ^= 0x46u;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
			v7 = data[1];
			data[1] = data[3];
			data[3] = v7;
			v8 = data[2];
			data[2] = data[1];
			data[1] = v8;
		}
	}
}

void PacketTwister_57::Twist(BYTE *data, int length)
{
	char v3; // ST31_1@9
	char v4; // ST30_1@9
	char v5; // ST2F_1@9
	char v6; // ST2A_1@21
	char v7; // ST25_1@35
	char v8; // ST24_1@35
	char v9; // ST23_1@37
	char v10; // ST20_1@43
	char v11; // ST1B_1@55
	char v12; // ST18_1@61
	char v13; // ST13_1@73
	char v14; // ST0E_1@86
	char v15; // ST0D_1@86
	char v16; // ST0A_1@92
	char v17; // ST07_1@98
	char v18; // ST06_1@98
	char v19; // [sp+4h] [bp-30h]@98
	char v20; // [sp+8h] [bp-2Ch]@92
	char v21; // [sp+Bh] [bp-29h]@86
	char v22; // [sp+Fh] [bp-25h]@80
	char v23; // [sp+11h] [bp-23h]@74
	char v24; // [sp+14h] [bp-20h]@67
	char v25; // [sp+16h] [bp-1Eh]@61
	char v26; // [sp+19h] [bp-1Bh]@55
	char v27; // [sp+1Ch] [bp-18h]@49
	char v28; // [sp+1Eh] [bp-16h]@43
	char v29; // [sp+21h] [bp-13h]@37
	char v30; // [sp+26h] [bp-Eh]@29
	char v31; // [sp+28h] [bp-Ch]@21
	char v32; // [sp+2Bh] [bp-9h]@15
	char v33; // [sp+2Dh] [bp-7h]@9
	char v2; // [sp+32h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v23 = (data[15] >> 7) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					if (v23)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v22 = (data[20] >> 7) & 1;
					if ((data[20] >> 7) & 1)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					if (v22)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					data[3] ^= 0x7Bu;
					v14 = data[19];
					data[19] = data[19];
					data[19] = v14;
					v15 = data[28] >> 5;
					data[28] *= 8;
					data[28] |= v15;
					data[14] ^= 0x92u;
					v21 = (data[25] >> 5) & 1;
					if ((data[25] >> 2) & 1)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					if (v21)
						data[25] |= 4u;
					else
						data[25] &= 0xFBu;
					v16 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v16;
					data[31] ^= 0xA3u;
					v20 = (data[26] >> 6) & 1;
					if ((data[26] >> 1) & 1)
						data[26] |= 0x40u;
					else
						data[26] &= 0xBFu;
					if (v20)
						data[26] |= 2u;
					else
						data[26] &= 0xFDu;
					v17 = data[31];
					data[31] = data[18];
					data[18] = v17;
					v18 = data[4];
					data[4] = data[17];
					data[17] = v18;
					v19 = (data[26] >> 5) & 1;
					if ((data[26] >> 5) & 1)
						data[26] |= 0x20u;
					else
						data[26] &= 0xDFu;
					if (v19)
						data[26] |= 0x20u;
					else
						data[26] &= 0xDFu;
				}
				else
				{
					v9 = *data >> 7;
					*data *= 2;
					*data |= v9;
					v29 = (data[4] >> 3) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v29)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					v10 = data[3];
					data[3] = data[12];
					data[12] = v10;
					v28 = (data[2] >> 2) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v28)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v27 = (data[9] >> 1) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v27)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v11 = data[6];
					data[6] = data[13];
					data[13] = v11;
					v26 = (data[6] >> 4) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v26)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					data[6] ^= 0x6Au;
					v12 = data[8] >> 7;
					data[8] *= 2;
					data[8] |= v12;
					v25 = (data[5] >> 1) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v25)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v24 = (data[8] >> 2) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v24)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					v13 = data[15];
					data[15] = data[1];
					data[1] = v13;
					data[12] ^= 0x51u;
				}
			}
			else
			{
				data[3] ^= 0x6Bu;
				v30 = (data[1] >> 3) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v30)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				v7 = data[5];
				data[5] = data[3];
				data[3] = v7;
				data[2] ^= 0xD6u;
				data[4] ^= 0x97u;
				v8 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v8;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			data[2] ^= 0x24u;
			v3 = *data;
			*data = data[3];
			data[3] = v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
			v33 = (data[3] >> 3) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v33)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v32 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v32)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v6 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v6;
			data[1] ^= 0x94u;
			data[1] ^= 0x45u;
			v31 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v31)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
		}
	}
}

void PacketTwister_57::Correct(BYTE *data, int length)
{
	char v3; // ST31_1@9
	char v4; // ST2C_1@21
	char v5; // ST2B_1@21
	char v6; // ST2A_1@21
	char v7; // ST27_1@29
	char v8; // ST26_1@29
	char v9; // ST23_1@37
	char v10; // ST1E_1@49
	char v11; // ST1B_1@55
	char v12; // ST16_1@67
	char v13; // ST13_1@73
	char v14; // ST10_1@80
	char v15; // ST0F_1@80
	char v16; // ST0C_1@86
	char v17; // ST09_1@92
	char v18; // ST08_1@92
	char v19; // [sp+4h] [bp-30h]@98
	char v20; // [sp+6h] [bp-2Eh]@92
	char v21; // [sp+Ah] [bp-2Ah]@86
	char v22; // [sp+Dh] [bp-27h]@80
	char v23; // [sp+11h] [bp-23h]@74
	char v24; // [sp+14h] [bp-20h]@67
	char v25; // [sp+17h] [bp-1Dh]@61
	char v26; // [sp+19h] [bp-1Bh]@55
	char v27; // [sp+1Ch] [bp-18h]@49
	char v28; // [sp+1Fh] [bp-15h]@43
	char v29; // [sp+21h] [bp-13h]@37
	char v30; // [sp+24h] [bp-10h]@29
	char v31; // [sp+28h] [bp-Ch]@21
	char v32; // [sp+2Dh] [bp-7h]@15
	char v33; // [sp+2Fh] [bp-5h]@9
	char v2; // [sp+32h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v23 = (data[26] >> 5) & 1;
					if ((data[26] >> 5) & 1)
						data[26] |= 0x20u;
					else
						data[26] &= 0xDFu;
					if (v23)
						data[26] |= 0x20u;
					else
						data[26] &= 0xDFu;
					v14 = data[4];
					data[4] = data[17];
					data[17] = v14;
					v15 = data[31];
					data[31] = data[18];
					data[18] = v15;
					v22 = (data[26] >> 6) & 1;
					if ((data[26] >> 1) & 1)
						data[26] |= 0x40u;
					else
						data[26] &= 0xBFu;
					if (v22)
						data[26] |= 2u;
					else
						data[26] &= 0xFDu;
					data[31] ^= 0xA3u;
					v16 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v16;
					v21 = (data[25] >> 5) & 1;
					if ((data[25] >> 2) & 1)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					if (v21)
						data[25] |= 4u;
					else
						data[25] &= 0xFBu;
					data[14] ^= 0x92u;
					v17 = data[28] >> 3;
					data[28] *= 32;
					data[28] |= v17;
					v18 = data[19];
					data[19] = data[19];
					data[19] = v18;
					data[3] ^= 0x7Bu;
					v20 = (data[20] >> 7) & 1;
					if ((data[20] >> 7) & 1)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					if (v20)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					v19 = (data[15] >> 7) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 0x80u;
					else
						data[15] &= 0x7Fu;
					if (v19)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
				}
				else
				{
					data[12] ^= 0x51u;
					v9 = data[15];
					data[15] = data[1];
					data[1] = v9;
					v29 = (data[8] >> 2) & 1;
					if ((data[8] >> 4) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v29)
						data[8] |= 0x10u;
					else
						data[8] &= 0xEFu;
					v28 = (data[5] >> 1) & 1;
					if ((data[5] >> 7) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v28)
						data[5] |= 0x80u;
					else
						data[5] &= 0x7Fu;
					v10 = data[8] >> 1;
					data[8] <<= 7;
					data[8] |= v10;
					data[6] ^= 0x6Au;
					v27 = (data[6] >> 4) & 1;
					if ((data[6] >> 1) & 1)
						data[6] |= 0x10u;
					else
						data[6] &= 0xEFu;
					if (v27)
						data[6] |= 2u;
					else
						data[6] &= 0xFDu;
					v11 = data[6];
					data[6] = data[13];
					data[13] = v11;
					v26 = (data[9] >> 1) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v26)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v25 = (data[2] >> 2) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v25)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v12 = data[3];
					data[3] = data[12];
					data[12] = v12;
					v24 = (data[4] >> 3) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v24)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					v13 = *data >> 1;
					*data <<= 7;
					*data |= v13;
				}
			}
			else
			{
				v7 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v7;
				data[4] ^= 0x97u;
				data[2] ^= 0xD6u;
				v8 = data[5];
				data[5] = data[3];
				data[3] = v8;
				v30 = (data[1] >> 3) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v30)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				data[3] ^= 0x6Bu;
			}
		}
		else
		{
			v2 = (data[3] >> 5) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x20u;
			else
				data[3] &= 0xDFu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			data[1] ^= 0x45u;
			data[1] ^= 0x94u;
			v3 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v3;
			v33 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v33)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v32 = (data[3] >> 3) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v32)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v4 = data[1];
			data[1] = *data;
			*data = v4;
			v5 = data[1];
			data[1] = data[3];
			data[3] = v5;
			v6 = *data;
			*data = data[3];
			data[3] = v6;
			data[2] ^= 0x24u;
			v31 = (data[1] >> 2) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v31)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
		}
	}
}

void PacketTwister_58::Twist(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST1B_1@3
	char v8; // ST1A_1@3
	char v9; // ST19_1@3
	char v10; // ST18_1@3
	char v11; // ST0F_1@29
	char v12; // ST0C_1@35
	char v13; // ST0B_1@35
	char v14; // ST0A_1@35
	char v15; // ST09_1@37
	char v16; // ST08_1@37
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // [sp+Dh] [bp-13h]@29
	char v20; // [sp+10h] [bp-10h]@23
	char v21; // [sp+12h] [bp-Eh]@17
	char v22; // [sp+14h] [bp-Ch]@9
	char v2; // [sp+16h] [bp-Ah]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[13];
					data[13] = data[31];
					data[31] = v17;
					data[31] ^= 0xCDu;
					v18 = data[25];
					data[25] = data[27];
					data[27] = v18;
				}
				else
				{
					data[12] ^= 0x9Au;
					data[7] ^= 0xC1u;
					data[8] ^= 0xF9u;
					*data ^= 0x9Du;
					v15 = data[4];
					data[4] = data[3];
					data[3] = v15;
					data[6] ^= 0xC3u;
					v16 = data[11];
					data[11] = data[11];
					data[11] = v16;
				}
			}
			else
			{
				v21 = (data[7] >> 1) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v21)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v20 = (data[3] >> 7) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v20)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				data[3] ^= 0xA1u;
				*data ^= 0x5Au;
				v11 = data[6] >> 5;
				data[6] *= 8;
				data[6] |= v11;
				v19 = (data[3] >> 5) & 1;
				if ((data[3] >> 5) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v19)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				v12 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v12;
				v13 = data[5] >> 7;
				data[5] *= 2;
				data[5] |= v13;
				data[7] ^= 0x53u;
				v14 = data[3];
				data[3] = data[4];
				data[4] = v14;
			}
		}
		else
		{
			data[3] ^= 0x87u;
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v4;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
			data[2] ^= 0x44u;
			v6 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
			v8 = data[2];
			data[2] = *data;
			*data = v8;
			v9 = data[3];
			data[3] = data[2];
			data[2] = v9;
			v10 = *data;
			*data = data[3];
			data[3] = v10;
			data[2] ^= 0x50u;
			v2 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v22 = (data[1] >> 3) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v22)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
		}
	}
}

void PacketTwister_58::Correct(BYTE *data, int length)
{
	char v3; // ST1B_1@15
	char v4; // ST1A_1@15
	char v5; // ST19_1@15
	char v6; // ST18_1@15
	char v7; // ST17_1@15
	char v8; // ST16_1@15
	char v9; // ST15_1@15
	char v10; // ST14_1@15
	char v11; // ST13_1@17
	char v12; // ST12_1@17
	char v13; // ST11_1@17
	char v14; // ST0E_1@23
	char v15; // ST09_1@37
	char v16; // ST08_1@37
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // [sp+Ah] [bp-16h]@29
	char v20; // [sp+Ch] [bp-14h]@23
	char v21; // [sp+Fh] [bp-11h]@17
	char v22; // [sp+1Ch] [bp-4h]@9
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[25];
					data[25] = data[27];
					data[27] = v17;
					data[31] ^= 0xCDu;
					v18 = data[13];
					data[13] = data[31];
					data[31] = v18;
				}
				else
				{
					v15 = data[11];
					data[11] = data[11];
					data[11] = v15;
					data[6] ^= 0xC3u;
					v16 = data[4];
					data[4] = data[3];
					data[3] = v16;
					*data ^= 0x9Du;
					data[8] ^= 0xF9u;
					data[7] ^= 0xC1u;
					data[12] ^= 0x9Au;
				}
			}
			else
			{
				v11 = data[3];
				data[3] = data[4];
				data[4] = v11;
				data[7] ^= 0x53u;
				v12 = data[5] >> 1;
				data[5] <<= 7;
				data[5] |= v12;
				v13 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v13;
				v21 = (data[3] >> 5) & 1;
				if ((data[3] >> 5) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v21)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				v14 = data[6] >> 3;
				data[6] *= 32;
				data[6] |= v14;
				*data ^= 0x5Au;
				data[3] ^= 0xA1u;
				v20 = (data[3] >> 7) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				if (v20)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v19 = (data[7] >> 1) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v19)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
			}
		}
		else
		{
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v22 = (data[1] >> 1) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v22)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			data[2] ^= 0x50u;
			v3 = *data;
			*data = data[3];
			data[3] = v3;
			v4 = data[3];
			data[3] = data[2];
			data[2] = v4;
			v5 = data[2];
			data[2] = *data;
			*data = v5;
			v6 = data[2];
			data[2] = *data;
			*data = v6;
			v7 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v7;
			data[2] ^= 0x44u;
			v8 = data[1];
			data[1] = *data;
			*data = v8;
			v9 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v9;
			v10 = data[3];
			data[3] = *data;
			*data = v10;
			data[3] ^= 0x87u;
		}
	}
}

void PacketTwister_59::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1D_1@15
	char v6; // ST1C_1@17
	char v7; // ST17_1@29
	char v8; // ST16_1@29
	char v9; // ST15_1@29
	char v10; // ST14_1@29
	char v11; // ST13_1@29
	char v12; // ST0E_1@43
	char v13; // ST0D_1@43
	char v14; // ST0C_1@43
	char v15; // ST09_1@50
	char v16; // ST08_1@50
	char v17; // ST07_1@50
	char v18; // ST06_1@50
	char v19; // [sp+Ah] [bp-1Ah]@43
	char v20; // [sp+Fh] [bp-15h]@37
	char v21; // [sp+11h] [bp-13h]@29
	char v22; // [sp+18h] [bp-Ch]@23
	char v23; // [sp+1Ah] [bp-Ah]@17
	char v24; // [sp+1Eh] [bp-6h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[20] >> 1;
					data[20] <<= 7;
					data[20] |= v15;
					v16 = data[14] >> 6;
					data[14] *= 4;
					data[14] |= v16;
					data[7] ^= 0x87u;
					v17 = data[17] >> 1;
					data[17] <<= 7;
					data[17] |= v17;
					v18 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v18;
				}
				else
				{
					v20 = (data[4] >> 7) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v20)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
					data[7] ^= 0xB6u;
					v12 = data[12];
					data[12] = data[6];
					data[6] = v12;
					v13 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v13;
					v14 = data[9];
					data[9] = data[7];
					data[7] = v14;
					v19 = (data[9] >> 7) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x80u;
					else
						data[9] &= 0x7Fu;
					if (v19)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					data[4] ^= 0x5Bu;
				}
			}
			else
			{
				data[1] ^= 0x9Du;
				data[7] ^= 0xC2u;
				v6 = data[3] >> 1;
				data[3] <<= 7;
				data[3] |= v6;
				v23 = (data[6] >> 2) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v23)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
				v22 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v22)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v7 = *data;
				*data = data[2];
				data[2] = v7;
				v8 = data[7];
				data[7] = data[2];
				data[2] = v8;
				v9 = *data >> 2;
				*data <<= 6;
				*data |= v9;
				v10 = data[5];
				data[5] = data[6];
				data[6] = v10;
				v11 = data[5];
				data[5] = data[6];
				data[6] = v11;
				v21 = (data[4] >> 1) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v21)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
			}
		}
		else
		{
			data[3] ^= 0x4Fu;
			v3 = data[2];
			data[2] = *data;
			*data = v3;
			v2 = (*data >> 1) & 1;
			if ((*data >> 3) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 8u;
			else
				*data &= 0xF7u;
			data[3] ^= 0x2Eu;
			v4 = data[3];
			data[3] = data[2];
			data[2] = v4;
			v24 = (*data >> 3) & 1;
			if ((*data >> 1) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v24)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v5 = data[3];
			data[3] = *data;
			*data = v5;
		}
	}
}

void PacketTwister_59::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1D_1@15
	char v6; // ST1A_1@23
	char v7; // ST19_1@23
	char v8; // ST18_1@23
	char v9; // ST17_1@23
	char v10; // ST16_1@23
	char v11; // ST11_1@35
	char v12; // ST0E_1@43
	char v13; // ST0D_1@43
	char v14; // ST0C_1@43
	char v15; // ST09_1@50
	char v16; // ST08_1@50
	char v17; // ST07_1@50
	char v18; // ST06_1@50
	char v19; // [sp+Ah] [bp-1Ah]@43
	char v20; // [sp+Fh] [bp-15h]@37
	char v21; // [sp+12h] [bp-12h]@29
	char v22; // [sp+14h] [bp-10h]@23
	char v23; // [sp+1Bh] [bp-9h]@17
	char v24; // [sp+1Eh] [bp-6h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[17] >> 2;
					data[17] <<= 6;
					data[17] |= v15;
					v16 = data[17] >> 7;
					data[17] *= 2;
					data[17] |= v16;
					data[7] ^= 0x87u;
					v17 = data[14] >> 2;
					data[14] <<= 6;
					data[14] |= v17;
					v18 = data[20] >> 7;
					data[20] *= 2;
					data[20] |= v18;
				}
				else
				{
					data[4] ^= 0x5Bu;
					v20 = (data[9] >> 7) & 1;
					if ((data[9] >> 1) & 1)
						data[9] |= 0x80u;
					else
						data[9] &= 0x7Fu;
					if (v20)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					v12 = data[9];
					data[9] = data[7];
					data[7] = v12;
					v13 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v13;
					v14 = data[12];
					data[12] = data[6];
					data[6] = v14;
					data[7] ^= 0xB6u;
					v19 = (data[4] >> 7) & 1;
					if ((data[4] >> 1) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v19)
						data[4] |= 2u;
					else
						data[4] &= 0xFDu;
				}
			}
			else
			{
				v23 = (data[4] >> 1) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v23)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				v6 = data[5];
				data[5] = data[6];
				data[6] = v6;
				v7 = data[5];
				data[5] = data[6];
				data[6] = v7;
				v8 = *data >> 6;
				*data *= 4;
				*data |= v8;
				v9 = data[7];
				data[7] = data[2];
				data[2] = v9;
				v10 = *data;
				*data = data[2];
				data[2] = v10;
				v22 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v22)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v21 = (data[6] >> 2) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 4u;
				else
					data[6] &= 0xFBu;
				if (v21)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
				v11 = data[3] >> 7;
				data[3] *= 2;
				data[3] |= v11;
				data[7] ^= 0xC2u;
				data[1] ^= 0x9Du;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v2 = (*data >> 3) & 1;
			if ((*data >> 1) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v4 = data[3];
			data[3] = data[2];
			data[2] = v4;
			data[3] ^= 0x2Eu;
			v24 = (*data >> 1) & 1;
			if ((*data >> 3) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v24)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v5 = data[2];
			data[2] = *data;
			*data = v5;
			data[3] ^= 0x4Fu;
		}
	}
}

void PacketTwister_60::Twist(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST19_1@3
	char v6; // ST16_1@9
	char v7; // ST15_1@9
	char v8; // ST14_1@11
	char v9; // ST13_1@11
	char v10; // ST10_1@19
	char v11; // ST0F_1@19
	char v12; // ST0E_1@19
	char v13; // ST0D_1@19
	char v14; // ST0C_1@19
	char v15; // ST0B_1@20
	char v16; // [sp+7h] [bp-15h]@26
	char v17; // [sp+9h] [bp-13h]@20
	char v18; // [sp+11h] [bp-Bh]@11
	char v2; // [sp+17h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[23] >> 5;
					data[23] *= 8;
					data[23] |= v15;
					v17 = (data[5] >> 2) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v17)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					v16 = (data[24] >> 3) & 1;
					if ((data[24] >> 1) & 1)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					if (v16)
						data[24] |= 2u;
					else
						data[24] &= 0xFDu;
					data[19] ^= 4u;
					data[12] ^= 0xB3u;
					*data ^= 0xC4u;
					data[3] ^= 0x39u;
				}
				else
				{
					v10 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v10;
					data[4] ^= 0xE3u;
					v11 = data[4] >> 3;
					data[4] *= 32;
					data[4] |= v11;
					data[4] ^= 0xE0u;
					v12 = data[7];
					data[7] = data[1];
					data[1] = v12;
					v13 = data[4] >> 2;
					data[4] <<= 6;
					data[4] |= v13;
					v14 = data[11];
					data[11] = data[6];
					data[6] = v14;
					data[14] ^= 0x68u;
				}
			}
			else
			{
				data[7] ^= 0xC2u;
				data[1] ^= 0xEAu;
				v8 = data[3];
				data[3] = data[7];
				data[7] = v8;
				v9 = data[2];
				data[2] = data[1];
				data[1] = v9;
				v18 = (*data >> 1) & 1;
				if ((*data >> 2) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v18)
					*data |= 4u;
				else
					*data &= 0xFBu;
			}
		}
		else
		{
			data[1] ^= 0xD0u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = data[1];
			data[1] = data[1];
			data[1] = v4;
			data[3] ^= 0xE8u;
			v5 = data[2];
			data[2] = data[1];
			data[1] = v5;
			v2 = (*data >> 4) & 1;
			if ((*data >> 5) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			v6 = data[1];
			data[1] = data[2];
			data[2] = v6;
			v7 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v7;
			*data ^= 0xEEu;
		}
	}
}

void PacketTwister_60::Correct(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST17_1@9
	char v6; // ST16_1@9
	char v7; // ST15_1@9
	char v8; // ST12_1@17
	char v9; // ST11_1@17
	char v10; // ST10_1@19
	char v11; // ST0F_1@19
	char v12; // ST0E_1@19
	char v13; // ST0D_1@19
	char v14; // ST0C_1@19
	char v15; // ST07_1@32
	char v16; // [sp+8h] [bp-14h]@26
	char v17; // [sp+Ah] [bp-12h]@20
	char v18; // [sp+13h] [bp-9h]@11
	char v2; // [sp+18h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[3] ^= 0x39u;
					*data ^= 0xC4u;
					data[12] ^= 0xB3u;
					data[19] ^= 4u;
					v17 = (data[24] >> 3) & 1;
					if ((data[24] >> 1) & 1)
						data[24] |= 8u;
					else
						data[24] &= 0xF7u;
					if (v17)
						data[24] |= 2u;
					else
						data[24] &= 0xFDu;
					v16 = (data[5] >> 2) & 1;
					if ((data[5] >> 2) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v16)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					v15 = data[23] >> 3;
					data[23] *= 32;
					data[23] |= v15;
				}
				else
				{
					data[14] ^= 0x68u;
					v10 = data[11];
					data[11] = data[6];
					data[6] = v10;
					v11 = data[4] >> 6;
					data[4] *= 4;
					data[4] |= v11;
					v12 = data[7];
					data[7] = data[1];
					data[1] = v12;
					data[4] ^= 0xE0u;
					v13 = data[4] >> 5;
					data[4] *= 8;
					data[4] |= v13;
					data[4] ^= 0xE3u;
					v14 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v14;
				}
			}
			else
			{
				v18 = (*data >> 1) & 1;
				if ((*data >> 2) & 1)
					*data |= 2u;
				else
					*data &= 0xFDu;
				if (v18)
					*data |= 4u;
				else
					*data &= 0xFBu;
				v8 = data[2];
				data[2] = data[1];
				data[1] = v8;
				v9 = data[3];
				data[3] = data[7];
				data[7] = v9;
				data[1] ^= 0xEAu;
				data[7] ^= 0xC2u;
			}
		}
		else
		{
			*data ^= 0xEEu;
			v3 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v3;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
			v2 = (*data >> 4) & 1;
			if ((*data >> 5) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			v5 = data[2];
			data[2] = data[1];
			data[1] = v5;
			data[3] ^= 0xE8u;
			v6 = data[1];
			data[1] = data[1];
			data[1] = v6;
			v7 = data[1];
			data[1] = data[2];
			data[2] = v7;
			data[1] ^= 0xD0u;
		}
	}
}

void PacketTwister_61::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST20_1@3
	char v7; // ST1F_1@3
	char v8; // ST1E_1@5
	char v9; // ST1D_1@5
	char v10; // ST1C_1@5
	char v11; // ST1B_1@5
	char v12; // ST1A_1@5
	char v13; // ST19_1@5
	char v14; // ST16_1@11
	char v15; // ST15_1@13
	char v16; // ST12_1@19
	char v17; // ST11_1@19
	char v18; // ST10_1@19
	char v19; // ST0F_1@19
	char v20; // ST0E_1@20
	char v21; // ST0B_1@26
	char v22; // ST0A_1@26
	char v23; // ST09_1@26
	char v24; // ST08_1@26
	char v25; // [sp+6h] [bp-1Eh]@26
	char v26; // [sp+Ch] [bp-18h]@20
	char v27; // [sp+13h] [bp-11h]@13
	char v2; // [sp+17h] [bp-Dh]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[24] ^= 0xF5u;
					v20 = data[10];
					data[10] = data[31];
					data[31] = v20;
					v26 = (data[31] >> 2) & 1;
					if ((data[31] >> 1) & 1)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
					if (v26)
						data[31] |= 2u;
					else
						data[31] &= 0xFDu;
					v21 = data[24];
					data[24] = data[8];
					data[8] = v21;
					v22 = data[17] >> 7;
					data[17] *= 2;
					data[17] |= v22;
					*data ^= 0x82u;
					data[27] ^= 0x75u;
					v23 = data[25] >> 4;
					data[25] *= 16;
					data[25] |= v23;
					v24 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v24;
					v25 = (data[13] >> 2) & 1;
					if ((data[13] >> 2) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v25)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					data[9] ^= 0x7Bu;
					data[25] ^= 0x1Du;
				}
				else
				{
					data[7] ^= 0x73u;
					v15 = data[10];
					data[10] = data[13];
					data[13] = v15;
					data[15] ^= 0x43u;
					v27 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v27)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v16 = data[11] >> 6;
					data[11] *= 4;
					data[11] |= v16;
					v17 = data[11] >> 5;
					data[11] *= 8;
					data[11] |= v17;
					v18 = data[15];
					data[15] = data[9];
					data[9] = v18;
					v19 = data[11];
					data[11] = data[13];
					data[13] = v19;
					data[1] ^= 0xCDu;
				}
			}
			else
			{
				data[1] ^= 0x1Eu;
				v8 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v8;
				v9 = *data >> 5;
				*data *= 8;
				*data |= v9;
				v10 = *data;
				*data = data[4];
				data[4] = v10;
				v11 = data[5];
				data[5] = data[6];
				data[6] = v11;
				v12 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v12;
				v13 = data[6];
				data[6] = data[6];
				data[6] = v13;
				v2 = (data[5] >> 4) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v2)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v14 = data[5];
				data[5] = data[5];
				data[5] = v14;
				data[7] ^= 0xF9u;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[3];
			data[3] = v3;
			v4 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v4;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v6 = *data;
			*data = *data;
			*data = v6;
			v7 = data[2];
			data[2] = data[1];
			data[1] = v7;
		}
	}
}

void PacketTwister_61::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST20_1@3
	char v7; // ST1F_1@3
	char v8; // ST1E_1@5
	char v9; // ST1B_1@11
	char v10; // ST1A_1@11
	char v11; // ST19_1@11
	char v12; // ST18_1@11
	char v13; // ST17_1@11
	char v14; // ST16_1@11
	char v15; // ST15_1@13
	char v16; // ST14_1@13
	char v17; // ST13_1@13
	char v18; // ST12_1@13
	char v19; // ST0F_1@19
	char v20; // ST0C_1@26
	char v21; // ST0B_1@26
	char v22; // ST0A_1@26
	char v23; // ST09_1@26
	char v24; // ST06_1@32
	char v25; // [sp+7h] [bp-1Dh]@26
	char v26; // [sp+Dh] [bp-17h]@20
	char v27; // [sp+10h] [bp-14h]@13
	char v2; // [sp+1Ch] [bp-8h]@5

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[25] ^= 0x1Du;
					data[9] ^= 0x7Bu;
					v26 = (data[13] >> 2) & 1;
					if ((data[13] >> 2) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v26)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					v20 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v20;
					v21 = data[25] >> 4;
					data[25] *= 16;
					data[25] |= v21;
					data[27] ^= 0x75u;
					*data ^= 0x82u;
					v22 = data[17] >> 1;
					data[17] <<= 7;
					data[17] |= v22;
					v23 = data[24];
					data[24] = data[8];
					data[8] = v23;
					v25 = (data[31] >> 2) & 1;
					if ((data[31] >> 1) & 1)
						data[31] |= 4u;
					else
						data[31] &= 0xFBu;
					if (v25)
						data[31] |= 2u;
					else
						data[31] &= 0xFDu;
					v24 = data[10];
					data[10] = data[31];
					data[31] = v24;
					data[24] ^= 0xF5u;
				}
				else
				{
					data[1] ^= 0xCDu;
					v15 = data[11];
					data[11] = data[13];
					data[13] = v15;
					v16 = data[15];
					data[15] = data[9];
					data[9] = v16;
					v17 = data[11] >> 3;
					data[11] *= 32;
					data[11] |= v17;
					v18 = data[11] >> 2;
					data[11] <<= 6;
					data[11] |= v18;
					v27 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v27)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					data[15] ^= 0x43u;
					v19 = data[10];
					data[10] = data[13];
					data[13] = v19;
					data[7] ^= 0x73u;
				}
			}
			else
			{
				data[7] ^= 0xF9u;
				v8 = data[5];
				data[5] = data[5];
				data[5] = v8;
				v2 = (data[5] >> 4) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v2)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v9 = data[6];
				data[6] = data[6];
				data[6] = v9;
				v10 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v10;
				v11 = data[5];
				data[5] = data[6];
				data[6] = v11;
				v12 = *data;
				*data = data[4];
				data[4] = v12;
				v13 = *data >> 3;
				*data *= 32;
				*data |= v13;
				v14 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v14;
				data[1] ^= 0x1Eu;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = *data;
			*data = *data;
			*data = v4;
			v5 = data[1];
			data[1] = data[2];
			data[2] = v5;
			v6 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v6;
			v7 = data[1];
			data[1] = data[3];
			data[3] = v7;
		}
	}
}

void PacketTwister_62::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2E_1@3
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST28_1@9
	char v9; // ST27_1@9
	char v10; // ST26_1@9
	char v11; // ST23_1@17
	char v12; // ST22_1@17
	char v13; // ST1F_1@23
	char v14; // ST1E_1@23
	char v15; // ST1D_1@23
	char v16; // ST1C_1@23
	char v17; // ST1B_1@23
	char v18; // ST1A_1@23
	char v19; // ST17_1@31
	char v20; // ST14_1@37
	char v21; // ST13_1@37
	char v22; // ST10_1@43
	char v23; // ST0D_1@49
	char v24; // [sp+7h] [bp-29h]@62
	char v25; // [sp+9h] [bp-27h]@56
	char v26; // [sp+Bh] [bp-25h]@49
	char v27; // [sp+Eh] [bp-22h]@43
	char v28; // [sp+11h] [bp-1Fh]@37
	char v29; // [sp+15h] [bp-1Bh]@31
	char v30; // [sp+18h] [bp-18h]@23
	char v31; // [sp+20h] [bp-10h]@17
	char v32; // [sp+24h] [bp-Ch]@9
	char v2; // [sp+2Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[3] ^= 0x9Du;
					data[14] ^= 0x36u;
					v25 = (data[15] >> 3) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					if (v25)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v24 = (data[25] >> 5) & 1;
					if ((data[25] >> 1) & 1)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					if (v24)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
				}
				else
				{
					v19 = data[12] >> 6;
					data[12] *= 4;
					data[12] |= v19;
					v29 = (data[8] >> 5) & 1;
					if ((data[8] >> 6) & 1)
						data[8] |= 0x20u;
					else
						data[8] &= 0xDFu;
					if (v29)
						data[8] |= 0x40u;
					else
						data[8] &= 0xBFu;
					v20 = data[2] >> 7;
					data[2] *= 2;
					data[2] |= v20;
					v21 = data[14] >> 6;
					data[14] *= 4;
					data[14] |= v21;
					v28 = (data[2] >> 7) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v28)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					v22 = data[4];
					data[4] = data[1];
					data[1] = v22;
					data[15] ^= 0xE4u;
					v27 = (*data >> 1) & 1;
					if ((*data >> 5) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v27)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					v23 = data[12] >> 5;
					data[12] *= 8;
					data[12] |= v23;
					v26 = (data[11] >> 1) & 1;
					if ((data[11] >> 2) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v26)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
				}
			}
			else
			{
				v11 = data[3];
				data[3] = data[3];
				data[3] = v11;
				v12 = data[2];
				data[2] = *data;
				*data = v12;
				v31 = (*data >> 6) & 1;
				if ((*data >> 4) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v31)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				v13 = data[4];
				data[4] = data[5];
				data[5] = v13;
				v14 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v14;
				*data ^= 0x7Du;
				data[6] ^= 0xAFu;
				v15 = data[3];
				data[3] = data[2];
				data[2] = v15;
				v16 = data[2] >> 7;
				data[2] *= 2;
				data[2] |= v16;
				v17 = data[1] >> 4;
				data[1] *= 16;
				data[1] |= v17;
				v18 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v18;
				v30 = (data[5] >> 1) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v30)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
			}
		}
		else
		{
			v3 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v3;
			v4 = data[3];
			data[3] = data[2];
			data[2] = v4;
			v2 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v5 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v5;
			v6 = *data;
			*data = data[3];
			data[3] = v6;
			v7 = *data >> 6;
			*data *= 4;
			*data |= v7;
			v8 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v8;
			v9 = data[2];
			data[2] = data[2];
			data[2] = v9;
			data[2] ^= 0xD2u;
			v10 = data[1] >> 3;
			data[1] *= 32;
			data[1] |= v10;
			v32 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v32)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
		}
	}
}

void PacketTwister_62::Correct(BYTE *data, int length)
{
	char v3; // ST2D_1@9
	char v4; // ST2C_1@9
	char v5; // ST2B_1@9
	char v6; // ST2A_1@9
	char v7; // ST29_1@9
	char v8; // ST28_1@9
	char v9; // ST25_1@15
	char v10; // ST24_1@15
	char v11; // ST21_1@23
	char v12; // ST20_1@23
	char v13; // ST1F_1@23
	char v14; // ST1E_1@23
	char v15; // ST1D_1@23
	char v16; // ST1C_1@23
	char v17; // ST19_1@29
	char v18; // ST18_1@29
	char v19; // ST15_1@37
	char v20; // ST12_1@43
	char v21; // ST0F_1@49
	char v22; // ST0E_1@49
	char v23; // ST0B_1@55
	char v24; // [sp+7h] [bp-29h]@62
	char v25; // [sp+9h] [bp-27h]@56
	char v26; // [sp+Ch] [bp-24h]@49
	char v27; // [sp+10h] [bp-20h]@43
	char v28; // [sp+13h] [bp-1Dh]@37
	char v29; // [sp+16h] [bp-1Ah]@31
	char v30; // [sp+1Ah] [bp-16h]@23
	char v31; // [sp+22h] [bp-Eh]@17
	char v32; // [sp+26h] [bp-Ah]@9
	char v2; // [sp+2Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v25 = (data[25] >> 5) & 1;
					if ((data[25] >> 1) & 1)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					if (v25)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
					v24 = (data[15] >> 3) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					if (v24)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					data[14] ^= 0x36u;
					data[3] ^= 0x9Du;
				}
				else
				{
					v29 = (data[11] >> 1) & 1;
					if ((data[11] >> 2) & 1)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					if (v29)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					v19 = data[12] >> 3;
					data[12] *= 32;
					data[12] |= v19;
					v28 = (*data >> 1) & 1;
					if ((*data >> 5) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v28)
						*data |= 0x20u;
					else
						*data &= 0xDFu;
					data[15] ^= 0xE4u;
					v20 = data[4];
					data[4] = data[1];
					data[1] = v20;
					v27 = (data[2] >> 7) & 1;
					if ((data[2] >> 6) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v27)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					v21 = data[14] >> 2;
					data[14] <<= 6;
					data[14] |= v21;
					v22 = data[2] >> 1;
					data[2] <<= 7;
					data[2] |= v22;
					v26 = (data[8] >> 5) & 1;
					if ((data[8] >> 6) & 1)
						data[8] |= 0x20u;
					else
						data[8] &= 0xDFu;
					if (v26)
						data[8] |= 0x40u;
					else
						data[8] &= 0xBFu;
					v23 = data[12] >> 2;
					data[12] <<= 6;
					data[12] |= v23;
				}
			}
			else
			{
				v31 = (data[5] >> 1) & 1;
				if ((data[5] >> 2) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v31)
					data[5] |= 4u;
				else
					data[5] &= 0xFBu;
				v11 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v11;
				v12 = data[1] >> 4;
				data[1] *= 16;
				data[1] |= v12;
				v13 = data[2] >> 1;
				data[2] <<= 7;
				data[2] |= v13;
				v14 = data[3];
				data[3] = data[2];
				data[2] = v14;
				data[6] ^= 0xAFu;
				*data ^= 0x7Du;
				v15 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v15;
				v16 = data[4];
				data[4] = data[5];
				data[5] = v16;
				v30 = (*data >> 6) & 1;
				if ((*data >> 4) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v30)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				v17 = data[2];
				data[2] = *data;
				*data = v17;
				v18 = data[3];
				data[3] = data[3];
				data[3] = v18;
			}
		}
		else
		{
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v3 = data[1] >> 5;
			data[1] *= 8;
			data[1] |= v3;
			data[2] ^= 0xD2u;
			v4 = data[2];
			data[2] = data[2];
			data[2] = v4;
			v5 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v5;
			v6 = *data >> 2;
			*data <<= 6;
			*data |= v6;
			v7 = *data;
			*data = data[3];
			data[3] = v7;
			v8 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v8;
			v32 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v32)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v9 = data[3];
			data[3] = data[2];
			data[2] = v9;
			v10 = data[3] >> 4;
			data[3] *= 16;
			data[3] |= v10;
		}
	}
}

void PacketTwister_63::Twist(BYTE *data, int length)
{
	char v3; // ST37_1@3
	char v4; // ST36_1@3
	char v5; // ST33_1@9
	char v6; // ST32_1@9
	char v7; // ST31_1@9
	char v8; // ST2E_1@15
	char v9; // ST2B_1@23
	char v10; // ST28_1@29
	char v11; // ST25_1@35
	char v12; // ST24_1@35
	char v13; // ST1F_1@49
	char v14; // ST1E_1@49
	char v15; // ST13_1@79
	char v16; // ST10_1@86
	char v17; // ST0D_1@92
	char v18; // ST0C_1@92
	char v19; // ST0B_1@92
	char v20; // ST0A_1@92
	char v21; // ST07_1@98
	char v22; // ST06_1@98
	char v23; // [sp+8h] [bp-30h]@92
	char v24; // [sp+Eh] [bp-2Ah]@86
	char v25; // [sp+11h] [bp-27h]@80
	char v26; // [sp+14h] [bp-24h]@73
	char v27; // [sp+16h] [bp-22h]@67
	char v28; // [sp+18h] [bp-20h]@61
	char v29; // [sp+1Ah] [bp-1Eh]@55
	char v30; // [sp+1Ch] [bp-1Ch]@49
	char v31; // [sp+20h] [bp-18h]@43
	char v32; // [sp+22h] [bp-16h]@35
	char v33; // [sp+26h] [bp-12h]@29
	char v34; // [sp+29h] [bp-Fh]@23
	char v35; // [sp+2Ch] [bp-Ch]@17
	char v36; // [sp+2Fh] [bp-9h]@9
	char v2; // [sp+34h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v25 = (data[12] >> 4) & 1;
					if ((data[12] >> 2) & 1)
						data[12] |= 0x10u;
					else
						data[12] &= 0xEFu;
					if (v25)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					v16 = data[2];
					data[2] = data[9];
					data[9] = v16;
					v24 = (data[10] >> 2) & 1;
					if ((data[10] >> 6) & 1)
						data[10] |= 4u;
					else
						data[10] &= 0xFBu;
					if (v24)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					v17 = data[8];
					data[8] = data[23];
					data[23] = v17;
					v18 = data[29] >> 6;
					data[29] *= 4;
					data[29] |= v18;
					v19 = data[25];
					data[25] = data[8];
					data[8] = v19;
					v20 = data[1] >> 1;
					data[1] <<= 7;
					data[1] |= v20;
					v23 = (data[15] >> 2) & 1;
					if ((data[15] >> 2) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v23)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					data[6] ^= 0x90u;
					v21 = data[16];
					data[16] = data[17];
					data[17] = v21;
					v22 = data[27] >> 3;
					data[27] *= 32;
					data[27] |= v22;
					data[13] ^= 0x81u;
				}
				else
				{
					v31 = (data[7] >> 3) & 1;
					if ((data[7] >> 6) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v31)
						data[7] |= 0x40u;
					else
						data[7] &= 0xBFu;
					v13 = *data;
					*data = data[14];
					data[14] = v13;
					v14 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v14;
					v30 = (data[6] >> 3) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					if (v30)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					v29 = (data[5] >> 4) & 1;
					if ((data[5] >> 5) & 1)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					if (v29)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					v28 = (data[12] >> 6) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v28)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					data[5] ^= 0xA9u;
					v27 = (data[4] >> 3) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v27)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					data[15] ^= 0x38u;
					*data ^= 0xE8u;
					v26 = (data[5] >> 1) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v26)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					data[7] ^= 0x50u;
					v15 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v15;
				}
			}
			else
			{
				data[2] ^= 0x98u;
				v35 = (data[5] >> 4) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v35)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				v9 = data[2];
				data[2] = data[5];
				data[5] = v9;
				v34 = (data[7] >> 7) & 1;
				if ((data[7] >> 5) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v34)
					data[7] |= 0x20u;
				else
					data[7] &= 0xDFu;
				v10 = data[3] >> 5;
				data[3] *= 8;
				data[3] |= v10;
				v33 = (data[3] >> 5) & 1;
				if ((data[3] >> 7) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v33)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				data[4] ^= 0x41u;
				data[3] ^= 0x44u;
				v11 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v11;
				v12 = data[2] >> 4;
				data[2] *= 16;
				data[2] |= v12;
				v32 = (*data >> 6) & 1;
				if ((*data >> 1) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v32)
					*data |= 2u;
				else
					*data &= 0xFDu;
			}
		}
		else
		{
			data[1] ^= 0xFCu;
			v3 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v3;
			v4 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v4;
			*data ^= 0xFDu;
			v2 = (data[3] >> 3) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v5 = data[3];
			data[3] = data[2];
			data[2] = v5;
			v6 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v6;
			v7 = data[1];
			data[1] = data[2];
			data[2] = v7;
			v36 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v36)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v8 = data[1];
			data[1] = data[2];
			data[2] = v8;
			data[3] ^= 0x66u;
			data[1] ^= 0xB2u;
		}
	}
}

void PacketTwister_63::Correct(BYTE *data, int length)
{
	char v3; // ST37_1@3
	char v4; // ST34_1@9
	char v5; // ST33_1@9
	char v6; // ST32_1@9
	char v7; // ST2F_1@15
	char v8; // ST2E_1@15
	char v9; // ST2B_1@23
	char v10; // ST2A_1@23
	char v11; // ST27_1@29
	char v12; // ST24_1@35
	char v13; // ST21_1@43
	char v14; // ST16_1@73
	char v15; // ST15_1@73
	char v16; // ST12_1@80
	char v17; // ST11_1@80
	char v18; // ST0E_1@86
	char v19; // ST0D_1@86
	char v20; // ST0C_1@86
	char v21; // ST0B_1@86
	char v22; // ST08_1@92
	char v23; // [sp+6h] [bp-32h]@92
	char v24; // [sp+9h] [bp-2Fh]@86
	char v25; // [sp+Fh] [bp-29h]@80
	char v26; // [sp+13h] [bp-25h]@73
	char v27; // [sp+17h] [bp-21h]@67
	char v28; // [sp+19h] [bp-1Fh]@61
	char v29; // [sp+1Bh] [bp-1Dh]@55
	char v30; // [sp+1Dh] [bp-1Bh]@49
	char v31; // [sp+1Fh] [bp-19h]@43
	char v32; // [sp+22h] [bp-16h]@35
	char v33; // [sp+25h] [bp-13h]@29
	char v34; // [sp+28h] [bp-10h]@23
	char v35; // [sp+2Ch] [bp-Ch]@17
	char v36; // [sp+30h] [bp-8h]@9
	char v2; // [sp+35h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[13] ^= 0x81u;
					v16 = data[27] >> 5;
					data[27] *= 8;
					data[27] |= v16;
					v17 = data[16];
					data[16] = data[17];
					data[17] = v17;
					data[6] ^= 0x90u;
					v25 = (data[15] >> 2) & 1;
					if ((data[15] >> 2) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v25)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					v18 = data[1] >> 7;
					data[1] *= 2;
					data[1] |= v18;
					v19 = data[25];
					data[25] = data[8];
					data[8] = v19;
					v20 = data[29] >> 2;
					data[29] <<= 6;
					data[29] |= v20;
					v21 = data[8];
					data[8] = data[23];
					data[23] = v21;
					v24 = (data[10] >> 2) & 1;
					if ((data[10] >> 6) & 1)
						data[10] |= 4u;
					else
						data[10] &= 0xFBu;
					if (v24)
						data[10] |= 0x40u;
					else
						data[10] &= 0xBFu;
					v22 = data[2];
					data[2] = data[9];
					data[9] = v22;
					v23 = (data[12] >> 4) & 1;
					if ((data[12] >> 2) & 1)
						data[12] |= 0x10u;
					else
						data[12] &= 0xEFu;
					if (v23)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
				}
				else
				{
					v13 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v13;
					data[7] ^= 0x50u;
					v31 = (data[5] >> 1) & 1;
					if ((data[5] >> 1) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v31)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					*data ^= 0xE8u;
					data[15] ^= 0x38u;
					v30 = (data[4] >> 3) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 8u;
					else
						data[4] &= 0xF7u;
					if (v30)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					data[5] ^= 0xA9u;
					v29 = (data[12] >> 6) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v29)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v28 = (data[5] >> 4) & 1;
					if ((data[5] >> 5) & 1)
						data[5] |= 0x10u;
					else
						data[5] &= 0xEFu;
					if (v28)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					v27 = (data[6] >> 3) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					if (v27)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					v14 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v14;
					v15 = *data;
					*data = data[14];
					data[14] = v15;
					v26 = (data[7] >> 3) & 1;
					if ((data[7] >> 6) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v26)
						data[7] |= 0x40u;
					else
						data[7] &= 0xBFu;
				}
			}
			else
			{
				v35 = (*data >> 6) & 1;
				if ((*data >> 1) & 1)
					*data |= 0x40u;
				else
					*data &= 0xBFu;
				if (v35)
					*data |= 2u;
				else
					*data &= 0xFDu;
				v9 = data[2] >> 4;
				data[2] *= 16;
				data[2] |= v9;
				v10 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v10;
				data[3] ^= 0x44u;
				data[4] ^= 0x41u;
				v34 = (data[3] >> 5) & 1;
				if ((data[3] >> 7) & 1)
					data[3] |= 0x20u;
				else
					data[3] &= 0xDFu;
				if (v34)
					data[3] |= 0x80u;
				else
					data[3] &= 0x7Fu;
				v11 = data[3] >> 3;
				data[3] *= 32;
				data[3] |= v11;
				v33 = (data[7] >> 7) & 1;
				if ((data[7] >> 5) & 1)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				if (v33)
					data[7] |= 0x20u;
				else
					data[7] &= 0xDFu;
				v12 = data[2];
				data[2] = data[5];
				data[5] = v12;
				v32 = (data[5] >> 4) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x10u;
				else
					data[5] &= 0xEFu;
				if (v32)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				data[2] ^= 0x98u;
			}
		}
		else
		{
			data[1] ^= 0xB2u;
			data[3] ^= 0x66u;
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v2 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
			v5 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v5;
			v6 = data[3];
			data[3] = data[2];
			data[2] = v6;
			v36 = (data[3] >> 3) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			if (v36)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			*data ^= 0xFDu;
			v7 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v7;
			v8 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v8;
			data[1] ^= 0xFCu;
		}
	}
}

void PacketTwister_64::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@11
	char v6; // ST1A_1@23
	char v7; // ST17_1@31
	char v8; // ST16_1@31
	char v9; // ST15_1@31
	char v10; // ST14_1@31
	char v11; // ST13_1@31
	char v12; // ST12_1@31
	char v13; // ST11_1@31
	char v14; // ST0E_1@38
	char v15; // ST0D_1@38
	char v16; // ST0C_1@38
	char v17; // ST0B_1@38
	char v18; // ST08_1@44
	char v19; // ST07_1@44
	char v20; // [sp+9h] [bp-1Bh]@38
	char v21; // [sp+Fh] [bp-15h]@32
	char v22; // [sp+18h] [bp-Ch]@23
	char v23; // [sp+1Bh] [bp-9h]@17
	char v24; // [sp+1Dh] [bp-7h]@11
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = (data[11] >> 7) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					if (v21)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					v14 = data[1];
					data[1] = data[13];
					data[13] = v14;
					v15 = data[3];
					data[3] = data[5];
					data[5] = v15;
					v16 = data[20];
					data[20] = data[7];
					data[7] = v16;
					v17 = data[15];
					data[15] = data[4];
					data[4] = v17;
					data[4] ^= 0x6Bu;
					v20 = (*data >> 1) & 1;
					if ((*data >> 3) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v20)
						*data |= 8u;
					else
						*data &= 0xF7u;
					v18 = data[1];
					data[1] = data[28];
					data[28] = v18;
					v19 = data[24];
					data[24] = data[9];
					data[9] = v19;
				}
				else
				{
					v7 = data[1];
					data[1] = data[2];
					data[2] = v7;
					v8 = data[12] >> 2;
					data[12] <<= 6;
					data[12] |= v8;
					v9 = data[12];
					data[12] = data[9];
					data[9] = v9;
					v10 = data[3];
					data[3] = data[7];
					data[7] = v10;
					v11 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v11;
					v12 = data[12] >> 1;
					data[12] <<= 7;
					data[12] |= v12;
					v13 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v13;
				}
			}
			else
			{
				*data ^= 0x4Du;
				v5 = data[4] >> 5;
				data[4] *= 8;
				data[4] |= v5;
				v24 = (data[7] >> 2) & 1;
				if ((data[7] >> 3) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v24)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				v23 = (data[4] >> 1) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v23)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v6 = data[5];
				data[5] = data[3];
				data[3] = v6;
				v22 = (data[3] >> 1) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				if (v22)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				data[7] ^= 0xC2u;
				data[7] ^= 0xCBu;
			}
		}
		else
		{
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			data[2] ^= 0x46u;
			v2 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v4 = data[1];
			data[1] = data[1];
			data[1] = v4;
		}
	}
}

void PacketTwister_64::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1D_1@17
	char v6; // ST18_1@29
	char v7; // ST17_1@31
	char v8; // ST16_1@31
	char v9; // ST15_1@31
	char v10; // ST14_1@31
	char v11; // ST13_1@31
	char v12; // ST12_1@31
	char v13; // ST11_1@31
	char v14; // ST10_1@32
	char v15; // ST0F_1@32
	char v16; // ST0C_1@38
	char v17; // ST0B_1@38
	char v18; // ST0A_1@38
	char v19; // ST09_1@38
	char v20; // [sp+7h] [bp-1Dh]@38
	char v21; // [sp+Dh] [bp-17h]@32
	char v22; // [sp+19h] [bp-Bh]@23
	char v23; // [sp+1Bh] [bp-9h]@17
	char v24; // [sp+1Eh] [bp-6h]@11
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[24];
					data[24] = data[9];
					data[9] = v14;
					v15 = data[1];
					data[1] = data[28];
					data[28] = v15;
					v21 = (*data >> 1) & 1;
					if ((*data >> 3) & 1)
						*data |= 2u;
					else
						*data &= 0xFDu;
					if (v21)
						*data |= 8u;
					else
						*data &= 0xF7u;
					data[4] ^= 0x6Bu;
					v16 = data[15];
					data[15] = data[4];
					data[4] = v16;
					v17 = data[20];
					data[20] = data[7];
					data[7] = v17;
					v18 = data[3];
					data[3] = data[5];
					data[5] = v18;
					v19 = data[1];
					data[1] = data[13];
					data[13] = v19;
					v20 = (data[11] >> 7) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x80u;
					else
						data[11] &= 0x7Fu;
					if (v20)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
				}
				else
				{
					v7 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v7;
					v8 = data[12] >> 7;
					data[12] *= 2;
					data[12] |= v8;
					v9 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v9;
					v10 = data[3];
					data[3] = data[7];
					data[7] = v10;
					v11 = data[12];
					data[12] = data[9];
					data[9] = v11;
					v12 = data[12] >> 6;
					data[12] *= 4;
					data[12] |= v12;
					v13 = data[1];
					data[1] = data[2];
					data[2] = v13;
				}
			}
			else
			{
				data[7] ^= 0xCBu;
				data[7] ^= 0xC2u;
				v24 = (data[3] >> 1) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				if (v24)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v5 = data[5];
				data[5] = data[3];
				data[3] = v5;
				v23 = (data[4] >> 1) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				if (v23)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v22 = (data[7] >> 2) & 1;
				if ((data[7] >> 3) & 1)
					data[7] |= 4u;
				else
					data[7] &= 0xFBu;
				if (v22)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				v6 = data[4] >> 3;
				data[4] *= 32;
				data[4] |= v6;
				*data ^= 0x4Du;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[1];
			data[1] = v3;
			v2 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			data[2] ^= 0x46u;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
		}
	}
}

void PacketTwister_65::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST23_1@3
	char v8; // ST20_1@9
	char v9; // ST1F_1@9
	char v10; // ST1E_1@9
	char v11; // ST1B_1@15
	char v12; // ST1A_1@15
	char v13; // ST19_1@17
	char v14; // ST18_1@17
	char v15; // ST17_1@17
	char v16; // ST16_1@19
	char v17; // ST15_1@19
	char v18; // ST10_1@31
	char v19; // ST0F_1@31
	char v20; // ST0E_1@31
	char v21; // ST0B_1@37
	char v22; // ST0A_1@38
	char v23; // [sp+6h] [bp-22h]@44
	char v24; // [sp+8h] [bp-20h]@38
	char v25; // [sp+Ch] [bp-1Ch]@31
	char v26; // [sp+11h] [bp-17h]@25
	char v27; // [sp+13h] [bp-15h]@19
	char v28; // [sp+1Ch] [bp-Ch]@9
	char v2; // [sp+21h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v22 = data[7];
					data[7] = data[4];
					data[4] = v22;
					v24 = (data[7] >> 3) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v24)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					data[12] ^= 0x56u;
					v23 = (data[13] >> 5) & 1;
					if ((data[13] >> 2) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v23)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
				}
				else
				{
					v16 = data[5] >> 3;
					data[5] *= 32;
					data[5] |= v16;
					v17 = data[11] >> 6;
					data[11] *= 4;
					data[11] |= v17;
					data[3] ^= 0xF4u;
					v27 = (data[1] >> 4) & 1;
					if ((data[1] >> 5) & 1)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					if (v27)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					v26 = (data[9] >> 1) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v26)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v18 = data[3];
					data[3] = data[7];
					data[7] = v18;
					v19 = data[3];
					data[3] = *data;
					*data = v19;
					v20 = data[14] >> 7;
					data[14] *= 2;
					data[14] |= v20;
					v25 = (data[2] >> 3) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					if (v25)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v21 = data[1] >> 3;
					data[1] *= 32;
					data[1] |= v21;
				}
			}
			else
			{
				v13 = data[2] >> 2;
				data[2] <<= 6;
				data[2] |= v13;
				data[2] ^= 0xFEu;
				v14 = data[5] >> 6;
				data[5] *= 4;
				data[5] |= v14;
				v15 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v15;
			}
		}
		else
		{
			data[3] ^= 0x96u;
			v3 = *data >> 5;
			*data *= 8;
			*data |= v3;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
			v5 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v5;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			v7 = data[3] >> 7;
			data[3] *= 2;
			data[3] |= v7;
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v8 = data[1];
			data[1] = data[3];
			data[3] = v8;
			v9 = data[2];
			data[2] = data[1];
			data[1] = v9;
			v10 = data[2];
			data[2] = data[2];
			data[2] = v10;
			v28 = (data[1] >> 6) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v28)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v11 = data[3];
			data[3] = *data;
			*data = v11;
			v12 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v12;
		}
	}
}

void PacketTwister_65::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST1E_1@15
	char v9; // ST1D_1@15
	char v10; // ST1C_1@15
	char v11; // ST1B_1@15
	char v12; // ST1A_1@15
	char v13; // ST19_1@17
	char v14; // ST18_1@17
	char v15; // ST17_1@17
	char v16; // ST16_1@19
	char v17; // ST13_1@25
	char v18; // ST12_1@25
	char v19; // ST11_1@25
	char v20; // ST0C_1@37
	char v21; // ST0B_1@37
	char v22; // ST06_1@50
	char v23; // [sp+7h] [bp-21h]@44
	char v24; // [sp+9h] [bp-1Fh]@38
	char v25; // [sp+Dh] [bp-1Bh]@31
	char v26; // [sp+Fh] [bp-19h]@25
	char v27; // [sp+14h] [bp-14h]@19
	char v28; // [sp+1Fh] [bp-9h]@9
	char v2; // [sp+24h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v24 = (data[13] >> 5) & 1;
					if ((data[13] >> 2) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v24)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					data[12] ^= 0x56u;
					v23 = (data[7] >> 3) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					if (v23)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					v22 = data[7];
					data[7] = data[4];
					data[4] = v22;
				}
				else
				{
					v16 = data[1] >> 5;
					data[1] *= 8;
					data[1] |= v16;
					v27 = (data[2] >> 3) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					if (v27)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v17 = data[14] >> 1;
					data[14] <<= 7;
					data[14] |= v17;
					v18 = data[3];
					data[3] = *data;
					*data = v18;
					v19 = data[3];
					data[3] = data[7];
					data[7] = v19;
					v26 = (data[9] >> 1) & 1;
					if ((data[9] >> 4) & 1)
						data[9] |= 2u;
					else
						data[9] &= 0xFDu;
					if (v26)
						data[9] |= 0x10u;
					else
						data[9] &= 0xEFu;
					v25 = (data[1] >> 4) & 1;
					if ((data[1] >> 5) & 1)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					if (v25)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					data[3] ^= 0xF4u;
					v20 = data[11] >> 2;
					data[11] <<= 6;
					data[11] |= v20;
					v21 = data[5] >> 5;
					data[5] *= 8;
					data[5] |= v21;
				}
			}
			else
			{
				v13 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v13;
				v14 = data[5] >> 2;
				data[5] <<= 6;
				data[5] |= v14;
				data[2] ^= 0xFEu;
				v15 = data[2] >> 6;
				data[2] *= 4;
				data[2] |= v15;
			}
		}
		else
		{
			v3 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v3;
			v4 = data[3];
			data[3] = *data;
			*data = v4;
			v2 = (data[1] >> 6) & 1;
			if ((data[1] >> 2) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v2)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			v5 = data[2];
			data[2] = data[2];
			data[2] = v5;
			v6 = data[2];
			data[2] = data[1];
			data[1] = v6;
			v7 = data[1];
			data[1] = data[3];
			data[3] = v7;
			v28 = (data[2] >> 2) & 1;
			if ((data[2] >> 5) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v28)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			v8 = data[3] >> 1;
			data[3] <<= 7;
			data[3] |= v8;
			v9 = *data;
			*data = data[2];
			data[2] = v9;
			v10 = data[1] >> 4;
			data[1] *= 16;
			data[1] |= v10;
			v11 = data[1];
			data[1] = data[2];
			data[2] = v11;
			v12 = *data >> 3;
			*data *= 32;
			*data |= v12;
			data[3] ^= 0x96u;
		}
	}
}

void PacketTwister_66::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST1C_1@15
	char v7; // ST1B_1@15
	char v8; // ST1A_1@15
	char v9; // ST17_1@23
	char v10; // ST12_1@35
	char v11; // ST11_1@35
	char v12; // ST10_1@35
	char v13; // ST0F_1@35
	char v14; // ST0E_1@35
	char v15; // ST0D_1@37
	char v16; // ST0A_1@43
	char v17; // ST09_1@43
	char v18; // ST08_1@43
	char v19; // ST07_1@43
	char v20; // ST06_1@43
	char v21; // ST05_1@44
	char v22; // ST04_1@44
	char v23; // [sp+Bh] [bp-19h]@37
	char v24; // [sp+13h] [bp-11h]@29
	char v25; // [sp+15h] [bp-Fh]@23
	char v26; // [sp+18h] [bp-Ch]@17
	char v27; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+1Fh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					*data ^= 0x26u;
					v21 = data[7];
					data[7] = data[15];
					data[15] = v21;
					v22 = data[29];
					data[29] = data[15];
					data[15] = v22;
				}
				else
				{
					data[11] ^= 0xF8u;
					v15 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v15;
					v23 = (data[7] >> 2) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 4u;
					else
						data[7] &= 0xFBu;
					if (v23)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					v16 = data[7] >> 5;
					data[7] *= 8;
					data[7] |= v16;
					v17 = data[15] >> 6;
					data[15] *= 4;
					data[15] |= v17;
					v18 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v18;
					v19 = data[8] >> 7;
					data[8] *= 2;
					data[8] |= v19;
					v20 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v20;
				}
			}
			else
			{
				v26 = (data[1] >> 3) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v26)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
				v9 = data[3];
				data[3] = data[3];
				data[3] = v9;
				data[5] ^= 0xDAu;
				v25 = (data[4] >> 5) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v25)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				v24 = (data[1] >> 2) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v24)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				v10 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v10;
				v11 = data[1] >> 6;
				data[1] *= 4;
				data[1] |= v11;
				v12 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v12;
				v13 = data[2];
				data[2] = *data;
				*data = v13;
				v14 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v14;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[1];
			data[1] = v3;
			v4 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v4;
			v5 = data[3];
			data[3] = data[2];
			data[2] = v5;
			data[2] ^= 0x7Au;
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			*data ^= 0x1Fu;
			v27 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v27)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v6 = data[2];
			data[2] = data[1];
			data[1] = v6;
			*data ^= 0x7Bu;
			v7 = data[1];
			data[1] = *data;
			*data = v7;
			data[1] ^= 0x57u;
			v8 = data[1];
			data[1] = data[2];
			data[2] = v8;
		}
	}
}

void PacketTwister_66::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST1C_1@15
	char v7; // ST1B_1@15
	char v8; // ST1A_1@15
	char v9; // ST19_1@17
	char v10; // ST18_1@17
	char v11; // ST17_1@17
	char v12; // ST16_1@17
	char v13; // ST15_1@17
	char v14; // ST10_1@29
	char v15; // ST0D_1@37
	char v16; // ST0C_1@37
	char v17; // ST0B_1@37
	char v18; // ST0A_1@37
	char v19; // ST09_1@37
	char v20; // ST06_1@43
	char v21; // ST05_1@44
	char v22; // ST04_1@44
	char v23; // [sp+7h] [bp-1Dh]@37
	char v24; // [sp+Eh] [bp-16h]@29
	char v25; // [sp+11h] [bp-13h]@23
	char v26; // [sp+13h] [bp-11h]@17
	char v27; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+1Fh] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = data[29];
					data[29] = data[15];
					data[15] = v21;
					v22 = data[7];
					data[7] = data[15];
					data[15] = v22;
					*data ^= 0x26u;
				}
				else
				{
					v15 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v15;
					v16 = data[8] >> 1;
					data[8] <<= 7;
					data[8] |= v16;
					v17 = data[9] >> 4;
					data[9] *= 16;
					data[9] |= v17;
					v18 = data[15] >> 2;
					data[15] <<= 6;
					data[15] |= v18;
					v19 = data[7] >> 3;
					data[7] *= 32;
					data[7] |= v19;
					v23 = (data[7] >> 2) & 1;
					if ((data[7] >> 1) & 1)
						data[7] |= 4u;
					else
						data[7] &= 0xFBu;
					if (v23)
						data[7] |= 2u;
					else
						data[7] &= 0xFDu;
					v20 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v20;
					data[11] ^= 0xF8u;
				}
			}
			else
			{
				v9 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v9;
				v10 = data[2];
				data[2] = *data;
				*data = v10;
				v11 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v11;
				v12 = data[1] >> 2;
				data[1] <<= 6;
				data[1] |= v12;
				v13 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v13;
				v26 = (data[1] >> 2) & 1;
				if ((data[1] >> 1) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v26)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				v25 = (data[4] >> 5) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v25)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				data[5] ^= 0xDAu;
				v14 = data[3];
				data[3] = data[3];
				data[3] = v14;
				v24 = (data[1] >> 3) & 1;
				if ((data[1] >> 6) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v24)
					data[1] |= 0x40u;
				else
					data[1] &= 0xBFu;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			data[1] ^= 0x57u;
			v4 = data[1];
			data[1] = *data;
			*data = v4;
			*data ^= 0x7Bu;
			v5 = data[2];
			data[2] = data[1];
			data[1] = v5;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			*data ^= 0x1Fu;
			v27 = (data[1] >> 4) & 1;
			if ((data[1] >> 3) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v27)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			data[2] ^= 0x7Au;
			v6 = data[3];
			data[3] = data[2];
			data[2] = v6;
			v7 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v7;
			v8 = data[1];
			data[1] = data[1];
			data[1] = v8;
		}
	}
}

void PacketTwister_67::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST20_1@9
	char v5; // ST1F_1@9
	char v6; // ST1C_1@17
	char v7; // ST17_1@29
	char v8; // ST16_1@29
	char v9; // ST11_1@43
	char v10; // ST10_1@43
	char v11; // ST0F_1@43
	char v12; // ST0C_1@49
	char v13; // ST0B_1@50
	char v14; // ST0A_1@50
	char v15; // ST09_1@50
	char v16; // ST08_1@50
	char v17; // [sp+6h] [bp-1Eh]@50
	char v18; // [sp+Dh] [bp-17h]@43
	char v19; // [sp+12h] [bp-12h]@35
	char v20; // [sp+14h] [bp-10h]@29
	char v21; // [sp+18h] [bp-Ch]@23
	char v22; // [sp+1Ah] [bp-Ah]@17
	char v23; // [sp+1Dh] [bp-7h]@9
	char v2; // [sp+21h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[27];
					data[27] = data[18];
					data[18] = v13;
					data[30] ^= 0x33u;
					v14 = data[3];
					data[3] = data[15];
					data[15] = v14;
					v15 = data[3];
					data[3] = data[3];
					data[3] = v15;
					v16 = data[16];
					data[16] = data[16];
					data[16] = v16;
					v17 = (data[25] >> 3) & 1;
					if ((data[25] >> 4) & 1)
						data[25] |= 8u;
					else
						data[25] &= 0xF7u;
					if (v17)
						data[25] |= 0x10u;
					else
						data[25] &= 0xEFu;
				}
				else
				{
					v9 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v9;
					data[7] ^= 0x88u;
					v10 = data[15];
					data[15] = data[11];
					data[11] = v10;
					data[4] ^= 0x3Bu;
					v11 = data[4] >> 1;
					data[4] <<= 7;
					data[4] |= v11;
					v18 = (data[15] >> 4) & 1;
					if ((data[15] >> 2) & 1)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					if (v18)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					v12 = data[9] >> 5;
					data[9] *= 8;
					data[9] |= v12;
					data[11] ^= 0x18u;
					data[6] ^= 0x26u;
					data[14] ^= 0x20u;
				}
			}
			else
			{
				*data ^= 0xCCu;
				v6 = data[2] >> 7;
				data[2] *= 2;
				data[2] |= v6;
				data[2] ^= 0x8Du;
				v22 = (data[6] >> 3) & 1;
				if ((data[6] >> 3) & 1)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				if (v22)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				*data ^= 0x10u;
				v21 = (data[1] >> 3) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v21)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				v7 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v7;
				v8 = data[6];
				data[6] = data[1];
				data[1] = v8;
				v20 = (data[2] >> 4) & 1;
				if ((data[2] >> 2) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v20)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				v19 = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v19)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
			}
		}
		else
		{
			v3 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v3;
			*data ^= 0x39u;
			data[3] ^= 0x63u;
			v2 = (data[2] >> 6) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			data[1] ^= 0x41u;
			v4 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v4;
			v5 = *data;
			*data = data[3];
			data[3] = v5;
			v23 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v23)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
		}
	}
}

void PacketTwister_67::Correct(BYTE *data, int length)
{
	char v3; // ST21_1@9
	char v4; // ST20_1@9
	char v5; // ST1D_1@15
	char v6; // ST18_1@29
	char v7; // ST17_1@29
	char v8; // ST12_1@41
	char v9; // ST11_1@43
	char v10; // ST0E_1@49
	char v11; // ST0D_1@49
	char v12; // ST0C_1@49
	char v13; // ST09_1@56
	char v14; // ST08_1@56
	char v15; // ST07_1@56
	char v16; // ST06_1@56
	char v17; // [sp+Ah] [bp-1Ah]@50
	char v18; // [sp+Fh] [bp-15h]@43
	char v19; // [sp+13h] [bp-11h]@35
	char v20; // [sp+15h] [bp-Fh]@29
	char v21; // [sp+19h] [bp-Bh]@23
	char v22; // [sp+1Bh] [bp-9h]@17
	char v23; // [sp+1Eh] [bp-6h]@9
	char v2; // [sp+22h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[25] >> 3) & 1;
					if ((data[25] >> 4) & 1)
						data[25] |= 8u;
					else
						data[25] &= 0xF7u;
					if (v17)
						data[25] |= 0x10u;
					else
						data[25] &= 0xEFu;
					v13 = data[16];
					data[16] = data[16];
					data[16] = v13;
					v14 = data[3];
					data[3] = data[3];
					data[3] = v14;
					v15 = data[3];
					data[3] = data[15];
					data[15] = v15;
					data[30] ^= 0x33u;
					v16 = data[27];
					data[27] = data[18];
					data[18] = v16;
				}
				else
				{
					data[14] ^= 0x20u;
					data[6] ^= 0x26u;
					data[11] ^= 0x18u;
					v9 = data[9] >> 3;
					data[9] *= 32;
					data[9] |= v9;
					v18 = (data[15] >> 4) & 1;
					if ((data[15] >> 2) & 1)
						data[15] |= 0x10u;
					else
						data[15] &= 0xEFu;
					if (v18)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					v10 = data[4] >> 7;
					data[4] *= 2;
					data[4] |= v10;
					data[4] ^= 0x3Bu;
					v11 = data[15];
					data[15] = data[11];
					data[11] = v11;
					data[7] ^= 0x88u;
					v12 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v12;
				}
			}
			else
			{
				v22 = (data[1] >> 2) & 1;
				if ((data[1] >> 4) & 1)
					data[1] |= 4u;
				else
					data[1] &= 0xFBu;
				if (v22)
					data[1] |= 0x10u;
				else
					data[1] &= 0xEFu;
				v21 = (data[2] >> 4) & 1;
				if ((data[2] >> 2) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v21)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				v6 = data[6];
				data[6] = data[1];
				data[1] = v6;
				v7 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v7;
				v20 = (data[1] >> 3) & 1;
				if ((data[1] >> 3) & 1)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				if (v20)
					data[1] |= 8u;
				else
					data[1] &= 0xF7u;
				*data ^= 0x10u;
				v19 = (data[6] >> 3) & 1;
				if ((data[6] >> 3) & 1)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				if (v19)
					data[6] |= 8u;
				else
					data[6] &= 0xF7u;
				data[2] ^= 0x8Du;
				v8 = data[2] >> 1;
				data[2] <<= 7;
				data[2] |= v8;
				*data ^= 0xCCu;
			}
		}
		else
		{
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v3 = *data;
			*data = data[3];
			data[3] = v3;
			v4 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v4;
			data[1] ^= 0x41u;
			v23 = (data[2] >> 6) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			if (v23)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			data[3] ^= 0x63u;
			*data ^= 0x39u;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
		}
	}
}

void PacketTwister_68::Twist(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST21_1@15
	char v6; // ST20_1@15
	char v7; // ST1F_1@15
	char v8; // ST1A_1@27
	char v9; // ST19_1@29
	char v10; // ST18_1@29
	char v11; // ST17_1@29
	char v12; // ST16_1@29
	char v13; // ST13_1@37
	char v14; // ST10_1@43
	char v15; // ST0F_1@44
	char v16; // ST0E_1@44
	char v17; // ST0D_1@44
	char v18; // ST08_1@56
	char v19; // [sp+6h] [bp-22h]@56
	char v20; // [sp+9h] [bp-1Fh]@50
	char v21; // [sp+Bh] [bp-1Dh]@44
	char v22; // [sp+11h] [bp-17h]@37
	char v23; // [sp+14h] [bp-14h]@29
	char v24; // [sp+1Bh] [bp-Dh]@21
	char v25; // [sp+1Dh] [bp-Bh]@15
	char v26; // [sp+22h] [bp-6h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = data[23] >> 1;
					data[23] <<= 7;
					data[23] |= v15;
					v16 = data[1];
					data[1] = data[17];
					data[17] = v16;
					v17 = data[4];
					data[4] = data[21];
					data[21] = v17;
					data[15] ^= 0x82u;
					v21 = (data[2] >> 2) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v21)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					v20 = (data[15] >> 2) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v20)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v18 = data[26] >> 4;
					data[26] *= 16;
					data[26] |= v18;
					v19 = (data[17] >> 5) & 1;
					if ((data[17] >> 4) & 1)
						data[17] |= 0x20u;
					else
						data[17] &= 0xDFu;
					if (v19)
						data[17] |= 0x10u;
					else
						data[17] &= 0xEFu;
					data[25] ^= 0x9Fu;
					data[1] ^= 0x14u;
				}
				else
				{
					v13 = data[15];
					data[15] = data[14];
					data[14] = v13;
					v22 = (data[1] >> 1) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v22)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v14 = data[12];
					data[12] = data[8];
					data[8] = v14;
				}
			}
			else
			{
				v9 = data[2];
				data[2] = data[6];
				data[6] = v9;
				v10 = data[7] >> 5;
				data[7] *= 8;
				data[7] |= v10;
				v11 = data[6];
				data[6] = data[6];
				data[6] = v11;
				data[1] ^= 0x5Cu;
				v12 = data[2];
				data[2] = data[1];
				data[1] = v12;
				v23 = (data[6] >> 7) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				if (v23)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
			}
		}
		else
		{
			v2 = (*data >> 5) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v2)
				*data |= 4u;
			else
				*data &= 0xFBu;
			v3 = data[2];
			data[2] = data[2];
			data[2] = v3;
			v4 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v4;
			v26 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v26)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v5 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v5;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
			v7 = data[2] >> 3;
			data[2] *= 32;
			data[2] |= v7;
			v25 = (data[2] >> 5) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v25)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			data[1] ^= 0x9Au;
			data[1] ^= 0x51u;
			v24 = (data[3] >> 2) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v24)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			v8 = *data;
			*data = data[1];
			data[1] = v8;
		}
	}
}

void PacketTwister_68::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST22_1@15
	char v5; // ST21_1@15
	char v6; // ST20_1@15
	char v7; // ST1D_1@21
	char v8; // ST1C_1@21
	char v9; // ST17_1@35
	char v10; // ST16_1@35
	char v11; // ST15_1@35
	char v12; // ST14_1@35
	char v13; // ST13_1@37
	char v14; // ST10_1@43
	char v15; // ST0D_1@50
	char v16; // ST08_1@62
	char v17; // ST07_1@62
	char v18; // ST06_1@62
	char v19; // [sp+9h] [bp-1Fh]@56
	char v20; // [sp+Bh] [bp-1Dh]@50
	char v21; // [sp+Eh] [bp-1Ah]@44
	char v22; // [sp+11h] [bp-17h]@37
	char v23; // [sp+18h] [bp-10h]@29
	char v24; // [sp+1Ah] [bp-Eh]@21
	char v25; // [sp+1Eh] [bp-Ah]@15
	char v26; // [sp+23h] [bp-5h]@9
	char v2; // [sp+25h] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[1] ^= 0x14u;
					data[25] ^= 0x9Fu;
					v21 = (data[17] >> 5) & 1;
					if ((data[17] >> 4) & 1)
						data[17] |= 0x20u;
					else
						data[17] &= 0xDFu;
					if (v21)
						data[17] |= 0x10u;
					else
						data[17] &= 0xEFu;
					v15 = data[26] >> 4;
					data[26] *= 16;
					data[26] |= v15;
					v20 = (data[15] >> 2) & 1;
					if ((data[15] >> 1) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v20)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					v19 = (data[2] >> 2) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					if (v19)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					data[15] ^= 0x82u;
					v16 = data[4];
					data[4] = data[21];
					data[21] = v16;
					v17 = data[1];
					data[1] = data[17];
					data[17] = v17;
					v18 = data[23] >> 7;
					data[23] *= 2;
					data[23] |= v18;
				}
				else
				{
					v13 = data[12];
					data[12] = data[8];
					data[8] = v13;
					v22 = (data[1] >> 1) & 1;
					if ((data[1] >> 1) & 1)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					if (v22)
						data[1] |= 2u;
					else
						data[1] &= 0xFDu;
					v14 = data[15];
					data[15] = data[14];
					data[14] = v14;
				}
			}
			else
			{
				v23 = (data[6] >> 7) & 1;
				if ((data[6] >> 5) & 1)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				if (v23)
					data[6] |= 0x20u;
				else
					data[6] &= 0xDFu;
				v9 = data[2];
				data[2] = data[1];
				data[1] = v9;
				data[1] ^= 0x5Cu;
				v10 = data[6];
				data[6] = data[6];
				data[6] = v10;
				v11 = data[7] >> 3;
				data[7] *= 32;
				data[7] |= v11;
				v12 = data[2];
				data[2] = data[6];
				data[6] = v12;
			}
		}
		else
		{
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			data[1] ^= 0x51u;
			data[1] ^= 0x9Au;
			v26 = (data[2] >> 5) & 1;
			if ((data[2] >> 6) & 1)
				data[2] |= 0x20u;
			else
				data[2] &= 0xDFu;
			if (v26)
				data[2] |= 0x40u;
			else
				data[2] &= 0xBFu;
			v4 = data[2] >> 5;
			data[2] *= 8;
			data[2] |= v4;
			v5 = data[1];
			data[1] = *data;
			*data = v5;
			v6 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v6;
			v25 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v25)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			v7 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v7;
			v8 = data[2];
			data[2] = data[2];
			data[2] = v8;
			v24 = (*data >> 5) & 1;
			if ((*data >> 2) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v24)
				*data |= 4u;
			else
				*data &= 0xFBu;
		}
	}
}

void PacketTwister_69::Twist(BYTE *data, int length)
{
	char v3; // ST19_1@9
	char v4; // ST18_1@9
	char v5; // ST17_1@9
	char v6; // ST14_1@15
	char v7; // ST13_1@15
	char v8; // ST12_1@17
	char v9; // ST11_1@19
	char v10; // ST0E_1@25
	char v11; // ST09_1@38
	char v12; // ST08_1@38
	char v13; // ST07_1@38
	char v14; // ST06_1@38
	char v15; // [sp+Ah] [bp-12h]@32
	char v16; // [sp+Ch] [bp-10h]@25
	char v17; // [sp+Fh] [bp-Dh]@19
	char v18; // [sp+15h] [bp-7h]@9
	char v2; // [sp+1Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v15 = (data[11] >> 6) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x40u;
					else
						data[11] &= 0xBFu;
					if (v15)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					v11 = data[17] >> 2;
					data[17] <<= 6;
					data[17] |= v11;
					data[2] ^= 0x73u;
					v12 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v12;
					data[29] ^= 0xCEu;
					v13 = data[5] >> 6;
					data[5] *= 4;
					data[5] |= v13;
					v14 = data[27];
					data[27] = data[30];
					data[30] = v14;
					data[5] ^= 0xE9u;
				}
				else
				{
					v9 = data[3];
					data[3] = data[2];
					data[2] = v9;
					v17 = (data[13] >> 6) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					if (v17)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v10 = data[15];
					data[15] = data[8];
					data[8] = v10;
					v16 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v16)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
				}
			}
			else
			{
				v8 = data[3];
				data[3] = data[6];
				data[6] = v8;
				data[3] ^= 0xC1u;
				data[5] ^= 0x29u;
				data[7] ^= 0xA9u;
			}
		}
		else
		{
			data[1] ^= 0x48u;
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			*data ^= 0xF5u;
			v3 = data[1];
			data[1] = data[1];
			data[1] = v3;
			v4 = data[1];
			data[1] = data[1];
			data[1] = v4;
			data[2] ^= 0x32u;
			*data ^= 0x51u;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
			v18 = (data[1] >> 4) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v18)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v6 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v6;
			v7 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v7;
		}
	}
}

void PacketTwister_69::Correct(BYTE *data, int length)
{
	char v3; // ST1B_1@3
	char v4; // ST1A_1@3
	char v5; // ST17_1@9
	char v6; // ST16_1@9
	char v7; // ST15_1@9
	char v8; // ST12_1@17
	char v9; // ST0F_1@25
	char v10; // ST0C_1@31
	char v11; // ST0B_1@32
	char v12; // ST0A_1@32
	char v13; // ST09_1@32
	char v14; // ST08_1@32
	char v15; // [sp+6h] [bp-16h]@32
	char v16; // [sp+Dh] [bp-Fh]@25
	char v17; // [sp+10h] [bp-Ch]@19
	char v18; // [sp+13h] [bp-9h]@9
	char v2; // [sp+18h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[5] ^= 0xE9u;
					v11 = data[27];
					data[27] = data[30];
					data[30] = v11;
					v12 = data[5] >> 2;
					data[5] <<= 6;
					data[5] |= v12;
					data[29] ^= 0xCEu;
					v13 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v13;
					data[2] ^= 0x73u;
					v14 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v14;
					v15 = (data[11] >> 6) & 1;
					if ((data[11] >> 3) & 1)
						data[11] |= 0x40u;
					else
						data[11] &= 0xBFu;
					if (v15)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
				}
				else
				{
					v17 = (data[3] >> 2) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 4u;
					else
						data[3] &= 0xFBu;
					if (v17)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v9 = data[15];
					data[15] = data[8];
					data[8] = v9;
					v16 = (data[13] >> 6) & 1;
					if ((data[13] >> 6) & 1)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					if (v16)
						data[13] |= 0x40u;
					else
						data[13] &= 0xBFu;
					v10 = data[3];
					data[3] = data[2];
					data[2] = v10;
				}
			}
			else
			{
				data[7] ^= 0xA9u;
				data[5] ^= 0x29u;
				data[3] ^= 0xC1u;
				v8 = data[3];
				data[3] = data[6];
				data[6] = v8;
			}
		}
		else
		{
			v3 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v3;
			v4 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v4;
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v5 = *data;
			*data = data[2];
			data[2] = v5;
			*data ^= 0x51u;
			data[2] ^= 0x32u;
			v6 = data[1];
			data[1] = data[1];
			data[1] = v6;
			v7 = data[1];
			data[1] = data[1];
			data[1] = v7;
			*data ^= 0xF5u;
			v18 = (data[2] >> 2) & 1;
			if ((data[2] >> 4) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v18)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			data[1] ^= 0x48u;
		}
	}
}

void PacketTwister_70::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST20_1@9
	char v9; // ST1F_1@9
	char v10; // ST1E_1@9
	char v11; // ST1B_1@17
	char v12; // ST1A_1@19
	char v13; // ST17_1@25
	char v14; // ST10_1@43
	char v15; // ST0B_1@55
	char v16; // ST0A_1@56
	char v17; // ST09_1@56
	char v18; // ST06_1@62
	char v19; // ST05_1@62
	char v20; // [sp+7h] [bp-21h]@56
	char v21; // [sp+Ch] [bp-1Ch]@49
	char v22; // [sp+Eh] [bp-1Ah]@43
	char v23; // [sp+11h] [bp-17h]@37
	char v24; // [sp+13h] [bp-15h]@31
	char v25; // [sp+15h] [bp-13h]@25
	char v26; // [sp+18h] [bp-10h]@19
	char v27; // [sp+1Ch] [bp-Ch]@9
	char v2; // [sp+23h] [bp-5h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[27];
					data[27] = data[1];
					data[1] = v16;
					v17 = data[28] >> 1;
					data[28] <<= 7;
					data[28] |= v17;
					v20 = (data[23] >> 6) & 1;
					if ((data[23] >> 6) & 1)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					if (v20)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					v18 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v18;
					v19 = data[4] >> 5;
					data[4] *= 8;
					data[4] |= v19;
				}
				else
				{
					v12 = data[6];
					data[6] = data[7];
					data[7] = v12;
					data[12] ^= 0x36u;
					data[1] ^= 0x86u;
					v26 = (data[5] >> 1) & 1;
					if ((data[5] >> 5) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v26)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					v13 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v13;
					data[9] ^= 0x95u;
					v25 = (data[2] >> 6) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v25)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					v24 = (data[6] >> 7) & 1;
					if ((data[6] >> 2) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v24)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					v23 = (*data >> 2) & 1;
					if ((*data >> 1) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v23)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v14 = data[13] >> 7;
					data[13] *= 2;
					data[13] |= v14;
					v22 = (data[6] >> 7) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v22)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					v21 = (data[11] >> 5) & 1;
					if ((data[11] >> 2) & 1)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
					if (v21)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					v15 = data[1] >> 3;
					data[1] *= 32;
					data[1] |= v15;
				}
			}
			else
			{
				data[3] ^= 0x45u;
				data[3] ^= 0x4Au;
				v11 = data[6];
				data[6] = data[5];
				data[5] = v11;
				data[4] ^= 0xB2u;
				*data ^= 0x19u;
				*data ^= 0x23u;
				data[2] ^= 0xB7u;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			v4 = *data;
			*data = *data;
			*data = v4;
			data[2] ^= 0xE6u;
			v5 = *data >> 1;
			*data <<= 7;
			*data |= v5;
			v2 = (data[2] >> 7) & 1;
			if ((data[2] >> 3) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v2)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			v6 = data[2];
			data[2] = data[1];
			data[1] = v6;
			v7 = *data;
			*data = data[2];
			data[2] = v7;
			*data ^= 0x6Du;
			v8 = *data;
			*data = data[1];
			data[1] = v8;
			v9 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v9;
			v10 = *data;
			*data = data[1];
			data[1] = v10;
			v27 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v27)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
		}
	}
}

void PacketTwister_70::Correct(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST1E_1@15
	char v9; // ST1D_1@15
	char v10; // ST1C_1@15
	char v11; // ST1B_1@17
	char v12; // ST1A_1@19
	char v13; // ST15_1@31
	char v14; // ST0E_1@49
	char v15; // ST0B_1@55
	char v16; // ST0A_1@56
	char v17; // ST09_1@56
	char v18; // ST06_1@62
	char v19; // ST05_1@62
	char v20; // [sp+7h] [bp-21h]@56
	char v21; // [sp+Ch] [bp-1Ch]@49
	char v22; // [sp+Fh] [bp-19h]@43
	char v23; // [sp+11h] [bp-17h]@37
	char v24; // [sp+13h] [bp-15h]@31
	char v25; // [sp+16h] [bp-12h]@25
	char v26; // [sp+18h] [bp-10h]@19
	char v27; // [sp+1Fh] [bp-9h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[4] >> 3;
					data[4] *= 32;
					data[4] |= v16;
					v17 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v17;
					v20 = (data[23] >> 6) & 1;
					if ((data[23] >> 6) & 1)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					if (v20)
						data[23] |= 0x40u;
					else
						data[23] &= 0xBFu;
					v18 = data[28] >> 7;
					data[28] *= 2;
					data[28] |= v18;
					v19 = data[27];
					data[27] = data[1];
					data[1] = v19;
				}
				else
				{
					v12 = data[1] >> 5;
					data[1] *= 8;
					data[1] |= v12;
					v26 = (data[11] >> 5) & 1;
					if ((data[11] >> 2) & 1)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
					if (v26)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					v25 = (data[6] >> 7) & 1;
					if ((data[6] >> 3) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v25)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					v13 = data[13] >> 1;
					data[13] <<= 7;
					data[13] |= v13;
					v24 = (*data >> 2) & 1;
					if ((*data >> 1) & 1)
						*data |= 4u;
					else
						*data &= 0xFBu;
					if (v24)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v23 = (data[6] >> 7) & 1;
					if ((data[6] >> 2) & 1)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					if (v23)
						data[6] |= 4u;
					else
						data[6] &= 0xFBu;
					v22 = (data[2] >> 6) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v22)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					data[9] ^= 0x95u;
					v14 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v14;
					v21 = (data[5] >> 1) & 1;
					if ((data[5] >> 5) & 1)
						data[5] |= 2u;
					else
						data[5] &= 0xFDu;
					if (v21)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					data[1] ^= 0x86u;
					data[12] ^= 0x36u;
					v15 = data[6];
					data[6] = data[7];
					data[7] = v15;
				}
			}
			else
			{
				data[2] ^= 0xB7u;
				*data ^= 0x23u;
				*data ^= 0x19u;
				data[4] ^= 0xB2u;
				v11 = data[6];
				data[6] = data[5];
				data[5] = v11;
				data[3] ^= 0x4Au;
				data[3] ^= 0x45u;
			}
		}
		else
		{
			v2 = (data[1] >> 3) & 1;
			if ((data[1] >> 1) & 1)
				data[1] |= 8u;
			else
				data[1] &= 0xF7u;
			if (v2)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v4 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v4;
			v5 = *data;
			*data = data[1];
			data[1] = v5;
			*data ^= 0x6Du;
			v6 = *data;
			*data = data[2];
			data[2] = v6;
			v7 = data[2];
			data[2] = data[1];
			data[1] = v7;
			v27 = (data[2] >> 7) & 1;
			if ((data[2] >> 3) & 1)
				data[2] |= 0x80u;
			else
				data[2] &= 0x7Fu;
			if (v27)
				data[2] |= 8u;
			else
				data[2] &= 0xF7u;
			v8 = *data >> 7;
			*data *= 2;
			*data |= v8;
			data[2] ^= 0xE6u;
			v9 = *data;
			*data = *data;
			*data = v9;
			v10 = data[3];
			data[3] = data[1];
			data[1] = v10;
		}
	}
}

void PacketTwister_71::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST20_1@3
	char v7; // ST1F_1@3
	char v8; // ST1C_1@9
	char v9; // ST1B_1@9
	char v10; // ST1A_1@11
	char v11; // ST19_1@11
	char v12; // ST18_1@11
	char v13; // ST17_1@11
	char v14; // ST16_1@11
	char v15; // ST13_1@19
	char v16; // ST0E_1@31
	char v17; // ST0D_1@31
	char v18; // ST0C_1@31
	char v19; // ST0B_1@31
	char v20; // ST0A_1@32
	char v21; // ST07_1@38
	char v22; // ST06_1@38
	char v23; // [sp+4h] [bp-20h]@38
	char v24; // [sp+8h] [bp-1Ch]@32
	char v25; // [sp+Fh] [bp-15h]@25
	char v26; // [sp+11h] [bp-13h]@19
	char v27; // [sp+14h] [bp-10h]@11
	char v2; // [sp+1Dh] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v20 = data[6];
					data[6] = data[17];
					data[17] = v20;
					data[12] ^= 0x39u;
					v24 = (data[26] >> 6) & 1;
					if ((data[26] >> 4) & 1)
						data[26] |= 0x40u;
					else
						data[26] &= 0xBFu;
					if (v24)
						data[26] |= 0x10u;
					else
						data[26] &= 0xEFu;
					v21 = *data >> 2;
					*data <<= 6;
					*data |= v21;
					v22 = data[16];
					data[16] = data[10];
					data[10] = v22;
					v23 = (data[13] >> 2) & 1;
					if ((data[13] >> 1) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v23)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
				}
				else
				{
					data[12] ^= 0xEAu;
					v15 = data[8] >> 3;
					data[8] *= 32;
					data[8] |= v15;
					v26 = (data[8] >> 2) & 1;
					if ((data[8] >> 2) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v26)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					data[7] ^= 0x8Du;
					v25 = (data[12] >> 1) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					if (v25)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					v16 = *data >> 1;
					*data <<= 7;
					*data |= v16;
					v17 = data[14];
					data[14] = data[13];
					data[13] = v17;
					v18 = data[4];
					data[4] = data[8];
					data[8] = v18;
					v19 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v19;
				}
			}
			else
			{
				v10 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v10;
				v11 = data[2];
				data[2] = data[2];
				data[2] = v11;
				data[2] ^= 0x74u;
				data[4] ^= 0x44u;
				v12 = data[3] >> 5;
				data[3] *= 8;
				data[3] |= v12;
				v13 = data[3];
				data[3] = data[6];
				data[6] = v13;
				data[3] ^= 0x3Fu;
				v14 = data[6];
				data[6] = data[4];
				data[4] = v14;
				data[7] ^= 0xF8u;
				v27 = (data[2] >> 6) & 1;
				if ((data[2] >> 2) & 1)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				if (v27)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				data[1] ^= 0x17u;
			}
		}
		else
		{
			v3 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v3;
			v4 = data[1];
			data[1] = data[3];
			data[3] = v4;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			v6 = *data >> 7;
			*data *= 2;
			*data |= v6;
			data[3] ^= 0xC2u;
			v7 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v7;
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			v8 = *data;
			*data = data[1];
			data[1] = v8;
			data[3] ^= 0x8Cu;
			v9 = data[3];
			data[3] = data[2];
			data[2] = v9;
			*data ^= 0xB6u;
		}
	}
}

void PacketTwister_71::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST1F_1@9
	char v6; // ST1E_1@9
	char v7; // ST1D_1@9
	char v8; // ST1C_1@9
	char v9; // ST1B_1@9
	char v10; // ST18_1@17
	char v11; // ST17_1@17
	char v12; // ST16_1@17
	char v13; // ST15_1@17
	char v14; // ST14_1@17
	char v15; // ST13_1@19
	char v16; // ST12_1@19
	char v17; // ST11_1@19
	char v18; // ST10_1@19
	char v19; // ST0B_1@31
	char v20; // ST08_1@38
	char v21; // ST07_1@38
	char v22; // ST04_1@44
	char v23; // [sp+5h] [bp-1Fh]@38
	char v24; // [sp+9h] [bp-1Bh]@32
	char v25; // [sp+Ch] [bp-18h]@25
	char v26; // [sp+Eh] [bp-16h]@19
	char v27; // [sp+19h] [bp-Bh]@11
	char v2; // [sp+20h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v24 = (data[13] >> 2) & 1;
					if ((data[13] >> 1) & 1)
						data[13] |= 4u;
					else
						data[13] &= 0xFBu;
					if (v24)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					v20 = data[16];
					data[16] = data[10];
					data[10] = v20;
					v21 = *data >> 6;
					*data *= 4;
					*data |= v21;
					v23 = (data[26] >> 6) & 1;
					if ((data[26] >> 4) & 1)
						data[26] |= 0x40u;
					else
						data[26] &= 0xBFu;
					if (v23)
						data[26] |= 0x10u;
					else
						data[26] &= 0xEFu;
					data[12] ^= 0x39u;
					v22 = data[6];
					data[6] = data[17];
					data[17] = v22;
				}
				else
				{
					v15 = data[5] >> 4;
					data[5] *= 16;
					data[5] |= v15;
					v16 = data[4];
					data[4] = data[8];
					data[8] = v16;
					v17 = data[14];
					data[14] = data[13];
					data[13] = v17;
					v18 = *data >> 7;
					*data *= 2;
					*data |= v18;
					v26 = (data[12] >> 1) & 1;
					if ((data[12] >> 1) & 1)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					if (v26)
						data[12] |= 2u;
					else
						data[12] &= 0xFDu;
					data[7] ^= 0x8Du;
					v25 = (data[8] >> 2) & 1;
					if ((data[8] >> 2) & 1)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					if (v25)
						data[8] |= 4u;
					else
						data[8] &= 0xFBu;
					v19 = data[8] >> 5;
					data[8] *= 8;
					data[8] |= v19;
					data[12] ^= 0xEAu;
				}
			}
			else
			{
				data[1] ^= 0x17u;
				v27 = (data[2] >> 6) & 1;
				if ((data[2] >> 2) & 1)
					data[2] |= 0x40u;
				else
					data[2] &= 0xBFu;
				if (v27)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				data[7] ^= 0xF8u;
				v10 = data[6];
				data[6] = data[4];
				data[4] = v10;
				data[3] ^= 0x3Fu;
				v11 = data[3];
				data[3] = data[6];
				data[6] = v11;
				v12 = data[3] >> 3;
				data[3] *= 32;
				data[3] |= v12;
				data[4] ^= 0x44u;
				data[2] ^= 0x74u;
				v13 = data[2];
				data[2] = data[2];
				data[2] = v13;
				v14 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v14;
			}
		}
		else
		{
			*data ^= 0xB6u;
			v3 = data[3];
			data[3] = data[2];
			data[2] = v3;
			data[3] ^= 0x8Cu;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v2 = (data[1] >> 4) & 1;
			if ((data[1] >> 5) & 1)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			if (v2)
				data[1] |= 0x20u;
			else
				data[1] &= 0xDFu;
			v5 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v5;
			data[3] ^= 0xC2u;
			v6 = *data >> 1;
			*data <<= 7;
			*data |= v6;
			v7 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v7;
			v8 = data[1];
			data[1] = data[3];
			data[3] = v8;
			v9 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v9;
		}
	}
}

void PacketTwister_72::Twist(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST20_1@15
	char v7; // ST1F_1@15
	char v8; // ST1E_1@17
	char v9; // ST1D_1@17
	char v10; // ST1C_1@19
	char v11; // ST1B_1@19
	char v12; // ST1A_1@19
	char v13; // ST17_1@25
	char v14; // ST16_1@25
	char v15; // ST15_1@25
	char v16; // ST14_1@26
	char v17; // ST0F_1@38
	char v18; // ST0C_1@44
	char v19; // ST0B_1@44
	char v20; // ST08_1@50
	char v21; // ST07_1@50
	char v22; // [sp+9h] [bp-1Fh]@44
	char v23; // [sp+Dh] [bp-1Bh]@38
	char v24; // [sp+10h] [bp-18h]@32
	char v25; // [sp+12h] [bp-16h]@26
	char v26; // [sp+18h] [bp-10h]@19
	char v27; // [sp+21h] [bp-7h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[17];
					data[17] = data[22];
					data[22] = v16;
					v25 = (data[4] >> 2) & 1;
					if ((data[4] >> 4) & 1)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					if (v25)
						data[4] |= 0x10u;
					else
						data[4] &= 0xEFu;
					v24 = (data[12] >> 6) & 1;
					if ((data[12] >> 6) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v24)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					v17 = data[23];
					data[23] = data[9];
					data[9] = v17;
					v23 = (data[25] >> 2) & 1;
					if ((data[25] >> 5) & 1)
						data[25] |= 4u;
					else
						data[25] &= 0xFBu;
					if (v23)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					data[22] ^= 0x1Fu;
					data[21] ^= 0x75u;
					v18 = data[23];
					data[23] = data[22];
					data[22] = v18;
					v19 = data[22] >> 6;
					data[22] *= 4;
					data[22] |= v19;
					v22 = (data[4] >> 6) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v22)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					v20 = data[13];
					data[13] = data[30];
					data[30] = v20;
					v21 = data[22];
					data[22] = data[11];
					data[11] = v21;
				}
				else
				{
					v10 = data[13];
					data[13] = data[8];
					data[8] = v10;
					v11 = data[13] >> 7;
					data[13] *= 2;
					data[13] |= v11;
					v12 = data[3];
					data[3] = data[7];
					data[7] = v12;
					v26 = (data[1] >> 6) & 1;
					if ((data[1] >> 4) & 1)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
					if (v26)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					v13 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v13;
					v14 = data[3];
					data[3] = data[4];
					data[4] = v14;
					v15 = data[8] >> 6;
					data[8] *= 4;
					data[8] |= v15;
					data[12] ^= 0xC3u;
					data[12] ^= 0x8Bu;
				}
			}
			else
			{
				v8 = data[6] >> 5;
				data[6] *= 8;
				data[6] |= v8;
				v9 = data[6];
				data[6] = data[5];
				data[5] = v9;
				data[7] ^= 0xDAu;
			}
		}
		else
		{
			v2 = (*data >> 2) & 1;
			if ((*data >> 3) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v2)
				*data |= 8u;
			else
				*data &= 0xF7u;
			v3 = *data >> 7;
			*data *= 2;
			*data |= v3;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			v5 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v5;
			v27 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v27)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v6 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v6;
			v7 = data[3] >> 2;
			data[3] <<= 6;
			data[3] |= v7;
			data[3] ^= 0xB4u;
		}
	}
}

void PacketTwister_72::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST23_1@9
	char v6; // ST22_1@9
	char v7; // ST21_1@9
	char v8; // ST1E_1@17
	char v9; // ST1D_1@17
	char v10; // ST1C_1@19
	char v11; // ST1B_1@19
	char v12; // ST1A_1@19
	char v13; // ST17_1@25
	char v14; // ST16_1@25
	char v15; // ST15_1@25
	char v16; // ST14_1@26
	char v17; // ST13_1@26
	char v18; // ST10_1@32
	char v19; // ST0F_1@32
	char v20; // ST0C_1@38
	char v21; // ST07_1@50
	char v22; // [sp+8h] [bp-20h]@44
	char v23; // [sp+Ah] [bp-1Eh]@38
	char v24; // [sp+Dh] [bp-1Bh]@32
	char v25; // [sp+11h] [bp-17h]@26
	char v26; // [sp+18h] [bp-10h]@19
	char v27; // [sp+1Fh] [bp-9h]@9
	char v2; // [sp+24h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[22];
					data[22] = data[11];
					data[11] = v16;
					v17 = data[13];
					data[13] = data[30];
					data[30] = v17;
					v25 = (data[4] >> 6) & 1;
					if ((data[4] >> 7) & 1)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					if (v25)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					v18 = data[22] >> 2;
					data[22] <<= 6;
					data[22] |= v18;
					v19 = data[23];
					data[23] = data[22];
					data[22] = v19;
					data[21] ^= 0x75u;
					data[22] ^= 0x1Fu;
					v24 = (data[25] >> 2) & 1;
					if ((data[25] >> 5) & 1)
						data[25] |= 4u;
					else
						data[25] &= 0xFBu;
					if (v24)
						data[25] |= 0x20u;
					else
						data[25] &= 0xDFu;
					v20 = data[23];
					data[23] = data[9];
					data[9] = v20;
					v23 = (data[12] >> 6) & 1;
					if ((data[12] >> 6) & 1)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					if (v23)
						data[12] |= 0x40u;
					else
						data[12] &= 0xBFu;
					v22 = (data[4] >> 2) & 1;
					if ((data[4] >> 4) & 1)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					if (v22)
						data[4] |= 0x10u;
					else
						data[4] &= 0xEFu;
					v21 = data[17];
					data[17] = data[22];
					data[22] = v21;
				}
				else
				{
					data[12] ^= 0x8Bu;
					data[12] ^= 0xC3u;
					v10 = data[8] >> 2;
					data[8] <<= 6;
					data[8] |= v10;
					v11 = data[3];
					data[3] = data[4];
					data[4] = v11;
					v12 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v12;
					v26 = (data[1] >> 6) & 1;
					if ((data[1] >> 4) & 1)
						data[1] |= 0x40u;
					else
						data[1] &= 0xBFu;
					if (v26)
						data[1] |= 0x10u;
					else
						data[1] &= 0xEFu;
					v13 = data[3];
					data[3] = data[7];
					data[7] = v13;
					v14 = data[13] >> 1;
					data[13] <<= 7;
					data[13] |= v14;
					v15 = data[13];
					data[13] = data[8];
					data[8] = v15;
				}
			}
			else
			{
				data[7] ^= 0xDAu;
				v8 = data[6];
				data[6] = data[5];
				data[5] = v8;
				v9 = data[6] >> 3;
				data[6] *= 32;
				data[6] |= v9;
			}
		}
		else
		{
			data[3] ^= 0xB4u;
			v3 = data[3] >> 6;
			data[3] *= 4;
			data[3] |= v3;
			v4 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v4;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 2) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			v5 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v5;
			v6 = *data >> 2;
			*data <<= 6;
			*data |= v6;
			v7 = *data >> 1;
			*data <<= 7;
			*data |= v7;
			v27 = (*data >> 2) & 1;
			if ((*data >> 3) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v27)
				*data |= 8u;
			else
				*data &= 0xF7u;
		}
	}
}

void PacketTwister_73::Twist(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2E_1@3
	char v5; // ST2B_1@9
	char v6; // ST26_1@21
	char v7; // ST25_1@23
	char v8; // ST22_1@29
	char v9; // ST1F_1@35
	char v10; // ST1C_1@41
	char v11; // ST13_1@67
	char v12; // ST12_1@67
	char v13; // ST11_1@67
	char v14; // ST10_1@67
	char v15; // ST0F_1@67
	char v16; // ST0E_1@68
	char v17; // ST0D_1@68
	char v18; // ST0C_1@68
	char v19; // ST09_1@74
	char v20; // ST08_1@74
	char v21; // ST07_1@74
	char v22; // ST06_1@74
	char v23; // [sp+4h] [bp-2Ch]@74
	char v24; // [sp+Ah] [bp-26h]@68
	char v25; // [sp+14h] [bp-1Ch]@61
	char v26; // [sp+16h] [bp-1Ah]@55
	char v27; // [sp+18h] [bp-18h]@49
	char v28; // [sp+1Ah] [bp-16h]@43
	char v29; // [sp+1Dh] [bp-13h]@35
	char v30; // [sp+20h] [bp-10h]@29
	char v31; // [sp+23h] [bp-Dh]@23
	char v32; // [sp+27h] [bp-9h]@15
	char v33; // [sp+29h] [bp-7h]@9
	char v2; // [sp+2Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = data[14];
					data[14] = data[1];
					data[1] = v16;
					v17 = data[29];
					data[29] = data[28];
					data[28] = v17;
					v18 = data[16];
					data[16] = data[11];
					data[11] = v18;
					v24 = (data[28] >> 3) & 1;
					if ((data[28] >> 6) & 1)
						data[28] |= 8u;
					else
						data[28] &= 0xF7u;
					if (v24)
						data[28] |= 0x40u;
					else
						data[28] &= 0xBFu;
					v19 = data[8];
					data[8] = data[12];
					data[12] = v19;
					v20 = data[7];
					data[7] = data[3];
					data[3] = v20;
					v21 = data[20] >> 2;
					data[20] <<= 6;
					data[20] |= v21;
					v22 = data[12] >> 6;
					data[12] *= 4;
					data[12] |= v22;
					v23 = (data[15] >> 1) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					if (v23)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
				}
				else
				{
					data[8] ^= 0x17u;
					v28 = (data[12] >> 3) & 1;
					if ((data[12] >> 2) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v28)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					v27 = (data[10] >> 1) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v27)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v26 = (data[4] >> 7) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v26)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v25 = (data[1] >> 5) & 1;
					if ((data[1] >> 7) & 1)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					if (v25)
						data[1] |= 0x80u;
					else
						data[1] &= 0x7Fu;
					v11 = *data >> 4;
					*data *= 16;
					*data |= v11;
					v12 = data[13];
					data[13] = data[8];
					data[8] = v12;
					data[9] ^= 0xD4u;
					v13 = *data >> 7;
					*data *= 2;
					*data |= v13;
					v14 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v14;
					*data ^= 0xB0u;
					v15 = data[12];
					data[12] = data[12];
					data[12] = v15;
				}
			}
			else
			{
				data[2] ^= 0x5Au;
				v7 = data[4] >> 4;
				data[4] *= 16;
				data[4] |= v7;
				v31 = (data[3] >> 4) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v31)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v8 = data[1];
				data[1] = *data;
				*data = v8;
				v30 = (*data >> 4) & 1;
				if ((*data >> 4) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v30)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				v9 = data[4];
				data[4] = data[7];
				data[7] = v9;
				v29 = (data[2] >> 2) & 1;
				if ((data[2] >> 3) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v29)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				v10 = data[5];
				data[5] = data[2];
				data[2] = v10;
			}
		}
		else
		{
			v3 = data[2];
			data[2] = data[3];
			data[3] = v3;
			v4 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v4;
			data[2] ^= 0xE4u;
			*data ^= 0x2Fu;
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v5 = data[2];
			data[2] = *data;
			*data = v5;
			*data ^= 0x6Du;
			v33 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v33)
				*data |= 2u;
			else
				*data &= 0xFDu;
			*data ^= 0x1Bu;
			data[2] ^= 0xDFu;
			v32 = (data[2] >> 2) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v32)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			*data ^= 0xC9u;
			v6 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v6;
		}
	}
}

void PacketTwister_73::Correct(BYTE *data, int length)
{
	char v3; // ST2F_1@3
	char v4; // ST2A_1@15
	char v5; // ST27_1@21
	char v6; // ST26_1@21
	char v7; // ST25_1@23
	char v8; // ST22_1@29
	char v9; // ST1F_1@35
	char v10; // ST1C_1@41
	char v11; // ST1B_1@43
	char v12; // ST1A_1@43
	char v13; // ST19_1@43
	char v14; // ST18_1@43
	char v15; // ST17_1@43
	char v16; // ST0C_1@74
	char v17; // ST0B_1@74
	char v18; // ST0A_1@74
	char v19; // ST09_1@74
	char v20; // ST06_1@80
	char v21; // ST05_1@80
	char v22; // ST04_1@80
	char v23; // [sp+7h] [bp-29h]@74
	char v24; // [sp+Dh] [bp-23h]@68
	char v25; // [sp+Fh] [bp-21h]@61
	char v26; // [sp+11h] [bp-1Fh]@55
	char v27; // [sp+13h] [bp-1Dh]@49
	char v28; // [sp+15h] [bp-1Bh]@43
	char v29; // [sp+1Dh] [bp-13h]@35
	char v30; // [sp+20h] [bp-10h]@29
	char v31; // [sp+23h] [bp-Dh]@23
	char v32; // [sp+28h] [bp-8h]@15
	char v33; // [sp+2Bh] [bp-5h]@9
	char v2; // [sp+2Dh] [bp-3h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v24 = (data[15] >> 1) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 2u;
					else
						data[15] &= 0xFDu;
					if (v24)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					v16 = data[12] >> 2;
					data[12] <<= 6;
					data[12] |= v16;
					v17 = data[20] >> 6;
					data[20] *= 4;
					data[20] |= v17;
					v18 = data[7];
					data[7] = data[3];
					data[3] = v18;
					v19 = data[8];
					data[8] = data[12];
					data[12] = v19;
					v23 = (data[28] >> 3) & 1;
					if ((data[28] >> 6) & 1)
						data[28] |= 8u;
					else
						data[28] &= 0xF7u;
					if (v23)
						data[28] |= 0x40u;
					else
						data[28] &= 0xBFu;
					v20 = data[16];
					data[16] = data[11];
					data[11] = v20;
					v21 = data[29];
					data[29] = data[28];
					data[28] = v21;
					v22 = data[14];
					data[14] = data[1];
					data[1] = v22;
				}
				else
				{
					v11 = data[12];
					data[12] = data[12];
					data[12] = v11;
					*data ^= 0xB0u;
					v12 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v12;
					v13 = *data >> 1;
					*data <<= 7;
					*data |= v13;
					data[9] ^= 0xD4u;
					v14 = data[13];
					data[13] = data[8];
					data[8] = v14;
					v15 = *data >> 4;
					*data *= 16;
					*data |= v15;
					v28 = (data[1] >> 5) & 1;
					if ((data[1] >> 7) & 1)
						data[1] |= 0x20u;
					else
						data[1] &= 0xDFu;
					if (v28)
						data[1] |= 0x80u;
					else
						data[1] &= 0x7Fu;
					v27 = (data[4] >> 7) & 1;
					if ((data[4] >> 6) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v27)
						data[4] |= 0x40u;
					else
						data[4] &= 0xBFu;
					v26 = (data[10] >> 1) & 1;
					if ((data[10] >> 4) & 1)
						data[10] |= 2u;
					else
						data[10] &= 0xFDu;
					if (v26)
						data[10] |= 0x10u;
					else
						data[10] &= 0xEFu;
					v25 = (data[12] >> 3) & 1;
					if ((data[12] >> 2) & 1)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					if (v25)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					data[8] ^= 0x17u;
				}
			}
			else
			{
				v7 = data[5];
				data[5] = data[2];
				data[2] = v7;
				v31 = (data[2] >> 2) & 1;
				if ((data[2] >> 3) & 1)
					data[2] |= 4u;
				else
					data[2] &= 0xFBu;
				if (v31)
					data[2] |= 8u;
				else
					data[2] &= 0xF7u;
				v8 = data[4];
				data[4] = data[7];
				data[7] = v8;
				v30 = (*data >> 4) & 1;
				if ((*data >> 4) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v30)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				v9 = data[1];
				data[1] = *data;
				*data = v9;
				v29 = (data[3] >> 4) & 1;
				if ((data[3] >> 1) & 1)
					data[3] |= 0x10u;
				else
					data[3] &= 0xEFu;
				if (v29)
					data[3] |= 2u;
				else
					data[3] &= 0xFDu;
				v10 = data[4] >> 4;
				data[4] *= 16;
				data[4] |= v10;
				data[2] ^= 0x5Au;
			}
		}
		else
		{
			v3 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v3;
			*data ^= 0xC9u;
			v2 = (data[2] >> 2) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 4u;
			else
				data[2] &= 0xFBu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			data[2] ^= 0xDFu;
			*data ^= 0x1Bu;
			v33 = (*data >> 2) & 1;
			if ((*data >> 1) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v33)
				*data |= 2u;
			else
				*data &= 0xFDu;
			*data ^= 0x6Du;
			v4 = data[2];
			data[2] = *data;
			*data = v4;
			v32 = (data[3] >> 2) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v32)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			*data ^= 0x2Fu;
			data[2] ^= 0xE4u;
			v5 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v5;
			v6 = data[2];
			data[2] = data[3];
			data[3] = v6;
		}
	}
}

void PacketTwister_74::Twist(BYTE *data, int length)
{
	char v3; // ST29_1@9
	char v4; // ST20_1@35
	char v5; // ST1D_1@41
	char v6; // ST1A_1@47
	char v7; // ST15_1@61
	char v8; // ST14_1@61
	char v9; // ST13_1@61
	char v10; // ST0E_1@73
	char v11; // ST0B_1@79
	char v12; // ST08_1@86
	char v13; // [sp+4h] [bp-28h]@92
	char v14; // [sp+6h] [bp-26h]@86
	char v15; // [sp+9h] [bp-23h]@79
	char v16; // [sp+Ch] [bp-20h]@73
	char v17; // [sp+Fh] [bp-1Dh]@67
	char v18; // [sp+11h] [bp-1Bh]@61
	char v19; // [sp+16h] [bp-16h]@55
	char v20; // [sp+18h] [bp-14h]@49
	char v21; // [sp+1Bh] [bp-11h]@41
	char v22; // [sp+1Eh] [bp-Eh]@35
	char v23; // [sp+21h] [bp-Bh]@29
	char v24; // [sp+23h] [bp-9h]@23
	char v25; // [sp+25h] [bp-7h]@15
	char v26; // [sp+27h] [bp-5h]@9
	char v2; // [sp+2Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v12 = data[6] >> 1;
					data[6] <<= 7;
					data[6] |= v12;
					v14 = (data[27] >> 1) & 1;
					if ((data[27] >> 6) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v14)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					v13 = (data[27] >> 4) & 1;
					if ((data[27] >> 6) & 1)
						data[27] |= 0x10u;
					else
						data[27] &= 0xEFu;
					if (v13)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					data[31] ^= 0xC7u;
					data[25] ^= 0x45u;
				}
				else
				{
					v20 = (data[14] >> 6) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v20)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
					v19 = (data[11] >> 3) & 1;
					if ((data[11] >> 5) & 1)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					if (v19)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
					v7 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v7;
					v8 = data[4];
					data[4] = data[14];
					data[14] = v8;
					data[3] ^= 0xADu;
					v9 = data[10];
					data[10] = data[13];
					data[13] = v9;
					v18 = (data[5] >> 2) & 1;
					if ((data[5] >> 3) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v18)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					v17 = (data[11] >> 2) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v17)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v10 = data[9];
					data[9] = data[6];
					data[6] = v10;
					v16 = (data[4] >> 7) & 1;
					if ((data[4] >> 5) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v16)
						data[4] |= 0x20u;
					else
						data[4] &= 0xDFu;
					v11 = data[6];
					data[6] = data[9];
					data[9] = v11;
					v15 = (data[3] >> 6) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					if (v15)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					data[10] ^= 3u;
				}
			}
			else
			{
				v24 = (data[5] >> 1) & 1;
				if ((data[5] >> 6) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v24)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
				v23 = (data[5] >> 6) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
				if (v23)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v4 = data[6] >> 7;
				data[6] *= 2;
				data[6] |= v4;
				v22 = (data[5] >> 5) & 1;
				if ((data[5] >> 7) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v22)
					data[5] |= 0x80u;
				else
					data[5] &= 0x7Fu;
				data[5] ^= 0x45u;
				*data ^= 0xF3u;
				v5 = data[1];
				data[1] = data[5];
				data[5] = v5;
				v21 = (data[4] >> 5) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v21)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				v6 = data[6] >> 6;
				data[6] *= 4;
				data[6] |= v6;
			}
		}
		else
		{
			v2 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			data[2] ^= 3u;
			v26 = (*data >> 2) & 1;
			if ((*data >> 2) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v26)
				*data |= 4u;
			else
				*data &= 0xFBu;
			v25 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v25)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
		}
	}
}

void PacketTwister_74::Correct(BYTE *data, int length)
{
	char v3; // ST27_1@15
	char v4; // ST24_1@23
	char v5; // ST21_1@29
	char v6; // ST1E_1@35
	char v7; // ST17_1@55
	char v8; // ST14_1@61
	char v9; // ST0F_1@73
	char v10; // ST0E_1@73
	char v11; // ST0D_1@73
	char v12; // ST04_1@98
	char v13; // [sp+5h] [bp-27h]@92
	char v14; // [sp+7h] [bp-25h]@86
	char v15; // [sp+9h] [bp-23h]@79
	char v16; // [sp+Bh] [bp-21h]@73
	char v17; // [sp+10h] [bp-1Ch]@67
	char v18; // [sp+12h] [bp-1Ah]@61
	char v19; // [sp+15h] [bp-17h]@55
	char v20; // [sp+18h] [bp-14h]@49
	char v21; // [sp+1Ah] [bp-12h]@41
	char v22; // [sp+1Ch] [bp-10h]@35
	char v23; // [sp+1Fh] [bp-Dh]@29
	char v24; // [sp+22h] [bp-Ah]@23
	char v25; // [sp+25h] [bp-7h]@15
	char v26; // [sp+28h] [bp-4h]@9
	char v2; // [sp+2Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[25] ^= 0x45u;
					data[31] ^= 0xC7u;
					v14 = (data[27] >> 4) & 1;
					if ((data[27] >> 6) & 1)
						data[27] |= 0x10u;
					else
						data[27] &= 0xEFu;
					if (v14)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					v13 = (data[27] >> 1) & 1;
					if ((data[27] >> 6) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v13)
						data[27] |= 0x40u;
					else
						data[27] &= 0xBFu;
					v12 = data[6] >> 7;
					data[6] *= 2;
					data[6] |= v12;
				}
				else
				{
					data[10] ^= 3u;
					v20 = (data[3] >> 6) & 1;
					if ((data[3] >> 1) & 1)
						data[3] |= 0x40u;
					else
						data[3] &= 0xBFu;
					if (v20)
						data[3] |= 2u;
					else
						data[3] &= 0xFDu;
					v7 = data[6];
					data[6] = data[9];
					data[9] = v7;
					v19 = (data[4] >> 7) & 1;
					if ((data[4] >> 5) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v19)
						data[4] |= 0x20u;
					else
						data[4] &= 0xDFu;
					v8 = data[9];
					data[9] = data[6];
					data[6] = v8;
					v18 = (data[11] >> 2) & 1;
					if ((data[11] >> 4) & 1)
						data[11] |= 4u;
					else
						data[11] &= 0xFBu;
					if (v18)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					v17 = (data[5] >> 2) & 1;
					if ((data[5] >> 3) & 1)
						data[5] |= 4u;
					else
						data[5] &= 0xFBu;
					if (v17)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					v9 = data[10];
					data[10] = data[13];
					data[13] = v9;
					data[3] ^= 0xADu;
					v10 = data[4];
					data[4] = data[14];
					data[14] = v10;
					v11 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v11;
					v16 = (data[11] >> 3) & 1;
					if ((data[11] >> 5) & 1)
						data[11] |= 8u;
					else
						data[11] &= 0xF7u;
					if (v16)
						data[11] |= 0x20u;
					else
						data[11] &= 0xDFu;
					v15 = (data[14] >> 6) & 1;
					if ((data[14] >> 1) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v15)
						data[14] |= 2u;
					else
						data[14] &= 0xFDu;
				}
			}
			else
			{
				v4 = data[6] >> 2;
				data[6] <<= 6;
				data[6] |= v4;
				v24 = (data[4] >> 5) & 1;
				if ((data[4] >> 4) & 1)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				if (v24)
					data[4] |= 0x10u;
				else
					data[4] &= 0xEFu;
				v5 = data[1];
				data[1] = data[5];
				data[5] = v5;
				*data ^= 0xF3u;
				data[5] ^= 0x45u;
				v23 = (data[5] >> 5) & 1;
				if ((data[5] >> 7) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v23)
					data[5] |= 0x80u;
				else
					data[5] &= 0x7Fu;
				v6 = data[6] >> 1;
				data[6] <<= 7;
				data[6] |= v6;
				v22 = (data[5] >> 6) & 1;
				if ((data[5] >> 3) & 1)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
				if (v22)
					data[5] |= 8u;
				else
					data[5] &= 0xF7u;
				v21 = (data[5] >> 1) & 1;
				if ((data[5] >> 6) & 1)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				if (v21)
					data[5] |= 0x40u;
				else
					data[5] &= 0xBFu;
			}
		}
		else
		{
			v2 = (data[3] >> 7) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			if (v2)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
			v26 = (*data >> 2) & 1;
			if ((*data >> 2) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v26)
				*data |= 4u;
			else
				*data &= 0xFBu;
			data[2] ^= 3u;
			v3 = data[1];
			data[1] = *data;
			*data = v3;
			v25 = (data[3] >> 2) & 1;
			if ((data[3] >> 1) & 1)
				data[3] |= 4u;
			else
				data[3] &= 0xFBu;
			if (v25)
				data[3] |= 2u;
			else
				data[3] &= 0xFDu;
		}
	}
}

void PacketTwister_75::Twist(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@11
	char v6; // ST16_1@23
	char v7; // ST15_1@23
	char v8; // ST12_1@31
	char v9; // ST11_1@31
	char v10; // ST10_1@31
	char v11; // ST0F_1@31
	char v12; // ST0E_1@31
	char v13; // ST0D_1@31
	char v14; // ST0A_1@37
	char v15; // ST09_1@37
	char v16; // ST08_1@37
	char v17; // ST05_1@44
	char v18; // [sp+6h] [bp-1Ah]@38
	char v19; // [sp+Bh] [bp-15h]@31
	char v20; // [sp+13h] [bp-Dh]@23
	char v21; // [sp+17h] [bp-9h]@17
	char v22; // [sp+19h] [bp-7h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = (data[14] >> 7) & 1;
					if ((data[14] >> 6) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v18)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					data[18] ^= 0x4Du;
					v17 = data[26];
					data[26] = data[6];
					data[6] = v17;
				}
				else
				{
					v8 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v8;
					data[12] ^= 0xFu;
					v9 = data[14];
					data[14] = data[1];
					data[1] = v9;
					v10 = data[7];
					data[7] = data[8];
					data[8] = v10;
					data[3] ^= 0xB8u;
					v11 = data[12];
					data[12] = data[3];
					data[3] = v11;
					v12 = data[14];
					data[14] = data[12];
					data[12] = v12;
					v13 = data[14] >> 5;
					data[14] *= 8;
					data[14] |= v13;
					v19 = (data[2] >> 5) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					if (v19)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					data[1] ^= 0xD5u;
					v14 = data[9];
					data[9] = data[14];
					data[14] = v14;
					v15 = data[12];
					data[12] = data[6];
					data[6] = v15;
					v16 = data[2];
					data[2] = data[4];
					data[4] = v16;
				}
			}
			else
			{
				data[2] ^= 0xC5u;
				v5 = data[5];
				data[5] = data[3];
				data[3] = v5;
				v22 = (data[4] >> 6) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				if (v22)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v21 = (data[1] >> 1) & 1;
				if ((data[1] >> 7) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v21)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				v6 = data[5] >> 5;
				data[5] *= 8;
				data[5] |= v6;
				v7 = data[7];
				data[7] = data[5];
				data[5] = v7;
				v20 = (data[6] >> 4) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v20)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
			}
		}
		else
		{
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
			v3 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v3;
			v4 = data[1];
			data[1] = data[2];
			data[2] = v4;
		}
	}
}

void PacketTwister_75::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST19_1@17
	char v6; // ST18_1@17
	char v7; // ST13_1@29
	char v8; // ST12_1@31
	char v9; // ST11_1@31
	char v10; // ST10_1@31
	char v11; // ST0D_1@37
	char v12; // ST0C_1@37
	char v13; // ST0B_1@37
	char v14; // ST0A_1@37
	char v15; // ST09_1@37
	char v16; // ST08_1@37
	char v17; // ST07_1@38
	char v18; // [sp+5h] [bp-1Bh]@38
	char v19; // [sp+Eh] [bp-12h]@31
	char v20; // [sp+14h] [bp-Ch]@23
	char v21; // [sp+16h] [bp-Ah]@17
	char v22; // [sp+1Ah] [bp-6h]@11
	char v2; // [sp+1Ch] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = data[26];
					data[26] = data[6];
					data[6] = v17;
					data[18] ^= 0x4Du;
					v18 = (data[14] >> 7) & 1;
					if ((data[14] >> 6) & 1)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					if (v18)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
				}
				else
				{
					v8 = data[2];
					data[2] = data[4];
					data[4] = v8;
					v9 = data[12];
					data[12] = data[6];
					data[6] = v9;
					v10 = data[9];
					data[9] = data[14];
					data[14] = v10;
					data[1] ^= 0xD5u;
					v19 = (data[2] >> 5) & 1;
					if ((data[2] >> 1) & 1)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					if (v19)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					v11 = data[14] >> 3;
					data[14] *= 32;
					data[14] |= v11;
					v12 = data[14];
					data[14] = data[12];
					data[12] = v12;
					v13 = data[12];
					data[12] = data[3];
					data[3] = v13;
					data[3] ^= 0xB8u;
					v14 = data[7];
					data[7] = data[8];
					data[8] = v14;
					v15 = data[14];
					data[14] = data[1];
					data[1] = v15;
					data[12] ^= 0xFu;
					v16 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v16;
				}
			}
			else
			{
				v22 = (data[6] >> 4) & 1;
				if ((data[6] >> 7) & 1)
					data[6] |= 0x10u;
				else
					data[6] &= 0xEFu;
				if (v22)
					data[6] |= 0x80u;
				else
					data[6] &= 0x7Fu;
				v5 = data[7];
				data[7] = data[5];
				data[5] = v5;
				v6 = data[5] >> 3;
				data[5] *= 32;
				data[5] |= v6;
				v21 = (data[1] >> 1) & 1;
				if ((data[1] >> 7) & 1)
					data[1] |= 2u;
				else
					data[1] &= 0xFDu;
				if (v21)
					data[1] |= 0x80u;
				else
					data[1] &= 0x7Fu;
				v20 = (data[4] >> 6) & 1;
				if ((data[4] >> 5) & 1)
					data[4] |= 0x40u;
				else
					data[4] &= 0xBFu;
				if (v20)
					data[4] |= 0x20u;
				else
					data[4] &= 0xDFu;
				v7 = data[5];
				data[5] = data[3];
				data[3] = v7;
				data[2] ^= 0xC5u;
			}
		}
		else
		{
			v3 = data[1];
			data[1] = data[2];
			data[2] = v3;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 3) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 8u;
			else
				data[3] &= 0xF7u;
		}
	}
}

void PacketTwister_76::Twist(BYTE *data, int length)
{
	char v3; // ST27_1@3
	char v4; // ST26_1@3
	char v5; // ST25_1@3
	char v6; // ST24_1@3
	char v7; // ST21_1@9
	char v8; // ST1C_1@23
	char v9; // ST19_1@29
	char v10; // ST18_1@29
	char v11; // ST17_1@29
	char v12; // ST16_1@29
	char v13; // ST15_1@29
	char v14; // ST14_1@31
	char v15; // ST13_1@31
	char v16; // ST0E_1@43
	char v17; // ST0D_1@43
	char v18; // ST0C_1@44
	char v19; // ST07_1@56
	char v20; // ST06_1@56
	char v21; // ST05_1@56
	char v22; // ST04_1@56
	char v23; // [sp+8h] [bp-20h]@50
	char v24; // [sp+Ah] [bp-1Eh]@44
	char v25; // [sp+Fh] [bp-19h]@37
	char v26; // [sp+11h] [bp-17h]@31
	char v27; // [sp+1Ah] [bp-Eh]@23
	char v28; // [sp+1Dh] [bp-Bh]@15
	char v29; // [sp+1Fh] [bp-9h]@9
	char v2; // [sp+22h] [bp-6h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[2] >> 5;
					data[2] *= 8;
					data[2] |= v18;
					data[18] ^= 0xEu;
					v24 = (data[20] >> 2) & 1;
					if ((data[20] >> 7) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v24)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					v23 = (data[4] >> 7) & 1;
					if ((data[4] >> 2) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v23)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					data[30] ^= 0x61u;
					v19 = data[24] >> 7;
					data[24] *= 2;
					data[24] |= v19;
					data[8] ^= 0x68u;
					v20 = data[17] >> 4;
					data[17] *= 16;
					data[17] |= v20;
					v21 = data[11];
					data[11] = data[24];
					data[24] = v21;
					v22 = data[30];
					data[30] = data[24];
					data[24] = v22;
				}
				else
				{
					v14 = data[6] >> 6;
					data[6] *= 4;
					data[6] |= v14;
					data[3] ^= 0xCEu;
					data[12] ^= 0xFAu;
					data[6] ^= 0x52u;
					data[5] ^= 0xF3u;
					v15 = data[6] >> 5;
					data[6] *= 8;
					data[6] |= v15;
					v26 = (*data >> 6) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x40u;
					else
						*data &= 0xBFu;
					if (v26)
						*data |= 2u;
					else
						*data &= 0xFDu;
					data[10] ^= 0xF0u;
					v25 = (data[2] >> 6) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v25)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					v16 = *data >> 6;
					*data *= 4;
					*data |= v16;
					v17 = data[12];
					data[12] = data[13];
					data[13] = v17;
				}
			}
			else
			{
				v8 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v8;
				v27 = (data[7] >> 1) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v27)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				data[6] ^= 0x73u;
				v9 = data[4];
				data[4] = data[4];
				data[4] = v9;
				v10 = data[5];
				data[5] = *data;
				*data = v10;
				v11 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v11;
				data[1] ^= 0x9Du;
				v12 = data[1] >> 7;
				data[1] *= 2;
				data[1] |= v12;
				v13 = data[7] >> 1;
				data[7] <<= 7;
				data[7] |= v13;
				data[3] ^= 0x48u;
				data[1] ^= 0x60u;
			}
		}
		else
		{
			v3 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v3;
			v4 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v4;
			v5 = *data;
			*data = data[1];
			data[1] = v5;
			v6 = *data >> 2;
			*data <<= 6;
			*data |= v6;
			v2 = (*data >> 3) & 1;
			if ((*data >> 4) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v2)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			v7 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v7;
			v29 = (*data >> 2) & 1;
			if ((*data >> 7) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v29)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v28 = (data[1] >> 6) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v28)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
		}
	}
}

void PacketTwister_76::Correct(BYTE *data, int length)
{
	char v3; // ST23_1@15
	char v4; // ST20_1@21
	char v5; // ST1F_1@21
	char v6; // ST1E_1@21
	char v7; // ST1D_1@21
	char v8; // ST1C_1@23
	char v9; // ST1B_1@23
	char v10; // ST1A_1@23
	char v11; // ST19_1@23
	char v12; // ST18_1@23
	char v13; // ST15_1@29
	char v14; // ST14_1@31
	char v15; // ST13_1@31
	char v16; // ST0E_1@43
	char v17; // ST0D_1@43
	char v18; // ST0C_1@44
	char v19; // ST0B_1@44
	char v20; // ST0A_1@44
	char v21; // ST09_1@44
	char v22; // ST04_1@56
	char v23; // [sp+5h] [bp-23h]@50
	char v24; // [sp+7h] [bp-21h]@44
	char v25; // [sp+Fh] [bp-19h]@37
	char v26; // [sp+11h] [bp-17h]@31
	char v27; // [sp+16h] [bp-12h]@23
	char v28; // [sp+21h] [bp-7h]@15
	char v29; // [sp+24h] [bp-4h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v18 = data[30];
					data[30] = data[24];
					data[24] = v18;
					v19 = data[11];
					data[11] = data[24];
					data[24] = v19;
					v20 = data[17] >> 4;
					data[17] *= 16;
					data[17] |= v20;
					data[8] ^= 0x68u;
					v21 = data[24] >> 1;
					data[24] <<= 7;
					data[24] |= v21;
					data[30] ^= 0x61u;
					v24 = (data[4] >> 7) & 1;
					if ((data[4] >> 2) & 1)
						data[4] |= 0x80u;
					else
						data[4] &= 0x7Fu;
					if (v24)
						data[4] |= 4u;
					else
						data[4] &= 0xFBu;
					v23 = (data[20] >> 2) & 1;
					if ((data[20] >> 7) & 1)
						data[20] |= 4u;
					else
						data[20] &= 0xFBu;
					if (v23)
						data[20] |= 0x80u;
					else
						data[20] &= 0x7Fu;
					data[18] ^= 0xEu;
					v22 = data[2] >> 3;
					data[2] *= 32;
					data[2] |= v22;
				}
				else
				{
					v14 = data[12];
					data[12] = data[13];
					data[13] = v14;
					v15 = *data >> 2;
					*data <<= 6;
					*data |= v15;
					v26 = (data[2] >> 6) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 0x40u;
					else
						data[2] &= 0xBFu;
					if (v26)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					data[10] ^= 0xF0u;
					v25 = (*data >> 6) & 1;
					if ((*data >> 1) & 1)
						*data |= 0x40u;
					else
						*data &= 0xBFu;
					if (v25)
						*data |= 2u;
					else
						*data &= 0xFDu;
					v16 = data[6] >> 3;
					data[6] *= 32;
					data[6] |= v16;
					data[5] ^= 0xF3u;
					data[6] ^= 0x52u;
					data[12] ^= 0xFAu;
					data[3] ^= 0xCEu;
					v17 = data[6] >> 2;
					data[6] <<= 6;
					data[6] |= v17;
				}
			}
			else
			{
				data[1] ^= 0x60u;
				data[3] ^= 0x48u;
				v8 = data[7] >> 7;
				data[7] *= 2;
				data[7] |= v8;
				v9 = data[1] >> 1;
				data[1] <<= 7;
				data[1] |= v9;
				data[1] ^= 0x9Du;
				v10 = data[3] >> 2;
				data[3] <<= 6;
				data[3] |= v10;
				v11 = data[5];
				data[5] = *data;
				*data = v11;
				v12 = data[4];
				data[4] = data[4];
				data[4] = v12;
				data[6] ^= 0x73u;
				v27 = (data[7] >> 1) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 2u;
				else
					data[7] &= 0xFDu;
				if (v27)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v13 = data[3] >> 6;
				data[3] *= 4;
				data[3] |= v13;
			}
		}
		else
		{
			v2 = (data[1] >> 6) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v29 = (*data >> 2) & 1;
			if ((*data >> 7) & 1)
				*data |= 4u;
			else
				*data &= 0xFBu;
			if (v29)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v3 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v3;
			v28 = (*data >> 3) & 1;
			if ((*data >> 4) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v28)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			v4 = *data >> 6;
			*data *= 4;
			*data |= v4;
			v5 = *data;
			*data = data[1];
			data[1] = v5;
			v6 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v6;
			v7 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v7;
		}
	}
}

void PacketTwister_77::Twist(BYTE *data, int length)
{
	char v3; // ST1D_1@9
	char v4; // ST1C_1@9
	char v5; // ST1B_1@9
	char v6; // ST1A_1@9
	char v7; // ST19_1@9
	char v8; // ST16_1@17
	char v9; // ST15_1@17
	char v10; // ST14_1@19
	char v11; // ST13_1@19
	char v12; // ST12_1@19
	char v13; // ST0F_1@25
	char v14; // ST0C_1@32
	char v15; // ST0B_1@32
	char v16; // ST0A_1@32
	char v17; // ST09_1@32
	char v18; // ST04_1@44
	char v19; // [sp+5h] [bp-1Bh]@38
	char v20; // [sp+7h] [bp-19h]@32
	char v21; // [sp+Dh] [bp-13h]@26
	char v22; // [sp+10h] [bp-10h]@19
	char v23; // [sp+17h] [bp-9h]@11
	char v2; // [sp+1Eh] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = (data[25] >> 1) & 1;
					if ((data[25] >> 1) & 1)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
					if (v21)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
					data[7] ^= 0x49u;
					v14 = *data >> 6;
					*data *= 4;
					*data |= v14;
					v15 = data[4] >> 2;
					data[4] <<= 6;
					data[4] |= v15;
					v16 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v16;
					v17 = data[16] >> 6;
					data[16] *= 4;
					data[16] |= v17;
					v20 = (data[2] >> 7) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v20)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					v19 = (data[12] >> 7) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 0x80u;
					else
						data[12] &= 0x7Fu;
					if (v19)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					data[25] ^= 0xBu;
					v18 = data[31];
					data[31] = data[22];
					data[22] = v18;
				}
				else
				{
					data[7] ^= 0x79u;
					data[12] ^= 0x2Eu;
					v10 = data[3];
					data[3] = data[6];
					data[6] = v10;
					v11 = data[14];
					data[14] = data[13];
					data[13] = v11;
					v12 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v12;
					v22 = (data[9] >> 6) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v22)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					v13 = data[8] >> 3;
					data[8] *= 32;
					data[8] |= v13;
					data[11] ^= 0x52u;
				}
			}
			else
			{
				v23 = (data[4] >> 3) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 8u;
				else
					data[4] &= 0xF7u;
				if (v23)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
				v8 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v8;
				data[1] ^= 0xC9u;
				data[7] ^= 0x22u;
				data[3] ^= 0xE8u;
				v9 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v9;
			}
		}
		else
		{
			v2 = (*data >> 1) & 1;
			if ((*data >> 3) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 8u;
			else
				*data &= 0xF7u;
			data[3] ^= 0x41u;
			v3 = *data;
			*data = data[1];
			data[1] = v3;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			v5 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v5;
			v6 = *data >> 4;
			*data *= 16;
			*data |= v6;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
		}
	}
}

void PacketTwister_77::Correct(BYTE *data, int length)
{
	char v3; // ST1F_1@3
	char v4; // ST1E_1@3
	char v5; // ST1D_1@3
	char v6; // ST1C_1@3
	char v7; // ST1B_1@3
	char v8; // ST18_1@11
	char v9; // ST17_1@11
	char v10; // ST14_1@19
	char v11; // ST11_1@25
	char v12; // ST10_1@25
	char v13; // ST0F_1@25
	char v14; // ST0E_1@26
	char v15; // ST09_1@38
	char v16; // ST08_1@38
	char v17; // ST07_1@38
	char v18; // ST06_1@38
	char v19; // [sp+4h] [bp-1Ch]@38
	char v20; // [sp+Ah] [bp-16h]@32
	char v21; // [sp+Ch] [bp-14h]@26
	char v22; // [sp+12h] [bp-Eh]@19
	char v23; // [sp+15h] [bp-Bh]@11
	char v2; // [sp+19h] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v14 = data[31];
					data[31] = data[22];
					data[22] = v14;
					data[25] ^= 0xBu;
					v21 = (data[12] >> 7) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 0x80u;
					else
						data[12] &= 0x7Fu;
					if (v21)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v20 = (data[2] >> 7) & 1;
					if ((data[2] >> 5) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v20)
						data[2] |= 0x20u;
					else
						data[2] &= 0xDFu;
					v15 = data[16] >> 2;
					data[16] <<= 6;
					data[16] |= v15;
					v16 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v16;
					v17 = data[4] >> 6;
					data[4] *= 4;
					data[4] |= v17;
					v18 = *data >> 2;
					*data <<= 6;
					*data |= v18;
					data[7] ^= 0x49u;
					v19 = (data[25] >> 1) & 1;
					if ((data[25] >> 1) & 1)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
					if (v19)
						data[25] |= 2u;
					else
						data[25] &= 0xFDu;
				}
				else
				{
					data[11] ^= 0x52u;
					v10 = data[8] >> 5;
					data[8] *= 8;
					data[8] |= v10;
					v22 = (data[9] >> 6) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v22)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					v11 = data[14] >> 4;
					data[14] *= 16;
					data[14] |= v11;
					v12 = data[14];
					data[14] = data[13];
					data[13] = v12;
					v13 = data[3];
					data[3] = data[6];
					data[6] = v13;
					data[12] ^= 0x2Eu;
					data[7] ^= 0x79u;
				}
			}
			else
			{
				v8 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v8;
				data[3] ^= 0xE8u;
				data[7] ^= 0x22u;
				data[1] ^= 0xC9u;
				v9 = data[3] >> 4;
				data[3] *= 16;
				data[3] |= v9;
				v23 = (data[4] >> 3) & 1;
				if ((data[4] >> 2) & 1)
					data[4] |= 8u;
				else
					data[4] &= 0xF7u;
				if (v23)
					data[4] |= 4u;
				else
					data[4] &= 0xFBu;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = *data;
			*data = v3;
			v4 = *data >> 4;
			*data *= 16;
			*data |= v4;
			v5 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v5;
			v6 = *data;
			*data = data[1];
			data[1] = v6;
			v7 = *data;
			*data = data[1];
			data[1] = v7;
			data[3] ^= 0x41u;
			v2 = (*data >> 1) & 1;
			if ((*data >> 3) & 1)
				*data |= 2u;
			else
				*data &= 0xFDu;
			if (v2)
				*data |= 8u;
			else
				*data &= 0xF7u;
		}
	}
}

void PacketTwister_78::Twist(BYTE *data, int length)
{
	char v3; // ST21_1@21
	char v4; // ST20_1@21
	char v5; // ST1F_1@21
	char v6; // ST1A_1@35
	char v7; // ST19_1@35
	char v8; // ST18_1@35
	char v9; // ST15_1@41
	char v10; // ST14_1@43
	char v11; // ST0F_1@55
	char v12; // ST0E_1@55
	char v13; // ST09_1@68
	char v14; // ST06_1@74
	char v15; // [sp+7h] [bp-21h]@68
	char v16; // [sp+Ah] [bp-1Eh]@62
	char v17; // [sp+Ch] [bp-1Ch]@56
	char v18; // [sp+10h] [bp-18h]@49
	char v19; // [sp+12h] [bp-16h]@43
	char v20; // [sp+16h] [bp-12h]@35
	char v21; // [sp+1Bh] [bp-Dh]@29
	char v22; // [sp+1Dh] [bp-Bh]@21
	char v23; // [sp+22h] [bp-6h]@15
	char v24; // [sp+24h] [bp-4h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[2] >> 1) & 1;
					if ((data[2] >> 2) & 1)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					if (v17)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
					v16 = (data[28] >> 4) & 1;
					if ((data[28] >> 5) & 1)
						data[28] |= 0x10u;
					else
						data[28] &= 0xEFu;
					if (v16)
						data[28] |= 0x20u;
					else
						data[28] &= 0xDFu;
					v13 = data[17] >> 6;
					data[17] *= 4;
					data[17] |= v13;
					v15 = (data[23] >> 1) & 1;
					if ((data[23] >> 5) & 1)
						data[23] |= 2u;
					else
						data[23] &= 0xFDu;
					if (v15)
						data[23] |= 0x20u;
					else
						data[23] &= 0xDFu;
					data[16] ^= 0x45u;
					v14 = data[27] >> 6;
					data[27] *= 4;
					data[27] |= v14;
				}
				else
				{
					v10 = data[11] >> 4;
					data[11] *= 16;
					data[11] |= v10;
					v19 = (data[5] >> 5) & 1;
					if ((data[5] >> 3) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v19)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					v18 = (data[2] >> 7) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v18)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v11 = data[10] >> 2;
					data[10] <<= 6;
					data[10] |= v11;
					data[2] ^= 0x24u;
					v12 = data[7] >> 6;
					data[7] *= 4;
					data[7] |= v12;
				}
			}
			else
			{
				data[4] ^= 0x17u;
				v21 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v21)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				data[6] ^= 0xBEu;
				v6 = data[4] >> 2;
				data[4] <<= 6;
				data[4] |= v6;
				v7 = data[3];
				data[3] = data[1];
				data[1] = v7;
				v8 = data[7] >> 6;
				data[7] *= 4;
				data[7] |= v8;
				v20 = (data[4] >> 7) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v20)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v9 = data[2];
				data[2] = data[3];
				data[3] = v9;
			}
		}
		else
		{
			data[3] ^= 0xC8u;
			v2 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v2)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[3] ^= 0xC0u;
			v24 = (data[1] >> 1) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v24)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v23 = (*data >> 5) & 1;
			if ((*data >> 7) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v23)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v3 = *data >> 6;
			*data *= 4;
			*data |= v3;
			data[3] ^= 0x80u;
			v4 = *data;
			*data = *data;
			*data = v4;
			v5 = data[2];
			data[2] = data[1];
			data[1] = v5;
			v22 = (data[3] >> 4) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v22)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			data[2] ^= 0x57u;
		}
	}
}

void PacketTwister_78::Correct(BYTE *data, int length)
{
	char v3; // ST25_1@9
	char v4; // ST24_1@9
	char v5; // ST23_1@9
	char v6; // ST1C_1@29
	char v7; // ST19_1@35
	char v8; // ST18_1@35
	char v9; // ST17_1@35
	char v10; // ST14_1@43
	char v11; // ST13_1@43
	char v12; // ST0E_1@55
	char v13; // ST0D_1@56
	char v14; // ST0A_1@62
	char v15; // [sp+6h] [bp-22h]@68
	char v16; // [sp+8h] [bp-20h]@62
	char v17; // [sp+Bh] [bp-1Dh]@56
	char v18; // [sp+Fh] [bp-19h]@49
	char v19; // [sp+11h] [bp-17h]@43
	char v20; // [sp+15h] [bp-13h]@35
	char v21; // [sp+1Ah] [bp-Eh]@29
	char v22; // [sp+1Dh] [bp-Bh]@21
	char v23; // [sp+1Fh] [bp-9h]@15
	char v24; // [sp+21h] [bp-7h]@9
	char v2; // [sp+26h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v13 = data[27] >> 2;
					data[27] <<= 6;
					data[27] |= v13;
					data[16] ^= 0x45u;
					v17 = (data[23] >> 1) & 1;
					if ((data[23] >> 5) & 1)
						data[23] |= 2u;
					else
						data[23] &= 0xFDu;
					if (v17)
						data[23] |= 0x20u;
					else
						data[23] &= 0xDFu;
					v14 = data[17] >> 2;
					data[17] <<= 6;
					data[17] |= v14;
					v16 = (data[28] >> 4) & 1;
					if ((data[28] >> 5) & 1)
						data[28] |= 0x10u;
					else
						data[28] &= 0xEFu;
					if (v16)
						data[28] |= 0x20u;
					else
						data[28] &= 0xDFu;
					v15 = (data[2] >> 1) & 1;
					if ((data[2] >> 2) & 1)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					if (v15)
						data[2] |= 4u;
					else
						data[2] &= 0xFBu;
				}
				else
				{
					v10 = data[7] >> 2;
					data[7] <<= 6;
					data[7] |= v10;
					data[2] ^= 0x24u;
					v11 = data[10] >> 6;
					data[10] *= 4;
					data[10] |= v11;
					v19 = (data[2] >> 7) & 1;
					if ((data[2] >> 7) & 1)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					if (v19)
						data[2] |= 0x80u;
					else
						data[2] &= 0x7Fu;
					v18 = (data[5] >> 5) & 1;
					if ((data[5] >> 3) & 1)
						data[5] |= 0x20u;
					else
						data[5] &= 0xDFu;
					if (v18)
						data[5] |= 8u;
					else
						data[5] &= 0xF7u;
					v12 = data[11] >> 4;
					data[11] *= 16;
					data[11] |= v12;
				}
			}
			else
			{
				v6 = data[2];
				data[2] = data[3];
				data[3] = v6;
				v21 = (data[4] >> 7) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v21)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v7 = data[7] >> 2;
				data[7] <<= 6;
				data[7] |= v7;
				v8 = data[3];
				data[3] = data[1];
				data[1] = v8;
				v9 = data[4] >> 6;
				data[4] *= 4;
				data[4] |= v9;
				data[6] ^= 0xBEu;
				v20 = (data[5] >> 5) & 1;
				if ((data[5] >> 1) & 1)
					data[5] |= 0x20u;
				else
					data[5] &= 0xDFu;
				if (v20)
					data[5] |= 2u;
				else
					data[5] &= 0xFDu;
				data[4] ^= 0x17u;
			}
		}
		else
		{
			data[2] ^= 0x57u;
			v2 = (data[3] >> 4) & 1;
			if ((data[3] >> 7) & 1)
				data[3] |= 0x10u;
			else
				data[3] &= 0xEFu;
			if (v2)
				data[3] |= 0x80u;
			else
				data[3] &= 0x7Fu;
			v3 = data[2];
			data[2] = data[1];
			data[1] = v3;
			v4 = *data;
			*data = *data;
			*data = v4;
			data[3] ^= 0x80u;
			v5 = *data >> 2;
			*data <<= 6;
			*data |= v5;
			v24 = (*data >> 5) & 1;
			if ((*data >> 7) & 1)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			if (v24)
				*data |= 0x80u;
			else
				*data &= 0x7Fu;
			v23 = (data[1] >> 1) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v23)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			data[3] ^= 0xC0u;
			v22 = (*data >> 4) & 1;
			if ((*data >> 1) & 1)
				*data |= 0x10u;
			else
				*data &= 0xEFu;
			if (v22)
				*data |= 2u;
			else
				*data &= 0xFDu;
			data[3] ^= 0xC8u;
		}
	}
}

void PacketTwister_79::Twist(BYTE *data, int length)
{
	char v3; // ST2B_1@3
	char v4; // ST2A_1@3
	char v5; // ST29_1@3
	char v6; // ST28_1@3
	char v7; // ST27_1@3
	char v8; // ST26_1@3
	char v9; // ST25_1@3
	char v10; // ST24_1@3
	char v11; // ST23_1@3
	char v12; // ST22_1@3
	char v13; // ST1F_1@11
	char v14; // ST1A_1@23
	char v15; // ST19_1@23
	char v16; // ST18_1@23
	char v17; // ST0F_1@49
	char v18; // ST0E_1@49
	char v19; // ST0D_1@49
	char v20; // ST0A_1@55
	char v21; // ST07_1@62
	char v22; // ST06_1@62
	char v23; // ST05_1@62
	char v24; // ST04_1@62
	char v25; // [sp+8h] [bp-24h]@56
	char v26; // [sp+Bh] [bp-21h]@49
	char v27; // [sp+10h] [bp-1Ch]@43
	char v28; // [sp+12h] [bp-1Ah]@37
	char v29; // [sp+14h] [bp-18h]@31
	char v30; // [sp+16h] [bp-16h]@25
	char v31; // [sp+1Bh] [bp-11h]@17
	char v32; // [sp+1Dh] [bp-Fh]@11
	char v2; // [sp+20h] [bp-Ch]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v25 = (data[11] >> 4) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v25)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
					data[13] ^= 0xF3u;
					data[18] ^= 0xB5u;
					v21 = data[9];
					data[9] = data[27];
					data[27] = v21;
					v22 = data[30] >> 3;
					data[30] *= 32;
					data[30] |= v22;
					v23 = data[25];
					data[25] = data[4];
					data[4] = v23;
					v24 = data[4] >> 7;
					data[4] *= 2;
					data[4] |= v24;
				}
				else
				{
					v30 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v30)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
					v29 = (data[9] >> 6) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v29)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					data[8] ^= 0x7Fu;
					v28 = (data[7] >> 7) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 0x80u;
					else
						data[7] &= 0x7Fu;
					if (v28)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					v27 = (data[15] >> 2) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v27)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					v17 = data[3];
					data[3] = data[13];
					data[13] = v17;
					data[1] ^= 0x89u;
					v18 = data[4] >> 6;
					data[4] *= 4;
					data[4] |= v18;
					v19 = data[14];
					data[14] = data[8];
					data[8] = v19;
					v26 = (data[2] >> 1) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					if (v26)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					data[12] ^= 0x76u;
					v20 = *data >> 6;
					*data *= 4;
					*data |= v20;
				}
			}
			else
			{
				data[5] ^= 0x78u;
				v13 = data[5];
				data[5] = data[4];
				data[4] = v13;
				v32 = (*data >> 4) & 1;
				if ((*data >> 5) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v32)
					*data |= 0x20u;
				else
					*data &= 0xDFu;
				*data ^= 0xB8u;
				v31 = (data[4] >> 7) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v31)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				v14 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v14;
				v15 = data[4];
				data[4] = data[6];
				data[6] = v15;
				data[4] ^= 0x4Fu;
				*data ^= 0xDu;
				v16 = data[7];
				data[7] = data[2];
				data[2] = v16;
			}
		}
		else
		{
			v3 = *data >> 6;
			*data *= 4;
			*data |= v3;
			v4 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v4;
			v5 = data[3];
			data[3] = data[3];
			data[3] = v5;
			data[1] ^= 0xA7u;
			v6 = data[1];
			data[1] = *data;
			*data = v6;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
			v8 = data[3];
			data[3] = *data;
			*data = v8;
			v9 = data[2] >> 1;
			data[2] <<= 7;
			data[2] |= v9;
			v10 = *data;
			*data = data[1];
			data[1] = v10;
			v11 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v11;
			data[2] ^= 0xCFu;
			v12 = *data;
			*data = data[2];
			data[2] = v12;
			v2 = (data[3] >> 6) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
		}
	}
}

void PacketTwister_79::Correct(BYTE *data, int length)
{
	char v3; // ST29_1@9
	char v4; // ST28_1@9
	char v5; // ST27_1@9
	char v6; // ST26_1@9
	char v7; // ST25_1@9
	char v8; // ST24_1@9
	char v9; // ST23_1@9
	char v10; // ST22_1@9
	char v11; // ST21_1@9
	char v12; // ST20_1@9
	char v13; // ST1F_1@11
	char v14; // ST1E_1@11
	char v15; // ST1D_1@11
	char v16; // ST18_1@23
	char v17; // ST17_1@25
	char v18; // ST14_1@31
	char v19; // ST13_1@31
	char v20; // ST12_1@31
	char v21; // ST09_1@56
	char v22; // ST08_1@56
	char v23; // ST07_1@56
	char v24; // ST06_1@56
	char v25; // [sp+4h] [bp-28h]@56
	char v26; // [sp+Ah] [bp-22h]@49
	char v27; // [sp+Ch] [bp-20h]@43
	char v28; // [sp+Eh] [bp-1Eh]@37
	char v29; // [sp+10h] [bp-1Ch]@31
	char v30; // [sp+15h] [bp-17h]@25
	char v31; // [sp+19h] [bp-13h]@17
	char v32; // [sp+1Bh] [bp-11h]@11
	char v2; // [sp+2Ah] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v21 = data[4] >> 1;
					data[4] <<= 7;
					data[4] |= v21;
					v22 = data[25];
					data[25] = data[4];
					data[4] = v22;
					v23 = data[30] >> 5;
					data[30] *= 8;
					data[30] |= v23;
					v24 = data[9];
					data[9] = data[27];
					data[27] = v24;
					data[18] ^= 0xB5u;
					data[13] ^= 0xF3u;
					v25 = (data[11] >> 4) & 1;
					if ((data[11] >> 1) & 1)
						data[11] |= 0x10u;
					else
						data[11] &= 0xEFu;
					if (v25)
						data[11] |= 2u;
					else
						data[11] &= 0xFDu;
				}
				else
				{
					v17 = *data >> 2;
					*data <<= 6;
					*data |= v17;
					data[12] ^= 0x76u;
					v30 = (data[2] >> 1) & 1;
					if ((data[2] >> 3) & 1)
						data[2] |= 2u;
					else
						data[2] &= 0xFDu;
					if (v30)
						data[2] |= 8u;
					else
						data[2] &= 0xF7u;
					v18 = data[14];
					data[14] = data[8];
					data[8] = v18;
					v19 = data[4] >> 2;
					data[4] <<= 6;
					data[4] |= v19;
					data[1] ^= 0x89u;
					v20 = data[3];
					data[3] = data[13];
					data[13] = v20;
					v29 = (data[15] >> 2) & 1;
					if ((data[15] >> 3) & 1)
						data[15] |= 4u;
					else
						data[15] &= 0xFBu;
					if (v29)
						data[15] |= 8u;
					else
						data[15] &= 0xF7u;
					v28 = (data[7] >> 7) & 1;
					if ((data[7] >> 3) & 1)
						data[7] |= 0x80u;
					else
						data[7] &= 0x7Fu;
					if (v28)
						data[7] |= 8u;
					else
						data[7] &= 0xF7u;
					data[8] ^= 0x7Fu;
					v27 = (data[9] >> 6) & 1;
					if ((data[9] >> 5) & 1)
						data[9] |= 0x40u;
					else
						data[9] &= 0xBFu;
					if (v27)
						data[9] |= 0x20u;
					else
						data[9] &= 0xDFu;
					v26 = (data[12] >> 2) & 1;
					if ((data[12] >> 3) & 1)
						data[12] |= 4u;
					else
						data[12] &= 0xFBu;
					if (v26)
						data[12] |= 8u;
					else
						data[12] &= 0xF7u;
				}
			}
			else
			{
				v13 = data[7];
				data[7] = data[2];
				data[2] = v13;
				*data ^= 0xDu;
				data[4] ^= 0x4Fu;
				v14 = data[4];
				data[4] = data[6];
				data[6] = v14;
				v15 = data[7] >> 4;
				data[7] *= 16;
				data[7] |= v15;
				v32 = (data[4] >> 7) & 1;
				if ((data[4] >> 1) & 1)
					data[4] |= 0x80u;
				else
					data[4] &= 0x7Fu;
				if (v32)
					data[4] |= 2u;
				else
					data[4] &= 0xFDu;
				*data ^= 0xB8u;
				v31 = (*data >> 4) & 1;
				if ((*data >> 5) & 1)
					*data |= 0x10u;
				else
					*data &= 0xEFu;
				if (v31)
					*data |= 0x20u;
				else
					*data &= 0xDFu;
				v16 = data[5];
				data[5] = data[4];
				data[4] = v16;
				data[5] ^= 0x78u;
			}
		}
		else
		{
			v2 = (data[3] >> 6) & 1;
			if ((data[3] >> 6) & 1)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			if (v2)
				data[3] |= 0x40u;
			else
				data[3] &= 0xBFu;
			v3 = *data;
			*data = data[2];
			data[2] = v3;
			data[2] ^= 0xCFu;
			v4 = data[2] >> 4;
			data[2] *= 16;
			data[2] |= v4;
			v5 = *data;
			*data = data[1];
			data[1] = v5;
			v6 = data[2] >> 7;
			data[2] *= 2;
			data[2] |= v6;
			v7 = data[3];
			data[3] = *data;
			*data = v7;
			v8 = data[3];
			data[3] = *data;
			*data = v8;
			v9 = data[1];
			data[1] = *data;
			*data = v9;
			data[1] ^= 0xA7u;
			v10 = data[3];
			data[3] = data[3];
			data[3] = v10;
			v11 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v11;
			v12 = *data >> 2;
			*data <<= 6;
			*data |= v12;
		}
	}
}

void PacketTwister_80::Twist(BYTE *data, int length)
{
	char v3; // ST15_1@9
	char v4; // ST14_1@9
	char v5; // ST13_1@11
	char v6; // ST12_1@11
	char v7; // ST11_1@11
	char v8; // ST10_1@13
	char v9; // ST0F_1@13
	char v10; // ST0E_1@13
	char v11; // ST0B_1@20
	char v12; // ST08_1@26
	char v13; // ST07_1@26
	char v14; // ST06_1@26
	char v15; // [sp+4h] [bp-14h]@26
	char v16; // [sp+9h] [bp-Fh]@20
	char v17; // [sp+Ch] [bp-Ch]@14
	char v2; // [sp+16h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[21] ^= 0x1Au;
					v17 = (data[13] >> 1) & 1;
					if ((data[13] >> 7) & 1)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					if (v17)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					v11 = data[4];
					data[4] = data[13];
					data[13] = v11;
					v16 = (data[27] >> 1) & 1;
					if ((data[27] >> 4) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v16)
						data[27] |= 0x10u;
					else
						data[27] &= 0xEFu;
					v12 = data[28];
					data[28] = data[6];
					data[6] = v12;
					v13 = data[27] >> 1;
					data[27] <<= 7;
					data[27] |= v13;
					data[13] ^= 0x77u;
					v14 = data[29] >> 2;
					data[29] <<= 6;
					data[29] |= v14;
					v15 = (data[26] >> 2) & 1;
					if ((data[26] >> 4) & 1)
						data[26] |= 4u;
					else
						data[26] &= 0xFBu;
					if (v15)
						data[26] |= 0x10u;
					else
						data[26] &= 0xEFu;
				}
				else
				{
					v8 = data[1] >> 6;
					data[1] *= 4;
					data[1] |= v8;
					data[13] ^= 0x5Du;
					v9 = *data;
					*data = data[10];
					data[10] = v9;
					v10 = data[6] >> 7;
					data[6] *= 2;
					data[6] |= v10;
				}
			}
			else
			{
				v5 = data[3];
				data[3] = data[1];
				data[1] = v5;
				v6 = data[3] >> 3;
				data[3] *= 32;
				data[3] |= v6;
				v7 = data[6];
				data[6] = data[7];
				data[7] = v7;
			}
		}
		else
		{
			v2 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
			v3 = data[2] >> 6;
			data[2] *= 4;
			data[2] |= v3;
			data[1] ^= 0x9Fu;
			v4 = data[1] >> 1;
			data[1] <<= 7;
			data[1] |= v4;
		}
	}
}

void PacketTwister_80::Correct(BYTE *data, int length)
{
	char v3; // ST17_1@3
	char v4; // ST16_1@3
	char v5; // ST13_1@11
	char v6; // ST12_1@11
	char v7; // ST11_1@11
	char v8; // ST10_1@13
	char v9; // ST0F_1@13
	char v10; // ST0E_1@13
	char v11; // ST0B_1@20
	char v12; // ST0A_1@20
	char v13; // ST09_1@20
	char v14; // ST06_1@26
	char v15; // [sp+4h] [bp-14h]@26
	char v16; // [sp+7h] [bp-11h]@20
	char v17; // [sp+Ch] [bp-Ch]@14
	char v2; // [sp+14h] [bp-4h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v17 = (data[26] >> 2) & 1;
					if ((data[26] >> 4) & 1)
						data[26] |= 4u;
					else
						data[26] &= 0xFBu;
					if (v17)
						data[26] |= 0x10u;
					else
						data[26] &= 0xEFu;
					v11 = data[29] >> 6;
					data[29] *= 4;
					data[29] |= v11;
					data[13] ^= 0x77u;
					v12 = data[27] >> 7;
					data[27] *= 2;
					data[27] |= v12;
					v13 = data[28];
					data[28] = data[6];
					data[6] = v13;
					v16 = (data[27] >> 1) & 1;
					if ((data[27] >> 4) & 1)
						data[27] |= 2u;
					else
						data[27] &= 0xFDu;
					if (v16)
						data[27] |= 0x10u;
					else
						data[27] &= 0xEFu;
					v14 = data[4];
					data[4] = data[13];
					data[13] = v14;
					v15 = (data[13] >> 1) & 1;
					if ((data[13] >> 7) & 1)
						data[13] |= 2u;
					else
						data[13] &= 0xFDu;
					if (v15)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					data[21] ^= 0x1Au;
				}
				else
				{
					v8 = data[6] >> 1;
					data[6] <<= 7;
					data[6] |= v8;
					v9 = *data;
					*data = data[10];
					data[10] = v9;
					data[13] ^= 0x5Du;
					v10 = data[1] >> 2;
					data[1] <<= 6;
					data[1] |= v10;
				}
			}
			else
			{
				v5 = data[6];
				data[6] = data[7];
				data[7] = v5;
				v6 = data[3] >> 5;
				data[3] *= 8;
				data[3] |= v6;
				v7 = data[3];
				data[3] = data[1];
				data[1] = v7;
			}
		}
		else
		{
			v3 = data[1] >> 7;
			data[1] *= 2;
			data[1] |= v3;
			data[1] ^= 0x9Fu;
			v4 = data[2] >> 2;
			data[2] <<= 6;
			data[2] |= v4;
			v2 = (data[2] >> 4) & 1;
			if ((data[2] >> 1) & 1)
				data[2] |= 0x10u;
			else
				data[2] &= 0xEFu;
			if (v2)
				data[2] |= 2u;
			else
				data[2] &= 0xFDu;
		}
	}
}

void PacketTwister_81::Twist(BYTE *data, int length)
{
	char v3; // ST23_1@3
	char v4; // ST22_1@3
	char v5; // ST21_1@3
	char v6; // ST20_1@3
	char v7; // ST1F_1@3
	char v8; // ST1A_1@15
	char v9; // ST15_1@29
	char v10; // ST12_1@35
	char v11; // ST11_1@35
	char v12; // ST0E_1@43
	char v13; // ST07_1@62
	char v14; // ST06_1@62
	char v15; // [sp+4h] [bp-20h]@62
	char v16; // [sp+8h] [bp-1Ch]@56
	char v17; // [sp+Ah] [bp-1Ah]@49
	char v18; // [sp+Ch] [bp-18h]@43
	char v19; // [sp+Fh] [bp-15h]@37
	char v20; // [sp+13h] [bp-11h]@29
	char v21; // [sp+16h] [bp-Eh]@23
	char v22; // [sp+18h] [bp-Ch]@15
	char v23; // [sp+1Bh] [bp-9h]@9
	char v2; // [sp+1Dh] [bp-7h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					v16 = (data[14] >> 6) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v16)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					v13 = data[9] >> 1;
					data[9] <<= 7;
					data[9] |= v13;
					data[4] ^= 0xCEu;
					v14 = data[22] >> 2;
					data[22] <<= 6;
					data[22] |= v14;
					v15 = (data[6] >> 3) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					if (v15)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					data[28] ^= 0x6Du;
				}
				else
				{
					v19 = (data[13] >> 5) & 1;
					if ((data[13] >> 7) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v19)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
					v12 = data[14] >> 3;
					data[14] *= 32;
					data[14] |= v12;
					v18 = (data[3] >> 5) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					if (v18)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					v17 = (data[14] >> 2) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v17)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					data[7] ^= 0x79u;
					data[4] ^= 0x3Au;
				}
			}
			else
			{
				v21 = (data[2] >> 4) & 1;
				if ((data[2] >> 4) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v21)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				v9 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v9;
				v20 = (data[7] >> 3) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				if (v20)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v10 = data[5];
				data[5] = data[2];
				data[2] = v10;
				v11 = *data;
				*data = data[4];
				data[4] = v11;
			}
		}
		else
		{
			v3 = data[3];
			data[3] = data[1];
			data[1] = v3;
			v4 = data[2];
			data[2] = *data;
			*data = v4;
			v5 = *data >> 5;
			*data *= 8;
			*data |= v5;
			v6 = data[1] >> 6;
			data[1] *= 4;
			data[1] |= v6;
			*data ^= 0x7Cu;
			v7 = *data;
			*data = data[1];
			data[1] = v7;
			v2 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v2)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v23 = (*data >> 3) & 1;
			if ((*data >> 5) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v23)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			v8 = data[3] >> 3;
			data[3] *= 32;
			data[3] |= v8;
			v22 = (data[1] >> 1) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v22)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
		}
	}
}

void PacketTwister_81::Correct(BYTE *data, int length)
{
	char v3; // ST21_1@9
	char v4; // ST1C_1@21
	char v5; // ST1B_1@21
	char v6; // ST1A_1@21
	char v7; // ST19_1@21
	char v8; // ST18_1@21
	char v9; // ST17_1@23
	char v10; // ST16_1@23
	char v11; // ST13_1@29
	char v12; // ST0C_1@49
	char v13; // ST07_1@62
	char v14; // ST06_1@62
	char v15; // [sp+4h] [bp-20h]@62
	char v16; // [sp+8h] [bp-1Ch]@56
	char v17; // [sp+Ah] [bp-1Ah]@49
	char v18; // [sp+Dh] [bp-17h]@43
	char v19; // [sp+Fh] [bp-15h]@37
	char v20; // [sp+11h] [bp-13h]@29
	char v21; // [sp+14h] [bp-10h]@23
	char v22; // [sp+1Dh] [bp-7h]@15
	char v23; // [sp+1Fh] [bp-5h]@9
	char v2; // [sp+22h] [bp-2h]@3

	if (length >= 4)
	{
		if (length >= 8)
		{
			if (length >= 16)
			{
				if (length >= 32)
				{
					data[28] ^= 0x6Du;
					v16 = (data[6] >> 3) & 1;
					if ((data[6] >> 7) & 1)
						data[6] |= 8u;
					else
						data[6] &= 0xF7u;
					if (v16)
						data[6] |= 0x80u;
					else
						data[6] &= 0x7Fu;
					v13 = data[22] >> 6;
					data[22] *= 4;
					data[22] |= v13;
					data[4] ^= 0xCEu;
					v14 = data[9] >> 7;
					data[9] *= 2;
					data[9] |= v14;
					v15 = (data[14] >> 6) & 1;
					if ((data[14] >> 2) & 1)
						data[14] |= 0x40u;
					else
						data[14] &= 0xBFu;
					if (v15)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
				}
				else
				{
					data[4] ^= 0x3Au;
					data[7] ^= 0x79u;
					v19 = (data[14] >> 2) & 1;
					if ((data[14] >> 7) & 1)
						data[14] |= 4u;
					else
						data[14] &= 0xFBu;
					if (v19)
						data[14] |= 0x80u;
					else
						data[14] &= 0x7Fu;
					v18 = (data[3] >> 5) & 1;
					if ((data[3] >> 5) & 1)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					if (v18)
						data[3] |= 0x20u;
					else
						data[3] &= 0xDFu;
					v12 = data[14] >> 5;
					data[14] *= 8;
					data[14] |= v12;
					v17 = (data[13] >> 5) & 1;
					if ((data[13] >> 7) & 1)
						data[13] |= 0x20u;
					else
						data[13] &= 0xDFu;
					if (v17)
						data[13] |= 0x80u;
					else
						data[13] &= 0x7Fu;
				}
			}
			else
			{
				v9 = *data;
				*data = data[4];
				data[4] = v9;
				v10 = data[5];
				data[5] = data[2];
				data[2] = v10;
				v21 = (data[7] >> 3) & 1;
				if ((data[7] >> 7) & 1)
					data[7] |= 8u;
				else
					data[7] &= 0xF7u;
				if (v21)
					data[7] |= 0x80u;
				else
					data[7] &= 0x7Fu;
				v11 = data[6] >> 4;
				data[6] *= 16;
				data[6] |= v11;
				v20 = (data[2] >> 4) & 1;
				if ((data[2] >> 4) & 1)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
				if (v20)
					data[2] |= 0x10u;
				else
					data[2] &= 0xEFu;
			}
		}
		else
		{
			v2 = (data[1] >> 1) & 1;
			if ((data[1] >> 4) & 1)
				data[1] |= 2u;
			else
				data[1] &= 0xFDu;
			if (v2)
				data[1] |= 0x10u;
			else
				data[1] &= 0xEFu;
			v3 = data[3] >> 5;
			data[3] *= 8;
			data[3] |= v3;
			v23 = (*data >> 3) & 1;
			if ((*data >> 5) & 1)
				*data |= 8u;
			else
				*data &= 0xF7u;
			if (v23)
				*data |= 0x20u;
			else
				*data &= 0xDFu;
			v22 = (data[1] >> 2) & 1;
			if ((data[1] >> 6) & 1)
				data[1] |= 4u;
			else
				data[1] &= 0xFBu;
			if (v22)
				data[1] |= 0x40u;
			else
				data[1] &= 0xBFu;
			v4 = *data;
			*data = data[1];
			data[1] = v4;
			*data ^= 0x7Cu;
			v5 = data[1] >> 2;
			data[1] <<= 6;
			data[1] |= v5;
			v6 = *data >> 3;
			*data *= 32;
			*data |= v6;
			v7 = data[2];
			data[2] = *data;
			*data = v7;
			v8 = data[3];
			data[3] = data[1];
			data[1] = v8;
		}
	}
}

PacketTwistRunner::PacketTwistRunner()
{

}

PacketTwistRunner::~PacketTwistRunner()
{

}

bool PacketTwistRunner::Install()
{
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x0E, new PacketTwister_1));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xF1, new PacketTwister_2));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xF3, new PacketTwister_3));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x00, new PacketTwister_4));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x01, new PacketTwister_5));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x02, new PacketTwister_6));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x03, new PacketTwister_7));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x18, new PacketTwister_8));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x19, new PacketTwister_9));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x22, new PacketTwister_10));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x23, new PacketTwister_11));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x24, new PacketTwister_12));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x26, new PacketTwister_13));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x30, new PacketTwister_14));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x31, new PacketTwister_15));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x32, new PacketTwister_16));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x33, new PacketTwister_17));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x34, new PacketTwister_18));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x36, new PacketTwister_19));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x37, new PacketTwister_20));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x3A, new PacketTwister_21));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x3C, new PacketTwister_22));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x3D, new PacketTwister_23));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x3F, new PacketTwister_24));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x40, new PacketTwister_25));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x41, new PacketTwister_26));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x42, new PacketTwister_27));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x43, new PacketTwister_28));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x4A, new PacketTwister_29));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x4B, new PacketTwister_30));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x50, new PacketTwister_31));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x51, new PacketTwister_32));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x52, new PacketTwister_33));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x53, new PacketTwister_34));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x54, new PacketTwister_35));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x55, new PacketTwister_36));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x57, new PacketTwister_37));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x61, new PacketTwister_38));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x66, new PacketTwister_39));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xB1, new PacketTwister_40));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xB2, new PacketTwister_41));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xB3, new PacketTwister_42));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xB7, new PacketTwister_43));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xBD, new PacketTwister_44));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xE2, new PacketTwister_45));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xE5, new PacketTwister_46));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x71, new PacketTwister_47));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x72, new PacketTwister_48));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x73, new PacketTwister_49));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x81, new PacketTwister_50));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x82, new PacketTwister_51));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x83, new PacketTwister_52));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x86, new PacketTwister_53));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x87, new PacketTwister_54));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x90, new PacketTwister_55));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x91, new PacketTwister_56));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x95, new PacketTwister_57));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x96, new PacketTwister_58));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x97, new PacketTwister_59));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xA0, new PacketTwister_60));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xA2, new PacketTwister_61));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xA7, new PacketTwister_62));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xA9, new PacketTwister_63));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x9A, new PacketTwister_64));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x9F, new PacketTwister_65));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0x9D, new PacketTwister_66));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC0, new PacketTwister_67));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC1, new PacketTwister_68));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC2, new PacketTwister_69));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC3, new PacketTwister_70));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC4, new PacketTwister_71));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC5, new PacketTwister_72));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC7, new PacketTwister_73));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC8, new PacketTwister_74));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xC9, new PacketTwister_75));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xCA, new PacketTwister_76));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xCB, new PacketTwister_77));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xF7, new PacketTwister_78));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xF6, new PacketTwister_79));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xD2, new PacketTwister_80));
	this->m_twisters.insert(std::pair<int, PacketTwister *>(0xFA, new PacketTwister_81));

	return true;
}

void PacketTwistRunner::Twist(BYTE headCode, BYTE *data, int length)
{
	std::map<int, PacketTwister *>::iterator twister = this->m_twisters.find(headCode);
	
	if (twister != this->m_twisters.end())
	{
		twister->second->Twist(data, length);
	}
}

void PacketTwistRunner::Correct(BYTE headCode, BYTE *data, int length)
{
	std::map<int, PacketTwister *>::iterator twister = this->m_twisters.find(headCode);

	if (twister != this->m_twisters.end())
	{
		twister->second->Correct(data, length);
	}
}

void PacketTwistRunner::Finish()
{
	for (std::map<int, PacketTwister *>::iterator It = this->m_twisters.begin(); It != this->m_twisters.end(); It++)
	{
		delete It->second;
		this->m_twisters.erase(It);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

