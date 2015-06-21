//
// Created by root on 20.06.15.
//

#ifndef LB4_TYPES_H
#define LB4_TYPES_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/config.hpp>

using namespace boost;

  typedef adjacency_list < listS, vecS, directedS,
    no_property, property < edge_weight_t, int > > graph_t;
  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef std::pair<int, int> Edge;

  typedef boost::adjacency_list<> Graph;
  typedef boost::erdos_renyi_iterator<boost::minstd_rand, Graph> SWGen;


#endif //LB4_TYPES_H
