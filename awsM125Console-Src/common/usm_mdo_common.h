#ifndef USM_MDO_COMMON
#define USM_MDO_COMMON

#pragma pack(push,1)
typedef struct  //total bytes 3
{
    u_int8_t x55;
    u_int8_t xAA;
    u_int8_t deviceAddress;
} USM_MDO_header;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct  //total bytes 3
{
    u_int8_t lowByte;
    u_int8_t middleByte;
    u_int8_t highByte;
} USM_MDO_timeFrame;
#pragma pack(pop)


#endif // USM_MDO_COMMON

