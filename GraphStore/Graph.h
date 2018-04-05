#pragma once
#include"stdafx.h"


class Graph
{
private:
	/// Useful variables
	using arc = std::pair<std::string, unsigned>;
	using enges = std::list<arc>;
	using vertex = std::string;

	typedef std::unordered_map<vertex, enges>::const_iterator peakListIterator;

	/// Graph Id
	std::string graphId;

	/// Is directed or not
	bool directed;

	/// List for all peaks and their arcs
	std::unordered_map <vertex, enges> peakValueList;

public:
	///
	/// Contructor
	///
	Graph(std::string id = "", bool isDirected = false);

	///
	/// Clean data
	///
	void cleanGraph();

private:
	/// Cannot make copies
	Graph(const Graph& other);
	Graph& operator=(const Graph& other);

public:
	///
	/// Get Graph Id
	///
	std::string getId() const;

	///
	/// Is directed
	///
	bool isDirected() const;
	
public:
	///
	/// Create Node if it not exist
	///
	void createNode(const std::string &id);

	///
	/// Delete Node if it exist
	///
	void deleteNode(const std::string &id);

public:
	///
	/// Create Arc between two Nodes, if it not exist
	///
	void createArc(const std::string & startNodeId, const std::string & endNodeId, unsigned weight = 1);

	///
	/// Delete Arc between two Nodes, if it exist
	///
	void deleteArc(const std::string & startNodeId, const std::string & endNodeId);

public:
	///
	/// Search path between two Nodes and print it, if it exist
	///
	void search(const std::string & startNodeId, const std::string & endNodeId, const std::string & algorithm);

public:
	void saveData(std::string& dir) const;
	void loadData(std::string& file);

private:

	///
	/// Return true if this node exist
	///
	bool nodeExist(const vertex & id) const;

	///
	/// Deletes the arc between two existing arcs
	///
	void findAndDeleteArc(const vertex & startNodeId, const vertex & endNodeId);

	/* Algorithms */

	std::list<std::string> BFS(const vertex & startNodeId, const vertex & endNodeId) const;

	std::list<std::string> DFS_Shortest(const vertex & startNodeId, const vertex & endNodeId) const;

	std::list<std::string> DFS_Longest(const vertex & startNodeId, const vertex & endNodeId)const;

	void dijkstra(const vertex & startNodeId,
				std::unordered_map<vertex,vertex> &parentList, 
				std::unordered_map<vertex,unsigned> &distances) const;

	///
	/// Return all path found with dfs algorithm
	///
	std::vector<std::list<std::string> > dfsPaths(const vertex & startNodeId, const vertex & endNodeId) const;

	///
	/// Recursive DFS
	///
	void dfsRec(const vertex & targetNode, 
				std::list<std::string>  &currentPath, 
				std::unordered_map<vertex,bool> &visited, 
				std::vector<std::list<std::string> > &allPaths) const;

	
	/// Helpful function to print the path
	void printPath(std::list<vertex>& path) const;
	void printPath(vertex startNode, vertex endNode, std::unordered_map<vertex, vertex> &parentList) const;

};

