#include"stdafx.h"

///
/// Constructor
///
GraphStore::GraphStore(const std::string & directory)
	:currentGraph(NULL), files(), haveCurrentGraph(false),workDir(directory)
{
	try
	{
		loadFiles(directory);
	}
	catch (const std::exception&)
	{
		throw;
	}
}

///
/// Private function to load all files in specified directory
///
void GraphStore::loadFiles(const std::string & directory)
{
	char path[_MAX_PATH + 3];

	strcpy_s(path, _MAX_PATH, directory.c_str());
	strcat_s(path, _MAX_PATH, "\\*");

	// Find the first file in the directory
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(path, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		throw std::exception("FindFirstFile failed! Error code is " + GetLastError());
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			File temp(directory,ffd.cFileName);
			if (temp.fileExtScan("?*.txt"))
			{
				Graph tempGraph;
				try
				{
					std::string path= temp.getFullPath();
					tempGraph.loadData(path);
					files[temp.getName()] = temp;
				}
				catch (const std::exception&)
				{
					/// if the read file is not Graph
					continue;
				}
				tempGraph.cleanGraph();
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		throw std::exception("FindNextFile failed! Error code is " + GetLastError());
	}

	FindClose(hFind);
}

///
/// Verify that this identifier exists
///
bool GraphStore::existGraph(const std::string & id) const
{
	oldGraphsIt it = files.find(id);
	if (it == files.end())
	{
		return false;
	}
	return true;
}

/* Graph Store command */
void GraphStore::useGraph(std::string &id)
{
	if (files.size() == 0)
	{
		throw std::logic_error("This directory is empty!");
	}

	if (!existGraph(id))
	{
		throw std::logic_error("Graph is not exist!");
	}
	if (haveCurrentGraph && currentGraph->getId() == id)
	{
		throw std::logic_error("This is your current loaded Graph.");
	}
	/// Save the current Graph
	if (haveCurrentGraph)
	{
		currentGraph->saveData(workDir);
	}
	Graph * buffer = new Graph;
	try
	{
		std::string path = files[id].getFullPath();
		buffer->loadData(path);
	}
	catch (const std::exception&)
	{
		delete buffer;
		throw;
	}
	
	delete currentGraph;
	currentGraph = buffer;
	haveCurrentGraph = true;
}

void GraphStore::deleteGraph(std::string &id)
{
	if (files.size() == 0)
	{
		throw std::logic_error("This directory is empty!");
	}

	if (!existGraph(id))
	{
		throw std::logic_error("Graph is not exist!");
	}


	if (haveCurrentGraph && id == currentGraph->getId())
	{
		// Clean data from the GraphStore obj
		currentGraph->cleanGraph();
		delete currentGraph;
		currentGraph = NULL;
		haveCurrentGraph = false;
	}
	
	// Remove file from the system
	std::string path = files[id].getFullPath();
	remove(path.c_str());
	files.erase(id);
}

void GraphStore::createGraph(std::string & id, bool isDirected)
{
	if (existGraph(id))
	{
		throw std::logic_error("This ID is already in use!");
	}
	if (haveCurrentGraph)
	{
		currentGraph->saveData(workDir);
		delete currentGraph;
		currentGraph = NULL;
	}

	Graph * buffer = new Graph(id, isDirected);
	
	currentGraph = buffer;
	haveCurrentGraph = true;
	std::string fileName = id + ".txt";
	files[id] = File(workDir, fileName);
}

///
/// Comand menu
///
void GraphStore::execute(std::string& command)
{
	Command inputCommand(command);
	unsigned size = inputCommand.argSize();
	
	std::string task = (size) ? inputCommand[0] : "";

	if (task == "")
	{
		throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
	}
	if (task == "HELP")
	{
		std::cout
			<< "*****************Command menu*******************\n\n"
			<< "For <id> use single word, please!\n\n"
			<< "     -------Graph Store commands-----\n"
			<< "CREATE GRAPH <id> [<directed>]\n"
			<< "USE GRAPH <id>\n"
			<< "DELETE GRAPH <id>\n\n"
			<< "     -------Graph commands-----------\n"
			<< "CREATE NODE <id>\n"
			<< "DELETE NODE <id>\n"
			<< "CREATE ARC <startNodeId> <endNodeId> [<weight>]\n"
			<< "DELETE NODE <startNodeId> <endNodeId>\n"
			<< "SEARCH <startNodeId> <endNodeId> <algorithm>\n"
			<< "  algorithms are:\n"
			<< "    - <bfs> \n"
			<< "    - <dfs-shortes>\n"
			<< "    - <dfs-longest>\n"
			<< "    - <dijkstra>\n\n"
			<< "     -------Help menu----------------\n"
			<< "HELP\n\n"
			<< "*************************************************\n\n";
	}
	else if (task == "CREATE")
	{
		
		std::string what =(size > 1)? inputCommand[1] : "";
		
		if (what == "GRAPH")
		{
			std::string id = (size > 2) ? inputCommand[2] : ""; 

			if (id == "")
			{
				throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
			}

			std::string direction = (size > 3) ? inputCommand[3] : "";
			if (direction != "")
			{
				if (direction == "directed")
				{
					createGraph(id, true);
				}
				else
				{
					throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
				}
			}
			else
			{
				createGraph(id);
			}
		}
		else if(what == "NODE")
		{
			std::string id = (size > 2) ? inputCommand[2] : "";
			if (id == "")
			{
				throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
			}
			if (!haveCurrentGraph)
			{
				throw std::logic_error("Invalid command! Please, load current Graph first. Open the help menu <HELP>.");
			}

			currentGraph->createNode(id);

		}
		else if(what == "ARC")
		{
			if (size < 4)
			{
				throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
			}
			if (!haveCurrentGraph)
			{
				throw std::logic_error("Invalid command! Please, load current Graph first. Open the help menu <HELP>.");
			}

			std::string startNode = inputCommand[2];	
			std::string endNode = inputCommand[3];
			
			std::string strWeight = (size > 4) ? inputCommand[4] : "";

			if (strWeight != "")
			{
				unsigned weight = atoi(strWeight.c_str());
				currentGraph->createArc(startNode, endNode, weight);	
			}
			else
			{
				currentGraph->createArc(startNode, endNode);
			}
			
		}
		else
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}
	}
	else if (task == "DELETE")
	{
		if (size < 3)
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}
		std::string what = inputCommand[1];
		std::string id = inputCommand[2];
		
		if (what == "GRAPH")
		{
			deleteGraph(id);
		}
		else if (what == "NODE")
		{
			if (!haveCurrentGraph)
			{
				throw std::logic_error("Invalid command! Please, load current Graph first. Open the help menu <HELP>.");
			}
			currentGraph->deleteNode(id);
		}
		else if (what == "ARC")
		{
			if (!haveCurrentGraph)
			{
				throw std::logic_error("Invalid command! Please, load current Graph first. Open the help menu <HELP>.");
			}
			if (size < 4)
			{
				throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
			}
			std::string endNode = inputCommand[3];
			currentGraph->deleteArc(id, endNode);
		}
		else
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}

	}
	else if (task == "USE")
	{
		if (size < 3)
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}
		std::string what = inputCommand[1];
		if (what == "GRAPH")
		{
			std::string id = inputCommand[2];
			useGraph(id);
		}
		else
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}
	}

	else if (task == "SEARCH")
	{
		if (size < 4)
		{
			throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
		}
		if (!haveCurrentGraph)
		{
			throw std::logic_error("Invalid command! Please, load current Graph first. Open the help menu <HELP>.");
		}
		std::string startNode = inputCommand[1];
		std::string endNode = inputCommand[2];
		std::string algorithm = inputCommand[3];
		currentGraph->search(startNode, endNode, algorithm);
	}
	else
	{
		throw std::logic_error("Invalid command! Please, load help menu <HELP>.");
	}
}