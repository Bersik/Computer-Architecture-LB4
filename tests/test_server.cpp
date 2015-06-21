//
// Created by bersik on 21.06.15.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Task_tests

#include <boost/test/unit_test.hpp>
#include "../server/task.hpp"
#include "../headers/types.hpp"
#include "../headers/serialization.hpp"

struct Fixture
{
    graph_t g;
    std::vector<vertex_descriptor> p;
    std::vector<int> d;
    vertex_descriptor s;
    int num_nodes;
    Fixture()
    {
        num_nodes = 5;
        enum nodes { A, B, C, D, E };
        char name[] = "ABCDE";
        Edge edge_array[] = { Edge(0, 2), Edge(1, 1), Edge(1, 3), Edge(1, 4),
                              Edge(2, 1), Edge(2, 3), Edge(3, 4), Edge(4, 0), Edge(4, 1)
        };
        int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
        int num_arcs = sizeof(edge_array) / sizeof(Edge);
        g = graph_t(edge_array, edge_array + num_arcs, weights, (unsigned long)num_nodes);
        p = std::vector<vertex_descriptor>(num_vertices(g));
        d = std::vector<int>(num_vertices(g));
        s = vertex(2, g);
    }
};

BOOST_FIXTURE_TEST_CASE(test_dijkstra,Fixture) {
    int results[] = {5, 5, 0, 3, 4};

    dijkstra(g,s,p,d);

    for (int i=0; i< num_nodes;i++)
        BOOST_CHECK(d[i] == results[i]);
}

BOOST_FIXTURE_TEST_CASE(test_serialisation1,Fixture) {

    string str = my_serialize(g,s);
    ser_parameters *ser_par = deserialize(str);

    BOOST_CHECK_EQUAL(ser_par->s,s);
}

BOOST_FIXTURE_TEST_CASE(test_serialisation2,Fixture) {
    dijkstra(g,s,p,d);
    string str = my_serialize2(d);
    vector<int> vect = deserialize2(str);
    BOOST_CHECK_EQUAL_COLLECTIONS(d.begin(),d.end(),vect.begin(),vect.end());
}