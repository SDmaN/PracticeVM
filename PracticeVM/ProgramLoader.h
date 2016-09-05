#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "DefinedTypes.h"

union Data;

// ����� ���������� ��������� �� ���������� �����
class ProgramLoader
{
public:
	static void load(const std::string &filename, Word *memory, Address &ip); // ��������� ��������� � ������

private:
	static std::string skipComments(std::ifstream &f, size_t &stringsCount); // ���������� ����������� � ���������� ��������� �������� ������
	static void parseLoad(const std::string &loadLine, Address &ip, size_t lineNumber); // ��������� ������� ��������� IP
	static void parseLine(const std::string &line, Word *memory, Address &i, size_t lineNumber); // ��������� "��������" ������ (����. �������)

	static void putWord(std::stringstream &ss, Word *memory, size_t lineNumber); // ��������� ����� � ������
	static void putUWord(std::stringstream &ss, Word *memory, size_t lineNumber); // ��������� ����������� ����� � ������
	static void putAddress(std::stringstream &ss, Word *memory, size_t lineNumber); // ��������� ����� � ������
	static void putDword(std::stringstream &ss, Word *memory, size_t lineNumber); // ��������� ������� ����� � ������
	static void putFloat(std::stringstream &ss, Word *memory, size_t lineNumber); // ��������� ������� ����� � ������

	static void putCommand(std::stringstream &ss, Word *memory, Address &i, size_t lineNumber); // ��������� ������� � ������
	static void putProcessingCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // ��������� ������� ��������� � ������
	static void putJumpCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // ��������� ������� �������� � ������ (�� ������� call)
	static void putCallCommand(std::stringstream &ss, Data &commandData, Address &i, Word *memory, size_t lineNumber); // ��������� ������� call � ������

	// ��������� 2 ������ ���������� ����������.
	static uWord parseRegister(std::stringstream &ss, size_t lineNumber); // ������ �������, ���� ��������. ����� - ����������
	static uWord parseAddress(std::stringstream &ss, size_t lineNumber); // ������ �����, ���� ��������. �����- ����������

	static void putSubProgram(std::stringstream &ss, Address &ip, size_t lineNumber); // ������������� ����� ����� �������� ������ (������������)

	static void putRet(Word *memory, Address &i); // ��������� Ret � ������
	static void putEnd(Word *memory, Address &i); // ��������� Stop � ������

	// ��������� ��������� ������. ���� fail, �� ������� ���������� ParseException � ���������� errorMessage.
	static void checkStream(const std::stringstream &checkingStream, const std::string &errorMessage);

	static Address m_mainAddress; // ����� �������� ��������� ������� ��� �������� ���������
};

#endif // PROGRAMLOADER_H

