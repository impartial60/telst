#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "extern.h"



void * Process_All_Threads(void *temp_pt)
{    
    //printf("Process_All_Threads...\n");

    pthread_t pthreadVKP;
    pthread_t pthreadP18;
    pthread_t pthread_LauncherOf;
    pthread_t pthread_Commander;
    pthread_t pthreadSYS;
    pthread_t pthreadCnvEth485;
    pthread_t pthreadSimulator;
    pthread_t pthreadSharedMem;
    pthread_t pthreadDiagnMonitor;
    pthread_t pthreadOperF1;
    pthread_t pthreadOperF2;
    pthread_t pthreadMDO1;
    pthread_t pthreadMDO1Echo;
    pthread_t pthread_awsInfoTable;

    pthread_t pthread_servodrive;

    void* ret_valVKP;
    void* ret_valP18;
    void* ret_valLauncherOf;
    void* ret_valCommander;
    void* ret_valSYS;
    void* ret_valCnvEth485;
    void* ret_valSimulator;
    void* ret_valSharedMem;
    void* ret_valDiagnMonitor;
    void* ret_valOperF1;
    void* ret_valOperF2;
    void* ret_valMDO1;
    void* ret_valMDO1Echo;
    void* ret_val_awsInfoTable;

    void* ret_val_pthread_servodrive;

	while(1)
    {
        if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_A)
        {
            if(threadStatus->thrStatus_VKP == inuse)
                threadStatus->thrStatus_VKP = cancel;

            if(threadStatus->thrStatus_P18 == inuse)
                threadStatus->thrStatus_P18 = cancel;

            if(threadStatus->thrStatus_Simulator == notuse)
                threadStatus->thrStatus_Simulator = cReate;
        }

        if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_K)
        {
            if(threadStatus->thrStatus_Simulator == inuse)
                threadStatus->thrStatus_Simulator = cancel;

            if(threadStatus->thrStatus_P18 == inuse)
                threadStatus->thrStatus_P18 = cancel;

            if(threadStatus->thrStatus_VKP == notuse)
                threadStatus->thrStatus_VKP = cReate;
        }

        if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::RR)
        {
            if(threadStatus->thrStatus_VKP == inuse)
                threadStatus->thrStatus_VKP = cancel;

            if(threadStatus->thrStatus_Simulator == inuse)
                threadStatus->thrStatus_Simulator = cancel;

            if(threadStatus->thrStatus_P18 == notuse)
                threadStatus->thrStatus_P18 = cReate;
        }

        //////////////////   З А П У С К   П О Т О К О В   ///////////////////////////
//        if(threadStatus->thrStatus_ServoDrive == cReate)
//        {
//            threadStatus->thrStatus_ServoDrive = inuse;
//            pthread_create( &pthread_servodrive, NULL, ProcessThreadServoDrive, NULL );
//            //qDebug()<<"threadStatus->ServoDrive == cReate";
//        }


        if(threadStatus->thrStatus_MDO1Echo == cReate)
        {
            threadStatus->thrStatus_MDO1Echo = inuse;
            pthread_create( &pthreadMDO1Echo, NULL, ProcessThreadMDO1Echo, NULL );
            //qDebug()<<"threadStatus->MDO1Echo == cReate";
        }

        if(threadStatus->thrStatus_MDO1 == cReate)
        {
            threadStatus->thrStatus_MDO1 = inuse;
            pthread_create( &pthreadMDO1, NULL, ProcessThreadMDO1, NULL );
            //qDebug()<<"threadStatus->MDO1 == cReate";
        }

        if(threadStatus->thrStatus_Commander == cReate)
        {
            threadStatus->thrStatus_Commander = inuse;
            pthread_create( &pthread_Commander, NULL, ProcessThreadCommander, NULL );
            //qDebug()<<"threadStatus->thrStatus_Commander == cReate";
        }

        if(threadStatus->thrStatus_LauncherOf == cReate)
        {
            threadStatus->thrStatus_LauncherOf = inuse;
            pthread_create( &pthread_LauncherOf, NULL, ProcessThreadLauncherOfficer, NULL );
            //qDebug()<<"threadStatus->thrStatus_LauncherOf == cReate";
        }

        if(threadStatus->thrStatus_SYS == cReate)
        {
            threadStatus->thrStatus_SYS = inuse;
            pthread_create( &pthreadSYS, NULL, ProcessThreadSYS, NULL );
            //qDebug()<<"threadStatus->tSYS == cReate";
        }

        if(threadStatus->thrStatus_CnvEth485 == cReate)
        {
            restartCntr++;
            threadStatus->thrStatus_CnvEth485 = inuse;
            pthread_create( &pthreadCnvEth485, NULL, ProcessThreadCnvEth485, NULL );
            //qDebug()<<"threadStatus->Cnv485Eth == cReate";
        } 

        if(threadStatus->thrStatus_Simulator == cReate)
        {
            threadStatus->thrStatus_Simulator = inuse;
            pthread_create( &pthreadSimulator, NULL, ProcessSimulator, NULL );
            //qDebug()<<"threadStatus->Simulator == cReate";
        }

        if(threadStatus->thrStatus_GuidanceOf == cReate)
        {
            threadStatus->thrStatus_GuidanceOf = inuse;            
            pthread_create( &pthreadSharedMem, NULL, ProcessThreadGuidanceOfficer, NULL );
            //qDebug()<<"threadStatus->SharedMem == cReate";
        }

        if(threadStatus->thrStatus_DiagnMonitor == cReate)
        {
            threadStatus->thrStatus_DiagnMonitor = inuse;
            pthread_create( &pthreadDiagnMonitor, NULL, ProcessThreadDiagnMonitor, NULL );
            //qDebug()<<"threadStatus->DiagnMonitor == cReate";
        }

        if(threadStatus->thrStatus_OperF1 == cReate)
        {
            threadStatus->thrStatus_OperF1 = inuse;
            pthread_create( &pthreadOperF1, NULL, ProcessThreadOperF1, NULL );
            //qDebug()<<"threadStatus->ProcessThreadOperF1 == cReate";
        }

        if(threadStatus->thrStatus_OperF2 == cReate)
        {
            threadStatus->thrStatus_OperF2 = inuse;
            pthread_create( &pthreadOperF2, NULL, ProcessThreadOperF2, NULL );
            //qDebug()<<"threadStatus->ProcessThreadOperF2 == cReate";
        }

        if(threadStatus->thrStatus_InfoTable == cReate)
        {
            threadStatus->thrStatus_InfoTable = inuse;
            pthread_create( &pthread_awsInfoTable, NULL, ProcessThreadInfoTable, NULL );
            //qDebug()<<"threadStatus->InfoTable == cReate";
        }

        if(threadStatus->thrStatus_VKP == cReate)
        {
            threadStatus->thrStatus_VKP = inuse;
            pthread_create( &pthreadVKP, NULL, ProcessThreadVKP, NULL );
            //qDebug()<<"threadStatus->VKP == cReate";
        }

        if(threadStatus->thrStatus_P18 == cReate)
        {
            threadStatus->thrStatus_P18 = inuse;
            pthread_create( &pthreadP18, NULL, ProcessThreadP18, NULL );
            //qDebug()<<"threadStatus->VKP == cReate";
        }

        //////////////////   А В А Р И Й Н О Е   З А В Е Р Ш Е Н И Е   П О Т О К О В   ///////////////////////////

        if(threadStatus->thrStatus_ServoDrive == cancel)
        {
            pthread_cancel(pthread_servodrive);
            //qDebug()<<"threadStatus->ProcessThreadServoDrive == cancel";
        }

        if(threadStatus->thrStatus_MDO1Echo == cancel)
        {
            pthread_cancel(pthreadMDO1Echo);
            //qDebug()<<"threadStatus->MDO1Echo == cancel";
        }

        if(threadStatus->thrStatus_MDO1 == cancel)
        {
            pthread_cancel(pthreadMDO1);
            //qDebug()<<"threadStatus->MDO1 == cancel";
        }                

        if(threadStatus->thrStatus_Commander == cancel)
        {
            pthread_cancel(pthread_Commander);
            //qDebug()<<"threadStatus->thrStatus_Commander == cancel";
        }

        if(threadStatus->thrStatus_LauncherOf == cancel)
        {
            pthread_cancel(pthread_LauncherOf);
            //qDebug()<<"threadStatus->thrStatus_LauncherOf == cancel";
        }

        if(threadStatus->thrStatus_SYS == cancel)
        {
            pthread_cancel(pthreadSYS);
            //qDebug()<<"threadStatus->tSYS == cancel";
        }

        if(threadStatus->thrStatus_CnvEth485 == cancel)
        {
            pthread_cancel(pthreadCnvEth485);
            //qDebug()<<"threadStatus->Cnv485Eth == cancel";
        }

        if(threadStatus->thrStatus_Simulator == cancel)
        {
            pthread_cancel(pthreadSimulator);
            //qDebug()<<"threadStatus->thrStatus_Simulator == cancel";
        }

        if(threadStatus->thrStatus_GuidanceOf == cancel)
        {
            pthread_cancel(pthreadSharedMem);
            //qDebug()<<"threadStatus->SharedMem == cancel";
        }

        if(threadStatus->thrStatus_DiagnMonitor == cancel)
        {
            pthread_cancel(pthreadDiagnMonitor);
            //qDebug()<<"threadStatus->DiagnMonitor == cancel";
        }

        if(threadStatus->thrStatus_OperF1 == cancel)
        {
            pthread_cancel(pthreadOperF1);
        }

        if(threadStatus->thrStatus_OperF2 == cancel)
        {
            pthread_cancel(pthreadOperF2);
        }

        if(threadStatus->thrStatus_InfoTable == cancel)
        {
            pthread_cancel(pthread_awsInfoTable);
        }

        if(threadStatus->thrStatus_VKP == cancel)
        {
            pthread_cancel(pthreadVKP);            
        }

        if(threadStatus->thrStatus_P18 == cancel)
        {
            pthread_cancel(pthreadP18);
        }

        //////////////////   Н О Р М А Л Ь Н О Е   З А В Е Р Ш Е Н И Е   П О Т О К О В   ///////////////////////////


        if(threadStatus->thrStatus_ServoDrive == cancel)
        {
            pthread_join(pthread_servodrive,&ret_val_pthread_servodrive);
          threadStatus->thrStatus_ServoDrive = notuse;
        }


        if(threadStatus->thrStatus_MDO1Echo == cancel)
        {
            pthread_join(pthreadMDO1Echo, &ret_valMDO1Echo);

            //if(threadStatus->thrStatus_MDO1Echo == cancel)
                threadStatus->thrStatus_MDO1Echo = notuse;//cReate;
        }

        if(threadStatus->thrStatus_MDO1 == cancel)
        {
            pthread_join(pthreadMDO1, &ret_valMDO1);

            //if(threadStatus->thrStatus_MDO1 == cancel)
                threadStatus->thrStatus_MDO1 = notuse;//cReate;
        }        

        if(threadStatus->thrStatus_Commander == cancel)
        {
            pthread_join(pthread_Commander, &ret_valCommander);

            //if(threadStatus->thrStatus_Commander == cancel)
                threadStatus->thrStatus_Commander = notuse;//cReate;
        }

        if(threadStatus->thrStatus_LauncherOf == cancel)
        {
            pthread_join(pthread_LauncherOf, &ret_valLauncherOf);

            //if(threadStatus->thrStatus_LauncherOf == cancel)
                threadStatus->thrStatus_LauncherOf = notuse;//cReate;
        }

        if(threadStatus->thrStatus_SYS == cancel)
        {
            pthread_join(pthreadSYS, &ret_valSYS);

            //if(threadStatus->thrStatus_SYS == cancel)
                threadStatus->thrStatus_SYS = notuse;//cReate;
        }

        if(threadStatus->thrStatus_CnvEth485 == cancel)
        {
            pthread_join( pthreadCnvEth485, &ret_valCnvEth485);

            //if(threadStatus->thrStatus_CnvEth485 == cancel)
                threadStatus->thrStatus_CnvEth485 = notuse;//cReate;
        }

        if(threadStatus->thrStatus_Simulator == cancel)
        {
            pthread_join( pthreadSimulator, &ret_valSimulator);

            //if(threadStatus->thrStatus_Simulator == cancel)
                threadStatus->thrStatus_Simulator = notuse;//cReate;
        }

        if(threadStatus->thrStatus_GuidanceOf == cancel)
        {
            pthread_join( pthreadSharedMem, &ret_valSharedMem);

            //if(threadStatus->thrStatus_GuidanceOf == cancel)
                threadStatus->thrStatus_GuidanceOf = notuse;//cReate;
        }

        if(threadStatus->thrStatus_DiagnMonitor == cancel)
        {
            pthread_join( pthreadDiagnMonitor, &ret_valDiagnMonitor);

            //if(threadStatus->thrStatus_DiagnMonitor == cancel)
                threadStatus->thrStatus_DiagnMonitor = notuse;//cReate;
        }

        if(threadStatus->thrStatus_OperF1 == cancel)
        {
            pthread_join( pthreadOperF1, &ret_valOperF1);

            //if(threadStatus->thrStatus_OperF1 == cancel)
                threadStatus->thrStatus_OperF1 = notuse;//cReate;
        }

        if(threadStatus->thrStatus_OperF2 == cancel)
        {
            pthread_join( pthreadOperF2, &ret_valOperF2);

            //if(threadStatus->thrStatus_OperF2 == cancel)
                threadStatus->thrStatus_OperF2 = notuse;//cReate;
        }

        if(threadStatus->thrStatus_InfoTable == cancel)
        {
            pthread_join(pthread_awsInfoTable, &ret_val_awsInfoTable);

            //if(threadStatus->thrStatus_InfoTable == cancel)
                threadStatus->thrStatus_InfoTable = notuse;//cReate;
        }

        if(threadStatus->thrStatus_VKP == cancel)
        {
            pthread_join(pthreadVKP, &ret_valVKP);

            //if(threadStatus->thrStatus_VKP == cancel)
                threadStatus->thrStatus_VKP = notuse;//cReate;
        }

        if(threadStatus->thrStatus_P18 == cancel)
        {
            pthread_join(pthreadP18, &ret_valP18);

            //if(threadStatus->thrStatus_P18 == cancel)
                threadStatus->thrStatus_P18 = notuse;//cReate;
        }

//        qDebug()<<"Process_All_Threads: threeds_status->encoderE:"<<arm->threeds_status->encoderE
//               <<"threeds_status->encoderB:"<<arm->threeds_status->encoderB
//              <<"threeds_status->Schnaider:"<<arm->threeds_status->Schnaider
//             <<"threeds_status->ASY:"<<arm->threeds_status->ASY
//            <<"threeds_status->ADSBmode:"<<arm->threeds_status->ADSBmode
//           <<"threeds_status->localARMV:"<<arm->threeds_status->localARMV
//          <<"threeds_status->TRIMBLE:"<<arm->threeds_status->TRIMBLE
//         <<"threeds_status->SignalProc:"<<arm->threeds_status->SignalProc;

        usleep(2000 * 1000);
	}
    /////////////////pthread_testcancel();

    return temp_pt;
}
