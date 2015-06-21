//
// Created by root on 20.06.15.
//

#ifndef LB4_SERIALIZATION_H
#define LB4_SERIALIZATION_H

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include "types.hpp"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#define BUF_SIZE 650000

using namespace boost::archive;
using namespace std;


struct ser_parameters{
    graph_t g;
    vertex_descriptor s;
    ser_parameters(graph_t &g,vertex_descriptor &s){
        this->g = g;
        this->s = s;
    }
};

string my_serialize(graph_t &g,vertex_descriptor &s) {
    stringstream ss_send;
    text_oarchive oa{ss_send};
    oa << g;
    oa << s;
    return ss_send.str() + '\n';;
}

ser_parameters* deserialize(const string &message) {
    stringstream ss;
    ss.str(message);
    boost::archive::text_iarchive ia(ss);

    graph_t g;
    vertex_descriptor s;
    ia >> g;
    ia >> s;

    return new ser_parameters(g,s);
}


string my_serialize2(std::vector<int> &d) {
    stringstream ss_send;
    text_oarchive oa{ss_send};
    oa << d;
    return ss_send.str()+'\n';
}

std::vector<int> deserialize2(const string &message) {
    stringstream ss;
    ss.str(message);
    boost::archive::text_iarchive ia(ss);

    std::vector<int> d;
    ia >> d;

    return d;
}

#endif //LB4_SERIALIZATION_H