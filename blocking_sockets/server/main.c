#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Failed to create socket\n");
    return EXIT_FAILURE;
  } else {
    printf("listen socket: %d\n", sock);
  }
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  int res = bind(sock, (struct sockaddr *)(&server_addr), 80);
  if (res == -1) {
    perror("failed to bind");
    close(sock);
    exit(EXIT_FAILURE);
  }
  socklen_t addr;

  // accept(sock, (struct sockaddr *)(&server_addr), &addr);
  printf("listening on port 8080\n");
  int result = listen(sock, 1);
  if (result == -1) {
    perror("failed to listen on socket\n");
    exit(EXIT_FAILURE);
  }
  for (;;) {
    printf("starting accepting\n");
    struct sockaddr_in connecting_addr;
    int new_sock = accept(sock, (struct sockaddr *)&connecting_addr, &addr);
    if (new_sock != -1) {
      char in_addr[INET_ADDRSTRLEN];
      printf("connection accepted to %s::%u\n",
             inet_ntop(AF_INET, &connecting_addr.sin_addr, in_addr,
                       INET_ADDRSTRLEN),
             connecting_addr.sin_port);

      printf("receiving on: %d \n", new_sock);
      char recv_buf[4];
      int result = recv(new_sock, recv_buf, 4, MSG_WAITALL);
      if (result != -1) {
        printf("data received: \n");
        for (int i = 0; i < sizeof(recv_buf) / sizeof(recv_buf[0]); i++) {
          printf("byte: %d\n", recv_buf[i]);
        }
        char ack_msg[] = "ACK";
        if (send(new_sock, &ack_msg, sizeof(ack_msg), 0) != -1) {
          printf("ack sent\n");
        }
        close(new_sock);
      } else {
        perror("Error receiving data");
        close(new_sock);
        exit(EXIT_FAILURE);
      }
    } else {
      printf("failed to accept connection\n");
    }
  }

  return EXIT_SUCCESS;
}
