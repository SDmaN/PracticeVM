#include <fstream>

#include "VirtualMachine.h"
#include "ProgramLoader.h"

VirtualMachine::VirtualMachine()
	: VirtualMachine(DefaultSize)
{
}

VirtualMachine::VirtualMachine(size_t wordsLength)
{
	m_vmMemory = std::make_shared<Memory>(wordsLength);
	m_processor = std::make_shared<Processor>(m_vmMemory->words(), m_vmMemory->wordsLength());
}

VirtualMachine::~VirtualMachine()
{
}

void VirtualMachine::loadProgram(const std::string &filename)
{
	ProgramLoader::load(filename, m_vmMemory->words(), m_processor->m_psw.IP);
}

void VirtualMachine::interpret()
{
	m_processor->interpret();
}
