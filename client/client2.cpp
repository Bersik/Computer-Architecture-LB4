//
// Created by root on 21.06.15.
//

#include "client2.h"
#include "client.hpp"
#include <boost/asio.hpp>
#include <fstream>

string serialize();

using boost::asio::ip::tcp;

tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    address = "";
}

/**
    Connect to a host on a certain port number
*/
bool tcp_client::conn(string address , int port)
{
    //create socket if it is not already created
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout<<"Socket created\n";
    }
    else    {   /* OK , nothing */  }

    //setup address structure
    if(inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";

            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];

            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;

            break;
        }
    }

        //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    cout<<"Connected\n";
    return true;
}

/**
    Send data to the connected host
*/
bool tcp_client::send_data(string data)
{
    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    return true;
}

/**
    Receive data from the connected host
*/
string tcp_client::receive(int size=512)
{
    char buffer[size];

    //Receive a reply from the server
    if( recv(sock, buffer, sizeof(buffer), 0) < 0)
    {
        puts("recv failed");
    }

    string reply = "";
    for(auto chr: buffer)
    {
        if(chr == '\n') break;
        reply += chr;
    }

    return reply;
}

int main(int argc , char *argv[])
{
    tcp_client c;
    string host = "localhost";

    int num_nodes = 100;
    unsigned long num_vert = 20;

    // Підключаємось
    c.conn(host , 5553);

    // Формуємо дані для відправки на сервер
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

    string outbound_data_ = my_serialize(g);

    // Відправляємо на сервер
    c.send_data(outbound_data_);

    // Приймаємо відповідь від сервера
    cout<<"----------------------------\n\n";
    d = deserialize2(c.receive(1024));

    std::cout << "distances and parents:" << std::endl;
    graph_traits < graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        std::cout << "distance(" << (*vi) << ") = " << d[*vi] << ", ";
        /* std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
         endl;*/
    }
    std::cout << std::endl;

    return 0;
}


