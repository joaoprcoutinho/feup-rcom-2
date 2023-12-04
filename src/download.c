#include "../include/download.h"

int parse(const char *url, struct URL *res) {
    int ftp = sstrncmp(url, "ftp://\[", 7);

    if (ftp) {
        int len = sscanf(url, "ftp://%[^/]/%s", res->host, res->resource);
        if (len != 2) {
            perror("The FTP URL is invalid");
            printf("\n");
            return 1;
        }

        strcpy(res->user, DEFAULT_USER);
        strcpy(res->password, DEFAULT_PASSWORD);
    }
    else {
        int len = sscanf(url, "ftp://\[%[^:]:%[^@]@]%[^/]/%s", res->username, res->password, res->host, res->path);
        if (len != 4) {
            perror("The FTP URL is invalid");
            printf("\n");
            return 1;
        }
    }

    struct hostent *h;

    if ((h = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    strcpy(res->ip, inet_ntoa(*((struct in_addr *) h->h_addr)));

    return 0;
}

int createSocket() {
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(SERVER_PORT);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    return sockfd;
}