#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int open_sock_and_send_to_server(const char *username, const char *msg, int *sock)
{

    char *extended_msg = NULL;
    struct sockaddr_in server_addr;

    if (!(extended_msg = (char *)malloc(strlen(username) + 1 + strlen(msg) + 1)))
    {
        goto out;
    }

    if (sprintf(extended_msg, "%s|%s", username, msg) != 2)
    {
        goto ext_msg_out;
    }

    *sock = 0;
    if ((*sock = socket(AF_INET, SOCK_SEQPACKET, 0)) == -1)
    {
        goto ext_msg_out;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(12345);

    if (connect(*sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        goto ext_msg_out;
    }

    if (send(*sock, extended_msg, strlen(extended_msg), 0) < 0)
    {
        goto sock_out;
    }

    free(extended_msg);
    return 0;

sock_out:
    close(*sock);
ext_msg_out:
    free(extended_msg);
out:
    return -1;
}

int main()
{
    int sock = 0;
    if (open_sock_and_send_to_server("My name", "Hello world", &sock))
    {
        printf("Error on sending message to server\n");
        return -1;
    }

    close(sock);
    return 0;
}