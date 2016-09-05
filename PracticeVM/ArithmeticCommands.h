#ifndef ARITHMETICCOMMANDS_H
#define ARITHMETICCOMMANDS_H

#include "ProcessingCommands.h"

// Класс арифметических команд над целыми беззнаковыми числами
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

// Класс арифметических команд над целыми беззнаковыми числами
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

// Класс арифметических команд над дробными числами
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

// Класс арифметической команды сложения целых чисел
class AddICommand : public IArithmeticCommand
{
public:
	explicit AddICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // Складывает слова
	virtual void operation(Long &dw1, Long &dw2) override; // Складывает двойные слова
};

// Класс арифметической команды вычитания целых чисел
class SubICommand : public IArithmeticCommand
{
public:
	explicit SubICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // Вычитает слова
	virtual void operation(Long &dw1, Long &dw2) override; // Вычитает двойные слова
};

// Класс арифметической команды умножения целых чисел
class MulICommand : public IArithmeticCommand
{
public:
	explicit MulICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // Умножает знаковые слова
	virtual void operation(Long &dw1, Long &dw2) override; // Умножает двойные слова
};

// Класс арифметической команды умножения беззнаковых целых чисел
// Если s = 1, то умножение производится на знаковых двойных словах
class MulUICommand : public UIArithmeticCommand
{
public:
	explicit MulUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // Умножает беззнаковые слова
	virtual void operation(Long &dw1, Long &dw2) override; // Умножает двойные слова
};

// Класс арифметической команды умножения целых чисел
class DivICommand : public IArithmeticCommand
{
public:
	explicit DivICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // Делит знаковые слова
	virtual void operation(Long &dw1, Long &dw2) override; // Делит двойные слова
};

// Класс арифметической моманды деления беззнаковых целых чисел
// Если s = 1, то деление производится на знаковых двойных словах
class DivUICommand : public UIArithmeticCommand
{
public:
	explicit DivUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // Делит беззнаковые слова
	virtual void operation(Long &dw1, Long &dw2) override; // Делит двойные слова
};

// Класс арифметической команды вычисления остатка от деления целых чисел
class ModICommand : public IArithmeticCommand
{
public:
	explicit ModICommand(Processor *processor);

protected:
	virtual void operation(Word &w1, Word &w2) override; // Находит остаток от деления знаковых слов
	virtual void operation(Long &dw1, Long &dw2) override; // Находит остаток от деления двойных слов
};

// Класс арифметической команды вычисления остатка от деления беззнаковых целых чисел
// Если s = 1, то остаток от деления вычисляется на знаковых двойных словах
class ModUICommand : public UIArithmeticCommand
{
public:
	explicit ModUICommand(Processor *processor);

protected:
	virtual void operation(uWord &uw1, uWord &uw2) override; // Находит остаток от деления беззнаковых слов
	virtual void operation(Long &dw1, Long &dw2) override; // Находит остаток от деления двойных слов
};

// Класс арифметической команды сложения дробных чисел
class AddFCommand : public FArithmeticCommand
{
public:
	explicit AddFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // Складывает дробные
};

// Класс арифметической команды вычитания дробных чисел
class SubFCommand : public FArithmeticCommand
{
public:
	explicit SubFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // Вычитает дробные
};

// Класс арифметической команды умножения дробных чисел
class MulFCommand : public FArithmeticCommand
{
public:
	explicit MulFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // Умножает дробные
};

// Класс арифметической команды деления дробных чисел
class DivFCommand : public FArithmeticCommand
{
public:
	explicit DivFCommand(Processor *processor);

protected:
	virtual void operation(Float &f1, Float f2) override; // Делит дробные
};

#endif // ARITHMETICCOMMANDS_H

