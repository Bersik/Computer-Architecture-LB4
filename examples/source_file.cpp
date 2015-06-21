
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"

#include "../headers/serialization.hpp"
#include "../headers/types.hpp"

using namespace boost;

int main(int, char *[])
{
  const int num_nodes = 100;
  unsigned long num_vert = 20;

  boost::random::uniform_int_distribution<> rand_weight(1, 10);
  boost::random::uniform_int_distribution<> rand_edge(0, num_vert);
  boost::random::minstd_rand gen_num;

  Edge edge_array[num_nodes];
  int weights[num_nodes];
  for(int i=0;i<num_nodes;i++){
    edge_array[i] = Edge(rand_edge(gen_num), rand_edge(gen_num));
    weights[i] = rand_weight(gen_num);
  }
  int num_arcs = sizeof(edge_array) / sizeof(Edge);

  graph_t g(edge_array, edge_array + num_arcs, weights, num_vert);

  std::vector<vertex_descriptor> p(num_vertices(g));
  std::vector<int> d(num_vertices(g));
  vertex_descriptor s = vertex(2, g);

  dijkstra_shortest_paths(g, s,
                          predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

  std::cout << "distances and parents:" << std::endl;
  graph_traits < graph_t >::vertex_iterator vi, vend;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distance(" << (*vi) << ") = " << d[*vi] << ", ";
    std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
      endl;
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}