#include <QDebug>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
//#include <errno.h>
#include <pthread.h>
#include <string.h>

#include <fcntl.h>

#include "common/structuresasterix.h"
//#include "extern.h"


#define SOCK_KEEPALIVEMODE_ON       1       // 1 - вкл режим SO_KEEPALIVE (0 - выкл)
#define SOCK_KEEPALIVEMODE_IDLE     1//3    // время до начала опроса, сек
#define SOCK_KEEPALIVEMODE_INTVL    1       // время между опросами, сек
#define SOCK_KEEPALIVEMODE_CNT      2       // кол-во опросов, после которых соединение считается разорванным
#define TIMEOUT_BEFORE_RECONNECT    2//5    // таймаут перед реконнектом. секунды




void CloseSocket(int socket)
{      // *not* the Windows closesocket()
   if (socket >= 0)
   {
      if (shutdown(socket, SHUT_RDWR) < 0) // secondly, terminate the 'reliable' delivery
         if (errno != ENOTCONN && errno != EINVAL) // SGI causes EINVAL
            puts("Error shutdown socket");
      if (close(socket) < 0) // finally call close()
         puts("Error close socket");
   }
}


void SetSocketKEEPALIVE(int sock)
{
    int keepval = SOCK_KEEPALIVEMODE_ON;
    int keepidle = SOCK_KEEPALIVEMODE_IDLE;
    int keepintvl = SOCK_KEEPALIVEMODE_INTVL;
    int keepcnt = SOCK_KEEPALIVEMODE_CNT;
    // включаю режим KEEPALIVE для socket-a и задаю параметры режима
    if ((setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepval, sizeof(keepval)) < 0) ||
        (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) < 0) ||
        (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) < 0) ||
        (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) < 0))
    {
       perror("setsockopt()");
    }
}


void clientConnect(int *sock, int port, const char *ipaddr, const char *NameOfThread)
{
    //enum eEExit eeexit = EE_norm;
    time_t tt;

    //qDebug()<<"TCP clientConnect:"<<ipaddr<<":"<<port<<"NameOfThread:"<<NameOfThread;

    qDebug()<<NameOfThread<<" TCP Client "<<ipaddr<<":"<<port;

    //TCP clientConnect: 10.0.87.184 : 17001 NameOfThread: VKP initial connect
    //ProcessInfoTable UDP Server port: 30014 connectStatus: true

    while (1)
    {
//        pthread_mutex_lock(&mutex_EExit);
//        eeexit = EExit;
//        pthread_mutex_unlock(&mutex_EExit);
        if (*sock > 0)
        {
            close(*sock);
            time(&tt);
            //printf("%s Ошибка обмена, попытка переподключения (client)... %s", NameOfThread, ctime(&tt));
            //////////////////////////////////qDebug()<<"Ошибка обмена, попытка переподключения (client)..."<<NameOfThread;
        }
        *sock = socket(AF_INET, SOCK_STREAM, 0);
        //fcntl(*sock, F_SETFL, O_NONBLOCK);

        struct  sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ipaddr);
        //inet_aton("127.0.0.1", &addr.sin_addr);

        if (connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            sleep(TIMEOUT_BEFORE_RECONNECT);
        else
        {
            time(&tt);
            //printf("%s Подключение выполнено! %s", NameOfThread, ctime(&tt));
            qDebug()<<"Подключение выполнено: "<<NameOfThread;
            //////////////////////////////////SetSocketKEEPALIVE(*sock);
            break;
        }
    }
}

