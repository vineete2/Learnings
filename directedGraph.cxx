#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>

struct VertexInfo
{
std::string vertex_name;
int num;
};

struct EdgeInfo
{
std::string edge_name;
double dist;
};

void directedExample()
{
//creating an adjacency list for directed 
typedef boost::adjacency_list<boost::vecS, boost::vecS,
                                boost::directedS,
                                boost::no_property,
                                boost::no_property
                                > graphDirected;

graphDirected D;
auto v1 = add_vertex(D);
auto v2 = add_vertex(D); 
auto e = add_edge(v1,v2,D);

auto vpair = vertices(D);
  for(auto iter=vpair.first; iter!=vpair.second; iter++) {
    std::cout << "vertex " << *iter << std::endl;
  }

  auto epair = edges(D);
  for(auto iter=epair.first; iter!=epair.second; iter++) {
    std::cout << "edge " << source(*iter, D) << " - " << target(*iter, D) << std::endl;
  }

}

int
main(int, char *[])
{
  directedExample();
}