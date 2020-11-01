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
    char *message;
    char buffer[10][10] = {};
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

    for(int i=0;i<100;++i){
        char shot[10][10] = {};
        int x, y;
        printf("Enter x coord from 1 to 10\n");
        cin >> x;
        printf("Enter y coord from 1 to 10\n");
        cin >> y;
        shot[x-1][y-1]='*';
        send(sock , shot , sizeof(shot) , 0 );
        printf("Shot sent\n");
        read(sock, message, 1024);
        char msg[] = "You win!";
        printf("%s%c", message, '\n');
        if(msg == message){
            break;
        }
    }
    return 0;
}
