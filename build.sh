g++ -std=c++11 src/client.cpp -o client
g++ -std=c++11 src/server.cpp src/ip2region.c -o server -pthread
