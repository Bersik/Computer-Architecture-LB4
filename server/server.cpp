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


using namespace boost::archive;
using namespace std;

#define SERVER_FORK 1
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

    // Receiving data
    ssize_t n = recv(rqst, buffer, BUF_SIZE, 0);
    if (n == -1) {
        perror("ERROR reading from socket");
        exit(1);
    }

    ser_parameters *ser_par = deserialize(buffer);
    graph_t g = ser_par->g;
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));

    dijkstra(g,ser_par->s,p,d);

    // Sending data

    strcpy(buffer,my_serialize2(d).c_str());

    n = send(rqst, buffer, BUF_SIZE, 0);

    if (n == -1) {
        perror("ERROR writing to socket.");
        exit(1);
    }

    shutdown(rqst, 2);    /* close the connection */
    cout << "Client #" <<client_number << ". Work with this client is finished." << endl << endl;
}

void *thread_function(void *sock) {
    Parameters* par = (Parameters*)sock;
    do_work(par->rqst,par->id);
    return NULL;
}

int main(int argc, char **argv) {
    int server_type = DEFAULT_SERVER_TYPE;
    if (argc == 2) {
        if (!strcmp(argv[1], "pthread"))
            server_type = SERVER_PTHREAD;
        else if (!strcmp(argv[1], "fork"))
            server_type = SERVER_FORK;
    }
    else {
        if (DEFAULT_SERVER_TYPE == SERVER_FORK) cout << "Incorrect type of server. Selected by default: fork." << endl;
        else if (DEFAULT_SERVER_TYPE == SERVER_PTHREAD) cout << "Incorrect type of server. Selected by default: pthread." << endl;
        else {
            cerr << "Incorrect type of server. Selected by default: pthread." << endl;
            return EXIT_FAILURE;
        }
    }
    uint16_t port = 5559;    /* port number */
    int rqst;       /* socket accepting the request */
    socklen_t alen;       /* length of address structure */
    struct sockaddr_in server_addr;    /* address of this service */
    struct sockaddr_in client_addr;  /* client's address */
    int sockoptval = 1;


    /* create a TCP/IP socket */
    int svc = socket(AF_INET, SOCK_STREAM, 0);
    if (svc < 0) {
        cerr << "Could not create socket." << endl;
        return EXIT_FAILURE;
    }
    cout << "Socket created." << endl;
    /* allow immediate reuse of the port */
    setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

    /* bind the socket to our source address */
    memset((char *) &server_addr, 0, sizeof(server_addr));  /* 0 out the structure */
    server_addr.sin_family = AF_INET;   /* address family */
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(svc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr <<  "Bind failed." << endl;
        return EXIT_FAILURE;
    }
    cout << "Bind done" << endl;

    /* set the socket for listening (queue backlog of 5) */
    if (listen(svc, 5) < 0) {
        cerr <<  "Listen failed." << endl;
        return EXIT_FAILURE;
    }
    cout << "Listening." << endl;
    long client_number = 0;
    while (true) {
        while ((rqst = accept(svc,
                              (struct sockaddr *) &client_addr, &alen)) < 0) {
            /* we may break out of accept if the system call */
            /* was interrupted. In this case, loop back and */
            /* try again */
            if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
                cerr << "Accept failed" << endl;
                return EXIT_FAILURE;
            }
        }
        client_number = client_number + 1;

        printf("Received a connection from: %s port %d.\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (!strcmp(argv[1], "fork")) {
            if (fork() == 0 ) {
                do_work(rqst, client_number);
                return EXIT_SUCCESS;
            }
        } else if (!strcmp(argv[1], "pthread")) {
            pthread_t thread;
            Parameters* par = new Parameters(rqst,client_number);
            pthread_create(&thread, NULL, &thread_function, (void *)par);
        }
    }

}



