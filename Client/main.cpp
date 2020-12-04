#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
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

    printf("Welcome to the battleship game!\n"
           "To start game, you should just enter integer coordinates of shoot from 1 to 10\n\n");

    for(int i=0;i<100;++i){
        try{
            char shot[10][10] = {};
            int x, y;
            string cmd1="", cmd2="";

            printf("Enter x coord from 1 to 10\n");
            cin >> cmd1;
            y = stoi(cmd1);
            if(y>10 || y<0)
                throw std::invalid_argument("Value must be integer from 1 to 10");

            printf("Enter y coord from 1 to 10\n");
            cin >> cmd2;
            x = stoi(cmd1);
            if(x>10 || x<0)
                throw std::invalid_argument("Value must be integer from 1 to 10");

            shot[x-1][y-1]='*';

            send(sock , shot , sizeof(shot) , 0 );
            printf("Shot sent\n");
            read(sock, message, 1024);
            char msg[] = "You win!";
            printf("%s", message);
            printf("%c", '\n');
            if(msg == message){
                break;
            }
        } catch (invalid_argument &argument) {
            printf("%s%s%c", "Invalid argument.", argument.what(), '\n');
        }
    }
    return 0;
}
