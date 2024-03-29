
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <set>
#include <queue>
using namespace std;
int const INF = numeric_limits<int>::max();

class Node;

/**
 * @brief Edge between two nodes. Only a one way edge with a set cost.
 */
struct Edge {
    // Constructor
    Edge(Node* const node, const int cost) : connection_node(node), edge_cost(cost) {}
    // Member variables
    Node* connection_node;
    int edge_cost;
};

/**
 * @brief Node class containing all necessary information for most basic search algorithms.
 */
class Node {
public:
    /**
     * @brief Construct a new Node object.
     * 
     * @param in_index Unique index of node as it is in the graph.
     * @param in_value Initial (start) value of node.
     */
    Node(const int in_index, const int in_value) : index(in_index), value(in_value), visited(false) {}
    
    /**
     * @brief Destroy the Node object.
     */
    ~Node(){
        for(auto edge : edges){
            delete edge;
        }
    }
    // Getters
    // ================================
    int get_index() const {
        return index;
    }

    int get_value() const {
        return value;
    }

    bool is_visited() const {
        return visited;
    }

    vector<Edge*> get_edges() const {
        return edges;
    }

    Edge* get_edge(int const edge_index) const {
        return edges[edge_index];
    }

    Node* get_prev_node() {
        return prev_node;
    }
    // Setters
    // ================================
    void set_value(int const new_value) {
        value = new_value;
    }

    void set_visited(bool const new_visited) {
        visited = new_visited;
    }

    void add_edge(Edge* new_edge) {
        edges.push_back(new_edge);
    }
    
    void set_prev_node(Node* new_pre) {
        prev_node = new_pre;
    }
    void set_no_good(bool new_no_good) {
        no_good = new_no_good;
    }

    bool is_no_good() {
        return no_good;
    }

private:
    // Member variables
    const int index;
    int value;
    bool visited;
    Node* prev_node;
    vector<Edge*> edges;
    bool no_good;
};

/**
 * @brief Class containing everything needed for a graph.
 */
class Graph {
public:
    /**
     * @brief Construct a new Graph object
     * 
     * @param num_nodes Number of nodes to be initialized in the graph.
     * @param init_value Int of the initial values for all nodes.
     * @param start_index Index of starting node.
     */
    Graph(int const num_nodes, int const init_value, int const start_index) : start_index(start_index) {
        for(int i = 0; i < num_nodes; i++) {
            Node* new_node = new Node{i, init_value};
            nodes.push_back(new_node);
            new_node->set_prev_node(nullptr);
        }
    }

    /**
     * @brief Destroy the Graph object
     */
    ~Graph() {
        for(Node* node : nodes) {
            delete node;
        }
    }

    Node* get_node(int const index) const {
        return nodes[index];
    }

    vector<Node*> get_nodes() {
        return nodes;
    }

    int get_start_index() {
        return start_index;
    }

    /**
     * @brief Get path from start node to given end node if it exists. 
     * 
     * @param end_node_index Index of node to get path to.
     * @return vector<Node*> with all visited nodes to the end node, (empty if not reached or error).
     */
    vector<Node*> get_path(int end_node_index) {
        vector<Node*> return_path;

        // Given index is out of range (no node of that index) -> return no path
        if(end_node_index >= get_nodes().size()) {
            return return_path;
        }
        
        Node* end_node = get_node(end_node_index);
        Node* start_node = get_node(get_start_index());
        Node* current_prev = end_node->get_prev_node();

        // Only add end node if it has been visited (has a path to start)
        if(end_node->is_visited()) {
            return_path.push_back(end_node);
        }
        
        // Follow path back to start
        while(current_prev != nullptr) {
            return_path.push_back(current_prev);
            current_prev = current_prev->get_prev_node();
        }
        // Reverse to have list in right order: start node -> end node.
        reverse(return_path.begin(), return_path.end());
        return return_path;
    }

    /**
     * @brief Add new connection between two nodes with a given weight cost.
     *
     * @param node1 Index of first node to be connected.
     * @param node2 Index of second node to be connected.
     * @param cost Path cost for the connection (edge).
     */
    void add_one_way_edge(int const node1, int const node2, int const cost) {
        // Get nodes to be connected
        Node* primary_node = nodes[node1];
        Node* secundary_node = nodes[node2];

        // Make new edge
        Edge* edge = new Edge{secundary_node, cost};

        // Add new edges to the nodes
        primary_node->add_edge(edge);
    }

    /**
     * @brief Reset all nodes to standard values and resets starting node to provided index.
     */
    void graph_reset(int new_start_index, int init_value) {
        for(Node* node : nodes) {
            node->set_prev_node(nullptr);
            node->set_value(init_value);
            node->set_visited(false);
        }
        start_index = new_start_index;
    }

private:
    vector<Node*> nodes;
    int start_index;
};

/**
 * @brief Find shortest path from given start node to all other nodes using dijkstras algorithm. 
 * 
 * @param graph Graph with all nodes and edges included. 
 * @param start_node_index Index of staring node.
 */
void dijkstra(Graph& graph, int start_node_index) {
    // Reset graph to be sure it's a clean search
    graph.graph_reset(start_node_index, INF);

    Node* start_node = graph.get_node(start_node_index);
    start_node->set_value(0);

    set<pair<int, Node*>> prio_queue;
    prio_queue.insert({start_node->get_value(), start_node});

    pair<int, Node*> curr_cost_node;
    while(!prio_queue.empty()) {
        curr_cost_node = *prio_queue.begin();
        prio_queue.erase(curr_cost_node);

        Node* curr_node = curr_cost_node.second;

        // Already visited this node.
        if(curr_node->is_visited()) {
            continue;
        } else {
            curr_node->set_visited(true);
        }

        // Go through all edges to update neighbour nodes.
        for(Edge* edge : curr_node->get_edges()) {
            Node* neighbour_node = edge->connection_node; 
            int travel_cost = edge->edge_cost;

            if(neighbour_node->is_visited()) {
                continue;
            }

            // Check if it is worth to go this new path
            int neighbour_value = neighbour_node->get_value();
            int upd_cost = curr_node->get_value() + travel_cost;
            if(upd_cost < neighbour_value) {
                neighbour_node->set_value(upd_cost);
                neighbour_node->set_prev_node(curr_node);
                prio_queue.insert({upd_cost, neighbour_node});
            }
        }
    }
}

/**
 * @brief Main function that takes inputs and outputs to the consol.
 * Finds the shortest (lowest cost) path to a given node in a given graph.
 *
 * @return int
 */
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    std::cout.tie(NULL);

    int num_nodes, num_edges;
    while((cin >> num_nodes) && num_nodes!=0) {
        cin >> num_edges;
        // Make graph and connect edges
        Graph graph = Graph(num_nodes, INF, 1);
        int node1, node2, weight;
        for(int i = 0; i < num_edges; i++) {
            cin >> node1 >> node2 >> weight;
            graph.add_one_way_edge(node1-1, node2-1, weight);
            //graph.add_one_way_edge(node2-1, node1-1, weight);
        }
        // cout << "came here" << endl;
        dijkstra(graph, 1);
        // cout << "done with floyd" << endl;
        // Now we have all the shortest paths
        int total_diff_paths = 0;
        queue<int> q;
        q.push(0);
        while(!q.empty()) {
            int current_node = q.front();
            q.pop();
            if(current_node == 1) {
                total_diff_paths++;
                continue;
            }
            Node* node_p = graph.get_node(current_node);
            if(node_p->is_no_good()) {
                continue;
            }

            int dist_to_home = node_p->get_value();
            for(Edge* edge : node_p->get_edges()) {
                Node* node_n = edge->connection_node;
                if(node_n->get_index() == 1) {
                    q.push(1);
                }
                else if(node_n->get_value() <= dist_to_home) {
                    q.push(node_n->get_index());
                } else {
                    node_n->set_no_good(true);
                }
            }

            //int dist_to_home = node_p->get_distances()[1];
            //int dist_to_home = grid[current_node][1];
        //     for(int neighbour : node_p->get_neighbours()) {
        //         Node* node_n = graph.get_node(neighbour);
        //         if(neighbour == 1) {
        //             q.push(1);
        //         }
        //         else if(grid[neighbour][1] <= dist_to_home) {
        //             q.push(neighbour);
        //         } else {
        //             node_n->set_no_good(true);
        //         }
        //     }
        // }
        }
        cout << total_diff_paths << "\n";
    }
}