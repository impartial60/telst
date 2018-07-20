#ifndef MDO
#define MDO

#include "usm_mdo_common.h"

#define MAX_DETECT_COUNT 200        // максимальное количество угломестных проходов за один азимутальный проход секторного поиска
#define MAX_MDO1_TARGETS_COUNT 48	// максимальное количество обнаруженных угломестных пачек за один угломестный проход
#define ECHO_LENGTH 256             //4096//19608   //      // 12288

//  MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1   MDO 1

#pragma pack(push,1)
typedef struct  //total bytes 11
{
    USM_MDO_header header;
    u_int8_t command;
    USM_MDO_timeFrame timeFrame;
    u_int8_t outset[2];
    u_int8_t CRC16[2];
} MDO1_commands;

typedef struct  //total bytes 11
{
    u_int8_t Nceli;
    u_int8_t range[2];
    u_int8_t doppler;
    u_int8_t elevationYV10[2];
    u_int8_t elevationF1[2];
    u_int8_t elevationF2[2];
    u_int8_t ARY;
} MDO1_targetData;

typedef struct  //total bytes 184 = 3(header) + 3(timeFrame) + 11x16(targetData) + 2(CRC16)
{
    USM_MDO_header header;
    USM_MDO_timeFrame timeFrame;
    MDO1_targetData targetData[16];
    u_int8_t CRC16[2];
} MDO1_incomingData;

typedef struct  //total bytes 58829 = 1+1(header) + 3(timeFrame) + 19608x3(echo)
{
    //u_int8_t x55;
    //u_int8_t xAA;
    //u_int8_t src;
    USM_MDO_header header;
    USM_MDO_timeFrame timeFrame;
    u_int8_t echo[MAX_MDO1_TARGETS_COUNT][ECHO_LENGTH];
} MDO1_echoData;


typedef struct
{
    float azimuth360;
    MDO1_targetData targetData[MAX_MDO1_TARGETS_COUNT];
} MDO1_nextDirection;


//  MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2   MDO 2

typedef struct  //total bytes 12
{
    u_int8_t Nceli;
    u_int8_t range[3];
    u_int8_t doppler[3];
    int8_t deltaF1;
    int8_t deltaF2;
    u_int8_t priznakSopr;
    u_int8_t tauImpulsaRecomend;
} MDO2_targetData;

typedef struct  //total bytes
{
    USM_MDO_header header;
    USM_MDO_timeFrame timeFrame;
    MDO2_targetData targetData;
    u_int8_t echo[4560];    //  240x19
    u_int8_t CRC16[2];
} MDO2_incomingData;
#pragma pack(pop)

#endif // MDO

