#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int USERS_NUM = 10000;

void cleanupsocket(int sock_fd) { close(sock_fd); }

int createsocket(struct sockaddr_in *target_addr) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Failed to create socket\n");
    return -1;
  }
  return sock;
}

void createtarget(struct sockaddr_in *target_addr, const char *addrstring) {
  memset(target_addr, 0, sizeof(*target_addr));
  target_addr->sin_family = AF_INET;
  target_addr->sin_port = htons(8080);
  if (inet_pton(AF_INET, (const char *)addrstring,
                (void *)&target_addr->sin_addr.s_addr) == 1) {
    printf("target addr string: %s\n", addrstring);
    printf("target addr bin: %u\n", target_addr->sin_addr.s_addr);
  } else {
    printf("failed to convert\n");
  }
}

// TODO: Add arg handler to parse user input for target
int main(int argc, char *argv[]) {
  const char target_addr[INET_ADDRSTRLEN] = "127.0.0.1";
  struct sockaddr_in server_addr;
  createtarget(&server_addr, (char *)&target_addr);
  for (int user = 0; user < USERS_NUM; ++user) {
    int sock = createsocket(&server_addr);
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
        cleanupsocket(sock);
      } else {
        printf("failed to receive any response\n");
        // TODO: could add retry logic here
        close(sock);
      }
    } else {
      perror("failed to connect to target");
      close(sock);
    }
  }

  return EXIT_SUCCESS;
}
