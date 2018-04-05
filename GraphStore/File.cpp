
#include"stdafx.h"
///
/// Constructors
///
File::File()
	:path(), fullName(), name()
{}
File::File(const std::string &path, const std::string &fullName)
	: path(path), fullName(),name()
{
	setName(fullName);
}

///
/// Copy contructor
///
File::File(const File& rhs)
	: path(rhs.path), fullName(rhs.fullName),name(rhs.name)
{}

///
/// Assignment from another obj
///
File& File::operator=(const File& rhs)
{
	if (this != &rhs)
	{
		setPath(rhs.path);
		setName(rhs.fullName);
	}
	return *this;
}


/* Get file information */
std::string File::getPath() const
{
	return path;
}

std::string File::getFullPath() const
{
	std::string fullpath = path + '\\' + fullName;
	return fullpath;
}
std::string File::getFullName() const
{
	return fullName;
}

std::string File::getName() const
{
	return name;
}
/* Set file information */
void File::setPath(const std::string &src)
{
	path = src;
}
void File::setName(const std::string &fullName)
{
	this->fullName = fullName;

	name.clear();
	std::stringstream text(fullName);
	std::getline(text, name, '.');
	
	text.clear();
}

///
/// Scan if extention is matches 
///
bool File::fileExtScan(const std::string &ext)
{
	size_t extLen = ext.size();
	size_t nameLen = fullName.size();

	size_t nameSymbolPos = 0;

	for (size_t i = 0; i < extLen; i++)
	{
		if (nameSymbolPos >= nameLen)
			break;

		//wildcards
		char symbol = ext[i];
		if (symbol == '?')
		{
			++nameSymbolPos;
		}
		else if (symbol == '*')
		{
			char nextSymbol = (i + 1 < extLen) ? ext[i + 1] : NULL;
			if (nextSymbol == '*')
				throw std::logic_error("To much * symbols!");

			bool findNextSymbol = false;

			for (size_t pos = nameSymbolPos; pos < nameLen; pos++)
			{
				if (fullName[pos] == nextSymbol)
				{
					findNextSymbol = true;
					break;
				}

				++nameSymbolPos;
			}
			if (!findNextSymbol)
				return false;

		}
		else
		{
			if (fullName[nameSymbolPos] != ext[i])
				return false;
			++nameSymbolPos;
		}
	}
	return true;
}