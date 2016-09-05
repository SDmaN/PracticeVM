#include "JumpCommands.h"
#include "Processor.h"

JmpCommand::JmpCommand(Processor *processor)
	: Command(processor)
{
}

bool JmpCommand::execute(const MemCmd48 &dc)
{
	Address jmpAddress = 0;
	m_processor->m_psw.JF = 1;

	switch(dc.dd)
	{
		case 0:
			return true;

		case 1:
			jmpAddress = dc.o1;
			break;

		case 2:
			jmpAddress = m_processor->gpr().uw[dc.r2];
			break;

		case 3:
			jmpAddress = m_processor->gpr().uw[dc.r2] + dc.o1;
			break;
	}

	if(dc.s == 0)
		m_processor->m_psw.IP = jmpAddress;
	else
		m_processor->m_psw.IP += jmpAddress;

	return false;
}

Address JmpCommand::readAddress(Address address, Word *memory) const
{
	Data addressData;
	addressData.w = memory[address];

	return addressData.a;
}

// =========== Переход "Равно" ===========

JeCommand::JeCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JeCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF == 0)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход "Не равно" ===========

JneCommand::JneCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JneCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF != 0)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход "меньше" ===========

JlCommand::JlCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JlCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF == -1)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход "Больше" ===========

JgCommand::JgCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JgCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF == +1)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход "Меньше или равно" ===========

JleCommand::JleCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JleCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF == -1 || m_processor->m_psw.CMPF == 0)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход "Больше или равно" ===========

JgeCommand::JgeCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool JgeCommand::execute(const MemCmd48 &dc)
{
	if(m_processor->m_psw.CMPF == +1 || m_processor->m_psw.CMPF == 0)
		return JmpCommand::execute(dc);

	return false;
}

// =========== Переход к подпрограмме ===========

CallCommand::CallCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool CallCommand::execute(const MemCmd48 &dc)
{
	m_processor->gpr().uw[dc.r1] = m_processor->m_psw.IP + dc.calculateSize();
	m_processor->m_retRegister = dc.r1;

	return JmpCommand::execute(dc);
}

// =========== Возврат к вызывающей подпрограмме ===========

RetCommand::RetCommand(Processor *processor)
	: JmpCommand(processor)
{
}

bool RetCommand::execute(const MemCmd48 &dc)
{
	MemCmd48 jmpCmd;
	jmpCmd.code = CommandCodes::Jmp; // Делаем прыжок
	jmpCmd.s = 0; // Прямой
	jmpCmd.dd = 1; // По адресу

	jmpCmd.o1 = m_processor->gpr().uw[m_processor->m_retRegister]; // Лежащему в данном регистре

	return JmpCommand::execute(jmpCmd);
}
