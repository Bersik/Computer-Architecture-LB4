//
// Created by bersik on 19.06.15.
//

#include "client.hpp"


using namespace std;
using namespace boost;
using namespace boost;

#define NUM_NODES 100
#define NUM_VERT 20

int get_rand_edge(int num_vert){
    return rand() % num_vert;
}

int get_rand_weight(){
    return rand() % 9 +1;
}


int main(int argc, char **argv) {
    srand(time(NULL));
    int num_nodes = NUM_NODES;
    unsigned long num_vert = NUM_VERT;
    int start_point = rand() % NUM_VERT;

    switch (argc){
        int a;
        case 4:
            if ((a = atoi(argv[3]))!=0){
                start_point = a;
            } else
                break;
        case 3:
            if ((a = atoi(argv[2]))!=0){
                num_vert = a;
            } else
                break;
        case 2:
            if ((a = atoi(argv[1]))!=0){
                num_nodes = a;
            }
            break;
        default:
            cout << "Incorrect parameters. Selected by default." << endl;
            break;
    }

    cout << "Count vertex=" << num_vert << ";" << endl;
    cout << "Count nodes=" << num_nodes << ";" << endl;
    cout << "Start vertex=" << start_point << "." << endl << endl;

    const char *host = "localhost";
    uint16_t port = 5559;
    int svc;
    string data;
    char buffer[BUF_SIZE];
    struct hostent *hp;    /* host information */
    unsigned int alen;    /* address length when we get the port number */
    struct sockaddr_in client_addr;    /* our address */
    struct sockaddr_in server_addr;    /* server address */

    cout << "Coonect(host=\""<< host << "\", port=\"" << port << "\")." <<endl;

    /* get a tcp/ip socket */
    /* We do this as we did it for the server */
    /* request the Internet address protocol */
    /* and a reliable 2-way byte stream */
    svc = socket(AF_INET, SOCK_STREAM, 0);
    if (svc < 0) {
        cerr << "Could not create socket." << endl;
        return EXIT_FAILURE;
    }

    /* bind to an arbitrary return address */
    /* because this is the client side, we don't care about the */
    /* address since no application will connect here  --- */
    /* INADDR_ANY is the IP address and 0 is the socket */
    /* htonl converts a long integer (e.g. address) to a network */
    /* representation (agreed-upon byte ordering */

    memset((char *) &client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(0);

    if (bind(svc, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
        cerr <<  "Bind failed." << endl;
        return EXIT_FAILURE;
    }

    /* this part is for debugging only - get the port # that the operating */
    /* system allocated for us. */
    alen = sizeof(client_addr);
    if (getsockname(svc, (struct sockaddr *) &client_addr, &alen) < 0) {
        cerr <<  "Getsockname error." << endl;
        return EXIT_FAILURE;
    }
    cout << "Local port number = \"" << ntohs(client_addr.sin_port) << "\")." <<endl;

    /* fill in the server's address and data */
    /* htons() converts a short integer to a network representation */

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    /* look up the address of the server given its name */
    hp = gethostbyname(host);
    if (!hp) {
        cerr << "Could not obtain address of " << host << "."  << endl;
        return EXIT_FAILURE;
    }

    /* put the host's address into the server address structure */
    memcpy((void *) &server_addr.sin_addr, hp->h_addr_list[0], (size_t) hp->h_length);

    /* connect to server */
    if (connect(svc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connect failed." << endl;
        return EXIT_FAILURE;
    }


    Edge edge_array[num_nodes];
    int weights[num_nodes];
    for(int i=0;i<num_nodes;i++){
        edge_array[i] = Edge(get_rand_edge(num_vert), get_rand_edge(num_vert));
        weights[i] = get_rand_weight();
    }
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    graph_t g(edge_array, edge_array + num_arcs, weights, num_vert);
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    vertex_descriptor s = vertex(start_point, g);


    // Sending data
    data = my_serialize(g,s);
    ssize_t n = send(svc, data.c_str(),strlen(data.c_str()) , 0);
    if (n == -1) {
        cerr << "ERROR writing to socket." << endl;
        return EXIT_FAILURE;
    }

    // Receiving data
    n = recv(svc, buffer, BUF_SIZE, 0);
    if (n == -1) {
        cerr << "ERROR reading from socket." << endl;
        return EXIT_FAILURE;
    }
    string str(buffer);
    d = deserialize2(str);
    close(svc);

    std::cout << "Results: " << std::endl;
    graph_traits < graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        std::cout << "distance(" << (*vi) << ") = " << d[*vi] << endl;

    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}