#ifndef JUMPCOMMANDS_H
#define JUMPCOMMANDS_H

#include "Commands.h"

// ����� ������� ��������
class JmpCommand : public Command
{
public:
	explicit JmpCommand(Processor *processor); // �����������
	virtual bool execute(const MemCmd48 &dc) override; // ��������� �������

private:
	Address readAddress(Address address, Word *memory) const; // ������ ����� (unsigned short) �� ��������� ������ � ������
};

// ����� ������� ��������� �������� "�����"
class JeCommand : public JmpCommand
{
public:
	explicit JeCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� ��������� �������� "�� �����"
class JneCommand : public JmpCommand
{
public:
	explicit JneCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� ��������� �������� "������"
class JlCommand : public JmpCommand
{
public:
	explicit JlCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� ��������� �������� "������"
class JgCommand : public JmpCommand
{
public:
	explicit JgCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� ��������� �������� "������ ��� �����"
class JleCommand : public JmpCommand
{
public:
	explicit JleCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� ��������� �������� "������ ��� �����"
class JgeCommand : public JmpCommand
{
public:
	explicit JgeCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// ����� ������� �������� � ������������
class CallCommand : public JmpCommand
{
public:
	explicit CallCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

class RetCommand : public JmpCommand
{
public:
	explicit RetCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

#endif // JUMPCOMMANDS_H

