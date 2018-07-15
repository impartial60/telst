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


void * ProcessThreadCommander( void * temp_pt )
{
    //qDebug()<<"ProcessThreadCommander 1";
    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    socklen_t clilen = sizeof(si_other);

    bool connectStatus = true;

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessThreadCommander: socket == -1";
        connectStatus = false;//die("socket");
    }
    fcntl(s, F_SETFL, O_NONBLOCK);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(settings->port_awsCommander);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //qDebug()<<"ProcessThreadCommander settings->port_awsCommander:"<<settings->port_awsCommander<<"si_me.sin_port:"<<si_me.sin_port;

    //bind socket to port
    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        connectStatus = false;//die("bind");

        qDebug()<<"ProcessThreadCommander: bind == -1";
    }

    int commanderCounterOld = 0;
    int commanderCounterNew = 0;

    //keep listening for data

    //qDebug()<<"ProcessThreadCommander connectStatus:"<<connectStatus;
    qDebug()<<"ProcessThreadCommander UDP Server port:"<<settings->port_awsCommander<<"connectStatus:"<<connectStatus;

    while(connectStatus)
    {
        usleep(35 * 1000);
        ////////////printf("Waiting for data...");
        ////////////fflush(stdout);

        //try to receive some data, this is a blocking call
//        if ((recv_len = recvfrom(s, &launcherOfficerCounterNew, sizeof(int), 0, (struct sockaddr *) &si_other, &clilen)) == -1)

        //if ((recv_len = recvfrom(s, &launcherUser2Console, sizeof(KOMANDI_OFICERA_PYSKA), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        if ((recv_len = recvfrom(s, &komKomM, sizeof(KOMANDI_KOMANDIRA), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        {
            //connectStatus = false;//die("recvfrom()");
        }

        //  П Р И Ё М   К О М А Н Д   К О М А Н Д И Р А        
        //  К О М А Н Д И Р   В К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А
//        if(snrInfoG.rejimRabotiZRK != RejimRabotiZRK::TR_K && snrInfoG.rejimRabotiZRK != RejimRabotiZRK::TR_A &&
//           (komKomG.rejimRabotiZRK == RejimRabotiZRK::TR_K ||  komKomG.rejimRabotiZRK == RejimRabotiZRK::TR_A))
//        {
//            qDebug()<<"ProcessThreadCommander К О М А Н Д И Р   В К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А";

//            for(int j = 0; j < MAXPY; j++)
//            {
//                for(int i = 0; i < MAXMISSILE; i++)
//                {
//                    if(oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i])
//                        startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::ISKHODNOE;
//                    else
//                        startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
//                }
//            }
//        }
        //  К О М А Н Д И Р   В Ы К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А
        if((snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_K || snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_A) &&
             komKomM.rejimRabotiZRK != RejimRabotiZRK::TR_K &&  komKomM.rejimRabotiZRK != RejimRabotiZRK::TR_A)
        {
            //qDebug()<<"ProcessThreadCommander К О М А Н Д И Р   В Ы К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А";

            for(int j = 0; j < MAXPY; j++)
            {
                for(int i = 0; i < MAXMISSILE; i++)
                {
                        startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
                }
            }
        }

        snrInfoG.rejimRabotiZRK = komKomM.rejimRabotiZRK;
        snrInfoG.remoteControlZRK = komKomM.remoteControlZRK;
        snrInfoG.showSimulationTrack = komKomM.showSimulationTrack;
        snrInfoG.showSimulationTarget = komKomM.showSimulationTarget;

        //qDebug()<<"ProcessThreadCommander recv_len:"<<recv_len<<"snrInfoG.rejimRabotiZRK:"<<snrInfoG.rejimRabotiZRK;

        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));   //printf("Data: %s\n" , buf);

        /////////////////memcpy(&launcherConsole2User.commander2Launcher.snrInfo, &snrInfoG, sizeof(SNR_INFO));
        /////////////////memcpy(&launcherConsole2User.startInfo, &startInfoG, sizeof(START_INFO));

        //////////launcherConsole2User.launcherInfo[1].missileInfo[0].missileStatus = MissileStatus::ISKHODNOE;

        //now reply the client with the same data
        if (recv_len > 0)//sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            //////////////////die("sendto()");

            //pthread_mutex_lock(&localConsole_memory_Mutex);

            //sendto(s, &launcherConsole2User, sizeof( LauncherConsole2User), 0, (struct sockaddr*) &si_other, slen);
            sendto(s, &snrInfoG, sizeof(SNR_INFO), 0, (struct sockaddr*) &si_other, slen);

                //qDebug()<<"ProcessThreadDiagnMonitor: totalSentBytes:"<<totalSentBytes<<"sizeof(AWSG_Console2User):"<<sizeof(AWSG_Console2User);

            //pthread_mutex_unlock(&localConsole_memory_Mutex);
        }

        if(commanderCounterOld != commanderCounterNew)
        {
            //qDebug()<<"ProcessThreadDiagnosticMonitor: diagnosticMonitorCounterNew"<<diagnosticMonitorCounterNew;
            commanderCounterOld = commanderCounterNew;
        }

    }

    close(s);
    return temp_pt;
}
