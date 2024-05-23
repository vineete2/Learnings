//
// Created by vinee on 13.05.24.
//

#include "quickTour.h"
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include "spdlog/spdlog.h"


using namespace boost;

template < typename VertexDescriptor, typename VertexNameMap > void
print_vertex_name(VertexDescriptor v, VertexNameMap name_map)
{
    std::cout << get(name_map, v);
}

template < typename Graph, typename TransDelayMap, typename VertexNameMap > void
print_trans_delay(typename graph_traits < Graph >::edge_descriptor e,
                  const Graph & g, TransDelayMap delay_map,
                  VertexNameMap name_map)
{
    std::cout << "trans-delay(" << get(name_map, source(e, g)) << ","
              << get(name_map, target(e, g)) << ") = " << get(delay_map, e);
}

template < typename Graph, typename VertexNameMap > void
print_vertex_names(const Graph & g, VertexNameMap name_map)
{
    std::cout << "vertices(g) = { ";
    typedef typename graph_traits < Graph >::vertex_iterator iter_t;
    for (std::pair < iter_t, iter_t > p = vertices(g); p.first != p.second;
         ++p.first) {
        print_vertex_name(*p.first, name_map);
        std::cout << ' ';
    }
    std::cout << "}" << std::endl;
}

template < typename Graph, typename TransDelayMap, typename VertexNameMap > void
print_trans_delays(const Graph & g, TransDelayMap trans_delay_map,
                   VertexNameMap name_map)
{
    typename graph_traits < Graph >::edge_iterator first, last;
    for (boost::tie(first, last) = edges(g); first != last; ++first) {
        print_trans_delay(*first, g, trans_delay_map, name_map);
        std::cout << std::endl;
    }
}

template < typename Graph, typename VertexNameMap, typename TransDelayMap >
void build_router_network(Graph & g, VertexNameMap name_map,
                     TransDelayMap delay_map)
{
    typename graph_traits < Graph >::vertex_descriptor a, b, c, d, e;
    a = add_vertex(g);
    name_map[a] = 'a';
    b = add_vertex(g);
    name_map[b] = 'b';
    c = add_vertex(g);
    name_map[c] = 'c';
    d = add_vertex(g);
    name_map[d] = 'd';
    e = add_vertex(g);
    name_map[e] = 'e';

    typename graph_traits < Graph >::edge_descriptor ed;
    bool inserted;

    boost::tie(ed, inserted) = add_edge(a, b, g);
    delay_map[ed] = 1.2;
    boost::tie(ed, inserted) = add_edge(a, d, g);
    delay_map[ed] = 4.5;
    boost::tie(ed, inserted) = add_edge(b, d, g);
    delay_map[ed] = 1.8;
    boost::tie(ed, inserted) = add_edge(c, a, g);
    delay_map[ed] = 2.6;
    boost::tie(ed, inserted) = add_edge(c, e, g);
    delay_map[ed] = 5.2;
    boost::tie(ed, inserted) = add_edge(d, c, g);
    delay_map[ed] = 0.4;
    boost::tie(ed, inserted) = add_edge(d, e, g);
    delay_map[ed] = 3.3;

}


int
main()
{
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // Note that this does not change the current log level, it will only
    // remove (depending on SPDLOG_ACTIVE_LEVEL) the call on the release code.
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");
    typedef adjacency_list < listS, listS, directedS,
            property < vertex_name_t, char >,
            property < edge_weight_t, double > > graph_t;
    graph_t g;

    property_map < graph_t, vertex_name_t >::type name_map =
            get(vertex_name, g);
    property_map < graph_t, edge_weight_t >::type delay_map =
            get(edge_weight, g);

    build_router_network(g, name_map, delay_map);
    print_vertex_names(g, name_map);
    print_trans_delays(g, delay_map, name_map);
}