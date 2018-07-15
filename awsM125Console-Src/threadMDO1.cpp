#include <QDateTime>
#include <QDir>
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

extern float convert360angle2PlusMinus180(float dltAz360);

unsigned char mdoArray[sizeof(MDO1_echoData)];
unsigned char mdoTrgtData[sizeof(MDO1_incomingData)];

extern int dCntr;

extern MDO1_nextDirection *slideSignalValues;

//voronin-aa@zrk-voronin-aa:~$ sudo su
//[sudo] password for voronin-aa:
//root@zrk-voronin-aa:/home/voronin-aa# sh /etc/rc.local

int dCntr = 0;

void azimuthPakageDetector()
{

// проход по всем 48x200 угломестным пачкам


    if(oficerNaved2Console.guidanceOfficerCmds.writeDataContinues)
    {
        QString echoDirName = settings->programAbsolutePath + "/PackageData";

        QDir echoDir(echoDirName);
        if (!echoDir.exists())
        {
            echoDir.mkpath(".");
        }

        QDateTime now =  QDateTime::currentDateTime();
        QString dtStr = "/package_" + now.toString("yyyy_MM_dd__") + now.toString("hh_mm_ss_zzz");

        QString echoFileName = echoDirName +  dtStr + ".txt";

        //////////////////////
        FILE* fp;
        char file_name[100];
        strcpy(file_name,  echoFileName.toStdString().c_str());

        fp = fopen(file_name, "wt");//"a+t" );

        if(fp != NULL )
        {
            for(int j = 0; j < dCntr; j++)
            {
                fprintf(fp, "\n<<<   dCntr: %d   %3.2f° >>>\n", j, slideSignalValues[j].azimuth360);
                for(int i = 0; i < 4; i++)//for(int i = 0; i < 255; i++)
                {
                    //fwrite(&slideSignalValues[0].ARY, sizeof(uint8_t), 1, fp);
                    //fwrite(&i, sizeof(uint8_t), 1, fp);
                    //fprintf(fp, "\n<<<<<<<<<<<<<<<   Col: %d   >>>>>>>>>>>>>>>>>", i);

                    fprintf(fp, "%d ", slideSignalValues[j].targetData[i].ARY);
                }
            }
            fclose( fp );
        }
    }

//    for(int i = 0; i < dCntr / 10; i++)
//        qDebug()<<"azimuthPakageDetector i:"<<i * 10<<"ARY:"
//                <<slideSignalValues[i * 10 + 0].ARY<<slideSignalValues[i * 10 + 1].ARY<<slideSignalValues[i * 10 + 2].ARY<<slideSignalValues[i * 10 + 3].ARY<<slideSignalValues[i * 10 + 4].ARY
//                <<slideSignalValues[i * 10 + 5].ARY<<slideSignalValues[i * 10 + 6].ARY<<slideSignalValues[i * 10 + 7].ARY<<slideSignalValues[i * 10 + 8].ARY<<slideSignalValues[i * 10 + 9].ARY;

    memset(slideSignalValues, 0, MAX_DETECT_COUNT * sizeof(MDO1_nextDirection));
}


void * ProcessThreadMDO1( void * temp_pt )
{
    struct sockaddr_in clientAddr_in;

    int clientSocketMDO1;

    memset(&clientAddr_in,0,sizeof(clientAddr_in));
    clientAddr_in.sin_family=AF_INET;
    //clientAddr_in.sin_addr.s_addr = inet_addr("127.0.0.11");//"192.168.20.98");"192.168.3.250");// kontron "192.168.3.135");// lunapark
    char ipSrv[255];
    //strcpy(ipSrv,  ipAddrPort[0].toStdString().c_str());  //
    strcpy(ipSrv,  "10.5.0.3");
    clientAddr_in.sin_addr.s_addr = inet_addr(ipSrv);
    clientAddr_in.sin_port=htons(1999);
    //bool ok;
    //clientAddr_in.sin_port = htons(ipAddrPort[1].toUShort(&ok, 10));

    clientSocketMDO1=socket(PF_INET, SOCK_DGRAM, 0);//SOCK_NONBLOCK);
    fcntl(clientSocketMDO1, F_SETFL, O_NONBLOCK);

    int serverSocketMDO1;
    struct sockaddr_in serverAddr_in;

    memset(&serverAddr_in,0,sizeof(serverAddr_in));

    serverAddr_in.sin_family=AF_INET;
    serverAddr_in.sin_port=htons(2000);
    serverAddr_in.sin_addr.s_addr=INADDR_ANY;

    serverSocketMDO1=socket(PF_INET,SOCK_DGRAM,0);
    fcntl(serverSocketMDO1, F_SETFL, O_NONBLOCK);
    bind(serverSocketMDO1,(struct sockaddr *)&serverAddr_in,sizeof(serverAddr_in));

    //struct sockaddr addrFromClient;
    //unsigned int lengthFromClient = sizeof(addrFromClient);

    //MDO1_incomingData nextMDO1Info;

    while(1)
    {
        usleep(12 * 1000);

        sendto(clientSocketMDO1, &oficerNaved2Console.mdo1Cmds[0], sizeof(MDO1_commands), 0, (sockaddr *)&clientAddr_in, sizeof(clientAddr_in));//2016-10-26
        MDO1_commands copy;
        memcpy(&copy, &oficerNaved2Console.mdo1Cmds[0],sizeof(MDO1_commands ));
        //recvfrom(serverSocketMDO1, &shMemC2U.mdo1Info[0], sizeof(MDO1_incomingData), 0, &addrFromClient, &lengthFromClient);//  2017-04-21
        //recvfrom(serverSocketMDO1, &nextMDO1Info, sizeof(MDO1_incomingData), 0, &addrFromClient, &lengthFromClient);//  2017-12-28

        memset(mdoTrgtData, 0, sizeof(MDO1_incomingData));

        int totalReadBytes = 0;
        ssize_t nextReadBytes;
        int readCirclCounter = 0;

        while (totalReadBytes < sizeof(MDO1_incomingData) && readCirclCounter++ < 1000)
        {
            //nextReadBytes = recv(serverSocket, mdoArray + totalReadBytes, sizeof(MDO1_echoData) - totalReadBytes, 0);

            nextReadBytes = recv(serverSocketMDO1, mdoTrgtData + totalReadBytes, sizeof(MDO1_incomingData) - totalReadBytes, 0);

            //recvfrom(serverSocketMDO1, &nextMDO1Info, sizeof(MDO1_incomingData), 0, &addrFromClient, &lengthFromClient);//  2017-12-28

            if(nextReadBytes < 0)
                nextReadBytes = 0;

            totalReadBytes += nextReadBytes;
        }

//        while (totalReadBytes < (sizeof(USM_MDO_header) + sizeof(USM_MDO_timeFrame)) && readCirclCounter++ < 1000)
//        {
//            //nextReadBytes = recv(serverSocket, mdoArray + totalReadBytes, sizeof(MDO1_echoData) - totalReadBytes, 0);

//            nextReadBytes = recv(serverSocketMDO1, mdoTrgtData + totalReadBytes, (sizeof(USM_MDO_header) + sizeof(USM_MDO_timeFrame)) - totalReadBytes, 0);

//            //recvfrom(serverSocketMDO1, &nextMDO1Info, sizeof(MDO1_incomingData), 0, &addrFromClient, &lengthFromClient);//  2017-12-28

//            if(nextReadBytes < 0)
//                nextReadBytes = 0;

//            totalReadBytes += nextReadBytes;
//        }


        //memcpy(&nextMDO1Info, mdoTrgtData, sizeof(MDO1_incomingData));

        int mdoInd = -1;

        switch(mdoTrgtData[2])//switch(nextMDO1Info.header.deviceAddress)
        {
            case 0x21: mdoInd = 0; break;
            case 0x22: mdoInd = 1; break;
            case 0x23: mdoInd = 2;
        }

//        if(mdoInd == -1)
//            qDebug()<<"ProcessMDO1: deviceAddress:"<<mdoTrgtData[2]<<"mdoInd:"<<mdoInd;

        if(mdoInd != -1)
            memcpy(&console2OficerNaved.mdo1Info[mdoInd], mdoTrgtData, sizeof(MDO1_incomingData));

        // test         test         test         test         test         test         test
        // test         test         test         test         test         test         test
        // test         test         test         test         test         test         test

//        uint16_t elevAngle;
//        memcpy(&elevAngle, console2OficerNaved.mdo1Info[0].targetData[0].elevationYV10, sizeof(uint16_t));

        memset(&console2OficerNaved.mdo1Info[0].targetData[0], 0, sizeof(MDO1_targetData));

        float deltaAz = fabs(convert360angle2PlusMinus180(135.0 - snrInfoG.currentAzimuth));

        float maxDelta = 2.0;// Rad2Grad 2.0

        if(deltaAz < maxDelta)//1.0)
        {
            if(deltaAz == 0.0)
                console2OficerNaved.mdo1Info[0].targetData[0].ARY = 255;    //dCntr;//
            else
                console2OficerNaved.mdo1Info[0].targetData[0].ARY = (uint8_t)(255.0 * sin(deltaAz * M_PI / maxDelta) / deltaAz / M_PI * maxDelta);  //dCntr;//
            //qDebug()<<"ARY:"<<console2OficerNaved.mdo1Info[0].targetData[0].ARY;
        }

        memcpy(&slideSignalValues[dCntr].targetData[0], &console2OficerNaved.mdo1Info[0].targetData, sizeof(MDO1_targetData) * 16);
        //memcpy(&slideSignalValues[dCntr].targetData[16], &console2OficerNaved.mdo1Info[0].targetData, sizeof(MDO1_targetData) * 16);
        slideSignalValues[dCntr].azimuth360 = snrInfoG.currentAzimuth;

        dCntr++;
        if(dCntr >= MAX_DETECT_COUNT)
            dCntr = 0;

        //qDebug()<<"dCntr:"<<dCntr;

        // test         test         test         test         test         test         test

            //memcpy(&shMemC2U.mdo1Info[mdoInd], &nextMDO1Info, sizeof(MDO1_incomingData));

        //qDebug()<<"ProcessMDO1:";

    };

    close(clientSocketMDO1);
    close(serverSocketMDO1);
}

//bool writeData = false;

int ind = 0;


void * ProcessThreadMDO1Echo( void * temp_pt )
{
    int serverSocket;
    struct sockaddr_in serverAddr_in;//, client={AF_INET,INADDR_ANY,INADDR_ANY};

    memset(&serverAddr_in,0,sizeof(serverAddr_in));

    serverAddr_in.sin_family=AF_INET;
    serverAddr_in.sin_port=htons(2001);
    serverAddr_in.sin_addr.s_addr=INADDR_ANY;
//    char ipSrv[255];
//    ////////////////strcpy(ipSrv,  settings->ipAddressARMVL.toStdString().c_str());    //strcpy(ipSrv,  "192.168.3.250");//    strcpy(ipSrv,  "192.168.21.185");
//    serverAddr_in.sin_addr.s_addr = inet_addr(ipSrv);

    serverSocket=socket(PF_INET,SOCK_DGRAM,0);
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);   //  без плат МДО раскомментировать
    bind(serverSocket,(struct sockaddr *)&serverAddr_in,sizeof(serverAddr_in));

    struct sockaddr addrFromClient;
    unsigned int lengthFromClient = sizeof(addrFromClient);

    //if(false)
    while(1)
    {
        usleep(10 * 1000);

        memset(mdoArray, 0, sizeof(MDO1_echoData));

        int totalReadBytes = 0;
        ssize_t nextReadBytes;
        int readCirclCounter = 0;

        while (totalReadBytes < sizeof(MDO1_echoData) && readCirclCounter++ < 1000)
        {
            //nextReadBytes = recvfrom(serverSocket, &shMemC2U.mdo1Echo.x55 + totalReadBytes, sizeof(MDO1_echoData) - totalReadBytes, 0, &addrFromClient, &lengthFromClient);

            //nextReadBytes = recvfrom(serverSocket, mdoArray + totalReadBytes, sizeof(MDO1_echoData) - totalReadBytes, 0, &addrFromClient, &lengthFromClient);
            nextReadBytes = recv(serverSocket, mdoArray + totalReadBytes, sizeof(MDO1_echoData) - totalReadBytes, 0);

            if(nextReadBytes < 0)
                nextReadBytes = 0;

            //qDebug()<<"CnvEth485 nextReadBytes:"<<nextReadBytes;

            totalReadBytes += nextReadBytes;

            //rsltStr += QString("crclN: %1  nextReadBytes: %2  totalReadBytes: %3  ").arg(crclN++).arg(nextReadBytes).arg(totalReadBytes);
        }

        if(totalReadBytes == sizeof(MDO1_echoData))
        {

//            for(int i = 0; i < sizeof(MDO1_echoData); i++)
//                mdoArray[i] = i % 255;

            memcpy(&console2DiagnosticMonitor.mdo1Echo, mdoArray, sizeof(MDO1_echoData));

//            qDebug()<<"ProcessMDO1Echo totalReadBytes:"<<totalReadBytes<<shMemC2U.mdo1Echo.header.x55<<shMemC2U.mdo1Echo.header.xAA
//               <<shMemC2U.mdo1Echo.header.deviceAddress<<"   "<<shMemC2U.mdo1Echo.echo[0][0]<<shMemC2U.mdo1Echo.echo[0][1]<<shMemC2U.mdo1Echo.echo[0][2]
//                    <<shMemC2U.mdo1Echo.echo[0][3]<<shMemC2U.mdo1Echo.echo[0][4]<<shMemC2U.mdo1Echo.echo[0][5]<<shMemC2U.mdo1Echo.echo[0][6]
//                    <<shMemC2U.mdo1Echo.echo[0][7]<<shMemC2U.mdo1Echo.echo[0][8]<<shMemC2U.mdo1Echo.echo[0][9]<<shMemC2U.mdo1Echo.echo[0][10]<<" ... "
//                    <<shMemC2U.mdo1Echo.echo[0][252]<<shMemC2U.mdo1Echo.echo[0][253]<<shMemC2U.mdo1Echo.echo[0][254]<<shMemC2U.mdo1Echo.echo[0][255];
        }
//        else
//            qDebug()<<"ProcessMDO1Echo totalReadBytes:"<<totalReadBytes;


        //totalReadBytes = recvfrom(serverSocket, &shMemC2U.mdo1Echo, sizeof(MDO1_echoData), 0, &addrFromClient, &lengthFromClient);//  2017-04-21

//        int timeIndex =shMemC2U.mdo1Info[0].timeFrame.lowByte + shMemC2U.mdo1Info[0].timeFrame.middleByte * 256 + shMemC2U.mdo1Info[0].timeFrame.highByte * 256 * 256;
//        int echoInd = -1;

//        for(int i = 0; i < ECHO_LENGTH; i++)
//        {
//            if(shMemC2U.mdo1Echo.echo[0][i] > 0xf0)
//            {
//                echoInd = i;
//                break;
//            }
//        }

//        shMemC2U.mdo1Echo.echo[0][ind    ] = 0x80; //  только для теста
//        shMemC2U.mdo1Echo.echo[1][ind / 2] = 0x80; //  только для теста
//        shMemC2U.mdo1Echo.echo[2][ind / 4] = 0x80; //  только для теста
//        if(ind++ >= ECHO_LENGTH)
//            ind = 0;
/*
        if(oficerNaved2Console.guidanceOfficerCmds.writeDataSingleShort)    //if(writeData)
        {
           // writeData = false;
            FILE* fp;
            char file_name[100];
            sprintf(file_name, "/home/voronin-aa/SignalData/mdo1Data.txt");//_%d_%d.txt", azimuth, recordCounter);
            //fp = fopen("/home/voronin/SignalData/dasqData33.txt", "wt" );
            fp = fopen(file_name, "wt");

            //fp = fopen(file_name, "wt");//"a+t" );
            if(fp != NULL )
            {
                fwrite(console2DiagnosticMonitor.mdo1Echo.echo[0], ECHO_LENGTH, 1, fp);
                //short int vl = 255;
                //fwrite(&vl, 2, 1, fp);
                //fwrite(shared_memoryTemp, ScanCount * GroupCount * RecordCount * sizeof(unsigned short), 1, fp);
                //int val = 234;
                //fprintf(fp, "qwerty: %d", val);
                //fwrite(shared_memoryTemp, ScanCount * GroupCount * RecordCount * sizeof(unsigned short), 1, fp);
                fclose( fp );
            }
            //qDebug()<<"ProcessMDO1Echo: file_name:"<<file_name<<"fp:"<<fp;
        }

        if(oficerNaved2Console.guidanceOfficerCmds.writeDataContinues)
        {
            QString echoDirName = "/home/voronin-aa/SignalData/Echos";

            QDir echoDir(echoDirName);
            if (!echoDir.exists())
            {
                echoDir.mkpath(".");
            }

            QDateTime now =  QDateTime::currentDateTime();
            QString dtStr = "/echo_" + now.toString("yyyy_MM_dd__") + now.toString("hh_mm_ss_zzz");

            QString echoFileName = echoDirName +  dtStr + ".txt";

            //////////////////////
            FILE* fp;
            char file_name[100];
            strcpy(file_name,  echoFileName.toStdString().c_str());

            fp = fopen(file_name, "wt");//"a+t" );

            if(fp != NULL )
            {
                fwrite(console2DiagnosticMonitor.mdo1Echo.echo[0], ECHO_LENGTH, 1, fp);
                fclose( fp );
            }
        }
        */

    };

    close(serverSocket);
}
