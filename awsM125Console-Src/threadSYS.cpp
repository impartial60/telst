#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "extern.h"


void snr2sysFixedValuesCoder()  //  П О С Т О Я Н Н А Я   Ч А С Т Ь
{
    ///////////////////////////////////////////////////////////////
    //  В Ы С О К О П Р И О Р И Т Е Т Н Ы Е   С О О Б Щ Е Н И Я  //
    ///////////////////////////////////////////////////////////////

    //  ID 0x040         У П Р А В Л Е Н И Е   П У С К О В О Й   У С Т А Н О В К О Й
    snr2sys.m044modeLCS.sid = 0x44;
    snr2sys.m044modeLCS.dlc = 1;
    snr2sys.m044modeLCS.data[0] = 0b10;   // всегда такое

    for(int i = 0; i < 4; i++)
    {
        //  ID 0x050 + N R   К О М А Н Д А   П У С К
        snr2sys.m05xStart[i].sid = 0x50 + i;
        snr2sys.m05xStart[i].dlc = 0x02;

        ///////////////////////////////////////////////////////
        //  В З А И М О Д Е Й С Т В И Е   С   Р А К Е Т О Й  //
        ///////////////////////////////////////////////////////

        //  ID 0x180 + N R   П О Д Г О Т О В К А   К   П У С К У
        snr2sys.m18xPrep[i].sid = 0x180 + i;
        snr2sys.m18xPrep[i].dlc = 0x01;

        //  ID 0x190 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   О П Ц И И   Н А В Е Д Е Н И Я
        snr2sys.m19xOptn[i].sid = 0x190 + i;
        snr2sys.m19xOptn[i].dlc = 0x06;

        //  ID 0x194 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   К О О Р Д И Н А Т Ы   Ц Е Л И   В   С К   А Н П
        snr2sys.m19xCoord[i].sid = 0x194 + i;
        snr2sys.m19xCoord[i].dlc = 0x08;

        //  ID 0x198 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   С К О Р О С Т И   Ц Е Л И   В   С К   А Н П
        snr2sys.m19xSpeed[i].sid = 0x198 + i;
        snr2sys.m19xSpeed[i].dlc = 0x08;

        //  ID 0x200 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   С Ф Е Р И Ч Е С К И Е   К О О Р Д И Н А Т Ы   Ц Е Л И   В   С К   А Н П
        snr2sys.m20xSpherCoord[i].sid = 0x200 + i;
        snr2sys.m20xSpherCoord[i].dlc = 0x08;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //  В З А И М О Д Е Й С Т В И Е   С   П У С К О В О Й   У С Т А Н О В К О Й  //
    ///////////////////////////////////////////////////////////////////////////////

    //  ID 0x288         Т Е К У Щ И Е   У Г Л Ы   Н А П Р А В Л Я Ю Щ И Х   П У

    //  ID 0x290         К О О Р Д И Н А Т Ы   П У   В   С К   А Н П

    //  ID 0x291         У Г Л Ы   З А П Р Е Т А   С Т Р Е Л Ь Б Ы

    // ID 0x292     Б А З О В Ы Й   У Г О Л   А  Н  П  (антенного поста)   В   Г Е О Г Р А Ф И Ч Е С К О Й   С К
    snr2sys.m292baseAngle.sid = 0x292;
    snr2sys.m292baseAngle.dlc = 2;
}

uint8_t komandaPYSK1prinata = 0;

void snr2sysVariableValuesCoder(int launcherIndex)  //  П Е Р Е М Е Н Н А Я   Ч А С Т Ь
{
    float az180 =  snrInfoG.currentAzimuth;
    if(az180 > 180.0)
        az180 -= 360.0;
    int16_t psi = (az180 / 180.0 * 32768);

    for(int i = 0; i < 4; i++)
    {

        ///////////////////////////////////////////////////////////////
        //  В Ы С О К О П Р И О Р И Т Е Т Н Ы Е   С О О Б Щ Е Н И Я  //
        ///////////////////////////////////////////////////////////////

        //  ID 0x050 + N R   К О М А Н Д А   П У С К
        // ............................

        ///////////////////////////////////////////////////////
        //  В З А И М О Д Е Й С Т В И Е   С   Р А К Е Т О Й  //
        ///////////////////////////////////////////////////////

        //  ID 0x180 + N R   П О Д Г О Т О В К А   К   П У С К У

        //if(launcherUser2Console.launcherUserCommands[1].nakal_ButtonPressedCounter[0] >= 10) //


        //    if(oficerNaved2Console.guidanceOfficerCmds.writeDataContinues)
        //        snr2sys.txmsg6_ID180h.data[0] = 0x05;
        //    else
        //        snr2sys.txmsg6_ID180h.data[0] = 0x00;

        qDebug()<<"launcherIndex:"<<launcherIndex<<"missileStatus:"<<startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus;

        uint8_t bitNakal = 0;
        uint8_t bitPodg = 0;
        uint8_t bitCelVZRP = 0b10000;  //  считаем, что цель всегда в зоне пуска - это проверка должна быть не здесь
        uint16_t kodPYSKA = 0;

        if(oficerPyska2Console.komandiPY[launcherIndex].ishdn_ButtonPressedCounter[i] == 0) // если не нажата кнопка "ИСХОДНОЕ"
        {
            if((oficerPyska2Console.komandiPY[launcherIndex].nakal_BtnPressedCounter[i] >= 10 &&
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::ISKHODNOE) ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::PEREHOD_ISKHODNOE_NAKAL ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::NAKAL ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::PEREHOD_NAKAL_GOTOVA ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::GOTOVA)
                bitNakal = 0b1;

            if((oficerPyska2Console.komandiPY[launcherIndex].podgt_BtnPressedCounter[i] >= 10 &&
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::NAKAL) ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::PEREHOD_NAKAL_GOTOVA ||
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::GOTOVA)
                bitPodg = 0b100;

            if(oficerPyska2Console.komandiPY[launcherIndex].pysk_BtnPressedCounter[i] >= 10 &&
               startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::GOTOVA)
            {
                if(komandaPYSK1prinata)         // доделать: если за 2 сек команда не принята - перейти в сотстояние MissileStatus::NEISPRAVNA
                    kodPYSKA = 0xA590 + 0;
                else
                    kodPYSKA = 0x5A68 + 0;
            }
        }

        memcpy(snr2sys.m05xStart[i].data, &kodPYSKA, sizeof(uint16_t));    //memcpy(snr2sys.txmsg2_ID050h.data, &kodPYSKA, sizeof(uint16_t));

        snr2sys.m18xPrep[i].data[0] = bitNakal + bitPodg + bitCelVZRP;    //snr2sys.txmsg6_ID180h.data[0] = bitNakal + bitPodg + bitCelVZRP;

        qDebug()<<"oficerPyska2Console.komandiPY[1].ishdn_ButtonPressedCounter[i]:"<<oficerPyska2Console.komandiPY[1].ishdn_ButtonPressedCounter[i]<<snr2sys.m18xPrep[i].data[0];


        //  ID 0x190 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   О П Ц И И   Н А В Е Д Е Н И Я

        //  ID 0x194 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   К О О Р Д И Н А Т Ы   Ц Е Л И   В   С К   А Н П

        //  ID 0x198 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   С К О Р О С Т И   Ц Е Л И   В   С К   А Н П

        //  ID 0x200 + N R   П О Л Е Т Н О Е   З А Д А Н И Е,   С Ф Е Р И Ч Е С К И Е   К О О Р Д И Н А Т Ы   Ц Е Л И   В   С К   А Н П

        uint16_t time16 = 0xEEEE;//programmStartTime.elapsed();  //  только для теста

        snr2sys.m20xSpherCoord[i].data[0] = time16 / 256;
        snr2sys.m20xSpherCoord[i].data[1] = time16 % 256;

        memcpy(snr2sys.m20xSpherCoord[i].data + 2, &psi, sizeof(int16_t));          //  только для теста  - это азимут директрисы УНВ - вращаем УНВ - убеждаемся, что азимут передаётся корректно

        int16_t teta = (snrInfoG.currentDirectrisaElevationAngle / 180.0 * 32768);
        memcpy(snr2sys.m20xSpherCoord[i].data + 4, &teta, sizeof(int16_t));         //  только для теста  - это угол места директрисы УНВ - качаем УНВ - убеждаемся, что угол места передаётся корректно

        uint16_t dalnost = (uint16_t)(131.07 / 0.002 * 65536);                      //  только для теста - 131.07 - максимально возможная дальность цели, км; 0.002 - ЦМР, км
        memcpy(snr2sys.m20xSpherCoord[i].data + 6, &dalnost, sizeof(uint16_t));     //  меняем дальность в коде - убеждаемся, что передаётся корректно
    }

    ///////////////////////////////////////////////////////////////////////////////
    //  В З А И М О Д Е Й С Т В И Е   С   П У С К О В О Й   У С Т А Н О В К О Й  //
    ///////////////////////////////////////////////////////////////////////////////


    //  ID 0x288         Т Е К У Щ И Е   У Г Л Ы   Н А П Р А В Л Я Ю Щ И Х   П У

    //  ID 0x290         К О О Р Д И Н А Т Ы   П У   В   С К   А Н П

    //  ID 0x291         У Г Л Ы   З А П Р Е Т А   С Т Р Е Л Ь Б Ы

    // ID 0x292     Б А З О В Ы Й   У Г О Л   А  Н  П  (антенного поста)   В   Г Е О Г Р А Ф И Ч Е С К О Й   С К
    memcpy(snr2sys.m292baseAngle.data, &psi, sizeof(int16_t));//memcpy(snr2sys.txmsg25_ID292h.data, &psi, sizeof(int16_t));

}


void sys2snrParser(int launcherIndex)
{
    //qDebug()<<"ProcessSYS totalReadBytes sid:"<<totalReadBytes<<sys2snr.rxmsg1_ID040h.sid<<"dlc:"<<sys2snr.rxmsg1_ID040h.dlc<<"data:"<<sys2snr.rxmsg1_ID040h.data[0]<<sys2snr.rxmsg1_ID040h.data[1];


//    for(int j = 0; j < MAXPY; j++)    {
//        for(int i = 0; i < MAXMISSILE; i++)        {
//            if(startInfoG.launcherInfo[j].missileInfo[i].missileStatus == MissileStatus::RAKETA_OTSYTSTVIET && oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i])
//                startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::ISKHODNOE;
//            else
//                if(startInfoG.launcherInfo[j].missileInfo[i].missileStatus != MissileStatus::NAVEDENIE && oficerPyska2Console.komandiPY[j].simulatorBortConctrsIsOn[i] == false)
//                    startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;        }    }

    for(int i = 0; i < 4; i++)
    {
        uint8_t priznakM1 = sys2snr.m10xMissileInfo[i].data[0] & 0b1;
        uint8_t priznakM2 = (sys2snr.m10xMissileInfo[i].data[0] & 0b10) >> 1;

        uint8_t pyIspr = (sys2snr.m10xMissileInfo[i].data[0] & 0b100) >> 2;

        uint8_t rakIspr = (sys2snr.m10xMissileInfo[i].data[0] & 0b1000) >> 3;
        uint8_t pitVkl = (sys2snr.m10xMissileInfo[i].data[0] & 0b10000) >> 4;
        uint8_t cB = (sys2snr.m10xMissileInfo[i].data[0] & 0b100000) >> 5;

        if((priznakM1 == 1 || priznakM2 == 1) && cB == 1)
            startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::ISKHODNOE;
        else
        {
            if(startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::GOTOVA &&
                    oficerPyska2Console.komandiPY[launcherIndex].pysk_BtnPressedCounter[i] > 10)  //komandaPYSK1prinata == 1
            {
                startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::NAVEDENIE;
                startInfoG.launcherInfo[launcherIndex].missileInfo[i].time_start_navedenie_milisec = programmStartTime.elapsed();
            }
            else
            {
                if(startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus == MissileStatus::NAVEDENIE)
                {
                    startInfoG.launcherInfo[launcherIndex].missileInfo[i].t_navedenie_sec =
                            0.001 * (programmStartTime.elapsed() - startInfoG.launcherInfo[launcherIndex].missileInfo[i].time_start_navedenie_milisec);

                    if(startInfoG.launcherInfo[launcherIndex].missileInfo[i].t_navedenie_sec > 70.0)
                        startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;
                }
            }
        }

        uint8_t res = (sys2snr.m10xMissileInfo[i].data[0] & 0b1000000) >> 6;
        uint8_t neshod = (sys2snr.m10xMissileInfo[i].data[0] & 0b10000000) >> 7;

        uint8_t postypilaKomandaNakal = sys2snr.m10xMissileInfo[i].data[1] & 0b1;
        if(postypilaKomandaNakal)
            startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::PEREHOD_ISKHODNOE_NAKAL;
        uint8_t nakalOtrabotan = (sys2snr.m10xMissileInfo[i].data[1] & 0b10) >> 1;
        if(nakalOtrabotan)
            startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::NAKAL;
        uint8_t postypilaKomandaPodgotovka = (sys2snr.m10xMissileInfo[i].data[1] & 0b100) >> 2;
        uint8_t poliotZadPolycheno = (sys2snr.m10xMissileInfo[i].data[1] & 0b1000) >> 3;
        uint8_t sGP = (sys2snr.m10xMissileInfo[i].data[1] & 0b10000) >> 4;
        if(postypilaKomandaPodgotovka == 1 && sGP == 0)
            startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::PEREHOD_NAKAL_GOTOVA;
        else
            if(sGP)
                startInfoG.launcherInfo[launcherIndex].missileInfo[i].missileStatus = MissileStatus::GOTOVA;

        uint8_t sVRprinat = (sys2snr.m10xMissileInfo[i].data[1] & 0b100000) >> 5;
        uint8_t pyskPrinat = (sys2snr.m10xMissileInfo[i].data[1] & 0b1000000) >> 6;
        komandaPYSK1prinata = pyskPrinat;
        uint8_t pyskOtrabotan = (sys2snr.m10xMissileInfo[i].data[1] & 0b10000000) >> 7;

        uint8_t rgsOk = sys2snr.m10xMissileInfo[i].data[2] & 0b1;
        uint8_t rgsP2polychena = (sys2snr.m10xMissileInfo[i].data[2] & 0b10) >> 1;
        uint8_t rgsYO = (sys2snr.m10xMissileInfo[i].data[2] & 0b100) >> 2;
        uint8_t rgs4P = (sys2snr.m10xMissileInfo[i].data[2] & 0b1000) >> 3;
        uint8_t rgsGG = (sys2snr.m10xMissileInfo[i].data[2] & 0b10000) >> 4;
        uint8_t as = (sys2snr.m10xMissileInfo[i].data[2] & 0b100000) >> 5;
        // дальше 2 резервных байта

        qDebug()<<"SYS data: РГС нач.тест Ok:"<<rgsOk<<"РГС Ком П2 получена:"<<rgsP2polychena<<"РГС углы отработаны:"<<rgsYO
                <<"РГС частота подстроена:"<<rgs4P<<"РГС головка готова:"<<rgsGG<<"АС  автосопр:"<<as;

        uint16_t t_nakala;
        memcpy(&t_nakala, sys2snr.m10xMissileInfo[i].data + 4, sizeof(int16_t)); //memcpy(&t_nakala, sys2snr.rxmsg2_ID100h.data + 4, sizeof(int16_t));
        uint16_t t_podg;
        memcpy(&t_podg, sys2snr.m10xMissileInfo[i].data + 6, sizeof(int16_t));   //memcpy(&t_podg, sys2snr.rxmsg2_ID100h.data + 6, sizeof(int16_t));

        switch((int)startInfoG.launcherInfo[1].missileInfo[i].missileStatus)
        {
            case MissileStatus::PEREHOD_ISKHODNOE_NAKAL: startInfoG.launcherInfo[1].missileInfo[i].t_skolko_ostalos_perehod_iskhodnoe_nakal_sec = t_nakala; break;
            case MissileStatus::NAKAL: startInfoG.launcherInfo[1].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_nakale_sec = t_nakala; break;

            case MissileStatus::GOTOVA: startInfoG.launcherInfo[1].missileInfo[i].t_obratnogo_otcheta_nahojdenia_v_gotova_sec = t_podg;
        }

        qDebug()<<"SYS data: M1:"<<priznakM1<<"M2:"<<priznakM2<<"ПУиспр:"<<pyIspr<<"РакИспр:"<<rakIspr
               <<"ПитВкл:"<<pitVkl<<"ЦБ:"<<cB<<"Резерв:"<<res<<"Несход:"<<neshod<<"t_nakala:"<<t_nakala<<"t_podg:"<<t_podg;

        qDebug()<<"SYS data: поступНАКАЛ:"<<postypilaKomandaNakal<<"НАКАЛ отраб:"<<nakalOtrabotan
               <<"поступПОДГ:"<<postypilaKomandaPodgotovka<<"полётЗад:"<<poliotZadPolycheno<<"СГП:"<<sGP<<"СВР:"<<sVRprinat
               <<"ПУСКпринят:"<<pyskPrinat<<"ПУСКотраб:"<<pyskOtrabotan;
    }
}


void * ProcessThreadSYS(void * temp_pt )
{
    int tcpSocketFileDescriptor = -1;

    unsigned char readBytes[sizeof(SYS2SNR)];

    clientConnect(&tcpSocketFileDescriptor, 9761, "10.5.0.31", "ProcessThreadSYS initial connect");
    fcntl(tcpSocketFileDescriptor, F_SETFL, O_NONBLOCK);

    snr2sysFixedValuesCoder();  //  П О С Т О Я Н Н А Я   Ч А С Т Ь

    while(1)
    {
        usleep(35 * 1000);          //qDebug()<<"********************************************************      ProcessSYS";

        snr2sysVariableValuesCoder(1);    //  П Е Р Е М Е Н Н А Я   Ч А С Т Ь           только для теста 1 - ПУ N 2

        //ssize_t totalSentBytes =
        send(tcpSocketFileDescriptor, &snr2sys, sizeof(SNR2SYS), 0);

        ssize_t nextReadBytes;

        ssize_t totalReadBytes = 0;

        int vsego_propyschenih_periodov = 0;

        while (totalReadBytes < sizeof(SYS2SNR) && vsego_propyschenih_periodov <= 10)
        {
            nextReadBytes = recv(tcpSocketFileDescriptor, readBytes + totalReadBytes, sizeof(SYS2SNR) - totalReadBytes, 0);

            //qDebug()<<"ProcessSYS nextReadBytes:"<<nextReadBytes;

            totalReadBytes += nextReadBytes;

            if(nextReadBytes <= 0)
            {
                vsego_propyschenih_periodov++;

                qDebug()<<"ProcessSYS: vsego_propyschenih_periodov:"<<vsego_propyschenih_periodov;
            }
            else
            {
                vsego_propyschenih_periodov = 0;
            }
        }

        if(vsego_propyschenih_periodov >= 10)
        {
//            for(int j = 0; j < MAXPY; j++)                                                                          //   временно      временно      временно
//                for(int i = 0; i < MAXMISSILE; i++)                                                                 //   временно      временно      временно
//                    startInfoG.launcherInfo[j].missileInfo[i].missileStatus = MissileStatus::RAKETA_OTSYTSTVIET;    //   временно      временно      временно

//            qDebug()<<"ProcessSYS : vsego_propyschenih_periodov >= 10  "<<startInfoG.launcherInfo[1].missileInfo[0].missileStatus;

            break;
        }

        memcpy(&sys2snr, readBytes, sizeof(SYS2SNR));
        memset(readBytes, 0, sizeof(SYS2SNR));

        if(snrInfoG.rejimRabotiZRK != RejimRabotiZRK::TR_A && snrInfoG.rejimRabotiZRK != RejimRabotiZRK::TR_K)
            sys2snrParser(1); // раскодикорвка информации с ПУ N 2 (1 - индекс считаем от нуля)

    };

    close(tcpSocketFileDescriptor);

    threadStatus->thrStatus_SYS = cancel;

    return temp_pt;
}
