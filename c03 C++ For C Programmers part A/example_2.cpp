/*
  Coursera course: C++ For C Programmers <cplusplus4c-002>
  Homework 2: "Implement Dijkstra's Algorithm"
  Implement a Monte Carlo simulation that calculates the average
  shortest path in a graph. The graph can be generated using a
  pseudo-random number generator to produce edges and their costs. The
  shortest path algorithm will be Dijkstra’s.
  Tested on Linux with GCC 4.8.2 and Clang 3.3:
  $ g++ --std=c++11 hw2_matrix.cpp -o hw2
  or
  $ clang++ --std=c++11 hw2_matrix.cpp -o hw2
  @author Boris Kaul <me@boriskaul.com>
  @date 10-27-2013
 */

#include <algorithm>
#include <cinttypes>
#include <random>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
#include <memory>

/*
  There is no `using namespace std;` because:
  - http://llvm.org/docs/CodingStandards.html#do-not-use-using-namespace-std
  - http://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice
 */


/*
  Indexed Minimum Priority Queue (fixed-capacity)
  nodes in the array starts from the position 1 to make more efficient
  calculation of the parent node and children.
 */
template<typename T>
class IndexedMinPriorityQueue {
public:
  /*
    Queue constructor
    capacity - maximum capacity
   */
  explicit IndexedMinPriorityQueue(uint32_t capacity)
      : _nodes(new Node[capacity+1]),
        _index(new uint32_t[capacity+1]),
        _capacity(capacity),
        _size(0) {

    std::fill_n(_index.get(), _capacity+1, 0);
  }

  /*
    returns true if the queue is empty
   */
  bool empty() const noexcept { return (_size == 0); }

  /*
    size of the queue
   */
  size_t size() const noexcept { return _size; }

  /*
    returns true if the queue contains item with the provided index
   */
  bool contains(uint32_t i) const noexcept { return !!_index[i]; }

  /*
    add new node
   */
  void add(uint32_t index, T v) noexcept {
    Node &n = _nodes[++_size];
    n.key = v;
    n.index = index;
    _index[index] = _size;

    _swim(_size);
  }

  /*
    get index of the minimum key
   */
  uint32_t get_min() const noexcept {
    return _nodes[1].index;
  }

  /*
    delete minimum key
   */
  void del_min() noexcept {
    _exch(1, _size--);
    _sink(1);
  }

  /*
    change key
   */
  void change(uint32_t index, T key) noexcept {
    index = _index[index];
    Node &n = _nodes[index];
    T tmp_key = n.key;
    n.key = key;

    if (key > tmp_key) {
      _sink(index);
    } else {
      _swim(index);
    }
  }

  /*
    get key by its index
   */
  const T &get_by_index(uint32_t index) const noexcept {
    return _nodes[_index[index]];
  }

private:
  /*
    Internal Structure to store index and key
   */
  struct Node {
    Node() : key(0), index(0) {}

    Node(const Node &o)
        : key(o.key), index(o.index) {}

    T key;
    uint32_t index;
  };

  /*
    reheapify up
   */
  void _swim(uint32_t index) noexcept {
    while (index > 1) {
      uint32_t parent_index = index >> 1;

      if (_nodes[index].key < _nodes[parent_index].key) {
        _exch(index, parent_index);
        index = parent_index;
      } else {
        break;
      }
    }
  }

  /*
    reheapify down
   */
  void _sink(uint32_t index) noexcept {
    uint32_t cindex = index << 1;

    while (cindex <= _size) {
      if (cindex != _size && (_nodes[cindex].key > _nodes[cindex+1].key))
        cindex++;

      if (_nodes[index].key <= _nodes[cindex].key)
        break;

      _exch(index, cindex);
      index = cindex;
      cindex = index << 1;
    }
  }

  /*
    swap items
   */
  void _exch(uint32_t a, uint32_t b) noexcept {
    std::swap(_nodes[a], _nodes[b]);
    std::swap(_index[_nodes[a].index], _index[_nodes[b].index]);
  }

private:
  std::unique_ptr<Node[]> _nodes;
  std::unique_ptr<uint32_t[]> _index;
  uint32_t _capacity;
  uint32_t _size;
};

/*
  Graph (Adjacency Matrix representation)
  +infinity values in the matrix indicates that there is no edge
  between vertexes
  I am not using vector of vectors, because:
  http://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
 */
class Graph {
public:
  explicit Graph(uint32_t nodes_count)
      : _matrix(new float[nodes_count*nodes_count]),
        _nodes_count(nodes_count) {}

  /*
    Reset graph
    removes all edges between vertexes
   */
  void reset() noexcept {
    std::fill_n(_matrix.get(),
                _nodes_count*_nodes_count,
                std::numeric_limits<float>::infinity());
  }

  /*
    number of nodes in the graph
   */
  size_t nodes_count() const noexcept { return _nodes_count; }

  /*
    returns weight of the edge
   */
  float get_weight(uint32_t a, uint32_t b) const noexcept {
    return _matrix[a * _nodes_count + b];
  }

  /*
    add bidirectional edge
   */
  void add_link(uint32_t a, uint32_t b, float weight) noexcept {
    _matrix[a * _nodes_count + b] = weight;
    _matrix[b * _nodes_count + a] = weight;
  }

private:
  std::unique_ptr<float[]> _matrix;
  uint32_t _nodes_count;
};


/*
  Find all shortest pathes from the source vertex to all other vertexes
  using Dijkstra algorithm
  Stores all results in the _dist_to array, initializes with the +inf
  values, so when there is no path to the vertex, its value stays
  +inf.
  Iterates through edges from left to right, to make it more
  cache-efficient.
  There is no checks if the edge exists, because when there is no edge
  in the graph it returns +inf value, so it will just skip this vertex
  because +inf > +inf returns false.
 */
class DijkstraShortestPath {
public:
  DijkstraShortestPath(Graph &g, uint32_t source)
      : _dist_to(new float[g.nodes_count()]),
        _pq(g.nodes_count()) {

    std::fill_n(_dist_to.get(), g.nodes_count(), std::numeric_limits<float>::infinity());

    _dist_to[source] = 0.0;
    _pq.add(source, 0.0);

    while (!_pq.empty()) {
      uint32_t i = _pq.get_min();
      _pq.del_min();

      for (int j = 0; j < g.nodes_count(); ++j) {
        float weight = g.get_weight(i, j);
        if (_dist_to[j] > _dist_to[i] + weight) {
          _dist_to[j] = _dist_to[i] + weight;
          if (_pq.contains(j)) {
            _pq.change(j, _dist_to[j]);
          } else {
            _pq.add(j, _dist_to[j]);
          }
        }
      }
    }
  }

  /*
    returns distance to the target vertex
   */
  float distance_to(uint32_t target) const noexcept {
    return _dist_to[target];
  }

  /*
    check if path exist
   */
  bool has_path_to(uint32_t target) const noexcept {
    return !(_dist_to[target] == std::numeric_limits<float>::infinity());
  }

private:
  std::unique_ptr<float[]> _dist_to;
  IndexedMinPriorityQueue<float> _pq;
};

/*
  Generate random graphs according to the Erdos-Renyi model: G(n,p)
  http://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model
 */
class GraphGenerator {
public:
  GraphGenerator(std::mt19937 &rng,
                 size_t nodes_count,
                 float density,
                 float max_distance)
      : _rng(rng),
        _nodes_count(nodes_count),
        _density(density),
        _max_distance(max_distance) {}

  void generate(Graph &g) {
    std::uniform_real_distribution<float> uniform_distance(1.0f, _max_distance);
    std::uniform_real_distribution<float> uniform_density;

    for (int i = 0; i < _nodes_count; ++i) {
      for (int j = i + 1; j < _nodes_count; ++j) {
        if (uniform_density(_rng) < _density) {
          float dist = uniform_distance(_rng);

          g.add_link(i, j, dist);
        }
      }
    }
  }

private:
  std::mt19937 &_rng;
  size_t _nodes_count;
  float _density;
  float _max_distance;
};

/*
  Monte carlo simulation
 */
float run_monte_carlo_simulation(std::mt19937 &rng,
                                 uint32_t iterations,
                                 uint32_t nodes_count,
                                 float density,
                                 float max_distance) {

  GraphGenerator gg(rng, nodes_count, density, max_distance);
  float result = 0.0;

  Graph graph(nodes_count);
  uint32_t iter = 0;

  while (iter < iterations) {
    graph.reset();
    gg.generate(graph);

    DijkstraShortestPath path = DijkstraShortestPath(graph, 0);

    float accum = 0.0;
    int pathes = 0;
    for (int i = 1; i < nodes_count; ++i) {
      if (path.has_path_to(i)) {
        accum += path.distance_to(i);
        pathes++;
      }
    }
    if (pathes > 0) {
      accum /= pathes;
      result += accum;
      iter++;
    }
  }

  return result / iterations;
}


int main() {
  // seed value for the random number generator
  uint32_t seed = 124446;

  // number of iterations in the monte carlo simulation
  uint32_t iterations = 100000;

  // random number generator (c++11)
  std::mt19937 rng(seed);

  std::cout << "0.2: " << run_monte_carlo_simulation(rng, iterations, 50, 0.2, 10.0) << std::endl;
  std::cout << "0.4: " << run_monte_carlo_simulation(rng, iterations, 50, 0.4, 10.0) << std::endl;

  return 0;
}
