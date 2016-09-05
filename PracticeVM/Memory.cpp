#include "Memory.h"

Memory::Memory(size_t bytesLength)
{
	setNullVariables();
	resize(bytesLength);
}

Memory::~Memory()
{
	clear();
}

Word * Memory::words() const
{
	return m_words;
}

size_t Memory::wordsLength() const
{
	return m_wordsLength;
}

void Memory::resize(size_t wordsLength)
{
	clear();
	m_wordsLength = wordsLength;

	m_words = new Word[m_wordsLength];
}

void Memory::clear()
{
	if(m_words != nullptr)
	{
		delete[] m_words;
		setNullVariables();
	}
}

void Memory::setNullVariables()
{
	m_words = nullptr;
	m_wordsLength = 0;
}
