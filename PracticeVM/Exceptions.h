#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// Исключение, выбрасываемое в случае, если не удалось открыть файл
class FileNotOpenedException : public std::exception
{
public:
	virtual char const * what() const override; // Сообщение об ошибке
};

// Исключение, выбрасываемое в случае, если произошла ошибка разбора файла программы
class ParsingException : public std::exception
{
public:
	explicit ParsingException(const std::string &errorMessage);
	virtual char const * what() const override; // Сообщение об ошибке

private:
	std::string m_errorMessage;
};

#endif // EXCEPTIONS_H

