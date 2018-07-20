#ifndef SHAREDMEM
#define SHAREDMEM

#include "mdo.h"
#include "usm.h"
#include "usm_mdo_common.h"
#include "snrServiceInfo.h"
#include "localstructures.h"
#include "structuresasterix.h"
#include "launcherInfo.h"
#include "zrkServiceInfo.h"

#pragma pack(push,1)

typedef struct
{
    int transActId;
    float multiTurnValue;
} OperF_to_AWSG;

enum HANDWHEEL_ANGLE_TYPE {AZIMUTH = 0, ELEVANG = 1, THRESHOLD = 2};

typedef struct
{
    float delta = 0.0;
    int executeCounter = 0;
}
SERVODRIVE_COMMANDS;

typedef struct
{
    int targetIndexCY;

    bool servoDriveExecute[2];
    float servoDriveDelta[2];

    bool writeDataSingleShort;
    bool writeDataContinues;

    int rejimPoiskaSNR;
    int rejimSoprovojdeniaSNR;
    int integerValue4Test;

    int languageIndex;

} GUIDANCE_OFFICER_COMMANDS;

typedef struct
{
    unsigned char stopConsolePrgrm;
    GUIDANCE_OFFICER_COMMANDS guidanceOfficerCmds;
    USM_Commands usmCmds;
    MDO1_commands mdo1Cmds[9];
} KOMANDI_OFICERA_NAVED;

typedef struct
{
    RejimRabotiZRK rejimRabotiZRK;
    SERVODRIVE_COMMANDS srv[2];

    int targetIndexCY;

    bool servoDriveSimulation;

    int poiskState;

    bool pyskRazreshen;
    bool showSimulationTrack;
    bool showSimulationTarget;
    bool remoteControlZRK;
    int languageIndex;
}
KOMANDI_KOMANDIRA;

typedef struct
{
    SNR_INFO snrInfo;
    USM_Info usmInfo;
    KOMANDI_KOMANDIRA komKom;
    MDO1_incomingData mdo1Info[3];
    MDO1_echoData mdo1Echo;//[3];
} Console2DiagnosticMonitor;

typedef struct
{
    SNR_INFO snrInfo;
    USM_Info usmInfo;
    MDO1_incomingData mdo1Info[3];
    KOMANDI_KOMANDIRA komKom;
} Console2OficerNavedenia;

typedef struct
{
    SNR_INFO snrInfo;
    //sTransferData new_state_C;
    VKP2ZRK vkp2zrk;
    //KOMANDI_KOMANDIRA ironBtnsKomandiKomandira;
} Console2Komandir;

typedef struct
{
    LauncherInfo launcherInfo[MAXPY];
} START_INFO;

typedef struct
{
    VKP2ZRK vkp2ZRK;
    SNR_INFO snrInfo;
    //Launcher2Commander_data_33 launcher2Commander;
    START_INFO startInfo;
    KOMANDI_KOMANDIRA komKom;

} Console2InfoTableKomandira;

typedef struct
{
    int8_t pysk_BtnPressedCounter[MAXMISSILE];
    int8_t podgt_BtnPressedCounter[MAXMISSILE];
    int8_t nakal_BtnPressedCounter[MAXMISSILE];
    int8_t ishdn_ButtonPressedCounter[MAXMISSILE];
    //  2018-03-27  int8_t bort_ConctrsPressedCounter[MAXMISSILE];
    bool simulatorBortConctrsIsOn[MAXMISSILE];

} KOMANDI_PY;

typedef struct
{    
    //Launcher2Commander_data_33 launcher2Commander;
    //bool bortConnectors[MAXPY][]

    KOMANDI_PY komandiPY[MAXPY];
    uint8_t timeZoomFactor;

} KOMANDI_OFICERA_PYSKA;

typedef struct
{
    //sTransferData new_state_L;
    START_INFO startInfo;
    SNR_INFO snrInfo;

} Console2OficerPyska;



#pragma pack(pop)

#endif // SHAREDMEM

