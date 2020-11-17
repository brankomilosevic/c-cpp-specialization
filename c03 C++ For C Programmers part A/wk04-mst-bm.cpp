#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <string>
#include <sstream>
#include <utility>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <iterator>

const int MAX_GRAPH_SIZE = 50;

// Class representing the Node in the graph
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

// Class representing the edge of the graph
template <typename T>
class Edge
{
public:
    Edge() : x(NULL), y(NULL), weight(0.0) {}

    Edge(Node<T>* x, Node<T>* y, double weight = 0.0) : x(x), y(y), weight(weight) {}

    ~Edge()
    {
        delete x;
        delete y;
    }

    inline bool operator==(const Edge<T>& other) { return (other.getX() == this->getX() && other.getY() == this->getY()); }

    inline Node<T>* getX() const { return x; }

    inline Node<T>* getY() const { return y; }

    inline double getWeight() const { return weight; }

    inline int either() { return getX()->getValue(); }

    // returns other vertex of the edge
    int other(int vertex)
    {
        if (vertex == getX()->getValue())
            return getY()->getValue();
        else if (vertex == getY()->getValue())
            return getX()->getValue();
        else
            throw std::invalid_argument("Illegal end-point");
    }

    // prints the instance of the node
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

// Class representing the Graph itself
class Graph
{
public:

    typedef std::list<Edge<int>*> listOfEdges;
    typedef listOfEdges::iterator listOfEdgesItr;
    typedef listOfEdges::const_iterator listOfEdgesConstItr;

    Graph(int V = MAX_GRAPH_SIZE) :
			V(V),
			E(0),
			adjacencyList(std::vector<listOfEdges>(V))
    {}

    Graph(const std::string& filename)
    {
        std::ifstream inFile(filename.c_str());
        std::istream_iterator<std::string> start(inFile), end;
        std::vector<std::string> uvw(start, end);

        if(!inFile.is_open())
        {
        	std::cout << "File: " << filename << " not found!" << std::endl;
        	exit(0);
        }

		inFile.close();

        this->V = convert<int>(uvw[0]);
        this->E = 0;
        this->adjacencyList = std::vector<listOfEdges>(V);

        for (unsigned int i = 1; i < uvw.size()-2; i+=3)
        {
        	int u = convert<int>(uvw[i]);
        	int v = convert<int>(uvw[i+1]);
        	double w = convert<double>(uvw[i+2]);
        	this->addEdge(u, v, w);
        }
    }

    ~Graph()
    {
		for (unsigned int i = 0; i < adjacencyList.size(); ++i)
        {
			listOfEdgesItr itr = adjacencyList[i].begin();
			while (itr != adjacencyList[i].end())
			{
				delete *itr;
				++itr;
			}
		}
	}

    Graph(const Graph& other)
    {
		if(this != &other)
			*this = other;
	}


    Graph* clone() const
    {
		Graph* g = new Graph();
		g->V = this->V;
		g->E = this->E;

		for (unsigned int i = 0; i < this->adjacencyList.size(); ++i)
        	std::copy(this->adjacencyList[i].begin(), this->adjacencyList[i].end(),
				g->adjacencyList[i].begin());

		return g;
	}

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


    bool operator==(const Graph& other) const
    {
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

    bool operator!=(const Graph& other) const
    {
        return !(*this == other);
    }

    inline int getV() const { return this->V; }

    inline int getE() const { return this->E; }

    bool isAdjacent(int x, int y) const
    {
		listOfEdgesConstItr itr = adjacencyList[x].begin();

		while (itr != adjacencyList[x].end()) {
			if( (*itr)->getX()->getValue() == x && (*itr)->getY()->getValue() == y)
				return true;
			++itr;
		}
		return false;
	}

    const listOfEdges getNeighbors(int x) const
    {
		return adjacencyList[x];
    }

    bool addEdge(int x, int y, double w = 0.0)
    {
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

    bool removeEdge(int x, int y)
    {
		assert(x > 0 || x <= V);
		assert(y > 0 || y <= V);

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

    const Node<int>* getNodeValue(int x) const
    {
		listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x)
				return (*itr)->getX();
			++itr;
		}
		return NULL;
	}

    void setNodeValue(int x, int a)
    {
        listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x)
				(*itr)->getX()->setValue(a);

			++itr;
		}
	}

    void setEdgeValue(int x, int y, double v);
    // to-do

    const Edge<int>* getEdgeValue(int x, int y) const
    {
        listOfEdgesConstItr itr = adjacencyList[x].begin();
		while (itr != adjacencyList[x].end())
        {
			if ((*itr)->getX()->getValue() == x && (*itr)->getY()->getValue() == y)
				return *itr;

			++itr;
		}
		return NULL;
	}

    int getDegree(int v) const
    {
        return adjacencyList[v].size();
    }

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
				continue;

			// create undirected edge with random distance
			double f = (double)rand() / RAND_MAX;
			double w = minDistance + f * (maxDistance - minDistance);
			this->addEdge(x, y, w);
		}
	}

    std::vector< std::list<Edge<int>* > >& getAdjacencyList() { return this->adjacencyList; }

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph)
    {
		std::stringstream ss;

		ss << "Graph (" << graph.getV() << "," << graph.getE() << ")" << std::endl;
		ss << "The Adjacency List K(" << graph.getV() << ")" << std::endl;

		for (unsigned int i = 0; i < graph.adjacencyList.size(); ++i)
        {
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

private:
    int V;
    int E;
    std::vector< std::list<Edge<int>* > > adjacencyList;

    template <class Type>
    Type convert(const std::string& str) {
            Type result;
            std::istringstream sin(str);
            sin >> result;
            return result;
    }
};

// Class representing priority queue
template <typename T>
class PriorityQueue
{
public:
    PriorityQueue(int maxCapacity) : heap(std::vector<HeapEntry>(maxCapacity)), currentSize(0) {}

    ~PriorityQueue() { this->clear(); }

    void push(T element, double priority)
    {
        int length = heap.size();
        if (currentSize == length - 1)
            heap.resize(2 * length);

        int hole = ++currentSize;

        heap[hole].value = element;
        heap[hole].priority = priority;
        percolateUp(hole);
    }

    void pop()
    {
//        assert (currentSize > 0);
        heap[1] = heap[currentSize--];
        percolateDown(1);
    }

    void changePriority(T element, double priority)
    {
        heap[currentSize].value = element;
        heap[currentSize].priority = priority;
        percolateUp(currentSize);
    }

    int delMin()
    {
        int u = top();
        pop();
        return u;
    }

    bool contains(T element)
    {
        for (int i = 0; i < currentSize; i++)
            if (heap[i].value == element)
                return true;
        return false;
    }

    T& top() { return heap[1].value; }

    int size() { return currentSize; }

    void clear() { heap.clear(); }

    bool isEmpty() const { return (currentSize == 0); }

    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
    {
        std::stringstream ss;

        for (int i = 0; i < pq.currentSize+1; i++)
            ss << pq.heap[i].value << "(" << pq.heap[i].priority << ")" << std::endl;

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

// MST class
class MST
{
public:
    MST(Graph& graph) :
		edgeTo(std::vector<Edge<int>* >(graph.getV())),
		distTo(std::vector<double>(graph.getV(), std::numeric_limits<double>::max())),
		marked(std::vector<bool>(graph.getV())),
		pq(graph.getV())
	{
		for (int v = 0; v < graph.getV(); v++)
			if (!marked[v])
				prim(graph, v);
	}

    void prim(Graph& g, int s)
    {
		distTo[s] = 0.0;
		pq.push(s, distTo[s]);

		while (!pq.isEmpty())
        {
			int v = pq.top(); //pq.delMin();
			pq.pop();
			scan(g, v);
		}
	}

    void scan(Graph& g, int v)
    {
		marked[v] = true;
		std::vector<std::list<Edge<int>*> > adj = g.getAdjacencyList();

		for (Edge<int>* e : adj[v])
        {
			int w = e->other(v);
			if (marked[w])
				continue;
			if (e->getWeight() < distTo[w])
			{
				distTo[w] = e->getWeight();
				edgeTo[w] = e;
				if (pq.contains(w))
					pq.changePriority(w, distTo[w]);
				else
					pq.push(w, distTo[w]);
			}
		}
	}

    const std::vector<Edge<int>*> edges()
    {
		std::vector<Edge<int>*> mst;
		for (unsigned int v = 0; v < edgeTo.size(); v++)
        {
			Edge<int>* e = edgeTo[v];
			if (e != NULL)
				mst.push_back(e);
		}
		return mst;
	}

    double cost()
    {
		double weight = 0.0;
		for (Edge<int>* e : edges())
			weight += e->getWeight();
		return weight;
	}

private:
    std::vector<Edge<int>* > edgeTo;
    std::vector<double> distTo;
    std::vector<bool> marked;
    PriorityQueue<double> pq;
};

// Prims algorithms output
int main(int argc, char** argv)
{
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "   Minimum Spanning Tree Algorithm   " << std::endl;
	std::cout << "-------------------------------------" << std::endl << std::endl;

	Graph randomG;
	randomG.generateRandomGraph(0.2, 0, 50);
	std::cout << randomG << std::endl;

	MST primMst(randomG);
	for (Edge<int>* e : primMst.edges())
		std::cout << e->getX()->getValue() << "-" << e->getY()->getValue() << " (" << e->getWeight() << ")" << std::endl;

	std::cout << std::endl;
	std::cout << "Total cost: " << primMst.cost() << std::endl;

	return 0;
}
