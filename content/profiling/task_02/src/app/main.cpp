#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "util.h"

/******************************************************************************
 * TCP client
 ******************************************************************************/

int32_t
tcp_connect(char *ip, uint16_t port)
{
    struct sockaddr_in server;      /* server address         */
    int32_t            sockfd;      /* socket file descriptor */
    int32_t            ans;         /* answer                 */

    /* create TCP socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    RET(sockfd == -1, -1, "unable to open TPC socket (%s)", strerror(errno));

    /* initialize server address */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    ans = inet_pton(AF_INET, ip, &server.sin_addr);
    GOTO(ans <= 0, out_close, "invalid IPv4 address (%s)", strerror(errno));

    /* connect to specified ip:port */
    ans = connect(sockfd, (struct sockaddr *) &server, sizeof(server));
    GOTO(ans == -1, out_close, "unable to connect (%s)", strerror(errno));

    /* success */
    return sockfd;

    /* error cleanup */
out_close:
    close(sockfd);

    return -1;
}

int32_t
send_query(int32_t sockfd, const char *query, size_t len)
{
    ssize_t ans;    /* answer */

    /* send HTTP GET */
    ans = send(sockfd, query, len, 0);
    RET(ans == -1, -1, "unable to send HTTP GET (%s)", strerror(errno));

    return ans;
}

int32_t
recv_response(int32_t sockfd, char *buffer, size_t len)
{
    ssize_t ans;    /* answer */

    /* receive HTTP response */
    ans = recv(sockfd, buffer, len, 0);
    RET(ans == -1, -1, "unable to get HTTP response (%s)", strerror(errno));

    return ans;
}

int32_t
main(int32_t argc, char *argv[])
{
    uint16_t   port;        /* server port number     */
    int32_t    sockfd;      /* socket file descriptor */
    int32_t    ans;         /* answer                 */
    int32_t    ret = -1;    /* return value           */
    char       resp[2048];  /* response buffer        */
    const char query[] =    /* HTTP GET query         */
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: curl/8.13.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    /* check number of arguments & parse port number */
    DIE(argc != 3, "usage: ./http-get <IPv4> <port>");
    ans = sscanf(argv[2], "%hu", &port);
    DIE(ans != 1, "unable to parse port number (%s)", strerror(errno));

    /* open TCP connection to server */
    sockfd = tcp_connect(argv[1], port);
    DIE(ans == -1, "unable to establish TCP connection");

    /* send HTTP query */
    ans = send_query(sockfd, query, sizeof(query));
    GOTO(ans == -1, out_close, "unable to send HTTP GET");

    /* receive HTTP response */
    memset(resp, 0, sizeof(resp));
    ans = recv_response(sockfd, resp, sizeof(resp));
    GOTO(ans == -1, out_close, "unable to receive HTTP response");

    /* display response */
    printf("%s\n", resp);

    /* success */
    ret = 0;

    /* cleanup */
out_close:
    close(sockfd);

    return 0;
}

