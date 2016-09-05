#ifndef DATACOMMANDS_H
#define DATACOMMANDS_H

#include "DefinedTypes.h"
#include "CommandCodes.h"

#pragma pack(push, 1)

// Структура команды (макс 6 байт)
struct MemCmd48
{
	uWord code : 7; // Код операции команды
	uWord s : 1; // Флаг, размер операнда
	uWord dd : 2; // Формат операнда

	uWord r1 : 3; // Регистр 1 
	uWord r2 : 3; // Регистр 2

	uWord o1 : 16; // Оффсет 1
	uWord o2 : 16; // Оффсет 2

	// Вычисляет размер команды в словах.
	uWord calculateSize() const
	{
		uWord size = 1; // Размер в словах

		if(code != CommandCodes::Stop && code != CommandCodes::Nop && code != CommandCodes::Ret) // Не зависят от операндов, а значит всегда минимальны
		{
			if(code < CommandCodes::Jmp) // У команд переходов размеры распределены по другому
			{
				if(dd == 1 || dd == 2) // 01 или 10
					++size;
				else if(dd == 3) // 11
					size += 2;
			}
			else
			{
				// Команды перехода не больше 2 слов
				if(dd == 0)
					size = 0;
				else if(dd == 1 || dd == 3)
					++size;
			}
		}

		return size;
	}
};

// Представление памяти
// Позволяет из слов получить двойные слова, вещественные числа, команды
union Data
{
	Word words[3]; // Для чтения из памяти

	Word w; // Вытаскивает слово
	uWord uw; // Вытаскивает беззнаковое слово
	Long dw; // Вытаскивает двойное слово
	Float f; // Вытаскивает с плавающей точкой
	Address a; // Вытаскивает адрес

	MemCmd48 cmd; // Команда в памяти
};

#pragma pack(pop)

#endif // DATACOMMANDS_H

