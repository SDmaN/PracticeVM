#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "DefinedTypes.h"

union Data;

// Класс загрузчика программы из текстового файла
class ProgramLoader
{
public:
	static void load(const std::string &filename, Word *memory, Address &ip); // Загружает программу в память

private:
	static std::string skipComments(std::ifstream &f, size_t &stringsCount); // Пропускает комментарии и возвращает следующую значащую строку
	static void parseLoad(const std::string &loadLine, Address &ip, size_t lineNumber); // Загружает команду установки IP
	static void parseLine(const std::string &line, Word *memory, Address &i, size_t lineNumber); // Загружает "значимую" строку (напр. команду)

	static void putWord(std::stringstream &ss, Word *memory, size_t lineNumber); // Вставляет слово в память
	static void putUWord(std::stringstream &ss, Word *memory, size_t lineNumber); // Вставляет беззнаковое слово в память
	static void putAddress(std::stringstream &ss, Word *memory, size_t lineNumber); // Вставляет адрес в память
	static void putDword(std::stringstream &ss, Word *memory, size_t lineNumber); // Вставляет двойное слово в память
	static void putFloat(std::stringstream &ss, Word *memory, size_t lineNumber); // Вставляет дробное число в память

	static void putCommand(std::stringstream &ss, Word *memory, Address &i, size_t lineNumber); // Вставляет команду в память
	static void putProcessingCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // Вставляет команду обработки в память
	static void putJumpCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // Вставляет команду перехода в память (не команду call)
	static void putCallCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // Вставляет команду call в память

	// Следующие 2 метода отличаются сообщением.
	static uWord parseRegister(std::stringstream &ss, size_t lineNumber); // Парсит регистр, если возможно. Иначе - исключение
	static uWord parseAddress(std::stringstream &ss, size_t lineNumber); // Парсит адрес, если возможно. Иначе- исключение

	static void putSubProgram(std::stringstream &ss, Address &ip, size_t lineNumber); // Устанавливает новый адрес загрузки команд (подпрограмма)

	static void putRet(Word *memory, Address &i); // Вставляет Ret в память
	static void putEnd(Word *memory, Address &i); // Вставляет Stop в память

	// Проверяет состояние потока. Если fail, то бросает исключение ParseException с сообщением errorMessage.
	static void checkStream(const std::stringstream &checkingStream, const std::string &errorMessage);

	static Address m_mainAddress; // Адрес загрузки очередной команды для основной программы
};

#endif // PROGRAMLOADER_H

