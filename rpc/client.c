#include "arith.h"
#include <stdio.h>
#include <tirpc/rpc/types.h>
#include <tirpc/rpc/xdr.h>
int main()
{

    CLIENT *client;
    client = clnt_create("127.0.0.1", PROG_ARITH, 1, "udp");
    if (client == NULL)
    {
        exit(EXIT_FAILURE);
    }

    u_int len = 2;

    sum_args args = {10, 20};
    int *result = proc_sum_1(&args, client);

    if (!result)
        exit(EXIT_FAILURE);

    printf("sum = %d", *result);

    clnt_destroy(client);

    return 0;
}