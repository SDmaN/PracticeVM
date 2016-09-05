#ifndef DATACOMMANDS_H
#define DATACOMMANDS_H

#include "DefinedTypes.h"
#include "CommandCodes.h"

#pragma pack(push, 1)

// ��������� ������� (���� 6 ����)
struct MemCmd48
{
	uWord code : 7; // ��� �������� �������
	uWord s : 1; // ����, ������ ��������
	uWord dd : 2; // ������ ��������

	uWord r1 : 3; // ������� 1 
	uWord r2 : 3; // ������� 2

	uWord o1 : 16; // ������ 1
	uWord o2 : 16; // ������ 2

	// ��������� ������ ������� � ������.
	uWord calculateSize() const
	{
		uWord size = 1; // ������ � ������

		if(code != CommandCodes::Stop && code != CommandCodes::Nop && code != CommandCodes::Ret) // �� ������� �� ���������, � ������ ������ ����������
		{
			if(code < CommandCodes::Jmp) // � ������ ��������� ������� ������������ �� �������
			{
				if(dd == 1 || dd == 2) // 01 ��� 10
					++size;
				else if(dd == 3) // 11
					size += 2;
			}
			else
			{
				// ������� �������� �� ������ 2 ����
				if(dd == 0)
					size = 0;
				else if(dd == 1 || dd == 3)
					++size;
			}
		}

		return size;
	}
};

// ������������� ������
// ��������� �� ���� �������� ������� �����, ������������ �����, �������
union Data
{
	Word words[3]; // ��� ������ �� ������

	Word w; // ����������� �����
	uWord uw; // ����������� ����������� �����
	Long dw; // ����������� ������� �����
	Float f; // ����������� � ��������� ������
	Address a; // ����������� �����

	MemCmd48 cmd; // ������� � ������
};

#pragma pack(pop)

#endif // DATACOMMANDS_H

