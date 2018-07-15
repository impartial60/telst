#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "extern.h"


UV11ANGLES  operatorPovorota(float b, float e)
{
    UV11ANGLES angls;

    angls.deltaF[1] = b * SIN45COS45 - e * SIN45COS45;
    angls.deltaF[0] = b * SIN45COS45 + e * SIN45COS45;

    return angls;
}

//int cntr8 = 0;

void assignGradysnikParams(int ii)
{
    // G R A D Y S N I K   G R A D Y S N I K   G R A D Y S N I K   G R A D Y S N I K   G R A D Y S N I K   G R A D Y S N I K   G R A D Y S N I K
    //azimuth_grad = prm.azimuth360;//ar[4];
    //height_km = prm.radarRelativeAltitude;//ar[6];  //временно
    /////////////////////////////////////////////////////////////////////radialSpeed_ms = prm.radialSpeed;//ar[7];
    //rangeHorizont_km = prm.groundRange_km;// ar[8];
    //kyrs_grad = prm.kyrs;// ar[10];
    //rakyrs_grad = prm.rakyrs;// ar[11];
    //////////////////////////////////////////////////////////////////////groundSpeed_km_s = 0.001 * prm.groundSpeed;//0.001 * ar[12];
    //elevationAngle_grad = prm.elevationAngle;// ar[13];

    // Т Е К У Щ И Й   К У Р С О В О Й   П А Р А М Е Т Р
    float deltaPrm = snrInfoG.nextTocka[ii].rakyrs / Rad2Grad;
    snrInfoG.nextTocka[ii].kyrsovojParameter_km = snrInfoG.nextTocka[ii].groundRange_km * sin(deltaPrm);
    snrInfoG.nextTocka[ii].rangeParametr_km = sqrt(snrInfoG.nextTocka[ii].groundRange_km * snrInfoG.nextTocka[ii].groundRange_km
                                                           - snrInfoG.nextTocka[ii].kyrsovojParameter_km * snrInfoG.nextTocka[ii].kyrsovojParameter_km);
    ZonaPorazhRanges zpr = ZP::glybina_ZP(snrInfoG.nextTocka[ii].kyrsovojParameter_km, snrInfoG.nextTocka[ii].radarRelativeAltitude);
    snrInfoG.nextTocka[ii].bGZoniPorParam_km = zpr.minValue_km;
    snrInfoG.nextTocka[ii].dGZoniPorParam_km = zpr.maxValue_km;
    snrInfoG.nextTocka[ii].maxKyrsovojParameter_km = zpr.maxParams;

    float dgzPor_abs = sqrt(zpr.maxValue_km * zpr.maxValue_km + snrInfoG.nextTocka[ii].kyrsovojParameter_km * snrInfoG.nextTocka[ii].kyrsovojParameter_km);
    float tPoliotaRaketiDGZPor = dgzPor_abs / VRAKETI;
    float groundSpeed_km_s = snrInfoG.nextTocka[ii].groundSpeed_ms * 1.0e-3;

    snrInfoG.nextTocka[ii].dGZoniPyskaParametr_km = zpr.maxValue_km + fabs(groundSpeed_km_s) * tPoliotaRaketiDGZPor;

//    qDebug()<<"assignGradysnikParams: zpr->maxValue_km:"<<zpr->maxValue_km<<"kyrsovojParameter_km:"<<snrInfoG.nextTocka[ii].kyrsovojParameter_km
//           <<"tPoliotaRaketiDGZPor:"<<tPoliotaRaketiDGZPor<<"dGZoniPyskaParametr_km:"<<snrInfoG.nextTocka[ii].dGZoniPyskaParametr_km;

    float a = groundSpeed_km_s * groundSpeed_km_s - VRAKETI * VRAKETI;

    float b =  - 2.0 * fabs(groundSpeed_km_s) * snrInfoG.nextTocka[ii].rangeParametr_km;

    float c = snrInfoG.nextTocka[ii].groundRange_km * snrInfoG.nextTocka[ii].groundRange_km;

    float discr = sqrt(b * b - 4 * a * c);

    //float x1 = (- b + discr)  / 2.0 / a;
    float x2 = (- b - discr)  / 2.0 / a;

    snrInfoG.nextTocka[ii].rangeParametrTV_km = snrInfoG.nextTocka[ii].rangeParametr_km - x2 * groundSpeed_km_s;

    if(groundSpeed_km_s != 0.0)
    {
        //snrInfoG.nextTocka[ii].podletnoeVrema = (snrInfoG.nextTocka[ii].rangeParametr_km / groundSpeed_km_s;
        snrInfoG.nextTocka[ii].podletnoeVrema = (snrInfoG.nextTocka[ii].rangeParametr_km - snrInfoG.nextTocka[ii].dGZoniPyskaParametr_km) / groundSpeed_km_s;
    }
    //qDebug()<<"assignGradysnikParams ii:"<<ii<<"rangeParametr_km"<<snrInfoG.nextTocka[ii].rangeParametr_km<<"groundSpeed_km_s:"<<groundSpeed_km_s<<"podletnoeVrema:"<<snrInfoG.nextTocka[ii].podletnoeVrema;

    //delete zpr;
}

void assignKlmSimulationTargetParams(KlmSimulatorData klmSimulatorData)
{
    if(klmSimulatorData.targetTotalNumber > MAXTOCKANUM)
        snrInfoG.totalSimTargetNum = MAXTOCKANUM;
    else
        snrInfoG.totalSimTargetNum = klmSimulatorData.targetTotalNumber;

    snrInfoG.klmControlByte = klmSimulatorData.controlByte;
    snrInfoG.klmTransactionId = klmSimulatorData.transactionId;

    for(int i = 0; i < snrInfoG.totalSimTargetNum; i++)
    {        
        snrInfoG.nextTocka[i].targetNo = klmSimulatorData.trgts[i].targetN;

        float az180 = klmSimulatorData.trgts[i].azimuth;
        if(az180 > 180.0)
            az180 -= 360.0;
        snrInfoG.nextTocka[i].kyrs = (float)klmSimulatorData.trgts[i].kyrs;

        snrInfoG.nextTocka[i].rakyrs = 180.0 - klmSimulatorData.trgts[i].kyrs + az180;
        snrInfoG.nextTocka[i].groundSpeed_ms = klmSimulatorData.trgts[i].speed;
        snrInfoG.nextTocka[i].radialSpeed_ms = snrInfoG.nextTocka[i].groundSpeed_ms * cos(snrInfoG.nextTocka[i].rakyrs / Rad2Grad);
        snrInfoG.nextTocka[i].azimuth360 = klmSimulatorData.trgts[i].azimuth;

        snrInfoG.nextTocka[i].seaLevelAltitude = klmSimulatorData.trgts[i].height;
        snrInfoG.nextTocka[i].radarRelativeAltitude = klmSimulatorData.trgts[i].height;

        snrInfoG.nextTocka[i].groundRange_km = klmSimulatorData.trgts[i].distance;
        snrInfoG.nextTocka[i].slantRange_km = sqrt(snrInfoG.nextTocka[i].groundRange_km * snrInfoG.nextTocka[i].groundRange_km +
            snrInfoG.nextTocka[i].radarRelativeAltitude * snrInfoG.nextTocka[i].radarRelativeAltitude);   //  временно для компиляции !!!

        snrInfoG.nextTocka[i].groundX = snrInfoG.nextTocka[i].groundRange_km * sin(az180 / Rad2Grad);
        snrInfoG.nextTocka[i].groundY = snrInfoG.nextTocka[i].groundRange_km * cos(az180 / Rad2Grad);

    //        qDebug()<<"targetSimulation: i:"<<i<<"snrInfoG.nextTocka.groundX:"<<snrInfoG.nextTocka[i].groundX<<"Y:"<<snrInfoG.nextTocka[i].groundY
    //                   <<"snrInfoG.nextTocka.azimuth360:"<<snrInfoG.nextTocka[i].azimuth360<<"az180:"<<az180;
//        qDebug()<<"assignKlmSimulationTargetParams: i:"<<i
//                   //<<"snrInfoG.nextTocka.groundX:"<<snrInfoG.nextTocka[i].groundX<<"Y:"<<snrInfoG.nextTocka[i].groundY
//                  <<"snrInfoG.nextTocka.groundRange_km:"<<snrInfoG.nextTocka[i].groundRange_km
//                   <<"snrInfoG.nextTocka.azimuth360:"<<snrInfoG.nextTocka[i].azimuth360;

        //  м е т о д   В О Р О Н И Н А
        float deltaH = (- R_ZemliEquiv + sqrt(R_ZemliEquiv * R_ZemliEquiv + snrInfoG.nextTocka[i].slantRange_km * snrInfoG.nextTocka[i].slantRange_km));

        float srednijUgol = 5.0;
        if(snrInfoG.nextTocka[i].slantRange_km != 0)
        {
            float asv = (snrInfoG.nextTocka[i].seaLevelAltitude - deltaH) / snrInfoG.nextTocka[i].slantRange_km;
//            if(asv > 1)
//                srednijUgol = 22.0;
//            if(asv < -1)
//                srednijUgol = 12.0;
//            if(-1 <= asv && asv <= 1)
                srednijUgol = asin(asv) * Rad2Grad;
        }

        snrInfoG.nextTocka[i].elevationAngle = srednijUgol;

        snrInfoG.nextTocka[i].uv11Angles = operatorPovorota(snrInfoG.nextTocka[i].azimuth360 - snrInfoG.currentAzimuth, snrInfoG.nextTocka[i].elevationAngle - snrInfoG.currentDirectrisaElevationAngle);

        //qDebug()<<"assignArtSimulationTargetParams: snrInfoG.nextTocka.azimuth360:"<<snrInfoG.nextTocka.azimuth360<<"snrInfoG.currentAzimuth:"<<snrInfoG.currentAzimuth;
        //snrInfoG.nextTocka.izmerTime = 0.001 * programmStartTime.elapsed();
        //snrInfoG.targetSimulationOn = shMemU2C.userCmds.startPointParams.targetSimulationOn;

        assignGradysnikParams(i);
    }
}


//float dAz = 0.0;
//int trd = 0;

void * ProcessSimulator(void * temp_pt )
{
    struct sockaddr_in udpSockAddr;
    int udpClientSocketFD;

    KlmSimulatorData klmSimulatorData;

//    struct sockaddr_in from;
//    socklen_t fromlen = sizeof(struct sockaddr_in);

    int counter = 0;

    // ИНИЦИАЛИЗАЦИЯ КЛИЕНТСКОГО UDP-СОКЕТА
    socklen_t udpSockAddrLength = sizeof(udpSockAddr);
    if ((udpClientSocketFD=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessSimulator UDP Client Socket failed "<<settings->ipAddressSimulator<<":"<<settings->portSimulator;//<<"connectStatus:"<<connectStatus;
    }

    udpSockAddr.sin_family = AF_INET;
    udpSockAddr.sin_port = htons(settings->portSimulator);
    udpSockAddr.sin_addr.s_addr = inet_addr(settings->ipAddressSimulator);  //"127.0.0.1");//

//    bind(udpClientSocketFD,(struct sockaddr *)&udpSockAddr, udpSockAddrLength);
//    static int so_broadcast = 1;
//    setsockopt(udpClientSocketFD, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof so_broadcast);
//    struct timeval tv;
//    tv.tv_sec = 1;
//    setsockopt(udpClientSocketFD, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

    fcntl(udpClientSocketFD, F_SETFL, O_NONBLOCK);

    qDebug()<<"ProcessSimulator UDP Client "<<settings->ipAddressSimulator<<":"<<settings->portSimulator;//<<"connectStatus:"<<connectStatus;

    ZRK2SimulatorData zrk2SimulatorData;
    //enum ZRKOPERATIONS {NET_DEISTVIJ, POISK, SOPROVOJDENIE_V_OBZORE, SOPROVOJDENIE, NAVEDENIE, VSTRECHA};

    zrk2SimulatorData.messageLength = 10;
    zrk2SimulatorData.targetTotalNumber = 1;
    zrk2SimulatorData.zrkTrgtOptions[0].targetN = 1;
    zrk2SimulatorData.zrkTrgtOptions[0].options = ZRKOPERATIONS::NET_DEISTVIJ;



    while (threadStatus->thrStatus_Simulator != cancel)
    {
        usleep(100 * 1000);

        zrk2SimulatorData.transactionId++;

        if(zrk2SimulatorData.transactionId % 10 == 0)
            qDebug()<<"ProcessSimulator "<<zrk2SimulatorData.transactionId<<"targetN:"<<zrk2SimulatorData.zrkTrgtOptions[0].targetN;

        if(snrInfoG.targetIndexCY >= 0 && snrInfoG.targetIndexCY < MAXTOCKANUM)
            zrk2SimulatorData.zrkTrgtOptions[0].targetN = snrInfoG.nextTocka[snrInfoG.targetIndexCY].targetNo;
        else
            zrk2SimulatorData.zrkTrgtOptions[0].targetN = 0;

        for(int j = 0; j < MAXPY; j++)  //for(int j = 1; j < 2; j++)//
        {
            for(int i = 0; i < MAXMISSILE; i++)   //for(int i = 0; i < 1; i++)
            {
                if(startInfoG.launcherInfo[j].missileInfo[i].missileStatus == MissileStatus::NAVEDENIE)
                {
                    if(zrk2SimulatorData.zrkTrgtOptions[0].options != ZRKOPERATIONS::VSTRECHA)
                    {
                        //float raketaVparam = snrInfoG.nextTocka[0].rangeParametr_km / sqrt(snrInfoG.nextTocka[0].rangeParametr_km *
                        //snrInfoG.nextTocka[0].rangeParametr_km + snrInfoG.nextTocka[0].kyrsovojParameter_km * snrInfoG.nextTocka[0].kyrsovojParameter_km) * VRAKETI;
                        //float xMissileR = raketaVparam * startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec;

                        if(startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec > 20.0)
                        //if(xMissileR < snrInfoG.nextTocka[0].rangeParametr_km && xMissileR + 0.2 > snrInfoG.nextTocka[0].rangeParametr_km)
                            zrk2SimulatorData.zrkTrgtOptions[0].options = ZRKOPERATIONS::VSTRECHA;
                        else
                            zrk2SimulatorData.zrkTrgtOptions[0].options = ZRKOPERATIONS::NAVEDENIE;
                    }

                    qDebug()<<"ProcessSimulator: i:"<<zrk2SimulatorData.transactionId<<"t_navedenie_sec:"<<startInfoG.launcherInfo[j].missileInfo[i].t_navedenie_sec
                            <<"options:"<<zrk2SimulatorData.zrkTrgtOptions[0].options;
                }
            }
        }

        sendto(udpClientSocketFD, &zrk2SimulatorData, zrk2SimulatorData.messageLength, 0, (struct sockaddr *) &udpSockAddr, udpSockAddrLength);

//        counter++;
//        sendto(udpClientSocketFD, &counter, sizeof(int), 0, (struct sockaddr *) &udpSockAddr, udpSockAddrLength);

        ssize_t totalReadBytes = 0;
        ssize_t nextReadBytes;
        int readCirclCounter = 0;
        uint16_t messageLength = 0;

        memset(&klmSimulatorData, 0, sizeof(KlmSimulatorData));        

        char readByteArray[sizeof(KlmSimulatorData)];        

        while (totalReadBytes < abs(sizeof(KlmSimulatorData)) && readCirclCounter++ < 10)        
        {
            nextReadBytes = recvfrom(udpClientSocketFD, readByteArray + totalReadBytes, sizeof(KlmSimulatorData) - totalReadBytes, 0, (struct sockaddr *) &udpSockAddr, &udpSockAddrLength);            

            if(nextReadBytes < 0)
                nextReadBytes = 0;

            totalReadBytes += nextReadBytes;

            if(totalReadBytes >= 8)
            {
                memcpy(&messageLength, readByteArray + sizeof(uint32_t), sizeof(uint16_t));

                if(totalReadBytes == messageLength)
                    break;
            }
        }

        if(messageLength >= 8 && totalReadBytes == messageLength)
        {
            memcpy(&klmSimulatorData, readByteArray, messageLength);

            assignKlmSimulationTargetParams(klmSimulatorData);

            qDebug()<<"Simulator Bytes:"<<totalReadBytes<<"mesLen:"<<messageLength
                    <<"trId:"<<klmSimulatorData.transactionId<<"controlByte:"<<klmSimulatorData.controlByte
                    <<"всего Ц:"<<klmSimulatorData.targetTotalNumber
                    <<"R:"<<klmSimulatorData.trgts[0].distance<<klmSimulatorData.trgts[1].distance<<klmSimulatorData.trgts[2].distance<<klmSimulatorData.trgts[254].distance
                    <<"Nц:"<<klmSimulatorData.trgts[0].targetN
                    <<"az:"<<klmSimulatorData.trgts[0].azimuth
                    <<"h:"<<klmSimulatorData.trgts[0].height
                    <<"k:"<<klmSimulatorData.trgts[0].kyrs
                    <<"V:"<<klmSimulatorData.trgts[0].speed
                    //<<"jammingPower:"<<klmSimulatorData.trgts[0].jammingPower
                    //<<"tip:"<<klmSimulatorData.trgts[0].tip
                      ;
        }

//<<<<<<<<<<<<<<<<<<<<<<<   тестовая цель летающая по кругу для проверки РС   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//            snrInfoG.maxTockaCurrentNum = 1;
//            snrInfoG.trackPointsNumber = trd++;
//            snrInfoG.nextTocka[0].targetNo = 234;
//            snrInfoG.nextTocka[0].elevationAngle = 3.0;
//            dAz += 0.01;
//            snrInfoG.nextTocka[0].azimuth360 = 130.0 + dAz;
//            snrInfoG.nextTocka[0].uv11Angles = operatorPovorota(snrInfoG.nextTocka[0].azimuth360 - snrInfoG.currentAzimuth,
//                                                                snrInfoG.nextTocka[0].elevationAngle - snrInfoG.currentDirectrisaElevationAngle);
//            snrInfoG.nextTocka[0].slantRange_km = 54.3;
//            qDebug()<<"snrInfoG.nextTocka[0].uv11Angles:"<<snrInfoG.nextTocka[0].uv11Angles.deltaF[0]<<snrInfoG.nextTocka[0].uv11Angles.deltaF[1];

    }

    qDebug()<<"ProcessSimulator canceled";

    return NULL;
}

