#include "device125.h"

Device125::Device125(QObject *parent) : QObject(parent)
{

 //   connect(&timer,SIGNAL(timeout()),this,SLOT(slot_timer_task));
 //   timer.setSingleShot(true);

}

Device125::~Device125()
{

}
void Device125::Device125_init()
{
   // if(exch.)

   // connect(&exch,SIGNAL(readyRead()),this,SLOT(slot_udpServer));
}
void Device125::slot_receive_packet(void)
{

}
void Device125::slot_timer_task(void)
{

}

void Device125::udpSocket()
{
//    int apu_s;

//    struct sockaddr_in apu_drive,apu_tmp;
//    int slen_apu = sizeof(apu_tmp);
//    bool connectStatus_apu = true;

//    //create a UDP socket for servo drives
//    if ((apu_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
//    {
//      //  qDebug()<<"device: apu_socket == -1";
//        connectStatus_apu = false;//die("socket");
//    }
//    fcntl(apu_s, F_SETFL, O_NONBLOCK);

//    memset((char *) &apu_drive, 0, sizeof(apu_drive));

//    apu_drive.sin_family = AF_INET;
//    apu_drive.sin_port = htons(port_125);
//    apu_drive.sin_addr.s_addr = htonl(*p_ip);

//    //bind socket to port
//    if(connectStatus_apu &&  bind(apu_s , (struct sockaddr*)&apu_drive, sizeof(apu_drive) ) == -1)
//    {
//        connectStatus_apu = false;//die("bind");

//       // qDebug()<<"ProcessThreadGuidanceOfficer: bind_apu == -1";
//    }

}
//void Device125::set_connect(int mode_udp)
//{

//}

void Device125::tp_update(/*tp_t *tp, double period*/)
{

        double max_dv, tiny_dp, pos_err, vel_req,period=rt.interval();

        tpaz.active = 0;

        max_dv = tpaz.max_acc * period;

        tiny_dp = max_dv * period * 0.001;

        if (tpaz.enable) {

        pos_err = tpaz.pos_cmd - tpaz.curr_pos;

        if (pos_err > tiny_dp) {
            vel_req = -max_dv +
                   sqrt(2.0 * tpaz.max_acc * pos_err + max_dv * max_dv);

            tpaz.active = 1;
        } else if (pos_err < -tiny_dp) {
            vel_req =  max_dv -
                   sqrt(-2.0 * tpaz.max_acc * pos_err + max_dv * max_dv);

            tpaz.active = 1;
        } else {

            vel_req = 0.0;
        }
        } else {

        vel_req = 0.0;
        tpaz.pos_cmd = tpaz.curr_pos;
        }

        if (vel_req > tpaz.max_vel) {
            vel_req = tpaz.max_vel;
        } else if (vel_req < -tpaz.max_vel) {
        vel_req = -tpaz.max_vel;
        }

        if (vel_req > tpaz.curr_vel + max_dv) {
        tpaz.curr_vel += max_dv;
        } else if (vel_req < tpaz.curr_vel - max_dv) {
        tpaz.curr_vel -= max_dv;
        } else {
        tpaz.curr_vel = vel_req;
        }
        if (tpaz.curr_vel != 0.0) {
        tpaz.active = 1;
        }

        tpaz.curr_pos += tpaz.curr_vel * period;
//---------------------------um
        tpum.active = 0;

        max_dv = tpum.max_acc * period;

        tiny_dp = max_dv * period * 0.001;

        if (tpum.enable) {

        pos_err = tpum.pos_cmd - tpum.curr_pos;

        if (pos_err > tiny_dp) {
            vel_req = -max_dv +
                   sqrt(2.0 * tpum.max_acc * pos_err + max_dv * max_dv);

            tpum.active = 1;
        } else if (pos_err < -tiny_dp) {
            vel_req =  max_dv -
                   sqrt(-2.0 * tpum.max_acc * pos_err + max_dv * max_dv);

            tpum.active = 1;
        } else {

            vel_req = 0.0;
        }
        } else {

        vel_req = 0.0;
        tpum.pos_cmd = tpum.curr_pos;
        }

        if (vel_req > tpum.max_vel) {
            vel_req = tpum.max_vel;
        } else if (vel_req < -tpum.max_vel) {
        vel_req = -tpum.max_vel;
        }

        if (vel_req > tpum.curr_vel + max_dv) {
        tpum.curr_vel += max_dv;
        } else if (vel_req < tpum.curr_vel - max_dv) {
        tpum.curr_vel -= max_dv;
        } else {
        tpum.curr_vel = vel_req;
        }
        if (tpum.curr_vel != 0.0) {
        tpum.active = 1;
        }

        tpum.curr_pos += tpum.curr_vel * period;


}
void Device125::slot_udpServer(void)
{
    int len;
    QHostAddress adr;
    quint16 port_tmp;
    while (exch.hasPendingDatagrams()) {

    len = exch.readDatagram((char*)&send_tmp,sizeof(send_tmp),&adr,&port_tmp);

    //  mono->start(main_clock_works+time_outs);
    }


    if(port_tmp == port_125)
    {

                memcpy(&send_training,&send_tmp,sizeof(send_training));
    }

   timer.start(main_clock_works+time_outs);


   tp_update();

    tpaz.enable = send_training.az_on?1:0;
    tpum.enable = send_training.elv_on?1:0;

    tpaz.pos_cmd = send_training.angle_pos_az;
    tpum.pos_cmd = send_training.angle_pos_elv;

    receive_training.enc_angle_pos_az = (int) tpaz.curr_pos;//*10800;
    receive_training.enc_angle_pos_elv =(int) tpum.curr_pos;//*10800;
    receive_training.motor_encoder_az = (int) tpaz.curr_pos;//*10000;
    receive_training.motor_encoder_elv =(int) tpum.curr_pos;//*10000;

  //  send_packet
    exch.writeDatagram((char*)&receive_training,sizeof(receive_training),adr,port_tmp);

}
void Device125::receive_packet(void)
{

}

void Device125::send_packet   (void)
{
    device_tmp.sin_family = AF_INET;
    device_tmp.sin_port = htons(port_125);

  if(get_type() != nodevice ){
    device_tmp.sin_addr.s_addr = htonl(get_ip_adress());
    sendto(device_s, p_send, sizeof( *p_send), 0,
           (struct sockaddr*) &device_tmp, slen_device);}
}

void Device125::open_socket(void)
{
    //---------Kramarenko 01/07/2018--------------------------

    //create a UDP socket for servo drives
    if ((device_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("ServoDrive socket open: no open device_socket == -1\n");
        connectStatus_device = false;
        return;
    }
    fcntl(device_s, F_SETFL, O_NONBLOCK);

    memset((char *) &device_drive, 0, sizeof(device_drive));

    device_drive.sin_family = AF_INET;
    device_drive.sin_port = htons(port_125);
    device_drive.sin_addr.s_addr = htonl(ip_training);

    //bind socket to port
    if(connectStatus_device &&  bind(device_s , (struct sockaddr*)&device_drive, sizeof(device_drive) ) == -1)
    {
        connectStatus_device = false;//die("bind");
         printf("ServoDrive socket open: no bind apu_socket == -1\n");
        return;
    }


}
