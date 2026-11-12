#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int USERS_NUM = 10000;
int main(int argc, char *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Failed to create socket\n");
    return EXIT_FAILURE;
  }
  const char target_addr[INET_ADDRSTRLEN] = "192.168.1.16";
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  if (inet_pton(AF_INET, (const char *)&target_addr,
                (void *)&server_addr.sin_addr.s_addr) == 1) {
    printf("target addr string: %s\n", target_addr);
    printf("target addr bin: %u\n", server_addr.sin_addr.s_addr);
  } else {
    printf("failed to convert\n");
    return EXIT_FAILURE;
  }

  for (int user = 0; user < USERS_NUM; ++user) {
    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr)) == 0) {
      printf("connected::user::%d\n", user);
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
    } else {
      perror("failed to connect to target");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
