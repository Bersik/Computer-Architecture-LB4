//
// Created by bersik on 21.06.15.
//


#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../headers/serialization.hpp"
#include "task.hpp"

#define PORT 5454

using boost::asio::ip::tcp;
using namespace std;

long client_number = 0;

class tcp_connection
    // Using shared_ptr and enable_shared_from_this
    // because we want to keep the tcp_connection object alive
    // as long as there is an operation that refers to it.
        : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    // Call boost::asio::async_write() to serve the data to the client.
    // We are using boost::asio::async_write(),
    // rather than ip::tcp::socket::async_write_some(),
    // to ensure that the entire block of data is sent.

    void start()
    {
        client_number++;
        cout << "Received a connection." << endl;

        char buffer[BUF_SIZE];
        boost::system::error_code error;

        size_t len = socket_.read_some(boost::asio::buffer(buffer), error);

        ser_parameters *ser_par = deserialize(buffer);

        std::vector<vertex_descriptor> p(num_vertices(ser_par->g));
        std::vector<int> d(num_vertices(ser_par->g));

        dijkstra(ser_par->g,ser_par->s,p,d);


        boost::asio::async_write(socket_, boost::asio::buffer(my_serialize2(d)),
                                 boost::bind(&tcp_connection::handle_write,
                                             shared_from_this(), boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        delete ser_par;
        cout << "Client #" <<client_number << ". Work with this client is finished." << endl << endl;
    }


private:
    tcp_connection(boost::asio::io_service& io_service) : socket_(io_service)
    {
    }

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/)
    {
    }

    tcp::socket socket_;
};

class tcp_server
{
public:

    tcp_server(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), PORT))
    {

        start_accept();
    }

private:
    void start_accept()
    {
        cout << "Starting accept." << endl;

        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());


        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    tcp::acceptor acceptor_;
};

int main()
{
    cout << "Server type: asio." << endl;
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}