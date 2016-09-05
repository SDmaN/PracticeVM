#ifndef JUMPCOMMANDS_H
#define JUMPCOMMANDS_H

#include "Commands.h"

// Класс команды перехода
class JmpCommand : public Command
{
public:
	explicit JmpCommand(Processor *processor); // Конструктор
	virtual bool execute(const MemCmd48 &dc) override; // Исполняет команду

private:
	Address readAddress(Address address, Word *memory) const; // Читает адрес (unsigned short) по заданному адресу в памяти
};

// Класс команды условного перехода "Равно"
class JeCommand : public JmpCommand
{
public:
	explicit JeCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды условного перехода "Не равно"
class JneCommand : public JmpCommand
{
public:
	explicit JneCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды условного перехода "Меньше"
class JlCommand : public JmpCommand
{
public:
	explicit JlCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды условного перехода "Больше"
class JgCommand : public JmpCommand
{
public:
	explicit JgCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды условного перехода "Меньше или равно"
class JleCommand : public JmpCommand
{
public:
	explicit JleCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды условного перехода "Больше или равно"
class JgeCommand : public JmpCommand
{
public:
	explicit JgeCommand(Processor *processor);
	virtual bool execute(const MemCmd48 &dc) override;
};

// Класс команды перехода к подпрограмме
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

