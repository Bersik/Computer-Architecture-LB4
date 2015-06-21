g++ client/client.cpp -o client/client -std=c++11 -lboost_system -lboost_serialization -pthread
g++ server/server.cpp -o server/server -std=c++11 -lboost_system -lboost_serialization -pthread
g++ server/server_asio.cpp -o server/server_asio -std=c++11 -lboost_system -lboost_serialization -pthread
g++ tests/test_server.cpp -o tests/test_server -lboost_unit_test_framework -std=c++11 -lboost_system -lboost_serialization -pthread -lpthread -lboost_program_options
tests/test_server