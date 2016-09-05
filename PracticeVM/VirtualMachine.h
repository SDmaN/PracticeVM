#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <memory>

#include "Memory.h"
#include "Processor.h"

class VirtualMachine
{
public:
	VirtualMachine();
	explicit VirtualMachine(size_t wordsLength);
	~VirtualMachine();

	void loadProgram(const std::string &filename);
	void interpret();

private:
	std::shared_ptr<Memory> m_vmMemory;
	std::shared_ptr<Processor> m_processor;
};

#endif // VIRTUALMACHINE_H

