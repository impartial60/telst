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
#include "srcKram/Drive125.h"



void * ProcessThreadGuidanceOfficer( void * temp_pt )
{
    //qDebug()<<"ProcessThreadGuidanceOfficer 1";

    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    socklen_t clilen = sizeof(si_other);

    bool connectStatus = true;



    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"ProcessThreadGuidanceOfficer: socket == -1";
        connectStatus = false;//die("socket");
    }
    fcntl(s, F_SETFL, O_NONBLOCK);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(settings->port_awsGuidanceOfficer);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if(connectStatus &&  bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        connectStatus = false;//die("bind");

        qDebug()<<"ProcessThreadGuidanceOfficer: bind == -1";
    }

    int oficerNavedCounterOld = 0;
    int oficerNavedCounterNew = 0;

    //keep listening for data

    qDebug()<<"ProcessThreadGuidanceOfficer UDP Server port:"<<settings->port_awsGuidanceOfficer<<"connectStatus:"<<connectStatus;

    while(connectStatus)
    {
        usleep(35  * 1000);
        ////////////printf("Waiting for data...");
        ////////////fflush(stdout);

        //try to receive some data, this is a blocking call
        //memcpy(&shMemU2C, shared_memory, sizeof(AWSG_User2Console));

        //if ((recv_len = recvfrom(s, &oficerPyska2Console, sizeof(KOMANDI_OFICERA_PYSKA), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        if ((recv_len = recvfrom(s, &oficerNaved2Console, sizeof(KOMANDI_OFICERA_NAVED), 0, (struct sockaddr *) &si_other, &clilen)) == -1)
        {
            //connectStatus = false;//die("recvfrom()");
        }

        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));   //printf("Data: %s\n" , buf);

        //memcpy(&console2OficerPyska.snrInfo, &snrInfoG, sizeof(SNR_INFO));
        //memcpy(&console2OficerPyska.startInfo, &startInfoG, sizeof(START_INFO));

        memcpy(&console2OficerNaved.snrInfo, &snrInfoG, sizeof(SNR_INFO));
        memcpy(&console2OficerNaved.komKom, &komKomM, sizeof(KOMANDI_KOMANDIRA));

        if (recv_len > 0)//sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            //////////////////die("sendto()");

            //pthread_mutex_lock(&localConsole_memory_Mutex);

                //ssize_t totalSentBytes =
                sendto(s, &console2OficerNaved, sizeof( Console2OficerNavedenia), 0, (struct sockaddr*) &si_other, slen);

                //qDebug()<<"ProcessThreadDiagnMonitor: totalSentBytes:"<<totalSentBytes<<"sizeof(AWSG_Console2User):"<<sizeof(AWSG_Console2User);

            //pthread_mutex_unlock(&localConsole_memory_Mutex);
        }

        if(oficerNavedCounterOld != oficerNavedCounterNew)
        {
            //qDebug()<<"ProcessThreadDiagnosticMonitor: diagnosticMonitorCounterNew"<<diagnosticMonitorCounterNew;
            oficerNavedCounterOld = oficerNavedCounterNew;
        }

        //qDebug()<<"ProcessSharedMemoryReadWrite: shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter:"<<shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter;

        servoDriveExecuter();

        if(snrInfoG.rejimPoiskaSNR == POISK_PEREKLYCHEN_V_SOPROVOJDENIE)
            rsTrgtPrmsCalculator();

        //targetSimulation();

        //lphCalculator();

        //qDebug()<<"ProcessSharedMemoryReadWrite: snrInfoG.currentAzimuth:"<<snrInfoG.currentAzimuth<<"Time:"<<0.001 * programmStartTime.elapsed();      //  Burtzev

        ///////////////////////////////////////////////////////////////////////////////////////snrInfoG.showSimulationTrack = shMemU2C.guidanceOfficerCmds.showSimulationTrack;
    }

    close(s);
    return temp_pt;
}

/*
void * ProcessSharedMemoryReadWrite(void * temp_pt )
{
    while(!clear_op)
    {
        // T E S T
//        if(sample++ > 255)
//            sample = 0;

//        shMemC2U.mdo1Echo.echo[0][100] = sample;

//        if(sample > 0 )
//            shMemC2U.mdo1Echo.echo[0][sample - 1] = 0;

//        shMemC2U.mdo1Echo.echo[0][sample] = 128;

//        snrInfo.currentAzimuth += 0.1;
//        if(snrInfo.currentAzimuth > 360.0)
//            snrInfo.currentAzimuth = 0;

//        pthread_mutex_lock(&localConsole_memory_Mutex);
//            memcpy(localConsole_memory, &snrInfo, sizeof(SNR_INFO));
//        pthread_mutex_unlock(&localConsole_memory_Mutex);

        //snrInfo.totalReadBytes = -10;

        usleep(35 * 1000);

        memcpy(&shMemC2U.komKom, &komKomG, sizeof(KOMANDI_KOMANDIRA));//memcpy(& commanderConsole2InfoTable.startInfo, &startInfoG, sizeof(START_INFO));
        memcpy(&shMemC2U.snrInfo, &snrInfoG, sizeof(SNR_INFO));
//        qDebug()<<"ProcessSharedMemoryReadWrite: shMemC2U.snrInfo.handWheelFSpeed[0]:"<<shMemC2U.snrInfo.handWheelFSpeed[0]
//               <<"snrInfoG.handWheelFSpeed[0]:"<<snrInfoG.handWheelFSpeed[0];

        int localWait = 0;
        bool zahvat = true;

        while(binary_semaphore_wait(semaphore_id) < 0)
        {
            if (++localWait > 5)
            {
                zahvat = false;

                break;
            }
            usleep(5 * 1000);
        }

        if(localWait > 0)
            printf("ironBtns: localWait:%d\n", localWait);
        //qDebug()<<"cnvrtEth485 ProcessThreadSchnaider: zahvat:"<<zahvat<<"cntr"<<cntr<<"tcpSocketFileDescriptor:"<<tcpSocketFileDescriptor<<"connectionStatus:"<<connectionStatus<<"sz:"<<sz<<"n:"<<n;

        if(zahvat)
        {
            shared_memory = (unsigned char*) shmat(segment_id, 0, 0);

            if(shared_memory == (unsigned char*)0xffffffff)
            {
               binary_semaphore_post(semaphore_id);
               //qDebug()<<"dasq sharedMemoryReadWrite(): shared_memory == (float*)0xffffffff";
               zahvat = false;
               continue;
            }

            //floatConsoleCounter += 0.001;

            memcpy(&shMemU2C, shared_memory, sizeof(AWSG_User2Console));

            if(shMemU2C.stopConsolePrgrm == 0xFF)
            {
                qDebug()<<"cnvrtEth485 clear_op = true;";
                clear_op = true;
            }

            //int timeFrameIncrNew = (int)(shMemU2C.mdo1Cmds[0].timeFrame.lowByte + shMemU2C.mdo1Cmds[0].timeFrame.middleByte * 256 + shMemU2C.mdo1Cmds[0].timeFrame.highByte * 256 * 256);

            //qDebug()<<"ProcessSharedMemoryReadWrite: timeFrameIncrNew"<<timeFrameIncrNew<<shMemU2C.mdo1Cmds[0].timeFrame.lowByte<<shMemU2C.mdo1Cmds[0].timeFrame.middleByte
              //     <<shMemU2C.mdo1Cmds[0].timeFrame.highByte;

            //if(timeFrameIncrNew > timeFrameIncrOld)
            {
                //timeFrameIncrOld = timeFrameIncrNew;

                // for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only
                //memset(&shMemC2U.mdo1Echo.echo[0], 0x00, ECHO_LENGTH);
//                if(ind++ > 8000)
//                    ind = 0;
                //shMemC2U.mdo1Echo.echo[0][ind] = 0x80;

                snrInfoG.totalReadBytes = sizeof(USM_Info);

//                shMemC2U.mdo1Info[0].header.x55 = 0x55;
//                shMemC2U.mdo1Info[0].header.xAA = 0xAA;
//                shMemC2U.mdo1Info[0].header.deviceAddress = 0x21;

//                shMemC2U.mdo1Info[1].header.deviceAddress = 0x22;

                shMemC2U.usmInfo.sync[0] = 0x55;
                shMemC2U.usmInfo.sync[1] = 0xAA;
                shMemC2U.usmInfo.sync[2] = 0x03;

                shMemC2U.usmInfo.sync[3] = 0x01;
                shMemC2U.usmInfo.sync[4] = 0x2D;
                shMemC2U.usmInfo.sync[5] = 0x2D;

                shMemC2U.usmInfo.sync[6] = 0x2D;
                shMemC2U.usmInfo.sync[7] = 0xFF;
                shMemC2U.usmInfo.sync[8] = 0x2D;
                // for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only     for test only

                pthread_mutex_lock(&localConsole_memory_Mutex);
                    memcpy(shared_memory + sizeof(AWSG_User2Console), &shMemC2U, sizeof(Console2OficerNavedenia));    //memcpy(shared_memory,   localConsole_memory, sizeof(SNR_INFO));
                pthread_mutex_unlock(&localConsole_memory_Mutex);
            }

            shmdt(shared_memory);
            binary_semaphore_post(semaphore_id);
        }

        //qDebug()<<"ProcessSharedMemoryReadWrite: shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter:"<<shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter;

        servoDriveExecuter();        

        if(snrInfoG.rejimPoiskaSNR == POISK_PEREKLYCHEN_V_SOPROVOJDENIE)
            rsTrgtPrmsCalculator();

        //targetSimulation();

        //lphCalculator();

        //qDebug()<<"ProcessSharedMemoryReadWrite: snrInfoG.currentAzimuth:"<<snrInfoG.currentAzimuth<<"Time:"<<0.001 * programmStartTime.elapsed();      //  Burtzev

        ///////////////////////////////////////////////////////////////////////////////////////snrInfoG.showSimulationTrack = shMemU2C.guidanceOfficerCmds.showSimulationTrack;

    }
    return temp_pt;
}
*/
/*
int lphCntr = 0;

float vesAz = 0.1;
float vesE = 0.1;


void lphCalculator()
{
    if(currentUNVScanIsOn)
    {
        snrInfoG.nextTocka.soprovojdCoord.l = -1.0;
        snrInfoG.nextTocka.soprovojdCoord.p = -1.0;
        snrInfoG.nextTocka.soprovojdCoord.h = -1.0;
    }
    else
    {
        if(lphCntr++ % 30 == 0)
        {
            float az = (vesAz * snrInfoG.currentAzimuth + snrInfoG.nextTocka.azimuth360) / (vesAz + 1.0)  / Rad2Grad;

            float newX = snrInfoG.nextTocka.groundRange_km * sin(az);
            float newY = snrInfoG.nextTocka.groundRange_km * cos(az);

//            float newX = snrInfoG.nextTocka.groundRange_km * sin(snrInfoG.currentAzimuth / Rad2Grad);//snrInfoG.nextTocka.groundX;//
//            float newY = snrInfoG.nextTocka.groundRange_km * cos(snrInfoG.currentAzimuth / Rad2Grad);//snrInfoG.nextTocka.groundY;//
            //float newX = snrInfoG.nextTocka.groundRange_km * sin(snrInfoG.nextTocka.azimuth360 / Rad2Grad);
            //float newY = snrInfoG.nextTocka.groundRange_km * cos(snrInfoG.nextTocka.azimuth360 / Rad2Grad);

            if(oldX != 0 && oldY != 0 && oldX != newX && oldY != newY)
            {
                float newKyrs = atan2(newX - oldX, newY - oldY) * Rad2Grad;

                if(newKyrs < 0)
                    newKyrs += 360.0;

//                qDebug()<<"b:"<<snrInfoG.currentAzimuth<<snrInfoG.nextTocka.azimuth360<<"dGor:"<<snrInfoG.nextTocka.groundRange_km<<"dX:"<<newX - oldX<<"dY:"<<newY - oldY
//                       <<"newKyrs:"<<newKyrs<<"newX:"<<newX<<snrInfoG.nextTocka.groundX<<"newY:"<<newY<<snrInfoG.nextTocka.groundY;

                float rakyrs = 180.0 - newKyrs + snrInfoG.currentAzimuth;

                qDebug()<<"dGor:"<<snrInfoG.nextTocka.groundRange_km<<"        newX:"<<newX<<snrInfoG.nextTocka.groundX<<"         newY:"<<newY<<snrInfoG.nextTocka.groundY
                <<"        dX:"<<newX - oldX<<snrInfoG.nextTocka.groundX - oldX_ist<<"        dY:"<<newY - oldY<<snrInfoG.nextTocka.groundY - oldY_ist
                <<"newKyrs:"<<newKyrs<<"rakyrs:"<<rakyrs;

                ///////////////snrInfoG.nextTocka.rakyrs = 180.0 - snrInfoG.nextTocka.kyrs + az180;  //  2017-05-13

                //float rangeTempKm = kmPerDiscret * trgt[n]->Col;//newDlCY;//
                //float angleTemp = snrInfoG.currentDirectrisaElevationAngle;//(slideAngles + trgt[n]->Row);//srednijUgol;

                //  м е т о д   В О Р О Н И Н А
                float dGorizont_m = 1000.0 * snrInfoG.nextTocka.groundRange_km;//rangeTempKm * cos(angleTemp  * 3.1415926 / 180.0);
                float deltaH = 0;//(- R_ZemliEquiv + sqrt(R_ZemliEquiv * R_ZemliEquiv + snrInfoG.nextTocka.groundRange_km * snrInfoG.nextTocka.groundRange_km));
                //trgt[n]->Altitude = 10.0 * (rangeTempKm * sin(angleTemp  * 3.1415926 / 180.0) + deltaH) + 0.01 * settings->seaLevel; // Гектометры

                snrInfoG.nextTocka.soprovojdCoord.h = 1000.0 * (snrInfoG.nextTocka.slantRange_km * sin(snrInfoG.currentDirectrisaElevationAngle / Rad2Grad) + deltaH);
                // + settings->seaLevel;



                //float deltaPrm = snrInfoG.nextTocka.rakyrs / Rad2Grad; //float deltaPrm = rakyrs_grad / Rad2Grad;
                float deltaPrm = rakyrs / Rad2Grad; //float deltaPrm = rakyrs_grad / Rad2Grad;
                snrInfoG.nextTocka.soprovojdCoord.p = 1000.0 * snrInfoG.nextTocka.groundRange_km * sin(deltaPrm);//kyrsovojParameter_km = rangeHorizont_km * sin(deltaPrm);

                snrInfoG.nextTocka.soprovojdCoord.l = sqrt(dGorizont_m * dGorizont_m - snrInfoG.nextTocka.soprovojdCoord.p * snrInfoG.nextTocka.soprovojdCoord.p);

                //rangeParametr_km = sqrt(rangeHorizont_km * rangeHorizont_km - kyrsovojParameter_km * kyrsovojParameter_km);
            }

            oldX = newX;
            oldY = newY;

            oldX_ist = snrInfoG.nextTocka.groundX;
            oldY_ist = snrInfoG.nextTocka.groundY;
        }
    }
}
*/
