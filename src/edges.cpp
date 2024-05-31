//
// Created by vinee on 27.05.24.
//
#include <iostream>
#include <vector>
#include <utility>  // For std::pair
#include <cmath>    // For std::abs and std::sqrt
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

// Define the vertex property to store 3D coordinates
struct VertexProperty {
    double x, y, z;
};

// Define the edge property to store the cost
struct EdgeProperty {
    double cost;
};

// Define the graph using adjacency_list, specifying vertex and edge properties
typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::directedS,
        VertexProperty, EdgeProperty
> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// Function to calculate edge cost based on elevation difference
double calculate_cost(double z1, double z2, double uphill_factor, double downhill_factor) {
    if (z2 > z1) {
        return (z2 - z1) * uphill_factor;
    } else {
        return (z1 - z2) * downhill_factor;
    }
}

// Heuristic function for A* algorithm (Euclidean distance)
class Heuristic : public boost::astar_heuristic<Graph, double> {
public:
    Heuristic(Vertex goal, const Graph& graph) : m_goal(goal), m_graph(graph) {}

    double operator()(Vertex u) {
        const auto& goal_coords = m_graph[m_goal];
        const auto& u_coords = m_graph[u];
        return std::sqrt(
                std::pow(goal_coords.x - u_coords.x, 2) +
                std::pow(goal_coords.y - u_coords.y, 2) +
                std::pow(goal_coords.z - u_coords.z, 2)
        );
    }

private:
    Vertex m_goal;
    const Graph& m_graph;
};

// Exception to throw when the goal is found
struct found_goal {};

// Visitor to terminate the search when the goal is found
class AStarGoalVisitor : public boost::default_astar_visitor {
public:
    AStarGoalVisitor(Vertex goal) : m_goal(goal) {}

    template <class Graph>
    void examine_vertex(Vertex u, const Graph& g) {
        if(u == m_goal) {
            throw found_goal();
        }
    }

private:
    Vertex m_goal;
};

int main() {
    // Factors for uphill and downhill costs
    const double uphill_factor = 2.0;
    const double downhill_factor = 1.0;

    // Create a graph object
    Graph g;

    // Add vertices with 3D coordinates
    Vertex v1 = boost::add_vertex({0.0, 0.0, 0.0}, g);
    Vertex v2 = boost::add_vertex({1.0, 1.0, 1.0}, g);
    Vertex v3 = boost::add_vertex({2.0, 0.0, 0.5}, g);

    // Add edges with costs
    Edge e1; bool inserted1;
    boost::tie(e1, inserted1) = boost::add_edge(v1, v2, g);
    g[e1].cost = calculate_cost(g[v1].z, g[v2].z, uphill_factor, downhill_factor);

    Edge e2; bool inserted2;
    boost::tie(e2, inserted2) = boost::add_edge(v2, v3, g);
    g[e2].cost = calculate_cost(g[v2].z, g[v3].z, uphill_factor, downhill_factor);

    Edge e3; bool inserted3;
    boost::tie(e3, inserted3) = boost::add_edge(v3, v1, g);
    g[e3].cost = calculate_cost(g[v3].z, g[v1].z, uphill_factor, downhill_factor);

    // Print the edges and their costs
    std::cout << "Edges and their costs:" << std::endl;
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
        std::cout << boost::source(*ei, g) << " -> " << boost::target(*ei, g)
                  << " with cost " << g[*ei].cost << std::endl;
    }

    // Define the goal vertex
    Vertex goal = v3;

    // Use A* algorithm to find the shortest path from v1 to v3
    std::vector<Vertex> predecessors(boost::num_vertices(g));
    std::vector<double> distances(boost::num_vertices(g));

    try {
        boost::astar_search(g, v1,
                            Heuristic(goal, g),
                            boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index, g))).
                                    distance_map(boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index, g))).
                                    visitor(AStarGoalVisitor(goal))
        );
    } catch(found_goal&) {
        // Goal found
    }

    // Print the shortest path from v1 to v3
    std::cout << "Shortest path from v1 to v3:" << std::endl;
    for (Vertex v = goal; v != v1; v = predecessors[v]) {
        std::cout << v << " <- ";
    }
    std::cout << v1 << std::endl;

    return 0;
}
