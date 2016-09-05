#include "ArithmeticCommands.h"
#include "Processor.h"

// =========== Арифметические над целыми ===========

IArithmeticCommand::IArithmeticCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool IArithmeticCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
		operation(m_processor->gpr().w[dc.r1], m_processor->gpr().w[dc.r2]);
	else
		operation(m_processor->gpr().dw[dc.r1], m_processor->gpr().dw[dc.r2]);

	return false;
}

bool IArithmeticCommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
		operation(m_processor->gpr().w[dc.r1], m_processor->memory()[dc.o1]);
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());
		operation(m_processor->gpr().dw[dc.r1], addressDw);
	}

	return false;
}

bool IArithmeticCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
		operation(m_processor->memory()[dc.o1], m_processor->gpr().w[dc.r1]);
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());
		operation(addressDw, m_processor->gpr().dw[dc.r1]);

		writeDword(addressDw, dc.o1, m_processor->memory());
	}

	return false;
}

bool IArithmeticCommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
		operation(m_processor->memory()[dc.o1], m_processor->memory()[dc.o2]);
	else
	{
		Long addresDw1 = readDword(dc.o1, m_processor->memory());
		Long addresDw2 = readDword(dc.o2, m_processor->memory());

		operation(addresDw1, addresDw2);
		writeDword(addresDw1, dc.o1, m_processor->memory());
	}

	return false;
}

// =========== Арифметические над беззнаковыми целыми ===========

UIArithmeticCommand::UIArithmeticCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool UIArithmeticCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
		operation(m_processor->gpr().uw[dc.r1], m_processor->gpr().uw[dc.r2]);
	else
		operation(m_processor->gpr().dw[dc.r1], m_processor->gpr().dw[dc.r2]);

	return false;
}

bool UIArithmeticCommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUWord = readUWord(dc.o1, m_processor->memory());
		operation(m_processor->gpr().uw[dc.r1], addressUWord);
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());
		operation(m_processor->gpr().dw[dc.r1], addressDw);
	}

	return false;
}

bool UIArithmeticCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUWord = readUWord(dc.o1, m_processor->memory());
		operation(addressUWord, m_processor->gpr().uw[dc.r1]);

		writeUWord(addressUWord, dc.o1, m_processor->memory());
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());
		operation(addressDw, m_processor->gpr().dw[dc.r1]);

		writeDword(addressDw, dc.o1, m_processor->memory());
	}

	return false;
}

bool UIArithmeticCommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUWord1 = readUWord(dc.o1, m_processor->memory());
		uWord addressUWord2 = readUWord(dc.o2, m_processor->memory());

		operation(addressUWord1, addressUWord2);
		writeUWord(addressUWord1, dc.o1, m_processor->memory());
	}
	else
	{
		Long addresDw1 = readDword(dc.o1, m_processor->memory());
		Long addresDw2 = readDword(dc.o2, m_processor->memory());

		operation(addresDw1, addresDw2);
		writeDword(addresDw1, dc.o1, m_processor->memory());
	}

	return false;
}

// =========== Арифметические над дробными ===========

FArithmeticCommand::FArithmeticCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool FArithmeticCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		operation(m_processor->gpr().f[dc.r1], m_processor->gpr().f[dc.r2]);
		return false;
	}

	/// TODO: сделать установку бита ошибки
	return true;
}

bool FArithmeticCommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat = readFloat(dc.o1, m_processor->memory());
		operation(m_processor->gpr().f[dc.r1], addressFloat);

		return false;
	}

	/// TODO: сделать установку бита ошибки
	return true;
}

bool FArithmeticCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat = readFloat(dc.o1, m_processor->memory());
		operation(addressFloat, m_processor->gpr().f[dc.r1]);

		writeFloat(addressFloat, dc.o1, m_processor->memory());

		return false;
	}

	/// TODO: сделать установку бита ошибки
	return true;
}

bool FArithmeticCommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat1 = readFloat(dc.o1, m_processor->memory());
		Float addressFloat2 = readFloat(dc.o2, m_processor->memory());

		operation(addressFloat1, addressFloat2);
		writeFloat(addressFloat1, dc.o1, m_processor->memory());

		return false;
	}

	/// TODO: сделать установку бита ошибки
	return true;
}

// =========== Сложение целых ===========

AddICommand::AddICommand(Processor *processor)
	: IArithmeticCommand(processor)
{
}

void AddICommand::operation(Word &w1, Word &w2)
{
	w1 += w2;
}

void AddICommand::operation(Long &dw1, Long &dw2)
{
	dw1 += dw2;
}

// =========== Вычитание целых ===========

SubICommand::SubICommand(Processor *processor)
	: IArithmeticCommand(processor)
{
}

void SubICommand::operation(Word &w1, Word &w2)
{
	w1 -= w2;
}

void SubICommand::operation(Long &dw1, Long &dw2)
{
	dw1 -= dw2;
}

// =========== Умножение целых ===========

MulICommand::MulICommand(Processor *processor)
	: IArithmeticCommand(processor)
{
}

void MulICommand::operation(Word &w1, Word &w2)
{
	w1 *= w2;
}

void MulICommand::operation(Long &dw1, Long &dw2)
{
	dw1 *= dw2;
}

// =========== Умножение беззнаковых целых ===========

MulUICommand::MulUICommand(Processor *processor)
	: UIArithmeticCommand(processor)
{
}

void MulUICommand::operation(uWord &uw1, uWord &uw2)
{
	uw1 *= uw2;
}

void MulUICommand::operation(Long &dw1, Long &dw2)
{
	dw1 *= dw2;
}

// =========== Деление целых ===========

DivICommand::DivICommand(Processor *processor)
	: IArithmeticCommand(processor)
{
}

void DivICommand::operation(Word &w1, Word &w2)
{
	w1 /= w2;
}

void DivICommand::operation(Long &dw1, Long &dw2)
{
	dw1 /= dw2;
}

// =========== Деление целых ===========

DivUICommand::DivUICommand(Processor *processor)
	: UIArithmeticCommand(processor)
{
}

void DivUICommand::operation(uWord &uw1, uWord &uw2)
{
	uw1 /= uw2;
}

void DivUICommand::operation(Long &dw1, Long &dw2)
{
	dw1 /= dw2;
}

// =========== Остаток от деления целых ===========

ModICommand::ModICommand(Processor *processor)
	: IArithmeticCommand(processor)
{
}

void ModICommand::operation(Word &w1, Word &w2)
{
	w1 %= w2;
}

void ModICommand::operation(Long &dw1, Long &dw2)
{
	dw1 %= dw2;
}

// =========== Остаток от деления беззнаковых целых ===========

ModUICommand::ModUICommand(Processor *processor)
	: UIArithmeticCommand(processor)
{
}

void ModUICommand::operation(uWord &uw1, uWord &uw2)
{
	uw1 %= uw2;
}

void ModUICommand::operation(Long &dw1, Long &dw2)
{
	dw1 %= dw2;
}

// =========== Сложение дробных ===========

AddFCommand::AddFCommand(Processor *processor)
	: FArithmeticCommand(processor)
{
}

void AddFCommand::operation(Float &f1, Float f2)
{
	f1 += f2;
}

// =========== Вычитание дробных ===========

SubFCommand::SubFCommand(Processor *processor)
	: FArithmeticCommand(processor)
{
}

void SubFCommand::operation(Float &f1, Float f2)
{
	f1 -= f2;
}

// =========== Умножение дробных ===========

MulFCommand::MulFCommand(Processor *processor)
	: FArithmeticCommand(processor)
{
}

void MulFCommand::operation(Float &f1, Float f2)
{
	f1 *= f2;
}

// =========== Деление дробных ===========

DivFCommand::DivFCommand(Processor *processor)
	: FArithmeticCommand(processor)
{
}

void DivFCommand::operation(Float &f1, Float f2)
{
	f1 /= f2;
}
