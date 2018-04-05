#pragma once
 
#include"stdafx.h"
class Command
{
public:
	Command(std::string & input);
	
	unsigned argSize() const;
	const std::string& operator[](int index) const;

private:
	std::vector<std::string> arguments;
};

