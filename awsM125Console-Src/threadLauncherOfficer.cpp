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

float timeFactor;
int noMissileReadyCntr;

int cntr23 = 0;
MissileStatus::Status *nextMissileStatus;



void launchersSimulator()
{
    timeFactor = 1000.0 / 0.050 / 5;//blinckingTimer->interval() / ui->doubleSpinBoxTimeZoom->value();

    //int maxCounter_podgotovka = timeFactor * TMAXGOTOVA;
    //////////////////maxCounter_navedenie = 1000.0 / blinckingTimer->interval() * TMAXNAVEDENIE;

    if(noMissileReadyCntr >= 0)
    {
        //////////////////////ui->labelIronBtns_3->setText("Нет ракет готовых к пуску");

        if(noMissileReadyCntr ++ > 50)
            noMissileReadyCntr = -1;
    }

    cntr23++;

//    qDebug()<<"launchersSimulator cntr23:"<<cntr23<<"bortConnectorsPressedCounter:"<<launcherUser2Console.launcherUserCommands[1].bortConnectorsPressedCounter[0]
//                <<"missileStatus:"<<launcherConsole2User.launcherInfo[1].missileInfo[0].missileStatus;


//    qDebug()<<"launchersSimulator bortConnectorsPressedCounter:"<<launcherUser2Console.launcherUserCommands[1].bortConnectorsPressedCounter[0]
//            <<"missileStatus:"<<launcherConsole2User.launcherInfo[1].missileInfo[0].missileStatus;

    //  К О М А Н Д И Р   В К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А
//    if(komandirPereklychilTrenaj)
//    {
//        qDebug()<<"launchersSimulator:       К О М А Н Д И Р   В К Л Ю Ч И Л   Р Е Ж И М   Т Р Е Н А Ж А";

//        for(int j = 0; j < MAXPY; j++)
//        {
//            for(int i = 0; i < MAXMISSILE; i++)
//            {
//                if(oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i])
//                    startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::ISKHODNOE;
//                else
//                    startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
//            }
//        }

//        komandirPereklychilTrenaj = false;

//        //startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
//    }

    for(int j = 0; j < MAXPY; j++)
    {
        for(int i = 0; i < MAXMISSILE; i++)
        {
            if(startInfoG.launcherInfo[j].missileInfo[i].missileStatus == MissileStatus::RAKETA_OTSYTSTVIET && oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i])
                startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::ISKHODNOE;
            else
                if(startInfoG.launcherInfo[j].missileInfo[i].missileStatus != MissileStatus::NAVEDENIE && oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i] == false)
                    startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
        }
    }



    for(int j = 0; j < MAXPY; j++)  //for(int j = 1; j < 2; j++)//
    {
        for(int i = 0; i < MAXMISSILE; i++)   //for(int i = 0; i < 1; i++)
        {

            nextMissileStatus = &startInfoG.launcherInfo[j].missileInfo[i].missileStatus;

            /*  2018-03-27
            if(oficerPyska2Console.komandiPY[j].bort_ConctrsPressedCounter[i] <= -10)
            {
                *nextMissileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
                //launcher[j]->podgotovka_Button[i]->setEnabled(false);
                //launcher[j]->nakal_Button[i]->setEnabled(false);
                //launcher[j]->ishodnoe_Button[i]->setEnabled(false);
            }
            */

            switch((int)*nextMissileStatus)
            {
            /*  2018-03-27
            case (MissileStatus::RAKETA_OTSYTSTVIET):
                if(oficerPyska2Console.komandiPY[j].bort_ConctrsPressedCounter[i] >= 10)
                {
                    *nextMissileStatus = MissileStatus::ISKHODNOE;
                    //launcher[j]->nakal_Button[i]->setEnabled(true);

                    qDebug()<<"launchersSimulator metka 1 cntr23:"<<cntr23<<"bortConnectorsPressedCounter:"<<oficerPyska2Console.komandiPY[1].bort_ConctrsPressedCounter[0]
                                <<"missileStatus:"<<startInfoG.launcherInfo[1].missileInfo[0].missileStatus;

                }
                break;
                */
            case (MissileStatus::ISKHODNOE):                

                startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_ostivanie_v_iskhodnom_sec =
                oficerPyska2Console.timeZoomFactor * 0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_ostivanie_v_iskhodnom_milisec);

                //if(startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_ostivanie_v_iskhodnom_milisec >= TMAXOSTIVANIE_V_ISKHODNOM)
                {
                    // *nextMissileStatus = MissileStatus::ISKHODNOE;
                }

                if(oficerPyska2Console.komandiPY[j].nakal_BtnPressedCounter[i] >= 10) // начала или продолжает поступать команда включить накал
                {
                    *nextMissileStatus = MissileStatus::PEREHOD_ISKHODNOE_NAKAL;//launcher[j]->missileStatus[i] = MissileStatus::ISKHODNOE;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_nakala_milisec = programmStartTime.elapsed();

                    //launcher[j]->nakal_Button[i]->setEnabled(true);

                    qDebug()<<"launchersSimulator metka 2 cntr23:"<<cntr23<<"bortConnectorsPressedCounter:"<<oficerPyska2Console.komandiPY[1].nakal_BtnPressedCounter[0]
                                <<"missileStatus:"<<*nextMissileStatus;
                }

/*                if(launcherConsole2User.launcherInfo[j].missileInfo[i].counter_nakal > 0)//if(launcher[j]->counter_nakal[i] > 0)
                {

                    if(launcherConsole2User.launcherInfo[j].missileInfo[i].counter_ostivanie_v_iskhodnom++ > maxCounter_ostivanie_v_iskhodnom)//if(launcher[j]->counter_ostivanie_v_iskhodnom[i]++ > maxCounter_ostivanie_v_iskhodnom)
                    {
                        launcherConsole2User.launcherInfo[j].missileInfo[i].counter_nakal = 0;//launcher[j]->counter_nakal[i] = 0;
                        launcherConsole2User.launcherInfo[j].missileInfo[i].counter_ostivanie_v_iskhodnom = 0;//launcher[j]->counter_ostivanie_v_iskhodnom[i] = 0;
                    }
                }
                */
                break;
            case (MissileStatus::PEREHOD_ISKHODNOE_NAKAL):                
                    startInfoG.launcherInfo[j].missileInfo[i].t_skolko_ostalos_perehod_iskhodnoe_nakal_sec = TMAXPEREHOD_ISKHODNOE_NAKAL -
                    oficerPyska2Console.timeZoomFactor * 0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_nakala_milisec);

                qDebug()<<"launchersSimulator metka 3 cntr23:"<<cntr23<<"case (MissileStatus::PEREHOD_ISKHODNOE_NAKAL)"<<startInfoG.launcherInfo[j].missileInfo[i].t_skolko_ostalos_perehod_iskhodnoe_nakal_sec;

                if(startInfoG.launcherInfo[j].missileInfo[i].t_skolko_ostalos_perehod_iskhodnoe_nakal_sec < 0)//>= TMAXPEREHOD_ISKHODNOE_NAKAL)  // переход в состояние "НАКАЛ" завершен
                {
                    *nextMissileStatus = MissileStatus::NAKAL;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_nakala_milisec = programmStartTime.elapsed();

                    //launcher[j]->nakal_Button[i]->setEnabled(false);
                    //launcher[j]->podgotovka_Button[i]->setEnabled(true);
                    //startInfoG.launcherInfo[j].missileInfo[i].counter_perehod_nakal_podgotovka = 0;
                    //startInfoG.launcherInfo[j].missileInfo[i].counter_nakal = 0;  //  ?
                }
                break;
            case (MissileStatus::NAKAL):    // находимся в состоянии "НАКАЛ"
                startInfoG.launcherInfo[j].missileInfo[i].t_skolko_ostalos_perehod_iskhodnoe_nakal_sec = TMAXPEREHOD_ISKHODNOE_NAKAL;
                startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_nakale_sec = TMAXNAKAL -
                oficerPyska2Console.timeZoomFactor * 0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_nakala_milisec);

                qDebug()<<"launchersSimulator metka 4 cntr23:"<<cntr23<<"case (MissileStatus::NAKAL)"<<startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_nakale_sec;

                if(startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_nakale_sec < 0)//>= TMAXNAKAL)
                {
                    *nextMissileStatus = MissileStatus::ISKHODNOE;
                    startInfoG.launcherInfo[j].missileInfo[i].counter_ostivanie_v_iskhodnom = 0;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_ostivanie_v_iskhodnom_milisec = programmStartTime.elapsed();
                }

                if(oficerPyska2Console.komandiPY[j].podgt_BtnPressedCounter[i] >= 10) // начала или продолжает поступать команда "ПОДГОТОВКА"
                {
                    *nextMissileStatus = MissileStatus::PEREHOD_NAKAL_GOTOVA;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_podgotovka_milisec = programmStartTime.elapsed();

                    //launcher[j]->nakal_Button[i]->setEnabled(true);

                    //qDebug()<<"launchersSimulator metka 5 cntr23:"<<cntr23<<"missileStatus:"<<*nextMissileStatus;
                }

/*
                if(launcherConsole2User.launcherInfo[j].missileInfo[i].counter_nakal ++ >= maxCounter_nakal)
                {
                    *nextMissileStatus = MissileStatus::ISKHODNOE;
                    launcherConsole2User.launcherInfo[j].missileInfo[i].counter_ostivanie_v_iskhodnom = 0;

                    //launcher[j]->pysk_Button[i]->setEnabled(false);
                    //launcher[j]->podgotovka_Button[i]->setEnabled(true);
                    //launcher[j]->nakal_Button[i]->setEnabled(true);
                    //launcher[j]->ishodnoe_Button[i]->setEnabled(false);

                    //launcher[j]->nakal_Button[i]->setStyleSheet(QString("background-repeat: no-repeat; background-position: right;"
                      //                "padding-right: 4px;background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGray024.png);"));
                }
                */
                break;
            case (MissileStatus::PEREHOD_NAKAL_GOTOVA):

                startInfoG.launcherInfo[j].missileInfo[i].t_perehod_nakal_podgotovka_sec = TMAXPEREHOD_NAKAL_GOTOVA -
                       oficerPyska2Console.timeZoomFactor * 0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_podgotovka_milisec);

                qDebug()<<"launchersSimulator metka 3 cntr23:"<<cntr23<<"case (MissileStatus::PEREHOD_NAKAL_GOTOVA)"<<startInfoG.launcherInfo[j].missileInfo[i].t_perehod_nakal_podgotovka_sec;

                if(startInfoG.launcherInfo[j].missileInfo[i].t_perehod_nakal_podgotovka_sec < 0)
                {
                    *nextMissileStatus = MissileStatus::GOTOVA;                    
                    //float t_obratnogo_otcheta_nahojdenia_v_gotova_sec;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_gotova_milisec = programmStartTime.elapsed();
                }

/*                if(launcherConsole2User.launcherInfo[j].missileInfo[i].counter_perehod_nakal_podgotovka ++ >= maxCounter_perehod_nakal_podgotovka)
                {
                    *nextMissileStatus = MissileStatus::GOTOVA;//launcher[j]->missileStatus[i] = MissileStatus::GOTOVA;
                    //if(shMemC2U.commander2Launcher.komKom.pyskRazreshen)
                        //launcher[j]->pysk_Button[i]->setEnabled(true);
                    launcherConsole2User.launcherInfo[j].missileInfo[i].counter_perehod_nakal_podgotovka = 0;//launcher[j]->counter_perehod_nakal_podgotovka[i] = 0;
                }
                else
                {
                    if(launcherConsole2User.launcherInfo[j].missileInfo[i].counter_perehod_nakal_podgotovka % 20 < 10)//if(launcher[j]->counter_perehod_nakal_podgotovka[i] % 20 < 10)
                    {
//                        launcher[j]->podgotovka_Button[i]->setStyleSheet(QString("background-repeat: no-repeat; background-position: right;"
//                                          "padding-right: 4px;background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGreen024.png);"));
                    }
                    else
                    {
//                        launcher[j]->podgotovka_Button[i]->setStyleSheet(QString("background-repeat: no-repeat; background-position: right;"
//                                          "padding-right: 4px;background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGray024.png);"));
                    }
                }
*/
                break;
            case (MissileStatus::GOTOVA):

                startInfoG.launcherInfo[j].missileInfo[i].t_perehod_nakal_podgotovka_sec = TMAXPEREHOD_NAKAL_GOTOVA;

                startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_gotova_sec = TMAXGOTOVA -
                oficerPyska2Console.timeZoomFactor * 0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_gotova_milisec);

                if(startInfoG.launcherInfo[j].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_gotova_sec < 0)//>= TMAXGOTOVA)
                {
                    *nextMissileStatus = MissileStatus::ISKHODNOE;
                    startInfoG.launcherInfo[j].missileInfo[i].counter_ostivanie_v_iskhodnom = 0;
                    startInfoG.launcherInfo[j].missileInfo[i].time_vklychenie_obratnogo_otscheta_ostivanie_v_iskhodnom_milisec = programmStartTime.elapsed();
                }
                else
                {
                    //if(shMemC2U.commander2Launcher.komKom.pyskRazreshen)
                      //  launcher[j]->pysk_Button[i]->setEnabled(true);//launcher[j]->pysk_Button[i]->setEnabled(true);

                    if(oficerPyska2Console.komandiPY[j].pysk_BtnPressedCounter[i] >= 10) // начала или продолжает поступать команда "ПУСК"
                    {
                        *nextMissileStatus = MissileStatus::NAVEDENIE;
                        startInfoG.launcherInfo[j].missileInfo[i].time_start_navedenie_milisec = programmStartTime.elapsed();
                    }
                }
                break;
            case (MissileStatus::NAVEDENIE):
                startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec =
                        0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[j].missileInfo[i].time_start_navedenie_milisec);

                if(startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec >= TMAXNAVEDENIE)
                {
                    //pushBtnBortConnectors[j][i]->setChecked(false);
                    *nextMissileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
                    startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec = 0.0;
                }
                break;
            }
        }
    }
}


void * ProcessThreadLauncherOfficer( void * temp_pt )
{
    //qDebug()<<"ProcessThreadLauncherOfficer 1";

    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    socklen_t clilen = sizeof(si_other);

    bool connectStatus = true;

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessThreadLauncherOfficer: socket == -1";
        connectStatus = false;//die("socket");
    }
    fcntl(s, F_SETFL, O_NONBLOCK);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(settings->port_awsLauncherOfficer);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        connectStatus = false;//die("bind");

        qDebug()<<"ProcessThreadLauncherOfficer: bind == -1";
    }

    int launcherOfficerCounterOld = 0;
    int launcherOfficerCounterNew = 0;

    //keep listening for data

    qDebug()<<"ProcessThreadLauncherOfficer UDP Server port:"<<settings->port_awsLauncherOfficer<<"connectStatus:"<<connectStatus;

    while(connectStatus)
    {
        usleep(35 * 1000);
        ////////////printf("Waiting for data...");
        ////////////fflush(stdout);

        //try to receive some data, this is a blocking call
//        if ((recv_len = recvfrom(s, &launcherOfficerCounterNew, sizeof(int), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        if ((recv_len = recvfrom(s, &oficerPyska2Console, sizeof(KOMANDI_OFICERA_PYSKA), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        {
            //connectStatus = false;//die("recvfrom()");
        }

        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));   //printf("Data: %s\n" , buf);

        memcpy(&console2OficerPyska.snrInfo, &snrInfoG, sizeof(SNR_INFO));

        //console2OficerPyska.startInfo.launcherInfo[1].missileInfo[0].missileStatus = MissileStatus::ISKHODNOE;

        //qDebug()<<"ProcessThreadLauncherOfficer missileStatus:"<<console2OficerPyska.startInfo.launcherInfo[1].missileInfo[0].missileStatus;

        //////////launcherConsole2User.launcherInfo[1].missileInfo[0].missileStatus = MissileStatus::ISKHODNOE;

        if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_A || snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_K)
        {
            launchersSimulator();
        }
        else
        {

        }
        memcpy(&console2OficerPyska.startInfo, &startInfoG, sizeof(START_INFO));


        //now reply the client with the same data
        if (recv_len > 0)//sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            //////////////////die("sendto()");

            //pthread_mutex_lock(&localConsole_memory_Mutex);

                sendto(s, &console2OficerPyska, sizeof( Console2OficerPyska), 0, (struct sockaddr*) &si_other, slen);

                //ssize_t totalSentBytes = sendto(s, &shMemC2U, 50000, 0, (struct sockaddr*) &si_other, slen); //  2017-12-29

                //qDebug()<<"ProcessThreadDiagnMonitor: totalSentBytes:"<<totalSentBytes<<"sizeof(AWSG_Console2User):"<<sizeof(AWSG_Console2User);

            //pthread_mutex_unlock(&localConsole_memory_Mutex);
        }

        if(launcherOfficerCounterOld != launcherOfficerCounterNew)
        {
            //qDebug()<<"ProcessThreadDiagnosticMonitor: diagnosticMonitorCounterNew"<<diagnosticMonitorCounterNew;
            launcherOfficerCounterOld = launcherOfficerCounterNew;
        }

//        if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_A || snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_K)
//        {
//            launchersSimulator();
//        }

    }

    close(s);
    return temp_pt;
}
