#ifndef Drive125_H
#define Drive125_H

#define Kramarenko

#include <math.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "clockrealtime.h"
#include "device125.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/times.h>
#include <signal.h>

//#ifdef Kramarenko
//#define task_interval 20
//#else
//#define task_interval 35
//#endif

#define main_clock_works 20
#define time_outs 10

#define N_ID_enc_new        0x3F // (63) 	При наладке адрес энкодера по умолчанию
#define N_ID_incl           0x3E // (62) 	Адрес инклинометра (при запуске нового и в процессе работы одинаковый)
#define N_ID_enc_azimuth    0x3D // (61)	Энкодер азимута.
#define N_ID_enc_elv        0x3C // (60) 	Энкодер угла места
#define N_ID_lenze_az       0x3B // (59) 	Привод азимута.
#define N_ID_lenze_elv      0x3A // (58)	Привод угла места.
#define N_ID_PLC            0x39 // (57)	PLC Schneider.

extern class Drive125 drive_unit;

//void * ProcessThreadServoDrive(void * temp_pt );


class Drive125:public QObject//,QThread
//typedef struct
{
    Q_OBJECT
 public:

    explicit Drive125(QObject *parent = nullptr);
    ~Drive125();
  void Drive125_init();

//void *ProcessThreadServoDrive(void * temp_pt );
    Device125 unv,p1,p2,p3,p4;
public slots:
void slot_receive_packet(void);
void slot_timer_task(void);
inline uint32_t get_ip_apu(void) {return *apu_ip;}
private:
    int status;
    int cnt=0;
    int apu_s;
    struct sockaddr_in apu_drive,apu_tmp;
    socklen_t  slen_apu = sizeof(apu_tmp);
    bool connectStatus_apu ;

QThread *trd;
 //   event
QUdpSocket *exch;
QTimer *timer;
uint32_t ip_apu,ip_apu_training,*apu_ip ;

//void send_packet   (void);

//void open_socket(void);


};// Drive125;


#endif // Drive125_H
