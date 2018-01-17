#include <netdb.h>
#include <netinet/in.h>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>


using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
   struct sockaddr_in serv_addr;
   int sockfd, portno, n;
   struct hostent *server;

   char buffer[256];

   if (argc < 3) {
      cerr << "usage " << argv[0] << " hostname port" << endl;
      exit(0);
   }

   portno = atoi(argv[2]);

   /* TCP Socket creation */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      cerr << "ERROR opening socket" << endl;
      exit(1);
   }

   server = gethostbyname(argv[1]);

   if (server == NULL) {
      cerr << "ERROR, no such host" << endl;
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   // Server connection
   if (connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   // We have a connection at this point, ask user for input

   cout << "Please enter your message: " << endl;
   bzero(buffer,256);
   fgets(buffer,255,stdin);

   // Send message to the server
   n = write(sockfd, buffer, strlen(buffer));

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }

   // read response
   bzero(buffer,256);
   n = read(sockfd, buffer, 255);

   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }

   // Read the server's response
   cout << buffer << endl;

   return 0;
}
