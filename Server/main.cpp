#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#define PORT 1041

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
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[10][10] = {};
    char boats[10][10] = {
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
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 1041
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
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

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int hits = 0;
    int misses = 0;

    while (read( new_socket , buffer, 1024) >= 0){

        char *message;
        std::string hit("You hit the target!");
        std::string miss("You missed!");
        std::string win("You win!\n\n Statistic:\n \tAmount of shots: ");

        bool exitFlag = false;

        for(int i=0;i<10;++i){
            for(int j=0;j<10;++j){
                if(buffer[i][j] == '*'){
                    if(boats[i][j] == '*'){
                        hitshot[i][j]='+';
                        hits++;
                        if(hits >=20){
                            win += std::to_string(hits+misses);
                            win += "\n \tAmount of hits: ";
                            win += std::to_string(hits);
                            win += "\n \tAmount of mises: ";
                            win += std::to_string(misses);
                            message = &win[0];
                        }
                        else
                            message = &hit[0];
                    }
                    else{
                        hitshot[i][j]='-';
                        misses++;
                        message = &miss[0];
                    }
                    exitFlag = true;
                    break;
                }
            }
            if(exitFlag)
                break;
        }

        send(new_socket , message , sizeof(hitshot) , 0 );
        //printf("Hitshot send\n");
    }
    return 0;
}