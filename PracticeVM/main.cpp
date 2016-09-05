#include <iostream>
#include <cmath>
#include <string>
#include "VirtualMachine.h"
#include "Exceptions.h"

using namespace std;

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "russian");

	try
	{
		if(argc > 1)
		{
			VirtualMachine m(stoi(argv[1]));;
			m.loadProgram(argv[2]);
			m.interpret();
		}
		else
			throw FileNotOpenedException();
	}
	catch(invalid_argument &)
	{
		cout << "Передан неверный размер памяти." << endl;
	}
	catch(exception &ex) // Исключения файла и парсинга
	{
		cout << ex.what() << endl;
	}

	system("pause");
	return 0;
}
