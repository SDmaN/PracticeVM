#include "Exceptions.h"

char const * FileNotOpenedException::what() const
{
	return "�� ������� ������� �������� ����.";
}

ParsingException::ParsingException(const std::string &errorMessage)
	: m_errorMessage(errorMessage)
{
}

char const * ParsingException::what() const
{
	return m_errorMessage.c_str();
}
