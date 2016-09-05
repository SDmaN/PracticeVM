#ifndef MEMORY_H
#define MEMORY_H

#include "DefinedTypes.h"

const size_t DefaultSize = 65536; // ������ � ������ ��-��������� (���� �� ����� ���� � ������������)
const size_t MaxSize = 131072; // ������������ ������ � ������.

// ����� ������ ����������� ������.
class Memory
{
public:
	explicit Memory(size_t bytesLength = DefaultSize);
	~Memory();

	Word * words() const; // ���������� ������� ��������� �� ������ (� ������)
	size_t wordsLength() const; // ������ ������ � ������ (� 2 ���� ������, ��� bytesLength)

	void resize(size_t wordsLength); // ������������� ����� ������ � ������. ���� �������� ��������, �� ��������� ���� ����������.
	void clear(); // ������� ������ (��������� ������)

private:
	Word *m_words; // ��������� �� ������ � ������
	size_t m_wordsLength; // ������ ������ � ������

	void setNullVariables();
};


#endif // MEMORY_H

