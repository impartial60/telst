#ifndef USM
#define USM

#define TIMER_INTERVAL 36
#define NAKAL_MAX_TIME_SECONDS 18


struct TransmitterStatus
{
    enum Status {ISKHODNOE, PEREHOD_ISKHODNOE_NAKAL, NAKAL, PEREHOD_NAKAL_VISOKOE, VISOKOE_EKV, VISOKOE_ANT, NEISPRAVNOST};
};

#pragma pack(push,1)
typedef struct //total bytes 146
{
    u_int8_t usm01[16];
    u_int8_t usm02[11];
    u_int8_t usm03[12];         //  добавлен 1 байт
    u_int8_t usm04_topo[10];
    u_int8_t usm05_sjo1[10];
    u_int8_t usm06_sjo2[10];
    u_int8_t usm07_svo[10];
    u_int8_t usm08_sid[10];
    u_int8_t usm09_istp[10];
    u_int8_t usm10_kond[10];
    u_int8_t sync[20] ;         // Syncno-Source Block
    u_int8_t oklik[17];//25];   // OKLIK                убрано 8 байт
} USM_Commands;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct  //total bytes 297
{
    u_int8_t usm01[22];
    u_int8_t usm02[14]; //14
    u_int8_t usm03[9];
    u_int8_t usm04_topo[137];
    u_int8_t usm05_sjo1[13];
    u_int8_t usm06_sjo2[13];
    u_int8_t usm07_svo[11];
    u_int8_t usm08_sid[12];
    u_int8_t usm09_istp[27];
    u_int8_t usm10_kond[12];
    u_int8_t sync[12];
    u_int8_t oklik[15];//9];   // OKLIK                добавлено 6 байт
} USM_Info;
#pragma pack(pop)


#endif // USM

