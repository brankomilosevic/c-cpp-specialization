
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>
#include <iomanip>

const int MAX_GRAPH_SIZE = 50;

/*
Graph representation class - adjacency-lists variant
Methods:
1.  V (G): returns the number of vertices in the graph
2.  E (G): returns the number of edges in the graph
3.  adjacent (G, x, y): tests whether there is an edge from node x to node y
4.  neighbors (G, x): lists all nodes y such that there is an edge from x to y
5.  add (G, x, y): adds to G the edge from x to y, if it is not there
6.  delete (G, x, y): removes the edge from x to y, if it is there
7.  get_node_value (G, x): returns the value associated with the node x
8.  set_node_value( G, x, a): sets the value associated with the node x to a
9.  get_edge_value( G, x, y): returns the value associated to the edge (x,y)
10. set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v

*/

template <typename T>
class Node
{

public:
    Node() : value(NULL) {}
    Node(T value) : value(value) {}

    inline bool operator ==(const Node<T>& other){ return (other.value == this->value); }
    inline bool operator !=(const Node<T>& other){ return !(other.value == this->value); }
    inline bool operator >(const Node<T>& other){ return (other.value > this->value); }
    inline bool operator <(const Node<T>& other){ return (other.value < this->value); }

    inline void setValue(T value) { this->value = value; }

    inline T getValue() const { return this->value; }

    friend std::ostream& operator<< (std::ostream& os, const Node<T>& node)
    {
        std::stringstream ss;
        ss << node.value;
        return (os << ss.str());
    }

private:
    T value;
};

template <typename T>
class Edge
{
public:
    Edge() : x(NULL), y(NULL), weight(0.0) {}

    Edge (Node<T>* x, Node<T>* y, double weight = 0.0) : x(x), y(y), weight(weight) {}

    ~Edge() {
        delete x;
        delete y;
    }

    inline bool operator==(const Edge<T>& other) { return (other.getX() == this->getX() && other.getY() == this->getY()); }

    inline Node<T>* getX() const { return x; }

    inline Node<T>* getY() const { return y;}

    inline double getWeight() const { return weight; }

    friend std::ostream& operator<< (std::ostream& os, const Edge<T>& edge)
    {
        std::stringstream ss;
        ss << "(" << edge.x << " " << "-" << " " << edge.y << ")";
        return (os << ss.str());
    }

private:
    Node<T>* x;
    Node<T>* y;
    double	weight;
};

class Graph
{
typedef std::list<Edge<int>*> listOfEdges;
typedef listOfEdges::iterator listOfEdgesItr;
typedef listOfEdges::const_iterator listOfEdgesConstItr;

public:

    // Initializes an empty graph with V vertices (50 by default) and no edges
	Graph(int V = MAX_GRAPH_SIZE) : V(V), E(0), adjacencyList(std::vector<listOfEdges>(V)) {}

    // De-initialize the graph and delete the adjacency list
    ~Graph()
    {
		for (int i = 0; i < adjacencyList.size(); ++i)
        {
			listOfEdgesItr itr = adjacencyList[i].begin();
			while (itr != adjacencyList[i].end())
			{
				delete *itr;
				++itr;
			}
		}
    }

    // Build a new graph
	Graph(const Graph& other)
	{
		if(this != &other)
			*this = other;
	}

    // Build a new graph as copy of existing one
	Graph* clone() const
	{
		Graph* g = new Graph();
		g->V = this->V;
		g->E = this->E;

		for (unsigned int i = 0; i < this->adjacencyList.size(); ++i)
			std::copy(this->adjacencyList[i].begin(), this->adjacencyList[i].end(), g->adjacencyList[i].begin());

		return g;
	}

    // copy assignment of the graph
    Graph& operator=(const Graph& other)
    {
		if (this != &other)
        {
			this->V = other.V;
			this->E = other.E;

			for (unsigned int i = 0; i < other.adjacencyList.size(); ++i)
				std::copy(other.adjacencyList[i].begin(), other.adjacencyList[i].end(), this->adjacencyList[i].begin());
		}
		return *this;
	}

    // check if the graphs are equal
    bool operator==(const Graph& other) const
    {
        // size is not the same
		if (this->getV() != other.getV() || this->getE() != other.getE() || this->adjacencyList.size() != other.adjacencyList.size())
			return false;

		for (unsigned int i = 0; i < this->adjacencyList.size(); ++i)
        {
			listOfEdgesConstItr thisItr = this->adjacencyList[i].begin();
			listOfEdgesConstItr otherItr = other.adjacencyList[i].begin();

			while (thisItr != adjacencyList[i].end())
            {
				if( (*thisItr) != (*otherItr))
					return false;

				++thisItr;
				++otherItr;
			}
		}
		return true;
	}

    // check is two graphs are not eqal
    bool operator!=(const Graph& other) const
    {
		return !(*this == other);
	}

    // Are vertices X and Y are adjacent
    bool isAdjacent(int x, int y) const
    {
//		assert(x > 0 || x <= V);
// 		assert(y > 0 || y <= V);

		listOfEdgesConstItr itr = adjacencyList[x].begin();

		while (itr != adjacencyList[x].end())
        {
			if( (*itr)->getX()->getValue() == x && (*itr)->getY()->getValue() == y)
				return true;

			++itr;
		}
		return false;
	}

    // List of all nodes adjacent to X (there is an edge between X and node)
    const listOfEdges getNeighbors(int x) const
    {
//		assert(x > 0 || x <= V);
		return adjacencyList[x];
	}

    // Adds an edge X-Y to the graph
    bool addEdge(int x, int y, double w = 0.0)
    {
//		assert(x > 0 || x <= V);
//		assert(y > 0 || y <= V);

		listOfEdgesItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if( (*itr)->getX()->getValue() == y)
				return false;

			++itr;
		}

		E++;

		adjacencyList[x].push_back(new Edge<int>(new Node<int>(x), new Node<int>(y), w));
		return true;
	}

    // Remove X-Y edge from graph
    bool removeEdge(int x, int y)
    {
//		assert(x > 0 || x <= V);
//		assert(y > 0 || y <= V);

		listOfEdgesItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == y)
            {
				adjacencyList[x].remove(*itr);
				E--;
				delete *itr;
				return true;
			}
			++itr;
		}
		return false;
	}

    // Returns a pointer to the node X
    const Node<int>* getNodeValue(int x) const
    {
//		assert(x > 0 || x <= V);

		listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x) {
				return (*itr)->getX();
			}
			++itr;
		}
		return NULL;
	}

    // Sets [value] to the node
    void setNodeValue(int x, int a)
    {
//		assert(x > 0 || x <= V);
//		assert(a > 0 || a <= V);

		listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x)
				(*itr)->getX()->setValue(a);

			++itr;
		}
	}

    // set value to the edge x-y
    void setEdgeValue(int x, int y, double v);
    // to-do


    // Gets edge value
    const Edge<int>* getEdgeValue(int x, int y) const
    {
//		assert(x > 0 || x <= V);
//		assert(y > 0 || y <= V);

		listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x && (*itr)->getY()->getValue() == y)
				return *itr;

			++itr;
		}
		return NULL;
	}

    // Returns the degree of the node
    int getDegree(int v) const
    {
//		assert(v > 0 || v <= V);
		return adjacencyList[v].size();
	}

    // build a randomly populate graph
    void generateRandomGraph(double density, double minDistance, double maxDistance)
    {

		const int MAX_NUM_EDGES = V * (V-1) / 2;
		const int EDGE_LIMIT = MAX_NUM_EDGES * density + 1;

		std::cout << "MAX_NUM_EDGES: " << MAX_NUM_EDGES << std::endl;
		std::cout << "EDGE_LIMIT: " << EDGE_LIMIT << std::endl;

		srand(time(NULL));

		while (this->getE() < EDGE_LIMIT)
        {
			int x = rand() % V;
			int y = rand() % V;

			if (x == y || this->isAdjacent(x, y))
				continue; // try another edge

			// create undirected edge with random distance
			double f = (double)rand() / RAND_MAX;
			double w = minDistance + f * (maxDistance - minDistance);
			this->addEdge(x, y, w);
		}
	}

    // prints graph
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph)
    {
		std::stringstream ss;

		ss << "Graph (" << graph.getV() << "," << graph.getE() << ")" << std::endl;
		ss << "The Adjacency List K(" << graph.getV() << ")" << std::endl;

		for (unsigned int i = 0; i < graph.adjacencyList.size(); ++i) {
			ss << "Adjacency List[" << i << "] ";

			Graph::listOfEdgesConstItr itr = graph.adjacencyList[i].begin();

			while (itr!= graph.adjacencyList[i].end()) {
				ss << std::setprecision(2);
				ss << " -> " << (*itr)->getY()->getValue() << "(" << (*itr)->getWeight() << ")";
				++itr;
			}
			ss << std::endl;
		}

		return (os << ss.str());
	}

    // returns adjacency list for this node
    std::vector< std::list<Edge<int>* > >& getAdjacencyList() { return this->adjacencyList; }

    // Returns number of vertices
    inline int getV() const { return this->V; }

    // Returns number of edges
    inline int getE() const { return this->E; }

private:
    int V;
    int E;
    std::vector< std::list<Edge<int>* > > adjacencyList;
};

// A priority queue implementation - vector bases
template <typename T>
class PriorityQueue
{
public:

    // constructor
    PriorityQueue(int maxCapacity) : heap(std::vector<HeapEntry>(maxCapacity)), currentSize(0) {}

    // destructor
    ~PriorityQueue() { this->clear(); }

    // adds element with priority
    void push(T element, double priority)
    {
        // double size of array if necessary
        int length = heap.size();
        if (currentSize == length - 1)
            heap.resize(2 * length);

        // add element, and percolate it up to maintain heap invariant
        int hole = ++currentSize;

        heap[hole].value = element;
        heap[hole].priority = priority;
        percolateUp(hole);
    }

    // remove the item with highet priority
    void pop()
    {
//        assert (currentSize > 0);
        heap[1] = heap[currentSize--];
        percolateDown(1);
    }

    // change the priority of element
    void changePriority(T element, double priority)
    {
        heap[currentSize].value = element;
        heap[currentSize].priority = priority;
        percolateUp(currentSize);
    }

    // remove top element
    void minPrioirty() { pop(); }

    // checks if there is an element
    bool contains(T element)
    {
        for (int i = 0; i < currentSize; i++)
            if (heap[i].value == element)
                return true;

        return false;
    }

    // return top element
    T& top() { return heap[1].value; }

    // return the queuq size
    int size() { return currentSize; }

    // clear the queuq
    void clear() { heap.clear(); }

    // checks if the queue is empty
    bool isEmpty() const { return (currentSize == 0); }

    // print the elements of the queue
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
    {
        std::stringstream ss;

        for (int i = 0; i < pq.currentSize+1; i++) {
            ss << pq.heap[i].value << "(" << pq.heap[i].priority << ")" << std::endl;
        }
        return (os << ss.str());
    }

private:

    void swap(int i, int j)
    {
        HeapEntry temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    bool greater(int i, int j) { return (heap[i].priority > heap[j].priority); }

    void percolateUp(int k)
    {
        while (k > 1 && greater(k / 2, k))
        {
            swap(k, k / 2);
            k = k / 2;
        }
    }

    void percolateDown(int k)
    {
        while (2 * k <= currentSize)
        {
            int j = 2 * k;
            if (j < currentSize && greater(j, j + 1))
                j++;
            if (!greater(k, j))
                break;
            swap(k, j);
            k = j;
        }
    }

    struct HeapEntry
    {
        T value;
        double priority;
    };

    std::vector<HeapEntry> heap;
    int currentSize;
};

// Implementation of Dijkstra algorithm
class ShortestPathDijkstra
{
    typedef std::vector<Node<int>*> listOfVertices;
    typedef std::list<Edge<int>*> listOfEdges;
    typedef listOfEdges::iterator listOfEdgesItr;
    typedef listOfEdges::const_iterator listOfEdgesConstItr;

public:

    ShortestPathDijkstra(Graph &graph) :
			graph(graph),
			distTo(std::vector<double>(graph.getV(), std::numeric_limits<double>::max())),
			edgeTo(std::vector<Edge<int>* >(graph.getV())),
			pq(graph.getV())
	{}

    ~ShortestPathDijkstra() {};

    // list of all vertices in the graph
    const listOfVertices vertices() const
    {
		listOfVertices v(graph.getV());

		for (unsigned int i = 0; i < graph.getAdjacencyList().size(); ++i)
        {
			listOfEdgesConstItr it = graph.getAdjacencyList()[i].begin();

			while (it != graph.getAdjacencyList()[i].end())
            {
				v.push_back((*it)->getX());
				++it;
			}
		}
		return v;
	}

	// is there a path to V
    bool hasPathTo(int v)
    {
		validateVertex(v);
		return distTo[v] < std::numeric_limits<double>::max();
	}

    // find the shortest path
    const listOfVertices path(int src, int dest)
    {
		std::vector< std::list<Edge<int>* > > adj = graph.getAdjacencyList();

		validateVertex(src);

		pq.push(src, 0);
		distTo[src] = 0;

		while (!pq.isEmpty())
        {
			int u = pq.top();
			pq.pop();

			listOfEdgesConstItr itr = adj[u].begin();
			while (itr != adj[u].end())
            {
				int y = (*itr)->getY()->getValue();
				double w = (*itr)->getWeight();

				if (distTo[y] > distTo[u] + w)
                {
					distTo[y] = distTo[u] + w;
					pq.push(y, distTo[y]);
					edgeTo[y] = (*itr);

					if (pq.contains(y))
						pq.changePriority(y, distTo[y]);
					else
						pq.push(y, distTo[y]);
				}
				++itr;
			}
		}

		listOfVertices shortestPath;
		if (hasPathTo(dest))
        {
			for (Edge<int>* e : pathTo(dest))
				shortestPath.push_back(e->getX());
		}

		return shortestPath;
	}

    // returns the size of the shortest path
    double pathSize(int x, int y)
    {
		validateVertex(x);
		validateVertex(y);

		return distTo[y];
	}

    // prints the shortest path
    void printShortestPath(int u, int w)
    {
		validateVertex(u);
		validateVertex(w);

		std::stringstream ss;
		ss << std::setprecision(2);

		const listOfVertices sp = path(u, w);

		ss << "Vertex   Distance from Source   Shortest Path" << std::endl;
		for (int t = u; t <= w; t++)
        {
			if (hasPathTo(t))
            {
				ss << t << "\t\t" << distTo[t] << "\t\t";

				for (Edge<int>* e : pathTo(t))
					ss << e->getX()->getValue() << "->";

				ss << t << std::endl;
			}
			else
				ss << u << "\t\t" << t << "\t\tno path" << std::endl;
		}

		ss << std::endl;

		ss << "path cost from " << u << " to " << w << ": " << pathSize(u, w) << std::endl;
		ss << "shortest path sequence from " << u << " to " << w << ": ";
		for ( int v = 0; v < sp.size(); ++v)
			ss << sp[v]->getValue() << "->";
		ss << w << std::endl;;

		std::cout << ss.str();
    }

private:

    // is vertex valid
    void validateVertex(int v)
    {
		int V = distTo.size();
//		assert(v > 0 || v <= V);
	}

    /**
     * Returns true if there is a path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path from the source vertex
     *         {@code s} to vertex {@code v}; {@code false} otherwise
     */
//    bool hasPathTo(int v);

    // returns shortest path to V
    std::vector<Edge<int>*> pathTo(int v)
    {
		validateVertex(v);

		if (!hasPathTo(v))
			return std::vector<Edge<int>*>();

		std::vector<Edge<int>*> path;

		for (auto e = edgeTo[v]; e != NULL; e = edgeTo[e->getX()->getValue()])
			path.push_back(e);

		std::reverse(path.begin(), path.end());

		return path;
	}

    Graph &graph;
    std::vector<double> distTo;
    std::vector<Edge<int>* > edgeTo;
    PriorityQueue<double> pq;
};


int main(int argc, char** argv)
{
	std::cout << "Dijkstra shortest path algorithm..." << std::endl;

	Graph randomG;
	randomG.generateRandomGraph(0.2, 0, 50);
	std::cout << randomG << std::endl;

	ShortestPathDijkstra shortestPath(randomG);
	shortestPath.printShortestPath(0, 49);

	return 0;
}
