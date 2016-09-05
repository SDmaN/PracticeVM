#include <iostream>

#include "ProcessingCommands.h"
#include "Processor.h"

ProcessingCommand::ProcessingCommand(Processor *processor)
	: Command(processor)
{
}

bool ProcessingCommand::execute(const MemCmd48 &dc)
{
	bool stop = false;

	switch(dc.dd)
	{
		case 0:
			stop = registerRegister(dc);
			break;

		case 1:
			stop = registerAddress(dc);
			break;

		case 2:
			stop = addressRegister(dc);
			break;

		case 3:
			stop = addressAddress(dc);
			break;
	}

	return stop;
}

uWord ProcessingCommand::readUWord(Address memoryAddress, Word *memory) const
{
	Data uWordData;
	uWordData.w = memory[memoryAddress];

	return uWordData.uw;
}

void ProcessingCommand::writeUWord(uWord uw, Address memoryAddress, Word *memory) const
{
	Data uWordData;
	uWordData.uw = uw;

	memory[memoryAddress] = uWordData.w;
}

Long ProcessingCommand::readDword(Address memoryAddress, Word *memory) const
{
	Data dwordData;
	dwordData.words[0] = memory[memoryAddress];
	dwordData.words[1] = memory[memoryAddress + 1];

	return dwordData.dw;
}

void ProcessingCommand::writeDword(Long dw, Address memoryAddress, Word *memory) const
{
	Data dwordData;
	dwordData.dw = dw;

	memory[memoryAddress] = dwordData.words[0];
	memory[memoryAddress + 1] = dwordData.words[1];
}

Float ProcessingCommand::readFloat(Address memoryAddress, Word *memory) const
{
	Data dwordData;
	dwordData.words[0] = memory[memoryAddress];
	dwordData.words[1] = memory[memoryAddress + 1];

	return dwordData.f;
}

void ProcessingCommand::writeFloat(Float f, Address memoryAddress, Word *memory) const
{
	Data dwordData;
	dwordData.f = f;

	memory[memoryAddress] = dwordData.words[0];
	memory[memoryAddress + 1] = dwordData.words[1];
}

bool ProcessingCommand::checkBit(uWord number, uWord bit) const
{
	return (number & 1 << bit) != 0;
}

// =========== ��������� ����� ===========

MovCommand::MovCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool MovCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
		m_processor->gpr().w[dc.r1] = m_processor->gpr().w[dc.r2];
	else
		m_processor->gpr().dw[dc.r1] = m_processor->gpr().dw[dc.r2];

	return false;
}

bool MovCommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
		m_processor->gpr().w[dc.r1] = m_processor->memory()[dc.o1];
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());
		m_processor->gpr().dw[dc.r1] = addressDw;
	}

	return false;
}

bool MovCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
		m_processor->memory()[dc.o1] = m_processor->gpr().w[dc.r1];
	else
	{
		Long addressDw = m_processor->gpr().dw[dc.r1];

		writeDword(addressDw, dc.o1, m_processor->memory());
	}

	return false;
}

bool MovCommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
		m_processor->memory()[dc.o1] = m_processor->memory()[dc.o2];
	else
	{
		Long addresDw2 = readDword(dc.o2, m_processor->memory());
		writeDword(addresDw2, dc.o1, m_processor->memory());
	}

	return false;
}

// =========== ���������� ����� � ������� ===========

ItFCommand::ItFCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool ItFCommand::registerRegister(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0) // ����� � ��������
	{
		// ������� r2 ������ ���� ������.
		// ���� ��������, �� ����� ������� �� ������������ �������

		if(dc.r2 % 2 != 0) // �������� �� ��������
			return true;

		castedFloat = m_processor->gpr().w[dc.r2]; // ���������� ����� � ��������
	}
	else
		castedFloat = static_cast<Float>(m_processor->gpr().dw[dc.r2]); // ���������� �������� ����� � ��������

	m_processor->gpr().f[dc.r1] = castedFloat;

	return false;
}

bool ItFCommand::registerAddress(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0)
		castedFloat = m_processor->memory()[dc.o1]; // ���������� ����� � ��������
	else
	{
		Long dw = readDword(dc.o1, m_processor->memory());
		castedFloat = static_cast<Float>(dw); // ���������� �������� ����� � ��������
	}

	m_processor->gpr().f[dc.r1] = castedFloat;

	return false;
}

bool ItFCommand::addressRegister(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0) // ����� � ��������
		castedFloat = m_processor->gpr().w[dc.r1];
	else
		castedFloat = static_cast<Float>(m_processor->gpr().dw[dc.r1]);

	writeFloat(castedFloat, dc.o1, m_processor->memory());

	return false;
}

bool ItFCommand::addressAddress(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0)
		castedFloat = m_processor->memory()[dc.o2];
	else
		castedFloat = static_cast<Float>(readDword(dc.o2, m_processor->memory()));

	writeFloat(castedFloat, dc.o1, m_processor->memory());

	return false;
}

// =========== ���������� ����������� ����� � ������� ===========

UItFCommand::UItFCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool UItFCommand::registerRegister(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0) // ����� � ��������
	{
		// ������� r2 ������ ���� ������.
		// ���� ��������, �� ����� ������� �� ������������ �������

		if(dc.r2 % 2 != 0) // �������� �� ��������
			return true;

		castedFloat = m_processor->gpr().uw[dc.r2]; // ���������� ������������ ����� � ��������
	}
	else
		castedFloat = static_cast<Float>(m_processor->gpr().dw[dc.r2]); // ���������� �������� ����� � ��������

	m_processor->gpr().f[dc.r1] = castedFloat;

	return false;
}

bool UItFCommand::registerAddress(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0)
		castedFloat = readUWord(dc.o1, m_processor->memory()); // ���������� ������������ ����� � ��������
	else
	{
		Long dw = readDword(dc.o1, m_processor->memory());
		castedFloat = static_cast<Float>(dw); // ���������� �������� ����� � ��������
	}

	m_processor->gpr().f[dc.r1] = castedFloat;

	return false;
}

bool UItFCommand::addressRegister(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0)
		castedFloat = m_processor->gpr().uw[dc.r1]; // ����������� ����� � ��������
	else
		castedFloat = static_cast<Float>(m_processor->gpr().dw[dc.r1]);

	writeFloat(castedFloat, dc.o1, m_processor->memory());

	return false;
}

bool UItFCommand::addressAddress(const MemCmd48 &dc)
{
	Float castedFloat;

	if(dc.s == 0)
		castedFloat = readUWord(dc.o2, m_processor->memory());
	else
		castedFloat = static_cast<Float>(readDword(dc.o2, m_processor->memory()));

	writeFloat(castedFloat, dc.o1, m_processor->memory());

	return false;
}

// =========== ���������� ������� � �������� ����� ===========

FtICommand::FtICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool FtICommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		// ������� r1 ������ ���� ������.
		// ���� ��������, �� ����� ������� �� ������������ �������

		if(dc.r1 % 2 != 0) // �������� �� ��������
			return true;

		Word castedWord = static_cast<Word>(m_processor->gpr().f[dc.r2]); // ���������� �������� � �����
		m_processor->gpr().w[dc.r1] = castedWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(m_processor->gpr().f[dc.r2]); // ���������� �������� � �������� �����
		m_processor->gpr().dw[dc.r1] = castedDword;
	}

	return false;
}

bool FtICommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		Word castedWord = static_cast<Word>(readFloat(dc.o1, m_processor->memory())); // ���������� �������� � �����
		m_processor->gpr().w[dc.r1] = castedWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(readFloat(dc.o1, m_processor->memory())); // ���������� �������� � �������� �����
		m_processor->gpr().dw[dc.r1] = castedDword;
	}

	return false;
}

bool FtICommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		Word castedWord = static_cast<Word>(m_processor->gpr().f[dc.r1]); // ���������� �������� � �����
		m_processor->memory()[dc.o1] = castedWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(m_processor->gpr().f[dc.r1]); // ���������� �������� � �������� �����
		writeDword(castedDword, dc.o1, m_processor->memory());
	}

	return false;
}

bool FtICommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		Word castedWord = static_cast<Word>(readFloat(dc.o2, m_processor->memory())); // ���������� �������� � �����
		m_processor->memory()[dc.o1] = castedWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(readDword(dc.o2, m_processor->memory())); // ���������� �������� � �������� �����
		writeDword(castedDword, dc.o1, m_processor->memory());
	}

	return false;
}

// =========== ���������� ������� � ����������� ����� ===========

FtUICommand::FtUICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool FtUICommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		// ������� r1 ������ ���� ������.
		// ���� ��������, �� ����� ������� �� ������������ �������

		if(dc.r1 % 2 != 0) // �������� �� ��������
			return true;

		uWord castedUWord = static_cast<uWord>(m_processor->gpr().f[dc.r2]); // ���������� �������� � ������������ �����
		m_processor->gpr().uw[dc.r1] = castedUWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(m_processor->gpr().f[dc.r2]); // ���������� �������� � �������� �����
		m_processor->gpr().dw[dc.r1] = castedDword;
	}

	return false;
}

bool FtUICommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord castedUWord = static_cast<uWord>(readFloat(dc.o1, m_processor->memory())); // ���������� �������� � ������������ �����
		m_processor->gpr().uw[dc.r1] = castedUWord;
	}
	else
	{
		Long castedDword = static_cast<Long>(readFloat(dc.o1, m_processor->memory())); // ���������� �������� � �������� �����
		m_processor->gpr().dw[dc.r1] = castedDword;
	}

	return false;
}

bool FtUICommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord castedUWord = static_cast<uWord>(m_processor->gpr().f[dc.r1]); // ���������� �������� � ������������ �����
		writeUWord(castedUWord, dc.o1, m_processor->memory());
	}
	else
	{
		Long castedDword = static_cast<Long>(m_processor->gpr().f[dc.r1]); // ���������� �������� � �������� �����
		writeDword(castedDword, dc.o1, m_processor->memory());
	}

	return false;
}

bool FtUICommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord castedUWord = static_cast<uWord>(readFloat(dc.o2, m_processor->memory())); // ���������� �������� � ������������ �����
		writeUWord(castedUWord, dc.o1, m_processor->memory());
	}
	else
	{
		Long castedDword = static_cast<Long>(readDword(dc.o2, m_processor->memory())); // ���������� �������� � �������� �����
		writeDword(castedDword, dc.o1, m_processor->memory());
	}

	return false;
}

// =========== ��������� ����� ===========

CmpICommand::CmpICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool CmpICommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		if(m_processor->gpr().w[dc.r1] == m_processor->gpr().w[dc.r2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().w[dc.r1] > m_processor->gpr().w[dc.r2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().w[dc.r1] < m_processor->gpr().w[dc.r2])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		if(m_processor->gpr().dw[dc.r1] == m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().dw[dc.r1] > m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().dw[dc.r1] < m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpICommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		if(m_processor->gpr().w[dc.r1] == m_processor->memory()[dc.o1])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().w[dc.r1] > m_processor->memory()[dc.o1])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().w[dc.r1] < m_processor->memory()[dc.o1])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());

		if(m_processor->gpr().dw[dc.r1] == addressDw)
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().dw[dc.r1] > addressDw)
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().dw[dc.r1] < addressDw)
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpICommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		if(m_processor->memory()[dc.o1] == m_processor->gpr().w[dc.r1])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->memory()[dc.o1] > m_processor->gpr().w[dc.r1])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->memory()[dc.o1] < m_processor->gpr().w[dc.r1])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());

		if(addressDw == m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = 0;

		else if(addressDw > m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = +1;

		else if(addressDw < m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpICommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		if(m_processor->memory()[dc.o1] == m_processor->memory()[dc.o2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->memory()[dc.o1] > m_processor->memory()[dc.o2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->memory()[dc.o1] < m_processor->memory()[dc.o2])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw1 = readDword(dc.o1, m_processor->memory());
		Long addressDw2 = readDword(dc.o2, m_processor->memory());

		if(addressDw1 == addressDw2)
			m_processor->m_psw.CMPF = 0;

		else if(addressDw1 > addressDw2)
			m_processor->m_psw.CMPF = +1;

		else if(addressDw1 < addressDw2)
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

// =========== ��������� ����� ===========

CmpUICommand::CmpUICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool CmpUICommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		if(m_processor->gpr().uw[dc.r1] == m_processor->gpr().uw[dc.r2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().uw[dc.r1] > m_processor->gpr().uw[dc.r2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().uw[dc.r1] < m_processor->gpr().uw[dc.r2])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		if(m_processor->gpr().dw[dc.r1] == m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().dw[dc.r1] > m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().dw[dc.r1] < m_processor->gpr().dw[dc.r2])
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpUICommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUword = readUWord(dc.o1, m_processor->memory());

		if(m_processor->gpr().uw[dc.r1] == addressUword)
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().uw[dc.r1] > addressUword)
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().uw[dc.r1] < addressUword)
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());

		if(m_processor->gpr().dw[dc.r1] == addressDw)
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().dw[dc.r1] > addressDw)
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().dw[dc.r1] < addressDw)
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpUICommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUWord = readUWord(dc.o1, m_processor->memory());

		if(addressUWord == m_processor->gpr().uw[dc.r1])
			m_processor->m_psw.CMPF = 0;

		else if(addressUWord > m_processor->gpr().uw[dc.r1])
			m_processor->m_psw.CMPF = +1;

		else if(addressUWord < m_processor->gpr().uw[dc.r1])
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw = readDword(dc.o1, m_processor->memory());

		if(addressDw == m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = 0;

		else if(addressDw > m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = +1;

		else if(addressDw < m_processor->gpr().dw[dc.r1])
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

bool CmpUICommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 0)
	{
		uWord addressUWord1 = readUWord(dc.o1, m_processor->memory());
		uWord addressUWord2 = readUWord(dc.o2, m_processor->memory());

		if(addressUWord1 == addressUWord2)
			m_processor->m_psw.CMPF = 0;

		else if(addressUWord1 > addressUWord2)
			m_processor->m_psw.CMPF = +1;

		else if(addressUWord1 < addressUWord2)
			m_processor->m_psw.CMPF = -1;
	}
	else
	{
		Long addressDw1 = readDword(dc.o1, m_processor->memory());
		Long addressDw2 = readDword(dc.o2, m_processor->memory());

		if(addressDw1 == addressDw2)
			m_processor->m_psw.CMPF = 0;

		else if(addressDw1 > addressDw2)
			m_processor->m_psw.CMPF = +1;

		else if(addressDw1 < addressDw2)
			m_processor->m_psw.CMPF = -1;
	}

	return false;
}

// =========== ��������� ������� ===========

CmpFCommand::CmpFCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool CmpFCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		if(m_processor->gpr().f[dc.r1] == m_processor->gpr().f[dc.r2])
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().f[dc.r1] > m_processor->gpr().f[dc.r2])
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().f[dc.r1] < m_processor->gpr().f[dc.r2])
			m_processor->m_psw.CMPF = -1;

		return false;
	}

	/// TODO: ������� ��������� �����
	return true;
}

bool CmpFCommand::registerAddress(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat = readFloat(dc.o1, m_processor->memory());

		if(m_processor->gpr().f[dc.r1] == addressFloat)
			m_processor->m_psw.CMPF = 0;

		else if(m_processor->gpr().f[dc.r1] > addressFloat)
			m_processor->m_psw.CMPF = +1;

		else if(m_processor->gpr().f[dc.r1] < addressFloat)
			m_processor->m_psw.CMPF = -1;

		return false;
	}

	/// TODO: ������� ��������� �����
	return true;
}

bool CmpFCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat = readFloat(dc.o1, m_processor->memory());

		if(addressFloat == m_processor->gpr().f[dc.r1])
			m_processor->m_psw.CMPF = 0;

		else if(addressFloat > m_processor->gpr().f[dc.r1])
			m_processor->m_psw.CMPF = +1;

		else if(addressFloat < m_processor->gpr().f[dc.r1])
			m_processor->m_psw.CMPF = -1;

		return false;
	}

	/// TODO: ������� ��������� �����
	return true;
}

bool CmpFCommand::addressAddress(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		Float addressFloat1 = readFloat(dc.o1, m_processor->memory());
		Float addressFloat2 = readFloat(dc.o2, m_processor->memory());

		if(addressFloat1 == addressFloat2)
			m_processor->m_psw.CMPF = 0;

		else if(addressFloat1 > addressFloat2)
			m_processor->m_psw.CMPF = +1;

		else if(addressFloat1 < addressFloat2)
			m_processor->m_psw.CMPF = -1;

		return true;
	}

	/// TODO: ������� ��������� �����
	return false;
}

// =========== ���� ����� ===========

InICommand::InICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool InICommand::registerRegister(const MemCmd48 &dc)
{
	std::cin.clear();

	if(checkBit(dc.r2, 0)) // r2 = xx1
		std::cout << "������� ����� �����: ";

	if(dc.s == 0)
	{
		if(!checkBit(dc.r2, 1)) // r2 == x0x
			std::cin >> m_processor->gpr().w[dc.r1];
		else // r2 == x1x
			std::cin >> m_processor->gpr().uw[dc.r1];
	}
	else
		std::cin >> m_processor->gpr().dw[dc.r1];

	return false;
}

bool InICommand::registerAddress(const MemCmd48 &dc)
{
	return true;
}

bool InICommand::addressRegister(const MemCmd48 &dc)
{
	std::cin.clear();

	if(checkBit(dc.r1, 0)) // r1 = xx1
		std::cout << "������� ����� �����: ";

	if(dc.s == 0)
	{
		if(!checkBit(dc.r1, 1)) // r1 == x0x
			std::cin >> m_processor->memory()[dc.o1];
		else // r1 == x1x
		{
			Data uwData;
			std::cin >> uwData.uw;

			m_processor->memory()[dc.o1] = uwData.w;
		}
	}
	else
	{
		Long enterDword;
		std::cin >> enterDword;

		writeDword(enterDword, dc.o1, m_processor->memory());
	}

	return false;
}

bool InICommand::addressAddress(const MemCmd48 &dc)
{
	return true;
}

// =========== ����� ����� ===========

OutICommand::OutICommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool OutICommand::registerRegister(const MemCmd48 &dc)
{
	if(checkBit(dc.r2, 0)) // r2 == xx1
		std::cout << "����� ������ �����: ";

	if(dc.s == 0)
	{
		if(!checkBit(dc.r2, 1)) // r2 == x0x
			std::cout << m_processor->gpr().w[dc.r1] << std::endl;
		else // r2 == x1x
			std::cout << m_processor->gpr().uw[dc.r1] << std::endl;
	}
	else
		std::cout << m_processor->gpr().dw[dc.r1] << std::endl;

	return false;
}

bool OutICommand::registerAddress(const MemCmd48 &dc)
{
	return true;
}

bool OutICommand::addressRegister(const MemCmd48 &dc)
{
	if(checkBit(dc.r1, 0)) // r1 == xx1
		std::cout << "����� ������ �����: ";

	if(dc.s == 0)
	{
		if(!checkBit(dc.r1, 1)) // r1 == x0x
			std::cout << m_processor->memory()[dc.o1] << std::endl;
		else // r1 == x1x
		{
			Data uwData;
			uwData.w = m_processor->memory()[dc.o1];
			std::cout << uwData.uw << std::endl;
		}
	}
	else
	{
		Long outDw = readDword(dc.o1, m_processor->memory());
		std::cout << outDw << std::endl;
	}

	return false;
}

bool OutICommand::addressAddress(const MemCmd48 &dc)
{
	return true;
}

// =========== ���� ������� ===========

InFCommand::InFCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool InFCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		if(dc.r2 == 1)
			std::cout << "������� ������� �����: ";

		std::cin >> m_processor->gpr().f[dc.r1];
		return false;
	}

	return true;
}

bool InFCommand::registerAddress(const MemCmd48 &dc)
{
	return true;
}

bool InFCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		if(dc.r1 == 1)
			std::cout << "������� ������� �����: ";

		Float inFloat;
		std::cin >> inFloat;

		writeFloat(inFloat, dc.o1, m_processor->memory());
		return false;
	}

	return true;
}

bool InFCommand::addressAddress(const MemCmd48 &dc)
{
	return true;
}

// =========== ����� ������� ===========

OutFCommand::OutFCommand(Processor *processor)
	: ProcessingCommand(processor)
{
}

bool OutFCommand::registerRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		if(dc.r2 == 1)
			std::cout << "����� �������� �����: ";

		std::cout << m_processor->gpr().f[dc.r1] << std::endl;
		return false;
	}

	return true;
}

bool OutFCommand::registerAddress(const MemCmd48 &dc)
{
	return true;
}

bool OutFCommand::addressRegister(const MemCmd48 &dc)
{
	if(dc.s == 1)
	{
		if(dc.r1 == 1)
			std::cout << "����� �������� �����: ";

		Float outFloat = readFloat(dc.o1, m_processor->memory());

		std::cout << outFloat << std::endl;
		return false;
	}

	return true;
}

bool OutFCommand::addressAddress(const MemCmd48 &dc)
{
	return true;
}
