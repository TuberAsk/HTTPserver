#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

char *render_file(const char *file_name) 
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "php %s", file_name);

    FILE* fp = popen(cmd, "r");
    if (fp == NULL) 
    {
        perror("popen");
        exit(-1);
    }

    char response[4096];
    size_t bytes_read = fread(response, 1, sizeof(response) - 1, fp);
    if (bytes_read > 0) 
    {
        response[bytes_read] = '\0';
    } 
    else 
    {
        response[0] = '\0';
    }

    char *http_header = (char *)malloc(sizeof(char) * 8192 + strlen(response));
    if (http_header == NULL) 
    {
        perror("malloc");
        exit(-1);
    }

    snprintf(http_header, 8192 + strlen(response) + 1, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", response);

    return http_header;
}

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("bind");
        return -1;
    }

    if (listen(sockfd, 10) < 0) 
    {
        perror("listen");
        return -1;
    }

    fprintf(stdout, "Listening on port 8080...\n");

    while (1)
    {
        int client_len = sizeof(client_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
        if (clientfd < 0) 
        {
            perror("accept");
            return -1;
        }

        char request[8192];
        recv(clientfd, request, sizeof(request), 0);

        char method[16], path[256];
        sscanf(request, "%15s %255s", method, path);

        char *http_header = NULL;

        if (strcmp(path, "/main.php") == 0) 
        {
            http_header = render_file("main.php");
        }

        else if (strcmp(path, "/about.php") == 0) 
        {
            http_header = render_file("about.php");
        }

        else 
        {
            // If there is no specified file, just render main.php
            http_header = render_file("main.php");
        }

        send(clientfd, http_header, strlen(http_header) + 1, 0);
        free(http_header);

        close(clientfd);
    }
    
    close(sockfd);
    return 0;
}
