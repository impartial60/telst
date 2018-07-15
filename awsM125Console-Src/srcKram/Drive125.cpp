#include "Drive125.h"
void *ProcessThreadServoDrive(void *temp_pt)
{
    //qDebug()<<"ProcessThreadThreadServoDrive: 1";

//    struct sockaddr_in si_me, si_other;

//    int s, slen = sizeof(si_other) , recv_len;
//    socklen_t clilen = sizeof(si_other);

//    bool connectStatus = true;

//    //create a UDP socket
//    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
//    {
//        qDebug()<<"ProcessThreadServoDrive: socket == -1";
//        connectStatus = false;//die("socket");
//    }
//    fcntl(s, F_SETFL, O_NONBLOCK);

//    // zero out the structure
//    memset((char *) &si_me, 0, sizeof(si_me));

//    if((connectStatus && setsockopt(s,SOL_SOCKET,SO_REUSEADDR,NULL,0))<0)
//       {
//        connectStatus = false;//die("bind");
//        qDebug()<<"ProcessThreadServoDrive:setsockopt == -1"<<errno<<strerror(errno);   ;
//            }

//    si_me.sin_family = AF_INET;
//    si_me.sin_port = htons(port_125);
//    si_me.sin_addr.s_addr = htonl(/*drive_unit.get_ip_apu()*/INADDR_ANY);

//    //bind socket to port
//    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
//    {
//        connectStatus = false;//die("bind");

//        qDebug()<<"ProcessThreadServoDrive: bind == -1"<<errno<<strerror(errno);
//    //keep listening for data
//}
// //   qDebug()<<"ProcessThreadGuidanceOfficer UDP Server port:"<<settings->port_awsGuidanceOfficer<<"connectStatus:"<<connectStatus;

//    while(/*connectStatus*/1)
//    {
//        usleep(main_clock_works  * 1000);
//        if ((recv_len = recvfrom(s, drive_unit.unv.get_adr_receive(), drive_unit.unv.get_len_receive(), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
//        {
//            //connectStatus = false;//die("recvfrom()");
//        }
 //   }
 //   close(s);
//    return temp_pt;
}

Drive125::Drive125(QObject *parent) : QObject(parent)
{
    status=inet_pton(AF_INET, "10.5.0.101",&unv.ip_combat);
    status=inet_pton(AF_INET,"127.0.0.101",&unv.ip_training);

    status=inet_pton(AF_INET, "10.5.0.102",&p1.ip_combat);
    status=inet_pton(AF_INET,"127.0.0.102",&p1.ip_training);

    status=inet_pton(AF_INET, "10.5.0.103",&p2.ip_combat);
    status=inet_pton(AF_INET,"127.0.0.103",&p2.ip_training);

    status=inet_pton(AF_INET, "10.5.0.104",&p3.ip_combat);
    status=inet_pton(AF_INET,"127.0.0.104",&p3.ip_training);

    status=inet_pton(AF_INET, "10.5.0.105",&p4.ip_combat);
    status=inet_pton(AF_INET,"127.0.0.105",&p4.ip_training);

    status=inet_pton(AF_INET,"10.5.0.225",&ip_apu);
    status=inet_pton(AF_INET,"127.0.0.1",&ip_apu_training);

    apu_ip = &ip_apu_training;

    unv.set_type(unv.unv);
    unv.set_mode(unv.training);
    p1.set_type(p1.pusk);
    p1.set_mode(p1.training);
    p2.set_type(p2.nodevice);
    p2.set_mode(p2.training);
    p3.set_type(p3.nodevice);
    p3.set_mode(p3.training);
    p4.set_type(p4.nodevice);
    p4.set_mode(p4.training);

//    unv.Device125_init();
//    p1.Device125_init();
//    p2.Device125_init();
//    p3.Device125_init();
//    p4.Device125_init();
    exch = new QUdpSocket;
    timer = new QTimer;
   // this->thread() = new QThread;
    trd = new QThread;//::create([]() {this->Drive125_init()});
this->moveToThread(trd);

   // timer->moveToThread(this->thread());
 //   exch->moveToThread(trd);
   // this->thread()->start();
//    trd->create(ProcessThreadServoDrive(timer));
    exch->bind(QHostAddress::AnyIPv4,port_125);
    connect(exch,SIGNAL(readyRead()),this,SLOT(slot_receive_packet));
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_timer_task));
    timer->start(main_clock_works);

       trd->start();
//this->thread()->start();

//    this->moveToThread(this->instance()->thread());
 //   this->thread()->start();
  //      this->thread()->startTimer(20);
  //  this->startTimer(20);

}
Drive125::~Drive125()
{

}

//void *Drive125::ProcessThreadServoDrive(void * temp_pt )
//{
//}

void Drive125::Drive125_init()
{


}

void Drive125::slot_receive_packet(void)
{

}
void Drive125::slot_timer_task(void)
{
    if(unv.get_mode() != unv.nodevice)
exch->writeDatagram(unv.get_adr_send(),unv.get_len_send(),QHostAddress(*unv.p_ip),port_125);
    if(p1.get_mode() != p1.nodevice)
exch->writeDatagram(p1.get_adr_send(),p1.get_len_send(),QHostAddress(*p1.p_ip),port_125);
    if(p2.get_mode() != p2.nodevice)
exch->writeDatagram(p2.get_adr_send(),p2.get_len_send(),QHostAddress(*p2.p_ip),port_125);
    if(p3.get_mode() != p3.nodevice)
exch->writeDatagram(p3.get_adr_send(),p3.get_len_send(),QHostAddress(*p3.p_ip),port_125);
    if(p4.get_mode() != p4.nodevice)
exch->writeDatagram(p4.get_adr_send(),p4.get_len_send(),QHostAddress(*p4.p_ip),port_125);


}
//void Drive125::open_socket(void)
//{
//    //---------Kramarenko 01/07/2018--------------------------

//    //create a UDP socket for servo drives
//    if ((apu_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
//    {
//        printf("ServoDrive socket open: no open apu_socket == -1\n");
//        connectStatus_apu = false;
//        return;
//    }
//    fcntl(apu_s, F_SETFL, O_NONBLOCK);

//    memset((char *) &apu_drive, 0, sizeof(apu_drive));

//    apu_drive.sin_family = AF_INET;
//    apu_drive.sin_port = htons(port_125);
//    apu_drive.sin_addr.s_addr = htonl(INADDR_ANY);

//    //bind socket to port
//    if(connectStatus_apu &&  bind(apu_s , (struct sockaddr*)&apu_drive, sizeof(apu_drive) ) == -1)
//    {
//        connectStatus_apu = false;//die("bind");
//         printf("ServoDrive socket open: no bind apu_socket == -1\n");
//        return;
//    }




//}
//void Drive125::send_packet   (void)
//{
//unv.send_packet();
//p1.send_packet();
//p2.send_packet();
//p3.send_packet();
//p4.send_packet();


//}
//void Drive125::receive_packet(void)
//{
// //   int recv_len;
//            //---------Kramarenko 01/07/2018--------------------------


//                    //тут я написал не правильно!!!!! надо распределять принятые пакеты по устройствам и чтобы
//                    //прием не захлебнулся несколькими пакетами. Сокет то открыт не блокируемым.
//                    //fcntl(apu_s, F_SETFL, O_NONBLOCK);

//                    //задержка в приеме очередного пакета - интервал потока. это очень много
//                    // если есть возможность сделай прием по событию приема пакета.

////                    if ((recv_len = recvfrom(apu_s, &unv.receive_tmp,
////                                             sizeof( unv.receive_tmp), 0,
////                                             (struct sockaddr *) &apu_drive, &slen_apu)) == -1)
////                    {
////                        connectStatus_apu = false;

////                    }
//}
