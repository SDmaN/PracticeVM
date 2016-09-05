#ifndef MEMORY_H
#define MEMORY_H

#include "DefinedTypes.h"

const size_t DefaultSize = 65536; // Размер в байтах по-умолчанию (если не задан явно в конструкторе)
const size_t MaxSize = 131072; // Максимальный размер в байтах.

// Класс памяти виртуальной машины.
class Memory
{
public:
	explicit Memory(size_t bytesLength = DefaultSize);
	~Memory();

	Word * words() const; // Возвращает базовый указатель на память (в словах)
	size_t wordsLength() const; // Размер памяти в словах (в 2 раза меньше, чем bytesLength)

	void resize(size_t wordsLength); // Устанавливает новую длинну в байтах. Если значение нечетное, то последний байт обрезается.
	void clear(); // Очищает память (удаляется массив)

private:
	Word *m_words; // Указатель на память в словах
	size_t m_wordsLength; // Размер памяти в словах

	void setNullVariables();
};


#endif // MEMORY_H

