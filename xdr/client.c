#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <rpc/xdr.h>
#include "test.h"


// gcc test_xdr.c client.c -o client -I/usr/include/ntirpc -lntirpc

int main()
{

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        exit(EXIT_FAILURE);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server_conn = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (server_conn < 0)
    {
        printf("Couldn't connect to server \n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    MyData data = {42, "Test Name", 3.14};
    XDR xdrs;
    xdrmem_create(&xdrs, buffer, sizeof(buffer), XDR_ENCODE);
    if (!xdr_mydata(&xdrs, &data))
    {
        fprintf(stderr, "Serialization failed\n");
        xdr_destroy(&xdrs);
        close(sock);
        exit(1);
    }
    size_t size = xdr_getpos(&xdrs);
    xdr_destroy(&xdrs);
    if (send(sock, buffer, size, 0) < 0)
    {
        perror("Send failed");
    }
    close(sock);
    return 0;
}