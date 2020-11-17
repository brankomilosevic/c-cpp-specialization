#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <limits>

class Node {
    public:
        Node() { clear_data(); }

        typedef unsigned int number_of_nodes_t;
        typedef double distance_t;
        typedef std::pair<number_of_nodes_t, distance_t> edge_t;
        typedef std::list<edge_t> neighbors_list_t;

        static const distance_t len_infinity;
        static const number_of_nodes_t maxvector;

        distance_t get_distance() const { return distance_from_source; }
        void set_distance(distance_t distance_) { distance_from_source = distance_; }

        Node* get_previous() const { return previous; }
        void set_previous(Node* previous_) { previous = previous_; }

        bool get_marked() const {return marked; }
        void mark() { marked = true; }
        void unmark() { marked = false; }

        void clear_data() {
            distance_from_source = Node::len_infinity;
            previous = nullptr;
            marked = false;
        }

        const neighbors_list_t& get_neighbors() const { return neighbors; }

        void add_neighbor(number_of_nodes_t node, distance_t distance = 0.0) {
            neighbors.push_back(std::make_pair(node, distance));
        }

        distance_t get_neighbor(number_of_nodes_t node) {
            for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
                if (std::get<0>(*it) == node) {
                    return std::get<1>(*it);
                }
            }
            return len_infinity;
        }

        void remove_neighbor(number_of_nodes_t node) {
            for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
                if (std::get<0>(*it) == node) {
                    neighbors.erase(it);
                    break;
                }
            }
        }

    private:
        neighbors_list_t neighbors;
        distance_t distance_from_source;
        Node* previous;
        bool marked;
};

const Node::distance_t Node::len_infinity = std::numeric_limits<double>::infinity();
const Node::number_of_nodes_t Node::maxvector = std::numeric_limits<unsigned int>::max();

class Graph {
    public:
        Graph(Node::number_of_nodes_t number_of_nodes) {
            vertices.resize(number_of_nodes);
        }

        bool has_edge(Node::number_of_nodes_t a, Node::number_of_nodes_t b) {
            if (vertices[a].get_neighbor(b) != Node::len_infinity) { return true; }
            else { return false; }
        }

        void create_edge(Node::number_of_nodes_t a, Node::number_of_nodes_t b, Node::distance_t distance = 0.0) {
            vertices[a].add_neighbor(b, distance);
        }

        void clear_data() {
            for (Node& v : vertices) {
                v.clear_data();
            }
        }

        Node& get_node(Node::number_of_nodes_t number_of_nodes) {
            return vertices[number_of_nodes];
        }

    private:
        std::vector<Node> vertices;
};

template <class T>
struct GreaterThanComparer {
    bool operator()(const T& s1, const T& s2) {
        return s1 > s2;
    }
};

template <class T>
class PriorityQueue {
    public:
        void push(T elem) {
            heap.push_back(elem);
            std::push_heap(heap.begin(), heap.end(), GreaterThanComparer<T>());
        }

        bool empty() const {
            return heap.empty();
        }

        T min() const {
            return heap.front();
        }

        void pop() {
            std::pop_heap(heap.begin(), heap.end(), GreaterThanComparer<T>());
            heap.pop_back();
        }
    private:
        std::vector<T> heap;
};

class ShortestPath {
    public:
        ShortestPath(Graph& graph_, Node::number_of_nodes_t start_, Node::number_of_nodes_t end_) : graph(graph_), start(start_), end(end_) {
            length_in_nodes = 0;
        }
        Node::distance_t path_length() {
            graph.clear_data();
            PriorityQueue<pathlen_t> pq;

            Node& source = graph.get_node(start);
            source.set_distance(0);
            pq.push(std::make_pair(0, start));
            Node* target_node = &(graph.get_node(end));

            while(!pq.empty()) {
                pathlen_t closest_node_pathlen = pq.min();
                pq.pop();
                Node& node = graph.get_node(std::get<1>(closest_node_pathlen));
                node.mark();

                if (&node == target_node) {
                    length_in_nodes = 0;
                    Node* p = &node;
                    while (p = p->get_previous()) {
                        length_in_nodes++;
                    }
                    return node.get_distance();
                }

                for (Node::edge_t edge : node.get_neighbors()) {
                    Node::number_of_nodes_t neighbor_no = std::get<0>(edge);
                    Node& neighbor = graph.get_node(neighbor_no);
                    Node::distance_t dist = node.get_distance() + std::get<1>(edge);
                    if (dist < neighbor.get_distance() && !neighbor.get_marked()) {
                        neighbor.set_distance(dist);
                        neighbor.set_previous(&node);
                        pq.push(std::make_pair(dist, neighbor_no));
                    }
                }
            }

            return Node::len_infinity;
        }

        Node::number_of_nodes_t get_length_in_nodes() { return length_in_nodes; }

    private:
        Graph& graph;
        Node::number_of_nodes_t start, end;
        Node::number_of_nodes_t length_in_nodes;

        typedef std::pair<Node::distance_t, Node::number_of_nodes_t> pathlen_t;
};

class MonteCarloSimulation {
    public:
        MonteCarloSimulation(int number_of_nodes_, double density_, double min_distance_, double max_distance_) :
            number_of_nodes(number_of_nodes_), density(density_),
            min_distance(min_distance_),
            max_distance(max_distance_) { }

        void RunSimulation() {
            Graph g(number_of_nodes);
            fill_random_edges(g);

            Node::distance_t total_path_length = 0.0;
            int number_of_paths = number_of_nodes - 1;
            Node::number_of_nodes_t total_length_in_nodes = 0;

            for (int i = 1; i < number_of_nodes; ++i) {
                ShortestPath path(g, 0, i);
                Node::distance_t length = path.path_length();
                if (length != Node::len_infinity) {
                    total_path_length += length;
                    total_length_in_nodes += path.get_length_in_nodes();
                } else {
                    number_of_paths -= 1;
                }
            }

            if (number_of_paths == 0) {
                shortest_path_in_nodes = 0;
                shortest_path_in_distance = 0;
            } else {
                shortest_path_in_distance = total_path_length / number_of_paths;
                shortest_path_in_nodes = total_length_in_nodes / number_of_paths;
            }
        }

        double get_shortest_path_in_distance() {
            return shortest_path_in_distance;
        }

        int get_shortest_path_in_nodes() {
            return shortest_path_in_nodes;
        }

    private:
        int number_of_nodes;
        double density;
        double min_distance, max_distance;
        double shortest_path_in_distance;
        int shortest_path_in_nodes;

        void fill_random_edges(Graph& g) {
            std::random_device randomdevice;
            std::mt19937 generator(randomdevice());
            std::uniform_real_distribution<> distance_distribution(min_distance, max_distance);
            std::uniform_int_distribution<> node_distribution(0, number_of_nodes-1);

            int edges_to_create = static_cast<int>(number_of_nodes * (number_of_nodes - 1) / 2 * density);
            while (edges_to_create > 0) {
                int a = node_distribution(generator);
                int b = node_distribution(generator);
                if (a == b || g.has_edge(a, b)) { continue; }
                auto distance = distance_distribution(generator);
                g.create_edge(a, b, distance);
                g.create_edge(b, a, distance);
                edges_to_create -= 1;
            }
        }
};

// Function that allows printing the simulation information and additionally calls the MonteCarloSimulation class
// to calculate the Average path length in distance and nodes.
void run_simulation(int number_of_nodes, double density, double min_distance, double max_distance, int iterations) {
    MonteCarloSimulation mc_sim(number_of_nodes, density, min_distance, max_distance);
    std::cout << "*************************************" << std::endl;
    std::cout << "BASE DATA FOR SIMULATION ************" << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << "Nodes: " << number_of_nodes << std::endl;
    std::cout << "Density: " << density << std::endl;
    std::cout << "Range: " << min_distance << " to " << max_distance << std::endl;
    std::cout << "Iterations: " << iterations << std::endl;

    double total_length = 0.0;
    int total_length_in_nodes = 0;
    int numtries = iterations;

    for (int i=0; i<iterations; ++i) {
        mc_sim.RunSimulation();
        if (mc_sim.get_shortest_path_in_distance() > 0) {
            total_length += mc_sim.get_shortest_path_in_distance();
            total_length_in_nodes += mc_sim.get_shortest_path_in_nodes();
        } else {
            numtries--;
        }
    }

    std::cout << "Average path length (distance): " << total_length / numtries << std::endl;
    std::cout << "Average path length (in nodes): " << static_cast<double>(total_length_in_nodes) / numtries << std::endl;
    std::cout << std::endl;
}

void print_explanation() {
    std::string explanation = "The algorithm programmed in C ++ as a solution to the subject of week 3 is Dijkstra's Algorithm. This algorithm is used to find the shortest routes between nodes in a graph, which can represent, for example, road networks. The algorithm creates a tree of shorter paths from the initial node to all other points on the graph. Lets will talk about the program: The program will take the number of 50 nods, a graph density (%20 and %40) with a distance range of 1.0 to 10.0. It will randomly generate an adjacency matrix for the graph of the size and density the user has specified. Then it will generate the shortest paths from the selected source node to all other nodes with the respective cost for that path.";
    std::cout << "*************************************" << std::endl;
    std::cout << "Solution explanation ****************" << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << explanation << std::endl;
}

// The main function allows running a simulation for each one of the list of values specified in the problem statement
// Additionally, an explanation of the algorithm used is printed.
int main(int argc, char **argv) {
    run_simulation(50, 0.2, 1.0, 10.0, 10000);
    run_simulation(50, 0.4, 1.0, 10.0, 10000);
    print_explanation();
    return 0;
}
