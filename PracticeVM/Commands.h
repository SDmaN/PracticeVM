#ifndef COMMANDS_H
#define COMMANDS_H

#include "Data.h"

class Processor; // ��� ���������

// ����� ������� ����������. ��������� �������
class Command
{
public:
	explicit Command(Processor *processor); // �����������, ����������� � ���� ��������� �� ���������, ������� ��������� �������.
	virtual ~Command(); // ����������

	bool operator ()(const MemCmd48 &dc); // ��������� ���������� � ������� ������� ��� � ��������. �������� execute()
	virtual bool execute(const MemCmd48 &dc) = 0; // ��������� �������. ���� ���������� true, �� ��������� ������������� ������.

protected:
	Processor *m_processor; // ���������, � ������� ����������� �������
};

// ����� ������� ����������, ������� ������ �� ������ (NOP)
class NopCommand : public Command
{
public:
	explicit NopCommand(Processor *processor); // �����������
	virtual bool execute(const MemCmd48 &dc) override; // ��������� �������. ������ ���������� false.
};

// ����� ������� ����������, ������� ������������� ������ ���������� (STOP).
class StopCommand : public Command
{
public:
	explicit StopCommand(Processor *processor); // �����������
	virtual bool execute(const MemCmd48 &dc) override; // ��������� �������. ������ ���������� true.
};

#endif // COMMANDS_H

