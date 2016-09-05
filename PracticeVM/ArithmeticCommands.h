#ifndef ARITHMETICCOMMANDS_H
#define ARITHMETICCOMMANDS_H

#include "ProcessingCommands.h"

// ����� �������������� ������ ��� ������ ������������ �������
class IArithmeticCommand : public ProcessingCommand
{
public:
	explicit IArithmeticCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;

	virtual void operation(Word &w1, Word &w2) = 0;
	virtual void operation(Long &dw1, Long &dw2) = 0;
};

// ����� �������������� ������ ��� ������ ������������ �������
class UIArithmeticCommand : public ProcessingCommand
{
public:
	explicit UIArithmeticCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;

	virtual void operation(uWord &uw1, uWord &uw2) = 0;
	virtual void operation(Long &dw1, Long &dw2) = 0;
};

// ����� �������������� ������ ��� �������� �������
class FArithmeticCommand : public ProcessingCommand
{
public:
	explicit FArithmeticCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override;
	virtual bool registerAddress(const MemCmd48 &dc) override;
	virtual bool addressRegister(const MemCmd48 &dc) override;
	virtual bool addressAddress(const MemCmd48 &dc) override;

	virtual void operation(Float &f1, Float f2) = 0;
};

// ����� �������������� ������� �������� ����� �����
class AddICommand : public IArithmeticCommand
{
public:
	explicit AddICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // ���������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // ���������� ������� �����
};

// ����� �������������� ������� ��������� ����� �����
class SubICommand : public IArithmeticCommand
{
public:
	explicit SubICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // �������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // �������� ������� �����
};

// ����� �������������� ������� ��������� ����� �����
class MulICommand : public IArithmeticCommand
{
public:
	explicit MulICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // �������� �������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // �������� ������� �����
};

// ����� �������������� ������� ��������� ����������� ����� �����
// ���� s = 1, �� ��������� ������������ �� �������� ������� ������
class MulUICommand : public UIArithmeticCommand
{
public:
	explicit MulUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // �������� ����������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // �������� ������� �����
};

// ����� �������������� ������� ��������� ����� �����
class DivICommand : public IArithmeticCommand
{
public:
	explicit DivICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // ����� �������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // ����� ������� �����
};

// ����� �������������� ������� ������� ����������� ����� �����
// ���� s = 1, �� ������� ������������ �� �������� ������� ������
class DivUICommand : public UIArithmeticCommand
{
public:
	explicit DivUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // ����� ����������� �����
	virtual void operation(Long &dw1, Long &dw2) override; // ����� ������� �����
};

// ����� �������������� ������� ���������� ������� �� ������� ����� �����
class ModICommand : public IArithmeticCommand
{
public:
	explicit ModICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // ������� ������� �� ������� �������� ����
	virtual void operation(Long &dw1, Long &dw2) override; // ������� ������� �� ������� ������� ����
};

// ����� �������������� ������� ���������� ������� �� ������� ����������� ����� �����
// ���� s = 1, �� ������� �� ������� ����������� �� �������� ������� ������
class ModUICommand : public UIArithmeticCommand
{
public:
	explicit ModUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // ������� ������� �� ������� ����������� ����
	virtual void operation(Long &dw1, Long &dw2) override; // ������� ������� �� ������� ������� ����
};

// ����� �������������� ������� �������� ������� �����
class AddFCommand : public FArithmeticCommand
{
public:
	explicit AddFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // ���������� �������
};

// ����� �������������� ������� ��������� ������� �����
class SubFCommand : public FArithmeticCommand
{
public:
	explicit SubFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // �������� �������
};

// ����� �������������� ������� ��������� ������� �����
class MulFCommand : public FArithmeticCommand
{
public:
	explicit MulFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // �������� �������
};

// ����� �������������� ������� ������� ������� �����
class DivFCommand : public FArithmeticCommand
{
public:
	explicit DivFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // ����� �������
};

#endif // ARITHMETICCOMMANDS_H

