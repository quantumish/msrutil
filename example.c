#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define BUFSIZE 2048
#define PORT 5001

int main() {
  int s;
  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("\n Error : Socket Failed \n");
  }
  struct sockaddr_in addr;
  memset((char *)&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET; // Specify address family.
  addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY just 0.0.0.0, machine IP address
  addr.sin_port = htons(PORT); // Specify port.

  // Connect to server
  if(connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("\n Error : Connect Failed \n");
  }
  // NOTE This can and will not work if flag argument set to 1
  int recvlen;
  socklen_t len = sizeof(addr);

  char confirmation[BUFSIZE];
  while (1==1) {
    sendto(s, "MSR A", BUFSIZE, 0, (struct sockaddr *)NULL, sizeof(addr));
    char buf[BUFSIZE];
    recvlen = recvfrom(s, confirmation, BUFSIZE, 0, (struct sockaddr *)&addr, &len);
    if (recvlen > 0) {
      printf("Received %d-byte message from server: \"%s\"\n", recvlen, buf);
    }
  }
}
