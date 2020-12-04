#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#define PORT 1055

/*Гра "морський бій".
Грає лише користувач на клієнті. Сервер відповідає лише про результат
попадання/знищення кораблика. Достатньо на сервері мати єдине статичне
представлення поля з кораблями. Клієнт в матриці 10х10 повинен передавати
координати вистрілу.Сервер (без зайвої графічної реалізації, - достатньо
використовувати кілька символів для позначень у матриці із символів)
передає матрицю 10х10 із необхідними позначками вистрілів та попадань.
Клієнт в ході гри може її завершити гру раніше. Після завершення сеансу
сервер до клієнта передає статистику гри: кількість стрільб, кількість
попадань та промахів.*/





int main()
{
    int server_fd, new_socket1, new_socket2;
    int client_socket1 = 0, client_socket2 = 0;
    struct sockaddr_in address1{};
    struct sockaddr_in address2{};
    int opt = 1;
    int addrlen1 = sizeof(address1);
    int addrlen2 = sizeof(address2);
    char buffer[10][10] = {};
    char boats1[10][10] = {
            //        1    2    3    4    5    6    7    8    9    10   X
            /* 1  */{'/', '*', '*', '/', '/', '/', '/', '/', '/', '/'},
            /* 2  */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 3  */{'/', '/', '*', '*', '/', '*', '*', '/', '/', '/'},
            /* 4  */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 5  */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 6  */{'*', '/', '/', '*', '*', '*', '*', '/', '/', '/'},
            /* 7  */{'/', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 8  */{'*', '/', '/', '/', '*', '*', '*', '/', '/', '/'},
            /* 9  */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 10 */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'}

            /* Y  */
    };
    char boats2[10][10] = {
            //        1    2    3    4    5    6    7    8    9    10   X
            /* 1  */{'/', '*', '*', '/', '/', '/', '/', '/', '/', '/'},
            /* 2  */{'*', '/', '/', '/', '/', '*', '*', '/', '/', '/'},
            /* 3  */{'/', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 4  */{'*', '/', '/', '/', '/', '/', '*', '*', '/', '/'},
            /* 5  */{'*', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 6  */{'*', '/', '/', '*', '*', '*', '*', '/', '/', '/'},
            /* 7  */{'/', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 8  */{'/', '*', '/', '/', '*', '*', '*', '/', '/', '/'},
            /* 9  */{'/', '*', '/', '/', '/', '/', '/', '/', '/', '/'},
            /* 10 */{'/', '*', '/', '/', '/', '/', '/', '/', '/', '/'}

            /* Y  */
    };


    char hitshot[10][10];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    // Forcefully attaching socket to the port 1041
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //  SOCKET 1

    address1.sin_family = AF_INET;
    address1.sin_addr.s_addr = INADDR_ANY;
    address1.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 1041
    if (bind(server_fd, (struct sockaddr *)&address1,
             sizeof(address1))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen up to 3 clients
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket1 = accept(server_fd, (struct sockaddr *)&address1,
                             (socklen_t*)&addrlen1))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //  SOCKET 2

    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 1041
    if (bind(server_fd, (struct sockaddr *)&address2,
             sizeof(address2))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen up to 3 clients
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket2 = accept(server_fd, (struct sockaddr *)&address2,
                             (socklen_t*)&addrlen2))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int hits = 0;
    int misses = 0;
    bool shot = false;


    while (true){
        if(read(new_socket1 , buffer, 1024) >= 0 ){
            bool exitFlag = false;

            for(int i=0;i<10;++i){
                for(int j=0;j<10;++j){
                    if(buffer[i][j] == '*'){
                        if(boats1[i][j] == '*'){
                            boats1[i][j]='+';
                            hits++;
                            if(hits >=20){
                                send(new_socket1, boats1, sizeof(boats1), 0);
                                return 0;
                            }
                        }
                        else{
                            boats1[i][j]='-';
                            misses++;
                        }
                        exitFlag = true;
                        break;
                    }
                }
                if(exitFlag)
                    break;
            }
            send(new_socket1, boats1, sizeof(hitshot) , 0 );
            //printf("Hitshot send\n");
        }
        else if(read(new_socket2, buffer, 1024)>=0){
            bool exitFlag = false;

            for(int i=0;i<10;++i){
                for(int j=0;j<10;++j){
                    if(buffer[i][j] == '*'){
                        if(boats2[i][j] == '*'){
                            boats2[i][j]='+';
                            hits++;
                            if(hits >=20){
                                send(new_socket2, boats2, sizeof(boats1), 0);
                                return 0;
                            }
                        }
                        else{
                            boats2[i][j]='-';
                            misses++;
                        }
                        exitFlag = true;
                        break;
                    }
                }
                if(exitFlag)
                    break;
            }
            send(new_socket2, boats2, sizeof(hitshot) , 0 );
        }
    }
    return 0;
}