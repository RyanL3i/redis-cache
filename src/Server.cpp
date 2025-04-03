#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  // create TCP socket, using IPv4 (AF_INET) and TCP (SOCK_STREAM)
  // stores it as an int, file descriptor, identifies open socket in process
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  // if the socket returns a negative number, means failed
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return 1;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  // takes in socket we created, sol_socket -> socket level and not TCP, so_reuseaddr -> allows reuse of local addresses
  // pass in pointer address to option_value "&reuse", and passes in 4 (size of int) meaning "read 4 bytes"
  // if 
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  // struct sockaddr_in is the address structure, contains address information, has a few fields:
  // server_addr specifies where server_fd will be listening for incoming connections
    // short sin_family -> address family, AF_INET for IPv4
    // unsigned short sin_port -> port number, must be in network byte order which is big endian
    // struct in_addr sin_addr -> IP address in network byte order, where the struct in_addr has field s_addr, 32-bit that stores IPv4 addr
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  // takes IP connections from anywhere
  server_addr.sin_addr.s_addr = INADDR_ANY;
  // htons converts to network byte order
  server_addr.sin_port = htons(6379);
  
  // takes socket file descriptor, casts the address sockaddr type, and tells it how many bytes to read, check not equal to 0 (fails)
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 6379\n";
    return 1;
  }
  
  // connection backlog -> max # of connections in queue before system starts rejecting connections
  // being listening on server_fd, or the server socket
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  // hold information about the client's IP address and port, similar to what we did above
  // hold size of the client_addr, will tell accept() function how much space available to store client addr
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  std::cout << "Waiting for a client to connect...\n";

  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage
  // wait here until client attempts to connect to server
    // server_fd -> server socket that is waiting for incoming connections
    // &client_addr is address of client_addr, which willl be populated with address info of client that is connecting
    // client_addr_len is the size/length of client's address, to see if IPv4 or IPv6
      // need to convert to socklen_t type type  from integerwhat is
  accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  std::cout << "Client connected\n";
  close(server_fd);

  return 0;
}
