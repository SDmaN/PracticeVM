#include "Commands.h"
#include "Processor.h"

// ========= Абстрактный Command =========

Command::Command(Processor *processor)
	: m_processor(processor)
{
}

Command::~Command()
{
}

bool Command::operator()(const MemCmd48 &dc)
{
	return execute(dc);
}


// ========= Команда NOP =========

NopCommand::NopCommand(Processor *processor)
	: Command(processor)
{
}

bool NopCommand::execute(const MemCmd48 &dc)
{
	return false;
}

// ========= Команда остановки =========

StopCommand::StopCommand(Processor *processor)
	: Command(processor)
{
}

bool StopCommand::execute(const MemCmd48 &dc)
{
	return true;
}
