#include"stdafx.h"
#pragma once
class GraphStore
{
private:
	///
	/// Useful declarations
	///
	using oldGraphs = std::unordered_map<std::string, File>;
	typedef std::unordered_map<std::string, File>::const_iterator oldGraphsIt;

private:
	///
	/// Save the path of the working directory
	///
	std::string workDir;

	///
	/// Container with all files (.txt) than maybe are Graph 
	///
	oldGraphs files;

	///
	/// Current loaded Graph
	///
	Graph * currentGraph;

	///
	/// False if current Graph is not loaded
	///
	bool haveCurrentGraph;

public:
	///
	/// Constructor
	///
	GraphStore(const std::string & directory);


	///
	/// Comand menu
	///
	void execute(std::string& command);

private:
	///
	/// Private function to load all files in specified directory
	///
	void loadFiles(const std::string & directory);

	///
	/// Verify that this identifier exists
	///
	bool existGraph(const std::string & id) const;

private: 
	/* Graph Store command */
	void createGraph(std::string & id, bool isDirected = false);
	void useGraph(std::string &id);
	void deleteGraph(std::string &id);


};

