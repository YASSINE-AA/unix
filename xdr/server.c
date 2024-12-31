#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ntirpc/rpc/xdr.h>
#include "test.h"
// gcc test_xdr.c server.c -o server -I/usr/include/ntirpc -lntirpc
int main()
{

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
        exit(EXIT_FAILURE);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        exit(EXIT_FAILURE);
    }
    int service_fd;

    while (1)
    {
        listen(sock, 10);
        struct sockaddr_in client_addr = {0};
        socklen_t client_addr_len;
        char buffer[4];
        if ((service_fd = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len)) != -1)
        {
            printf("Client with addr %s \n", inet_ntoa(client_addr.sin_addr));
            char buffer[1024];
            ssize_t bytes_received = recv(service_fd, buffer, sizeof(buffer), 0);
            if (bytes_received < 0)
            {
                perror("Receive failed");
                close(service_fd);
                close(sock);
                exit(1);
            }

            MyData data;
            XDR xdrs;
            xdrmem_create(&xdrs, buffer, bytes_received, XDR_DECODE);

            if (!xdr_mydata(&xdrs, &data))
            {
                fprintf(stderr, "Deserialization failed\n");
            }
            else
            {
                printf("Received Data - ID: %d, Name: %s, Value: %.2f\n", data.id, data.name, (float) data.value);
            }

            xdr_destroy(&xdrs);
        }
    }
    close(service_fd);
    close(sock);
    return 0;
}