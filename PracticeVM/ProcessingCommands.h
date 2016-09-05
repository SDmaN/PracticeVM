#ifndef PROCESSINGCOMMANDS_H
#define PROCESSINGCOMMANDS_H

#include "Commands.h"

// ����� ������� ��� ��������� ������ (��������������, ���������, ��������� � �.�.)
class ProcessingCommand : public Command
{
public:
	explicit ProcessingCommand(Processor *processor); // �����������, ����������� ���������
	virtual bool execute(const MemCmd48 &dc) override; // ��������� �������

protected:
	virtual bool registerRegister(const MemCmd48 &dc) = 0; // �������-�������
	virtual bool registerAddress(const MemCmd48 &dc) = 0; // �������-������
	virtual bool addressRegister(const MemCmd48 &dc) = 0; // ������-�������
	virtual bool addressAddress(const MemCmd48 &dc) = 0; // ������-������

	uWord readUWord(Address memoryAddress, Word *memory) const; // ������ ����������� ����� �� ������
	void writeUWord(uWord uw, Address memoryAddress, Word *memory) const; // ���������� ����������� ����� � ������

	Long readDword(Address memoryAddress, Word *Memory) const; // ������ ������� ����� �� ������
	void writeDword(Long dw, Address memoryAddress, Word *memory) const; // ���������� ������� ����� � ������

	Float readFloat(Address memoryAddress, Word *memory) const; // ������ ������� �� ������
	void writeFloat(Float f, Address memoryAddress, Word *memory) const; // ���������� ������� � ������

	bool checkBit(uWord number, uWord bit) const; // ���������, ���������� �� ��� � �������� bit (������� � ��������)
};

// ����� ��������� �����
class MovCommand : public ProcessingCommand
{
public:
	explicit MovCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ����� ������� = ������ �������
	virtual bool registerAddress(const MemCmd48 &dc) override; // ������� = �����
	virtual bool addressRegister(const MemCmd48 &dc) override; // ����� = �������
	virtual bool addressAddress(const MemCmd48 &dc) override; // ����� ����� = ������ �����
};

// ����� ���������� ������ � ��������
class ItFCommand : public ProcessingCommand
{
public:
	explicit ItFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ����� ������� <- ������ �������
	virtual bool registerAddress(const MemCmd48 &dc) override; // ������� <- ������
	virtual bool addressRegister(const MemCmd48 &dc) override; // ������ <- �������
	virtual bool addressAddress(const MemCmd48 &dc) override; // ������ <- ������
};

// ����� ���������� ������������ ������ � �������
class UItFCommand : public ProcessingCommand
{
public:
	explicit UItFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ����� ������� <- ������ �������
	virtual bool registerAddress(const MemCmd48 &dc) override; // ������� <- ������
	virtual bool addressRegister(const MemCmd48 &dc) override; // ������ <- �������
	virtual bool addressAddress(const MemCmd48 &dc) override; // ������ <- ������
};

// ����� ���������� �������� � ������
class FtICommand : public ProcessingCommand
{
public:
	explicit FtICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;
};

// ����� ���������� �������� � ������������ ������
class FtUICommand : public ProcessingCommand
{
public:
	explicit FtUICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;
};

// ����� ��������� ����� �����
class CmpICommand : public ProcessingCommand
{
public:
	explicit CmpICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;
};

// ����� ��������� ����������� ����� �����
// ���� s = 1, �� ��������� ������������ �� �������� ������� ������
class CmpUICommand : public ProcessingCommand
{
public:
	explicit CmpUICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;
};

// ����� ��������� ������� �����
class CmpFCommand : public ProcessingCommand
{
public:
	explicit CmpFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;
};

// ����� ����� ����� ����� � ����������
class InICommand : public ProcessingCommand
{
public:
	explicit InICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ���� � ������ �������
	virtual bool registerAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
	virtual bool addressRegister(const MemCmd48 &dc) override; // ���� � �����
	virtual bool addressAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
};

// ����� ������ ����� ����� �� �����
class OutICommand : public ProcessingCommand
{
public:
	explicit OutICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ����� ������� ��������
	virtual bool registerAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
	virtual bool addressRegister(const MemCmd48 &dc) override; // ����� �������� �� ������
	virtual bool addressAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
};

// ����� ����� ������� ����� � ����������
class InFCommand : public ProcessingCommand
{
public:
	explicit InFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ���� � ������ �������
	virtual bool registerAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
	virtual bool addressRegister(const MemCmd48 &dc) override; // ���� � �����
	virtual bool addressAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
};

// ����� ������ ������� ����� �� �����
class OutFCommand : public ProcessingCommand
{
public:
	explicit OutFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // ����� ������� ��������
	virtual bool registerAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
	virtual bool addressRegister(const MemCmd48 &dc) override; // ����� �������� �� ������
	virtual bool addressAddress(const MemCmd48 &dc) override; // �� ������������. ���������� true
};

#endif // PROCESSINGCOMMANDS_H

