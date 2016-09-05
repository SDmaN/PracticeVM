#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// ����������, ������������� � ������, ���� �� ������� ������� ����
class FileNotOpenedException : public std::exception
{
public:
	virtual char const * what() const override; // ��������� �� ������
};

// ����������, ������������� � ������, ���� ��������� ������ ������� ����� ���������
class ParsingException : public std::exception
{
public:
	explicit ParsingException(const std::string &errorMessage);
	virtual char const * what() const override; // ��������� �� ������

private:
	std::string m_errorMessage;
};

#endif // EXCEPTIONS_H

