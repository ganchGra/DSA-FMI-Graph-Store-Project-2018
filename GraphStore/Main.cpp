// GraphStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << "Enter the path of the working directory: ";
	std::string path;
	std::getline(std::cin, path);

	GraphStore testStore(path);
	
	std::cout << "Enter command: ";
	std::string command;
	std::getline(std::cin, command);

	while (command != "EXIT" && command != "exit")
	{
		try
		{
			testStore.execute(command);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		std::cout << "Enter command: ";
		std::getline(std::cin, command);
	}

	return 0;
}
