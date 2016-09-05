#ifndef COMMANDS_H
#define COMMANDS_H

#include "Data.h"

class Processor; // Для указателя

// Класс команды процессора. Исполняет команду
class Command
{
public:
	explicit Command(Processor *processor); // Конструктор, принимающий в себя указатель на процессор, который выполняет команду.
	virtual ~Command(); // Деструктор

	bool operator ()(const MemCmd48 &dc); // Позволяет обращаться к объекту команды как к функтору. Вызывает execute()
	virtual bool execute(const MemCmd48 &dc) = 0; // Выполняет команду. Если возвращает true, то процессор останавливает работу.

protected:
	Processor *m_processor; // Процессор, в котором выполняется команда
};

// Класс команды процессора, которая ничего не делает (NOP)
class NopCommand : public Command
{
public:
	explicit NopCommand(Processor *processor); // Конструктор
	virtual bool execute(const MemCmd48 &dc) override; // Выполняет команду. Всегда возвращает false.
};

// Класс команды процессора, которая останавливает работу процессора (STOP).
class StopCommand : public Command
{
public:
	explicit StopCommand(Processor *processor); // Конструктор
	virtual bool execute(const MemCmd48 &dc) override; // Выполняет команду. Всегда возвращает true.
};

#endif // COMMANDS_H

