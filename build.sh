g++ -std=c++11 src/client.cpp -o client
g++ -std=c++11 src/server.cpp  src/ipip.c -o server -pthread
