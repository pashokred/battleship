#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 1041

using namespace std;

int main()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[10][10] = {{}};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    for(int i=0;i<20;++i){
        char shot[10][10];
        int x, y;
        printf("Enter x coord from 1 to 10\n");
        cin >> x;
        printf("Enter y coord from 1 to 10\n");
        cin >> y;
        shot[x][y]='*';
        send(sock , shot , 100 , 0 );
        printf("Shot sent\n");
        valread = read( sock , buffer, 1024);
    }
    printf("%s\n",buffer );
    return 0;
}
