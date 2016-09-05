#ifndef PROCESSINGCOMMANDS_H
#define PROCESSINGCOMMANDS_H

#include "Commands.h"

// Класс команды для обработки данных (арифметические, пересылка, сравнения и т.д.)
class ProcessingCommand : public Command
{
public:
	explicit ProcessingCommand(Processor *processor); // Конструктор, принимающий процессор
	virtual bool execute(const MemCmd48 &dc) override; // Исполняет команду

protected:
	virtual bool registerRegister(const MemCmd48 &dc) = 0; // Регистр-регистр
	virtual bool registerAddress(const MemCmd48 &dc) = 0; // Регистр-память
	virtual bool addressRegister(const MemCmd48 &dc) = 0; // Память-регистр
	virtual bool addressAddress(const MemCmd48 &dc) = 0; // Память-память

	uWord readUWord(Address memoryAddress, Word *memory) const; // Читает беззнаковое слово из памяти
	void writeUWord(uWord uw, Address memoryAddress, Word *memory) const; // Записывает беззнаковое слово в память

	Long readDword(Address memoryAddress, Word *Memory) const; // Читает двойное слово из памяти
	void writeDword(Long dw, Address memoryAddress, Word *memory) const; // Записывает двойное число в память

	Float readFloat(Address memoryAddress, Word *memory) const; // Читает дробное из памяти
	void writeFloat(Float f, Address memoryAddress, Word *memory) const; // Записывает дробное в память

	bool checkBit(uWord number, uWord bit) const; // Проверяет, установлен ли бит с индексом bit (начиная с младшего)
};

// Класс пересылки чисел
class MovCommand : public ProcessingCommand
{
public:
	explicit MovCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Левый регистр = правый регистр
	virtual bool registerAddress(const MemCmd48 &dc) override; // Регистр = адрес
	virtual bool addressRegister(const MemCmd48 &dc) override; // Адрес = регистр
	virtual bool addressAddress(const MemCmd48 &dc) override; // Левый адрес = правый адрес
};

// Класс приведения целого к дробному
class ItFCommand : public ProcessingCommand
{
public:
	explicit ItFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Левый регистр <- правый регистр
	virtual bool registerAddress(const MemCmd48 &dc) override; // Регистр <- память
	virtual bool addressRegister(const MemCmd48 &dc) override; // Память <- регистр
	virtual bool addressAddress(const MemCmd48 &dc) override; // Память <- память
};

// Класс приведения беззнакового целого к добному
class UItFCommand : public ProcessingCommand
{
public:
	explicit UItFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Левый регистр <- правый регистр
	virtual bool registerAddress(const MemCmd48 &dc) override; // Регистр <- память
	virtual bool addressRegister(const MemCmd48 &dc) override; // Память <- регистр
	virtual bool addressAddress(const MemCmd48 &dc) override; // Память <- память
};

// Класс приведения дробного к целому
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

// Класс приведения дробного к беззнаковому целому
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

// Класс сравнения целых чисел
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

// Класс сравнения беззнаковых целых чисел
// Если s = 1, то сравнение производится на знаковых двойных словах
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

// Класс сравнения дробных чисел
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

// Класс ввода целых чисел с клавиатуры
class InICommand : public ProcessingCommand
{
public:
	explicit InICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Ввод в первый регистр
	virtual bool registerAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
	virtual bool addressRegister(const MemCmd48 &dc) override; // Ввод в адрес
	virtual bool addressAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
};

// Класс вывода целых чисел на экран
class OutICommand : public ProcessingCommand
{
public:
	explicit OutICommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Вывод первого регистра
	virtual bool registerAddress(const MemCmd48 &dc) override; // Не использутеся. Возвращает true
	virtual bool addressRegister(const MemCmd48 &dc) override; // Вывод значения по адресу
	virtual bool addressAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
};

// Класс ввода дробных чисел с клавиатуры
class InFCommand : public ProcessingCommand
{
public:
	explicit InFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Ввод в первый регистр
	virtual bool registerAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
	virtual bool addressRegister(const MemCmd48 &dc) override; // Ввод в адрес
	virtual bool addressAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
};

// Класс вывода дробных чисел на экран
class OutFCommand : public ProcessingCommand
{
public:
	explicit OutFCommand(Processor *processor);

protected:
	virtual bool registerRegister(const MemCmd48 &dc) override; // Вывод первого регистра
	virtual bool registerAddress(const MemCmd48 &dc) override; // Не использутеся. Возвращает true
	virtual bool addressRegister(const MemCmd48 &dc) override; // Вывод значения по адресу
	virtual bool addressAddress(const MemCmd48 &dc) override; // Не используется. Возвращает true
};

#endif // PROCESSINGCOMMANDS_H

