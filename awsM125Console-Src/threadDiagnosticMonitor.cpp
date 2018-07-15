#include <math.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "extern.h"

int cntr12 = 0;

void * ProcessThreadDiagnMonitor( void * temp_pt )
{
    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    socklen_t clilen = sizeof(si_other);

    bool connectStatus = true;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessThreadDiagnosticMonitor: socket == -1";
        connectStatus = false;//die("socket");
    }
    fcntl(s, F_SETFL, O_NONBLOCK);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(settings->port_awsDiagnMonitor);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        connectStatus = false;//die("bind");

        qDebug()<<"ProcessThreadDiagnosticMonitor: bind == -1";
    }

    int diagnosticMonitorCounterOld = 0;
    int diagnosticMonitorCounterNew = 0;

    qDebug()<<"ProcessThreadDiagnosticMonitor UDP Server port:"<<settings->port_awsDiagnMonitor<<"connectStatus:"<<connectStatus;

    while(connectStatus)
    {
        usleep(35 * 1000);

        if ((recv_len = recvfrom(s, &diagnosticMonitorCounterNew, sizeof(int), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        {
            //connectStatus = false;//die("recvfrom()");
        }

        //print details of the client/peer and the data received
        //////////////printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //////////////printf("Data: %s\n" , buf);

        //now reply the client with the same data
        if (recv_len > 0)//sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            //////////////////die("sendto()");

            pthread_mutex_lock(&localConsole_memory_Mutex);
                //sendto(s, localConsole_memory + SERVICE_TOTAL_LENGTH + OUTPUT_TOTAL_LENGTH, INPUT_TOTAL_LENGTH, 0, (struct sockaddr*) &si_other, slen);
//                sendto(s, &shMemC2U, sizeof(AWSG_Console2User), 0, (struct sockaddr*) &si_other, slen);
                ///////////////sendto(s, &shMemC2U.usmInfo, sizeof(USM_Info), 0, (struct sockaddr*) &si_other, slen);

            //ssize_t totalSentBytes = sendto(s, &console2OficerNaved, sizeof(Console2OficerNavedenia), 0, (struct sockaddr*) &si_other, slen); //  2017-12-29

//            for(int i = 0; i < 256; i++)
//                console2DiagnosticMonitor.mdo1Echo.echo[0][i] = i % 255;

            ssize_t totalSentBytes = sendto(s, &console2DiagnosticMonitor, sizeof(Console2DiagnosticMonitor), 0, (struct sockaddr*) &si_other, slen); //  2017-12-29

                //ssize_t totalSentBytes = sendto(s, &shMemC2U, 50000, 0, (struct sockaddr*) &si_other, slen); //  2017-12-29

                //qDebug()<<"ProcessThreadDiagnMonitor: totalSentBytes:"<<totalSentBytes<<"sizeof(AWSG_Console2User):"<<sizeof(AWSG_Console2User);

            pthread_mutex_unlock(&localConsole_memory_Mutex);
        }

        if(diagnosticMonitorCounterOld != diagnosticMonitorCounterNew)
        {
            //qDebug()<<"ProcessThreadDiagnosticMonitor: diagnosticMonitorCounterNew"<<diagnosticMonitorCounterNew;
            diagnosticMonitorCounterOld = diagnosticMonitorCounterNew;
        }
    }

    close(s);
    return temp_pt;
}


void operatorF_common_operation(int roleNo)
{
    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    socklen_t clilen = sizeof(si_other);

    bool connectStatus = true;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessThreadOperF roleNo"<<roleNo<<"socket == -1";
        connectStatus = false;//die("socket");
    }
    fcntl(s, F_SETFL, O_NONBLOCK);

    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(settings->portOperatorF[roleNo]);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        connectStatus = false;

        qDebug()<<"ProcessThreadOperF roleNo"<<roleNo<<"bind == -1";
    }

    OperF_to_AWSG operF2AWSG;
    operF2AWSG.transActId = 0;

    int counterOld = 0;

    qDebug()<<"ProcessThreadOperF roleNo"<<roleNo<<"UDP Server port:"<<settings->portOperatorF[roleNo]<<"connectStatus:"<<connectStatus;

    while(connectStatus)
    {
        usleep(35 * 1000);

        if ((recv_len = recvfrom(s, &operF2AWSG, sizeof(OperF_to_AWSG), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        {
            //snrInfoG.handWheelFSpeed[1] = settings->handWheelSpeedFactor * operF2AWSG.multiTurnValue;   //  for work
            //connectStatus = false;//die("recvfrom()");
        }
        else
        {
//            qDebug()<<"ProcessThreadOperF1: recv_len:"<<recv_len<<"operF2AWSG: transactionId:"<<operF2AWSG.transActId<<"multiTurnValue:"<<operF2AWSG.multiTurnValue;

            //////////////////////////////////////if(settings->useVirtualHandWheel == 1)
                snrInfoG.handWheelFSpeed[roleNo] = settings->handWheelSpeedFactor * operF2AWSG.multiTurnValue;   //  for work

            //qDebug()<<"ProcessThreadOperF1: snrInfoG.handWheelFSpeed[0]:"<<snrInfoG.handWheelFSpeed[0];
        }

        //now reply the client with the same data
        if (recv_len > 0)
        {
            pthread_mutex_lock(&localConsole_memory_Mutex);

                //memcpy(&awsg2OperF.snrInfo, &snrInfoG, sizeof(SNR_INFO));
                //memcpy(&awsg2OperF.komKom, &komKomG, sizeof(KOMANDI_KOMANDIRA));

                sendto(s, &snrInfoG, sizeof(SNR_INFO), 0, (struct sockaddr*) &si_other, slen);

            pthread_mutex_unlock(&localConsole_memory_Mutex);
        }

        if(counterOld != operF2AWSG.transActId)//if(counterOld != counterNew)
        {
            //qDebug()<<"ProcessLauncher: diagnosticMonitorCounterNew"<<diagnosticMonitorCounterNew;
            counterOld = operF2AWSG.transActId;//counterOld = counterNew;
        }
    }
    close(s);
}


void * ProcessThreadOperF1( void * temp_pt )
{
    //qDebug()<<"ProcessThreadOperF1 1";

    operatorF_common_operation(0);

    return temp_pt;
}

void * ProcessThreadOperF2( void * temp_pt )
{
    //qDebug()<<"ProcessThreadOperF2 1";

    operatorF_common_operation(1);

    return temp_pt;
}
