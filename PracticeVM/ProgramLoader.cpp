#include <fstream>
#include <sstream>

#include "ProgramLoader.h"
#include "Data.h"
#include "Exceptions.h"

Address ProgramLoader::m_mainAddress;

void ProgramLoader::load(const std::string &filename, Word *memory, Address &ip)
{
	std::ifstream f(filename);

	if(!f.is_open())
		throw FileNotOpenedException();

	std::string line;
	size_t stringsCount = 0;

	if(!f.eof())
	{
		line = skipComments(f, stringsCount);
		parseLoad(line, ip, stringsCount);

		Address i = ip;

		while(!f.eof())
		{
			parseLine(line, memory, i, stringsCount);
			line = skipComments(f, stringsCount);
		}

		if(line != "")
			parseLine(line, memory, i, stringsCount);
	}
}

std::string ProgramLoader::skipComments(std::ifstream &f, size_t &stringsCount)
{
	std::string nextLine; // Следующая строка. Проверяется на комментарий 

	do // Пропускаем строки-комментарии
	{
		stringsCount++;
		getline(f, nextLine);
	} while(!f.eof() && nextLine[0] == ';'); // ; - знак начала коментария

	return nextLine;
}

void ProgramLoader::parseLoad(const std::string &loadLine, Address &ip, size_t lineNumber)
{
	std::stringstream ss(loadLine);
	std::string tag;

	ss >> tag;

	if(tag == "load")
	{
		Address loadAddress = 0;
		ss >> loadAddress;

		checkStream(ss, "Не удалось прочитать адрес загрузки. Строка: " + std::to_string(lineNumber));

		ip = loadAddress;
	}
	else
		ip = 1;
}

void ProgramLoader::parseLine(const std::string &line, Word *memory, Address &i, size_t lineNumber)
{
	std::stringstream ss(line);
	std::string tag;

	ss >> tag;

	if(tag == "a")
		putAddress(ss, memory, lineNumber);

	else if(tag == "i")
		putWord(ss, memory, lineNumber);

	else if(tag == "u")
		putUWord(ss, memory, lineNumber);

	else if(tag == "d")
		putDword(ss, memory, lineNumber);

	else if(tag == "f")
		putFloat(ss, memory, lineNumber);

	else if(tag == "c")
		putCommand(ss, memory, i, lineNumber);

	else if(tag == "sp")
	{
		m_mainAddress = i; // Запоминаем адрес, где была остановлена загрузка главной программы
		putSubProgram(ss, i, lineNumber);
	}

	else if(tag == "ret")
	{
		putRet(memory, i);
		i = m_mainAddress; // Восстанавливаем адрес
	}

	else if(tag == "end")
		putEnd(memory, i);

	
}

void ProgramLoader::putWord(std::stringstream &ss, Word *memory, size_t lineNumber)
{
	Address wherePut; // Куда положить
	ss >> wherePut;

	checkStream(ss, "Не удалось получить адрес загрузки слова. Строка: " + std::to_string(lineNumber));

	Data whatPut; // Что положить
	ss >> whatPut.w;

	checkStream(ss, "Не удалось получить значение слова. Строка: " + std::to_string(lineNumber));

	memory[wherePut] = whatPut.words[0];
}

void ProgramLoader::putUWord(std::stringstream &ss, Word *memory, size_t lineNumber)
{
	Address wherePut;
	ss >> wherePut;

	checkStream(ss, "Не удалось получить адрес загрузки беззнакового слова. Строка: " + std::to_string(lineNumber));

	Data whatPut;
	ss >> whatPut.uw;

	checkStream(ss, "Не удалось получить значение беззнакового слова. Строка: " + std::to_string(lineNumber));

	memory[wherePut] = whatPut.words[0];
}

void ProgramLoader::putAddress(std::stringstream &ss, Word *memory, size_t lineNumber)
{
	putUWord(ss, memory, lineNumber); // Адрес является беззнаковым словом
}

void ProgramLoader::putDword(std::stringstream &ss, Word *memory, size_t lineNumber)
{
	Address wherePut;
	ss >> wherePut;

	checkStream(ss, "Не удалось получить адрес загрузки двойного слова. Строка: " + std::to_string(lineNumber));

	Data whatPut;
	ss >> whatPut.dw;

	checkStream(ss, "Не удалось получить значение двойного слова. Строка: " + std::to_string(lineNumber));

	memory[wherePut] = whatPut.words[0];
	memory[wherePut + 1] = whatPut.words[1];
}

void ProgramLoader::putFloat(std::stringstream &ss, Word *memory, size_t lineNumber)
{
	Address wherePut;
	ss >> wherePut;

	checkStream(ss, "Не удалось получить адрес загрузки числа с плавающей точкой. Строка: " + std::to_string(lineNumber));

	Data whatPut;
	ss >> whatPut.f;

	checkStream(ss, "Не удалось получить значение числа с плавающей точкой. Строка: " + std::to_string(lineNumber));

	memory[wherePut] = whatPut.words[0];
	memory[wherePut + 1] = whatPut.words[1];
}

void ProgramLoader::putCommand(std::stringstream &ss, Word *memory, Address &i, size_t lineNumber)
{
	// Данный метод считывает информацию, которая присутствует в каждой команде (КОП, s, dd)
	// Затем вызывается метод, характерный для данного типа команды

	Data commandData;

	uWord code;
	uWord s;
	uWord dd;

	ss >> code;
	checkStream(ss, "Не удалось получить код операции. Строка: " + std::to_string(lineNumber));

	ss >> s;
	checkStream(ss, "Не удалось получить бит s. Строка: " + std::to_string(lineNumber));

	ss >> dd;
	checkStream(ss, "Не удалось получить биты dd. Строка: " + std::to_string(lineNumber));

	commandData.cmd.code = code;
	commandData.cmd.s = s;
	commandData.cmd.dd = dd;

	if(commandData.cmd.code < CommandCodes::Jmp)
		putProcessingCommand(ss, commandData, i, memory, lineNumber);

	else if(commandData.cmd.code >= CommandCodes::Jmp && commandData.cmd.code < CommandCodes::Call)
		putJumpCommand(ss, commandData, i, memory, lineNumber);

	else if(commandData.cmd.code == CommandCodes::Call)
		putCallCommand(ss, commandData, i, memory, lineNumber);
}

void ProgramLoader::putProcessingCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber)
{
	switch(commandData.cmd.dd)
	{
		case 0:
			commandData.cmd.r1 = parseRegister(ss, lineNumber);
			commandData.cmd.r2 = parseRegister(ss, lineNumber);
			memory[i++] = commandData.words[0];
			break;

		case 1:
			commandData.cmd.r1 = parseRegister(ss, lineNumber);
			commandData.cmd.o1 = parseAddress(ss, lineNumber);

			memory[i++] = commandData.words[0];
			memory[i++] = commandData.words[1];
			break;

		case 2:
			commandData.cmd.o1 = parseAddress(ss, lineNumber);
			commandData.cmd.r1 = parseRegister(ss, lineNumber);

			memory[i++] = commandData.words[0];
			memory[i++] = commandData.words[1];
			break;

		case 3:
			commandData.cmd.o1 = parseAddress(ss, lineNumber);
			commandData.cmd.o2 = parseAddress(ss, lineNumber);

			memory[i++] = commandData.words[0];
			memory[i++] = commandData.words[1];
			memory[i++] = commandData.words[2];
			break;
	}
}

void ProgramLoader::putJumpCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber)
{
	switch(commandData.cmd.dd)
	{
		case 0:
			throw ParsingException("Бит dd = 0 запрещен для команд переходов. Строка: " + std::to_string(lineNumber));

		case 1:
			commandData.cmd.o1 = parseAddress(ss, lineNumber);

			memory[i++] = commandData.words[0];
			memory[i++] = commandData.words[1];
			break;

		case 2:
			commandData.cmd.r2 = parseRegister(ss, lineNumber);
			memory[i++] = commandData.words[0];
			break;

		case 3:
			commandData.cmd.r2 = parseRegister(ss, lineNumber);
			commandData.cmd.o1 = parseRegister(ss, lineNumber);

			memory[i++] = commandData.words[0];
			memory[i++] = commandData.words[1];
			break;
	}
}

void ProgramLoader::putCallCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber)
{
	commandData.cmd.r1 = parseRegister(ss, lineNumber);
	putJumpCommand(ss, commandData, i, memory, lineNumber);
}

uWord ProgramLoader::parseRegister(std::stringstream &ss, size_t lineNumber)
{
	uWord reg;
	ss >> reg;

	checkStream(ss, "Не удалось получить регистр. Строка: " + std::to_string(lineNumber));

	return reg;
}

uWord ProgramLoader::parseAddress(std::stringstream &ss, size_t lineNumber)
{
	uWord address;
	ss >> address;

	checkStream(ss, "Не удалось получить адрес. Строка: " + std::to_string(lineNumber));

	return address;
}

void ProgramLoader::putSubProgram(std::stringstream &ss, Address &ip, size_t lineNumber)
{
	Address subProgramAddress = parseAddress(ss, lineNumber);
	ip = subProgramAddress;
}

void ProgramLoader::putRet(Word *memory, Address &i)
{
	Data data;

	data.cmd.code = CommandCodes::Ret;
	data.cmd.s = 0;
	data.cmd.dd = 0;

	memory[i++] = data.words[0];
}

void ProgramLoader::putEnd(Word *memory, Address &i)
{
	Data data;

	data.cmd.code = CommandCodes::Stop;
	data.cmd.s = 0;
	data.cmd.dd = 0;

	memory[i++] = data.words[0];
}

void ProgramLoader::checkStream(const std::stringstream &checkingStream, const std::string &errorMessage)
{
	if(checkingStream.fail())
		throw ParsingException(errorMessage);
}
