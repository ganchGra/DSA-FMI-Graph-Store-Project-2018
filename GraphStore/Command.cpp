#include"stdafx.h"


Command::Command(std::string & input)
	:arguments()
{
	arguments.reserve(5);
	if (!input.size())
	{
		throw std::logic_error("Invalid command.");
	}

	std::stringstream text(input);
	while (text)
	{
		std::string arg;
		std::getline(text, arg, ' ');
		if (arg.size())
		{
			arguments.push_back(arg);
		}
	}
}
const std::string& Command::operator[](int index) const
{
	return arguments[index];
}
unsigned Command::argSize() const
{
	return arguments.size();
}