#ifndef EXTERN_H
#define EXTERN_H
#include <QTime>
#include <QDebug>

#include "../../aws125CommonInclude/sharedMem.h"
#include "../../aws125CommonInclude/externCommon.h"
#include "../../aws125CommonInclude/sys.h"
#include "threadstatus.h"
//#include "srcKram/tp.h"
#include "mainConsoleSettings.h"
#include  "srcKram/Drive125.h"

extern class Drive125 drive_unit;

extern START_INFO startInfoG;
extern VKP2ZRK vkp2zrkG;
extern P18_2ZRK p18_2ZRKG;
extern SNR_INFO snrInfoG;

extern bool komandirPereklychilTrenaj;
extern KOMANDI_KOMANDIRA komKomM;
extern KOMANDI_OFICERA_NAVED oficerNaved2Console;
extern KOMANDI_OFICERA_PYSKA oficerPyska2Console;

extern Console2OficerNavedenia console2OficerNaved;
extern Console2DiagnosticMonitor console2DiagnosticMonitor;
extern Console2OficerPyska console2OficerPyska;
extern Console2InfoTableKomandira console2InfoTableKomandira;

extern SNR2SYS snr2sys;
extern SYS2SNR sys2snr;

extern QTime programmStartTime;

//extern tp_t p1,p2;

extern void* ret_val_All_Threads;
extern ThreadStatus *threadStatus;

extern pthread_mutex_t localConsole_memory_Mutex;

extern int restartCntr;

extern SettingsConsoleM *settings;
extern ZP zonaPorazh;

extern void rsTrgtPrmsCalculator();
extern void servoDriveExecuter();
extern uint16_t uint16_t_byte_reorder(uint16_t inputValue);
extern void asterix_I126_050_Coder(uint8_t *sck);   //  I126/050        С О С Т О Я Н И Е   Ц Е Л Е В О Г О   К А Н А Л А   1


extern void * ProcessThreadCommander( void * temp_pt );
extern void * ProcessThreadLauncherOfficer( void * temp_pt );
extern void * ProcessThreadSYS(void * temp_pt );
extern void * ProcessThreadCnvEth485(void * temp_pt );
extern void * ProcessSimulator(void * temp_pt );
extern void * ProcessThreadGuidanceOfficer(void * temp_pt );
extern void * ProcessThreadDiagnMonitor( void * temp_pt );
extern void * ProcessThreadOperF1( void * temp_pt );
extern void * ProcessThreadOperF2( void * temp_pt );
extern void * ProcessThreadMDO1( void * temp_pt );
extern void * ProcessThreadMDO1Echo( void * temp_pt );
extern void * ProcessThreadInfoTable( void * temp_pt );
extern void * ProcessThreadVKP(void * temp_pt );
extern void * ProcessThreadP18(void * temp_pt );

//extern void * ProcessThreadServoDrive(void * temp_pt );



#endif // EXTERN_H
