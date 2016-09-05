#include <memory>

#include "Processor.h"
#include "Commands.h"
#include "ProcessingCommands.h"
#include "ArithmeticCommands.h"
#include "JumpCommands.h"

Processor::Processor(Word *memory, size_t memorySize)
	: m_memory(memory), m_memorySize(memorySize)
{
	initKnownCommands();
}

Processor::~Processor()
{
	clearKnownCommands();
}

Processor::GeneralPurposeRegisters & Processor::gpr()
{
	return m_gpr;
}

Word * Processor::memory() const
{
	return m_memory;
}

size_t Processor::memorySize() const
{
	return m_memorySize;
}

void Processor::setMemory(Word *memory, size_t memorySize)
{
	m_memory = memory;
	m_memorySize = memorySize;
}

void Processor::interpret()
{
	reset();

	bool stoping = false;

	while(!stoping)
	{
		m_psw.JF = 0;

		uWord commandSize = readCommand(); // Считываем очередную команду

		if(m_PC.code <= Ret)
			stoping = m_knownCommands[m_PC.code]->execute(m_PC); // Выполняем ее
		else
			stoping = true;

		if(m_psw.JF == 0) // Прыжок не был совершен
			m_psw.IP += commandSize;
	}
}

void Processor::reset()
{
	clearSystemRegisters();
	clearGPRegisters();
}

void Processor::clearSystemRegisters()
{
	m_psw.CMPF = 0;
	m_psw.JF = 0;

	clearPC();
}

void Processor::clearPC()
{
	m_PC.code = 0;
	m_PC.s = 0;
	m_PC.dd = 0;
	m_PC.o1 = 0;
	m_PC.o2 = 0;
	m_PC.r1 = 0;
	m_PC.r2 = 0;
}

void Processor::clearGPRegisters()
{
	for(auto &dw : m_gpr.dw)
		dw = 0;
}

void Processor::initKnownCommands()
{
	const size_t knownCommandsSize = 35;
	m_knownCommands.resize(knownCommandsSize);

	m_knownCommands[CommandCodes::Nop] = new NopCommand(this);
	m_knownCommands[CommandCodes::Stop] = new StopCommand(this);

	m_knownCommands[CommandCodes::AddI] = new AddICommand(this);
	m_knownCommands[CommandCodes::SubI] = new SubICommand(this);
	m_knownCommands[CommandCodes::MulI] = new MulICommand(this);
	m_knownCommands[CommandCodes::MulUI] = new MulUICommand(this);
	m_knownCommands[CommandCodes::DivI] = new DivICommand(this);
	m_knownCommands[CommandCodes::DivUI] = new DivUICommand(this);
	m_knownCommands[CommandCodes::ModI] = new ModICommand(this);
	m_knownCommands[CommandCodes::ModUI] = new ModUICommand(this);

	m_knownCommands[CommandCodes::AddF] = new AddFCommand(this);
	m_knownCommands[CommandCodes::SubF] = new SubFCommand(this);
	m_knownCommands[CommandCodes::MulF] = new MulFCommand(this);
	m_knownCommands[CommandCodes::DivF] = new DivFCommand(this);

	m_knownCommands[CommandCodes::Mov] = new MovCommand(this);

	m_knownCommands[CommandCodes::ItF] = new ItFCommand(this);
	m_knownCommands[CommandCodes::UItF] = new UItFCommand(this);
	m_knownCommands[CommandCodes::FtI] = new FtICommand(this);
	m_knownCommands[CommandCodes::FtUI] = new FtUICommand(this);

	m_knownCommands[CommandCodes::CmpI] = new CmpICommand(this);
	m_knownCommands[CommandCodes::CmpUI] = new CmpUICommand(this);
	m_knownCommands[CommandCodes::CmpF] = new CmpFCommand(this);

	m_knownCommands[CommandCodes::InI] = new InICommand(this);
	m_knownCommands[CommandCodes::OutI] = new OutICommand(this);

	m_knownCommands[CommandCodes::InF] = new InFCommand(this);
	m_knownCommands[CommandCodes::OutF] = new OutFCommand(this);

	m_knownCommands[CommandCodes::Jmp] = new JmpCommand(this);
	m_knownCommands[CommandCodes::Je] = new JeCommand(this);
	m_knownCommands[CommandCodes::Jne] = new JneCommand(this);
	m_knownCommands[CommandCodes::Jl] = new JlCommand(this);
	m_knownCommands[CommandCodes::Jg] = new JgCommand(this);
	m_knownCommands[CommandCodes::Jle] = new JleCommand(this);
	m_knownCommands[CommandCodes::Jge] = new JgeCommand(this);
	m_knownCommands[CommandCodes::Call] = new CallCommand(this);
	m_knownCommands[CommandCodes::Ret] = new RetCommand(this);
}

void Processor::clearKnownCommands()
{
	for(size_t i = 0; i < m_knownCommands.size(); i++)
		delete m_knownCommands[i];

	m_knownCommands.clear();
}

uWord Processor::readCommand()
{
	Data readBuffer;
	uWord rc = 0; // Количество прочитанных слов

	readBuffer.words[rc++] = m_memory[m_psw.IP]; // Минимальный размер команды равен 1 слово, так что читаем 1.

	uWord commandSize = readBuffer.cmd.calculateSize(); // Вычисляем размер в зависимости от dd

	for(rc; rc < commandSize; ++rc) // Считываем то, что нужно дочитать (добираем размер)
		readBuffer.words[rc] = m_memory[m_psw.IP + rc];

	m_PC = readBuffer.cmd;

	return commandSize;
}
