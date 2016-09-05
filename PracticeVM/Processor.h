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
	// Объединение регистров общего назначения (РОН).
	union GeneralPurposeRegisters
	{
		Word w[8]; // Целые со знаком, 16 бит.
		uWord uw[8]; // Целые без знака, 16 бит.
		Long dw[4]; // Целые со знаком, 32 бит.
		Float f[4]; // С плавающей точкой, 32 бит.
	};

	explicit Processor(Word *memory, size_t memorySize);
	~Processor();

	GeneralPurposeRegisters & gpr(); // Возвращает ссылку на регистры общего назначения

	Word * memory() const;
	size_t memorySize() const;
	void setMemory(Word *memory, size_t memorySize);

	void interpret();

private:
	// Тип регистра с флагами состояния процессора
	struct PSW
	{
		Address IP; // Адрес команды в памяти

		Word CMPF : 2; // Флаг сравнения. (0 - равны, -1 - первый больше, +1 - второй больше) 
		uWord JF : 1; // Флаг перехода

		/// TODO: нужны еще несколько флагов
		uWord : 12; /// Зарезервировано
	};

	GeneralPurposeRegisters m_gpr; // Объект РОН
	PSW m_psw; // Флаги состояния
	MemCmd48 m_PC; // Регистр, содержащий команду, считанную из памяти

	uWord m_retRegister; // Индекс регистра, содержащего адрес возврата

	Word *m_memory; // Указатель на память машины (по-сути шина)
	size_t m_memorySize; // Размер памяти в словах

	std::vector<Command *> m_knownCommands; // Массив, содержащий в себе указатели на команды, известные процессору

	void reset(); // Обнуляет все регистры
	void clearSystemRegisters(); // Обнуляет системные регистры
	void clearPC(); // Обнуляет регистр команды
	void clearGPRegisters(); // Обнуляет регистры общего назначения

	void initKnownCommands(); // Инициализирует m_knownCommands
	void clearKnownCommands(); // Чистит память m_knownCommands

	uWord readCommand(); // Считывает следующую команду из памяти в m_PC и возвращает ее размер
};

#endif // PROCESSOR_H

