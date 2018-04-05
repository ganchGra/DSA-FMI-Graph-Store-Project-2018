#include"stdafx.h"

///
/// Contructor
///
Graph::Graph(std::string id, bool isDirected)
	:graphId(id), directed(isDirected), peakValueList()
{}

///
/// Clean data
///
void Graph::cleanGraph()
{
	graphId.clear();
	directed = false;
	
	peakValueList.clear();
}
///
/// Get Graph Id
///
std::string Graph::getId() const
{
	return graphId;
}

///
/// Is directed
///
bool Graph::isDirected() const
{
	return directed;
}
///
/// Return true if this node exist
///
bool Graph::nodeExist(const vertex & id) const
{
	peakListIterator findElement = peakValueList.find(id);

	if (findElement != peakValueList.end())
	{
		return true;
	}

	return false;
}
///
/// Create Node if it not exist
///
void Graph::createNode(const std::string &id)
{
	if (nodeExist(id))
	{
		throw std::logic_error("Node is already exist.");
	}
	peakValueList[id];
}

///
/// Delete Node if it exist
///
void Graph::deleteNode(const std::string &id)
{
	if (!nodeExist(id))
	{
		throw std::logic_error("Node is not exist!");
	}

	for (auto it = peakValueList.cbegin(); it != peakValueList.cend(); ++it)
	{
		findAndDeleteArc(it->first, id);
	}
	
	peakValueList[id].clear();
	peakValueList.erase(id);
	
}

///
/// Create Arc between two Nodes, if it not exist
///
void Graph::createArc(const std::string & startNodeId, const std::string & endNodeId, unsigned weight)
{	
	if (!nodeExist(startNodeId))
	{
		throw std::logic_error("Start node is not exist!");
	}

	if (!nodeExist(endNodeId))
	{
		throw std::logic_error("End node is not exist!");
	}

	if (directed)
	{
		peakValueList[startNodeId].push_front({ endNodeId,weight });
	}
	else
	{
		peakValueList[startNodeId].push_front({ endNodeId,weight });
		peakValueList[endNodeId].push_front({ startNodeId,weight });
	}

}

///
/// Delete Arc between two Nodes, if it exist
///
void Graph::deleteArc(const std::string & startNodeId, const std::string & endNodeId)
{
	if (!nodeExist(startNodeId))
	{
		throw std::logic_error("Start node is not exist!");
	}

	if (!nodeExist(endNodeId))
	{
		throw std::logic_error("End node is not exist!");
	}
	findAndDeleteArc(startNodeId, endNodeId);
}

///
/// Deletes the arc between two existing arcs
///
void Graph::findAndDeleteArc(const vertex & startNodeId, const vertex & endNodeId)
{
	std::list<arc>::iterator it = peakValueList[startNodeId].begin();
	std::list<arc>::iterator endList = peakValueList[startNodeId].end();

	while (it != endList)
	{
		if (it->first == endNodeId)
		{
			it = peakValueList[startNodeId].erase(it);
		}
		else
		{
			++it;
		}
	}
}


void Graph::saveData(std::string& dir) const
{
	std::string file = dir + "\\" + graphId + ".txt";
	std::ofstream out(file.c_str());

	out << graphId << "\n"
		<< directed << "\n"
		<< peakValueList.size() << "\n";

	peakListIterator it = peakValueList.begin();

	while (it != peakValueList.end())
	{
		out << it->first << "\n";
		++it;
	}

	it = peakValueList.begin();
	while (it != peakValueList.end())
	{
		std::list<arc>::const_iterator listIterator = it->second.begin();

		while (listIterator != it->second.end())
		{
			out << it->first << "\\" << listIterator->first << "\\" << listIterator->second << "\n";
			++listIterator;
		}
		++it;
	}
	out.close();
}

void Graph::loadData(std::string& file)
{
	
	std::ifstream from(file.c_str());
	if (!from)
	{
		throw std::logic_error("Somethings is wrong. Try again...");
	}
	cleanGraph();

	std::getline(from, graphId);

	if (!from)
	{
		cleanGraph();
		from.close();
		throw std::exception("This file is not Graph!");
	}
	from >> directed;
	if (!from)
	{
		cleanGraph();
		from.close();
		throw std::exception("This file is not Graph!");
	}
	unsigned numberOfVertix;
	from >> numberOfVertix;
	from.ignore();
	if (!from)
	{
		cleanGraph();
		from.close();
		throw std::exception("This file is not Graph!");
	}
	if (!numberOfVertix)
	{
		from.close();
		return;
	}
	peakValueList.reserve(numberOfVertix);

	for (size_t i = 0; from && i < numberOfVertix ; i++)
	{
		vertex node;	
		std::getline(from, node);
		peakValueList[node];
	}
	if (!from)
	{
		cleanGraph();
		from.close();
		throw std::exception("This file is not Graph!");
	}
	while (from.good() && !from.eof())
	{
		vertex startNodeId;
		vertex endNodeId;
		unsigned weight;

		std::getline(from, startNodeId, '\\');
		if (startNodeId == "")
		{
			break;
		}
		if (!from)
		{
			cleanGraph();
			from.close();
			throw std::exception("This file is not Graph!");
		}
		std::getline(from, endNodeId, '\\');
		if (!from)
		{
			cleanGraph();
			from.close();
			throw std::exception("This file is not Graph!");
		}
		from >> weight;
		if (!from)
		{
			cleanGraph();
			from.close();
			throw std::exception("This file is not Graph!");
		}

		peakValueList[startNodeId].push_front({ endNodeId,weight });

		from.ignore();
	}
	
}

///
/// Search path between two Nodes and print it, if it exist
///
void Graph::search(const std::string & startNodeId, const std::string & endNodeId, const std::string & algorithm)
{
	/*****************Nodes is not exist*******************/
	if (!nodeExist(startNodeId))
		throw std::logic_error("Start node is not exist!");

	if (!nodeExist(endNodeId))
		throw std::logic_error("End node is not exist!");
	/*******************************************************/

	std::list<vertex> path;
	
	if (algorithm == "bfs" || algorithm == "BFS")
	{
		path = BFS(startNodeId, endNodeId);
	}
	else if (algorithm == "dfs-shortest" || algorithm == "DFS-SHORTEST")
	{
		path = DFS_Shortest(startNodeId, endNodeId);
	}
	else if (algorithm == "dfs-longest" || algorithm == "DFS-LONGEST")
	{
		path = DFS_Longest(startNodeId, endNodeId);
	}
	else if (algorithm == "dijkstra" || algorithm == "DIJKSTRA")
	{
		std::unordered_map<vertex, vertex> parentList;
		std::unordered_map<vertex, unsigned> distances;
		dijkstra(startNodeId, parentList, distances);
		if (parentList[endNodeId] != "NO_PARENT")
		{
			std::cout
				<< "Exist path betweet node: " << startNodeId << " and node: " << endNodeId << std::endl
				<< "With total weight: " << distances[endNodeId] << std::endl
				<< "[ START -> " << startNodeId ;
			printPath(startNodeId, endNodeId, parentList);
			std::cout << " -> END ]\n";

			return;
		}
	}
	else
	{
		throw std::logic_error("Select between this algorithms: <bfs>, <dfs-shortest>, <dfs-longest>, <dijikstra>");
	}

	if (path.size())
	{
		std::cout << "Exist path betweet node: " << startNodeId << " and node: " << endNodeId << std::endl;
		printPath(path);
	}
	else
	{
		std::cout << "Is not exist path betweet node: " << startNodeId << " and node: " << endNodeId << std::endl;
	}
}
std::list<std::string> Graph::BFS(const vertex & startNodeId, const vertex & endNodeId) const
{
	std::unordered_map<vertex,vertex> parents;
	
	std::unordered_set<vertex> visitedNodes;
	visitedNodes.reserve(peakValueList.size());
	visitedNodes.insert(startNodeId);

	std::queue<vertex> wave;
	wave.push(startNodeId);

	peakListIterator vertexIt;

	bool foundPath = false;
	vertex finalVertex;

	while (!wave.empty() && !foundPath)
	{
		vertex currentVertex = wave.front();
		wave.pop();

		vertexIt = peakValueList.find(currentVertex);
		// This vertex is not exist
		if (vertexIt == peakValueList.end())
		{
			wave.pop();
			continue;
		}

		std::list<arc>::const_iterator engesIt = vertexIt->second.begin();
		for ( ; engesIt != vertexIt->second.end(); ++engesIt)
		{
			vertex nextNode = engesIt->first;
			std::unordered_set<std::string>::const_iterator visitIterator = visitedNodes.find(nextNode);
			if (visitIterator == visitedNodes.end())
			{
				wave.push(nextNode);
				visitedNodes.insert(nextNode);
				parents[nextNode]=currentVertex;
			}

			// We found it
			if (nextNode == endNodeId)
			{
				foundPath = true;
				finalVertex = nextNode;
			}
		}	
	}

	if (!foundPath)
	{
		// Path is not exist
		return std::list<vertex>();
	}
	
	std::list<vertex> path;
	while (finalVertex != "")
	{
		path.push_front(finalVertex);
		finalVertex = parents[finalVertex];
	}
	return path;
}

std::vector<std::list<std::string> > Graph::dfsPaths(const vertex & startNodeId, const vertex & endNodeId) const
{
	std::vector<std::list<std::string> > paths;

	std::list<std::string> initialPath;
	initialPath.push_front(startNodeId);

	std::unordered_map<vertex, bool> visited;
	visited.reserve(peakValueList.size());
	peakListIterator it = peakValueList.begin();
	// set all vertex like unvisited
	for (; it != peakValueList.end(); ++it)
		visited[it->first] = false;

	visited[startNodeId] = true;

	dfsRec(endNodeId, initialPath, visited, paths);

	return paths;
}
std::list<std::string> Graph::DFS_Shortest(const vertex & startNodeId, const vertex & endNodeId) const
{
	std::vector<std::list<std::string> > paths = dfsPaths(startNodeId, endNodeId);

	if (!paths.size())
	{
		return std::list<vertex>();
	}
	unsigned pos = 0;
	unsigned shortest = paths[0].size();
	for (size_t i = 1; i < paths.size(); i++)
	{
		if (shortest > paths[i].size())
		{
			shortest = paths[i].size();
			pos = i;
		}
	}
	paths[pos].reverse();
	return paths[pos];
}
std::list<std::string> Graph::DFS_Longest(const vertex & startNodeId, const vertex & endNodeId) const
{
	std::vector<std::list<std::string> > paths = dfsPaths(startNodeId, endNodeId);

	if (!paths.size())
	{
		return std::list<vertex>();
	}
	unsigned pos = 0;
	unsigned longest = paths[0].size();
	for (size_t i = 1; i < paths.size(); i++)
	{
		if (longest < paths[i].size())
		{
			longest = paths[i].size();
			pos = i;
		}
	}

	paths[pos].reverse();
	return paths[pos];
}

void Graph::dfsRec( const vertex & targetNode,
					std::list<std::string>  &currentPath,
					std::unordered_map<vertex,bool> &visited,
					std::vector<std::list<std::string> > &allPaths) const
{
	vertex lastNode = currentPath.front();

	if (lastNode == targetNode)
	{
		allPaths.push_back(currentPath);
	}
	else
	{
		peakListIterator lastNodeIt = peakValueList.find(lastNode);
		std::list<arc>::const_iterator engesIt = lastNodeIt->second.begin();
		std::list<arc>::const_iterator engesItEnd = lastNodeIt->second.end();
		for (; engesIt != engesItEnd; ++engesIt)
		{
			vertex neighbor = engesIt->first;
			bool isVisited = visited[neighbor];
			if (!isVisited)
			{
				currentPath.push_front(neighbor);
				visited[neighbor] = true;
				dfsRec(targetNode, currentPath, visited, allPaths);
				visited[neighbor] = false;
				currentPath.pop_front();
			}
		}
	}
}


void Graph::dijkstra(const vertex & startNodeId,
					std::unordered_map<vertex,vertex> &parentList,
					std::unordered_map<vertex, unsigned> &distances) const
{
	std::unordered_map<vertex, bool> visited;
	visited.reserve(peakValueList.size());
	peakListIterator it = peakValueList.begin();
	// set all vertex like unvisited
	for (; it != peakValueList.end(); ++it)
	{
		visited[it->first] = false;
		distances[it->first] = UINT_MAX;
		parentList[it->first] = "NO_PARENT";
	}

	distances[startNodeId] = 0;

	auto cmp = [](const arc &lhs, const arc &rhs) ->bool
	{
		return lhs.second > rhs.second;
	};
	std::priority_queue<arc, std::vector<arc>, decltype(cmp) > wave(cmp);

	wave.push(std::make_pair(startNodeId,distances[startNodeId]));

	while (!wave.empty())
	{
		arc current = wave.top();
		wave.pop();
		visited[current.first] = true;

		peakListIterator it = peakValueList.find(current.first);
		if (it == peakValueList.end())
			continue;

		std::list<arc>::const_iterator engesIt = it->second.begin();

		for (;engesIt != it->second.end();++engesIt)
		{
			if (!visited[engesIt->first])
			{
				if (distances[engesIt->first] > distances[current.first] + engesIt->second)
				{
					parentList[engesIt->first] = current.first;
					distances[engesIt->first] = distances[current.first] + engesIt->second;

					wave.push(std::make_pair(engesIt->first, distances[engesIt->first]));
				}
			}
		}
	}
}

/// Helpful function to print the path
void Graph::printPath(vertex startNode, vertex endNode,std::unordered_map<vertex, vertex> &parentList) const
{
	if (parentList[endNode] != startNode)
		printPath(startNode, parentList[endNode],parentList);
	
	std::cout << " -> " << endNode;

}
void Graph::printPath(std::list<vertex>& path) const
{
	std::list<vertex>::const_iterator it = path.begin();
	std::cout << "[ START -> ";
	for (; it != path.end(); ++it)
	{
		std::cout << *it << " -> ";
	}
	std::cout << " END ]\n";
}