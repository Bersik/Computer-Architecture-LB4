//
// Created by bersik on 17.06.15.
//

#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <cstdlib>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <arpa/inet.h>	/* for inet_ntoa */
#include <pthread.h>
#include "../headers/types.hpp"
#include "../headers/serialization.hpp"
#include "task.hpp"

#define PORT 5454


using namespace boost::archive;
using namespace std;

//1 fork
#define SERVER_FORK 1
//another pthread
#define SERVER_PTHREAD 2
#define DEFAULT_SERVER_TYPE SERVER_PTHREAD

struct Parameters{
    int rqst;
    long id;
    Parameters(int rqst,long id){
        this->id = id;
        this->rqst = rqst;
    }
};

void do_work(int rqst, long client_number) {
    char buffer[BUF_SIZE];

    ssize_t n = recv(rqst, buffer, BUF_SIZE, 0);
    if (n == -1) {
        cerr << "ERROR reading from socket." << endl;
        exit(1);
    }

    ser_parameters *ser_par = deserialize(buffer);
    graph_t g = ser_par->g;
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));

    dijkstra(g,ser_par->s,p,d);

    string res = my_serialize2(d);

    delete ser_par;

    write(rqst, res.c_str(), res.length());

    close(rqst);
    cout << "Client #" <<client_number << ". Work with this client is finished." << endl << endl;
}

void *thread_function(void *sock) {
    Parameters* par = (Parameters*)sock;
    do_work(par->rqst,par->id);
    delete par;
    return NULL;
}

int main(int argc, char **argv) {
    int server_type = 2;
    if (argc == 2) {
        if (!strcmp(argv[1], "fork"))
            server_type = SERVER_FORK;
    }
    cout << "Server type: " << (server_type == SERVER_FORK ? "fork" :"pthread") << "." << endl;

    uint16_t port = PORT;
    int rqst;
    socklen_t alen;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sockoptval = 1;

    int svc = socket(AF_INET, SOCK_STREAM, 0);
    if (svc < 0) {
        cerr << "Could not create socket." << endl;
        return EXIT_FAILURE;
    }
    cout << "Socket created." << endl;

    setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));


    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(svc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr <<  "Bind failed." << endl;
        return EXIT_FAILURE;
    }
    cout << "Bind done" << endl;

    if (listen(svc, 5) < 0) {
        cerr <<  "Listen failed." << endl;
        return EXIT_FAILURE;
    }
    cout << "Listening..." << endl;
    long client_number = 0;
    while (true) {
        while ((rqst = accept(svc,
                              (struct sockaddr *) &client_addr, &alen)) < 0) {
        }
        client_number = client_number + 1;

        cout << "Received a connection from: " << inet_ntoa(client_addr.sin_addr) <<
        "port " << ntohs(client_addr.sin_port) << "." << endl;

        if (server_type == SERVER_FORK) {
            if (fork() == 0 ) {
                do_work(rqst, client_number);
                return EXIT_SUCCESS;
            }else{
                close(rqst);
            }
        } else {
            pthread_t thread;
            Parameters* par = new Parameters(rqst,client_number);
            pthread_create(&thread, NULL, &thread_function, (void *)par);
        }
    }

}



