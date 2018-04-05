/// Class File desription
/**
* Rapresent the path and file name of a File obj
*/
#include"stdafx.h"
#pragma once

class File
{
private:
	std::string path;
	std::string fullName;
	std::string name;

public:
	///
	/// Constructors
	///
	File();
	File(const std::string &path,const std::string &fullName);

	///
	/// Copy contructor
	///
	File(const File& rhs);

	///
	/// Assignment from another obj
	///
	File& operator=(const File& rhs);

public:
	/* Get file information */
	std::string getPath() const;
	std::string getFullPath() const;

	/// Return name without extension
	std::string getName() const;

	std::string getFullName() const;

	/* Set file information */

	void setPath(const std::string &src);

	void setName(const std::string &name);

public:
	///
	/// Scan if extention is matches 
	/// The extension must be with wildcards
	///
	bool fileExtScan(const std::string &ext);


};
