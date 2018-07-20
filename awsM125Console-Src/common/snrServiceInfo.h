#ifndef SNRSERVICEINFO
#define SNRSERVICEINFO

#include "zp.h"
#include "ironBtnsG.h"
#include "targetPlot.h"
#include "localstructures.h"
#include "zrkServiceInfo.h"

#pragma pack(push,1)

//typedef struct
//{
//    int16_t  gradY;    // широта, целые градусы
//    uint8_t  minY;     // широта, целые минуты
//    uint8_t  secY;     // широта, целые секунды
//    uint8_t  sec100Y;  // широта, сотые доли секунды

//    int16_t  gradX;    // долгота, целые градусы
//    uint8_t  minX;     // долгота, целые минуты
//    uint8_t  secX;     // долгота, целые секунды
//    uint8_t  sec100X;  // долгота, сотые доли секунды

//    int16_t seaLevelH; // высота над уровнем моря фазового центра антенны СНР с учётом горки, целые метры с

//    uint32_t        Position[3]; // текущая позиция енкодера; 0..2 = node 50..52
//    uButtons        Buttons;        // кнопки

//} antennaPosition;


#define MONOTRACKSIZE 6000
#define MAXTOCKANUM 9

enum RejimPoiskaSNR {POISK_VIKLUCHEN, POISK_Kryg, POISK_BSP, POISK_MSP, POISK_PEREKLYCHEN_V_ZAHVAT, POISK_PEREKLYCHEN_V_SOPROVOJDENIE};
enum RejimSoprovojdeniaSNR {SOPROVOJDENIE_AS, SOPROVOJDENIE_ASDRSF, SOPROVOJDENIE_RS};

typedef struct
{
    int consoleCntr;
    unsigned char consoleLocalWait;
    int receiveBytesNumber;
    int cntr4;
    int restartCntr;
    ssize_t totalReadBytes;
    ssize_t totalSentBytes;

    //ArtSimulatorData artSimData;
    bool simulatorIsOn;

    int komJustExecuteCounter[2] = {-1, - 1};

    int rejimPoiskaSNR;
    int rejimSoprovojdeniaSNR;
    float directrisaAzimuth;
    float azimuthScanSectorHalfWidth;

    float currentAzimuth;
    float currentDirectrisaElevationAngle;

    float handWheelAzimuthSpeed;
    float handWheelElevAngSpeed;
    float handWheelFSpeed[2];

    IRON_BUTTONS_G ironBtnsG;

    geoPosition unvGeoPosition;

    int totalSimTargetNum;     //  общее количество целей в налёте

    TargetParams nextTocka[MAXTOCKANUM];
    TargetParams nextTockaRS[MAXTOCKANUM];

    int klmTransactionId;      //  iD очередного сообщения имитированного налёта (нужен для определения момента перезапуска налёта)
    uint8_t klmControlByte;    //  команды управления имитаторами ЗРК с АРМ руководителя тренировки

    int targetIndexCY;

    RejimRabotiZRK rejimRabotiZRK;
    bool remoteControlZRK;

    bool showSimulationTrack;
    bool showSimulationTarget;

    uint32_t p18TransactionId = 0;

} SNR_INFO;
#pragma pack(pop)


#endif // SNRSERVICEINFO

