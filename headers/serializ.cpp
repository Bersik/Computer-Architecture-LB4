//
// Created by root on 21.06.15.
//

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include "types.hpp"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <string>

using namespace std;
using namespace boost::archive;


string asd(){

    int num_nodes = 100;
    unsigned long num_vert = 20;

    //create graph
    boost::random::uniform_int_distribution<> rand_weight(1, 10);
    boost::random::uniform_int_distribution<> rand_edge(0, int(num_vert));
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



    int myints[] = {16,2,77,29};
    std::vector<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );
    int myints2[] = {1,2,7,2};
    std::vector<int> fifth2 (myints2, myints2 + sizeof(myints2) / sizeof(int) );


    stringstream ss_send;
    text_oarchive oa{ss_send};
    oa << fifth;
    oa << fifth2;
    oa << g;
    oa << p;
    oa << d;
    return ss_send.str() + '\n';
}

void qwe(string &s){

    stringstream ss;
    ss.str(s);
    boost::archive::text_iarchive ia(ss);

    std::vector<int> asd,fifth2;
    graph_t g;
    std::vector<vertex_descriptor> p;
    std::vector<int> d;
    ia >> asd;
    ia >> fifth2;

    ia >> g;
    ia >> p;
    ia >> d;
    for (std::vector<int>::iterator it = asd.begin(); it != asd.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
    for (std::vector<int>::iterator it = fifth2.begin(); it != fifth2.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
    cout <<"OK2" << endl;
}


int main(int argc, char **argv) {
    string s = asd();
    qwe(s);

}