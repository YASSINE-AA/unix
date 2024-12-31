#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        exit(EXIT_FAILURE);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buffer[] = "test";
    sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    close(sock);
    return 0;
}