//
// Created by root on 20.06.15.
//

#ifndef LB4_TASK_HPP
#define LB4_TASK_HPP

#include <algorithm>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../headers/types.hpp"

using namespace std;
using namespace boost;

void  dijkstra(graph_t &g,vertex_descriptor s,std::vector<vertex_descriptor> &p, std::vector<int> &d) {
    dijkstra_shortest_paths(g, s,
                            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

}

#endif //LB4_TASK_HPP
