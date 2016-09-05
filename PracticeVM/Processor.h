#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "DefinedTypes.h"
#include "Data.h"

class Command;

class Processor
{
	friend class VirtualMachine;

	friend class CmpICommand;
	friend class CmpUICommand;
	friend class CmpFCommand;

	friend class JmpCommand;
	friend class JeCommand;
	friend class JneCommand;
	friend class JlCommand;
	friend class JgCommand;
	friend class JleCommand;
	friend class JgeCommand;
	friend class CallCommand;
	friend class RetCommand;

public:
	// ����������� ��������� ������ ���������� (���).
	union GeneralPurposeRegisters
	{
		Word w[8]; // ����� �� ������, 16 ���.
		uWord uw[8]; // ����� ��� �����, 16 ���.
		Long dw[4]; // ����� �� ������, 32 ���.
		Float f[4]; // � ��������� ������, 32 ���.
	};

	explicit Processor(Word *memory, size_t memorySize);
	~Processor();

	GeneralPurposeRegisters & gpr(); // ���������� ������ �� �������� ������ ����������

	Word * memory() const;
	size_t memorySize() const;
	void setMemory(Word *memory, size_t memorySize);

	void interpret();

private:
	// ��� �������� � ������� ��������� ����������
	struct PSW
	{
		Address IP; // ����� ������� � ������

		Word CMPF : 2; // ���� ���������. (0 - �����, -1 - ������ ������, +1 - ������ ������) 
		uWord JF : 1; // ���� ��������

		/// TODO: ����� ��� ��������� ������
		uWord : 12; /// ���������������
	};

	GeneralPurposeRegisters m_gpr; // ������ ���
	PSW m_psw; // ����� ���������
	MemCmd48 m_PC; // �������, ���������� �������, ��������� �� ������

	uWord m_retRegister; // ������ ��������, ����������� ����� ��������

	Word *m_memory; // ��������� �� ������ ������ (��-���� ����)
	size_t m_memorySize; // ������ ������ � ������

	std::vector<Command *> m_knownCommands; // ������, ���������� � ���� ��������� �� �������, ��������� ����������

	void reset(); // �������� ��� ��������
	void clearSystemRegisters(); // �������� ��������� ��������
	void clearPC(); // �������� ������� �������
	void clearGPRegisters(); // �������� �������� ������ ����������

	void initKnownCommands(); // �������������� m_knownCommands
	void clearKnownCommands(); // ������ ������ m_knownCommands

	uWord readCommand(); // ��������� ��������� ������� �� ������ � m_PC � ���������� �� ������
};

#endif // PROCESSOR_H

