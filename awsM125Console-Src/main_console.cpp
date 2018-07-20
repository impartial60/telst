#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <QThread>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>


#include  "srcKram/Drive125.h"
#include "common/sharedMem.h"
#include "common/sys.h"

#include "threadstatus.h"
#include "mainConsoleSettings.h"

//all definition for mechanical
#define max_vel_const  5000.0  /* в делениях выходного энкодера в секунду ( 44(ед/оборот) * 100(об/сек) = 4400 --- 6000 об/мин )*/
#define max_accel_const 88000.0


pthread_t pthreadAll;
void* ret_val_All_Threads;
void * Process_All_Threads( void * temp_pt );

MDO1_nextDirection *slideSignalValues;

pthread_mutex_t localConsole_memory_Mutex;

ThreadStatus *threadStatus;
int restartCntr = 0;

//tp_t p1,p2;

bool komandirPereklychilTrenaj = false;
KOMANDI_KOMANDIRA komKomM;
KOMANDI_OFICERA_NAVED oficerNaved2Console;
KOMANDI_OFICERA_PYSKA oficerPyska2Console;

Console2OficerNavedenia console2OficerNaved;
Console2DiagnosticMonitor console2DiagnosticMonitor;

Console2OficerPyska console2OficerPyska;
Console2InfoTableKomandira console2InfoTableKomandira;

SNR2SYS snr2sys;
SYS2SNR sys2snr;
START_INFO startInfoG;
VKP2ZRK vkp2zrkG;
P18_2ZRK p18_2ZRKG;
SNR_INFO snrInfoG;

QTime programmStartTime;
SettingsConsoleM *settings;
ZP zonaPorazh;

extern pid_t proc_find(const char* name, long secondPid);

int main_console(int argc, char *argv[])
{    


    (void)argc; (void)argv;




    //   П Р О В Е Р К А   Н А   Е Д И Н С Т В Е Н Н О С Т Ь   З А П У Щ Е Н Н О Г О   Э К З Е М П Л Я Р А   П Р О Г Р А М М Ы
    pid_t own_pid = getpid();           //  это пид самой себя
    printf("awsM125Console own pid: % d\n", own_pid);

    pid_t pid = proc_find("awsM125Console", own_pid);
    if(pid >= 0)    // найден старый работающий экземпляр программы
    {
        printf("!!!OLD awsM125Console DETECTED!!! pid: %d - it will be killed\n", pid);
        kill(pid, SIGTERM);
        sleep(1);
    }
    else
        printf("OLD awsM125Console NOT FOUND - it`s Ok...\n");

    settings = new SettingsConsoleM(&snrInfoG.unvGeoPosition);
    //qDebug()<<"***  snrInfoG.unvGeoPosition:"<<snrInfoG.unvGeoPosition.geoY.integerDegr<<snrInfoG.unvGeoPosition.geoY.integerMin<<snrInfoG.unvGeoPosition.geoY.integerSec<<snrInfoG.unvGeoPosition.geoY.integerSec100
    //      <<snrInfoG.unvGeoPosition.geoX.integerDegr<<snrInfoG.unvGeoPosition.geoX.integerMin<<snrInfoG.unvGeoPosition.geoX.integerSec<<snrInfoG.unvGeoPosition.geoX.integerSec100;

    programmStartTime.start();

    snrInfoG.totalSimTargetNum = 1;

    komKomM.targetIndexCY = -1;
    //snrInfoG.trackPointsNumber = 0;

    snrInfoG.rejimPoiskaSNR = POISK_VIKLUCHEN;
    snrInfoG.rejimSoprovojdeniaSNR = SOPROVOJDENIE_AS;

    slideSignalValues = (MDO1_nextDirection *)calloc(MAX_DETECT_COUNT, sizeof(MDO1_nextDirection));  //  105600 байт
    qDebug()<<"sizeof(slideSignalValues):"<<sizeof(MDO1_nextDirection) * MAX_DETECT_COUNT;

    oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR = RejimPoiskaSNR::POISK_VIKLUCHEN;

    threadStatus = new ThreadStatus();

    pthread_mutex_init(&localConsole_memory_Mutex, NULL);    

    snrInfoG.directrisaAzimuth = 133.0;
    snrInfoG.currentAzimuth = snrInfoG.directrisaAzimuth;
    snrInfoG.currentDirectrisaElevationAngle = 5.34;

    snrInfoG.ironBtnsG.isHoldingDown = false;

  //  drive_unit = new Drive125;
 //   printf("Servo drive controll init\n");

//    p1.active=0;
//    p1.curr_pos=snrInfoG.currentAzimuth;
//    p1.curr_vel=0;
//    p1.enable=1;
//    p1.max_acc=max_accel_const;
//    p1.max_vel=max_vel_const;
//    p1.pos_cmd=snrInfoG.currentAzimuth;
//    p2.active=0;
//    p2.curr_pos=snrInfoG.currentDirectrisaElevationAngle;
//    p2.curr_vel=0;
//    p2.enable=1;
//    p2.max_acc=max_accel_const;
//    p2.max_vel=max_vel_const;
//    p2.pos_cmd=snrInfoG.currentDirectrisaElevationAngle;

    pthread_create( &pthreadAll, NULL, Process_All_Threads, NULL );
    qDebug()<<"pthreadAll: pthread_create";

    pthread_join( pthreadAll, &ret_val_All_Threads);
    qDebug()<<"pthreadAll: pthread_join";

    return 123;//a.exec();
}
