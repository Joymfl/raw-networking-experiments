#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Failed to create socket\n");
    return EXIT_FAILURE;
  }
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) ==
      0) {
    printf("connected\n");
    char buf[] = {1, 2, 3, 4};
    int res = send(sock, buf, sizeof(buf), 0);
    if (res != -1) {
      printf("bytes sent: %d\n", res);
    } else {
      printf("failed to send\n");
    }
    int recv_result = recv(sock, buf, sizeof(buf), 0);
    if (recv_result != -1) {
      printf("message confirmed\n");
      close(sock);
      exit(0);
    } else {
      printf("failed to receive any response\n");
    }
  }

  return EXIT_SUCCESS;
}
