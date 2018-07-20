#ifndef SYS
#define SYS


#pragma pack(push,1)
typedef  struct {
    u_int16_t   sid;
    u_int8_t    dlc;
    u_int8_t    data[8];
}MY_SID_CAN_MSG;

//typedef union
//{
//    u_int8_t all[352];
//    struct {
//        MY_SID_CAN_MSG txmsg1_ID044h;  // Mode LCS
//        MY_SID_CAN_MSG txmsg2_ID050h;  //Command START for reel 0
//        MY_SID_CAN_MSG txmsg3_ID051h;  //Command START for reel 1
//        MY_SID_CAN_MSG txmsg4_ID052h;  //Command START for reel 2
//        MY_SID_CAN_MSG txmsg5_ID053h;  //Command START for reel 3
//        MY_SID_CAN_MSG txmsg6_ID180h;  //Prepare for START for reel 0
//        MY_SID_CAN_MSG txmsg7_ID181h;  //Prepare for START for reel 1
//        MY_SID_CAN_MSG txmsg8_ID182h;  //Prepare for START for reel 2
//        MY_SID_CAN_MSG txmsg9_ID183h;  //Prepare for START for reel 3
//        MY_SID_CAN_MSG txmsg10_ID190h;  //Aimpoint Options reel 0
//        MY_SID_CAN_MSG txmsg11_ID191h;  //Aimpoint Options reel 1
//        MY_SID_CAN_MSG txmsg12_ID192h;  //Aimpoint Options reel 2
//        MY_SID_CAN_MSG txmsg13_ID193h;  //Aimpoint Options reel 3
//        MY_SID_CAN_MSG txmsg14_ID194h;  //Target speed for reel 0
//        MY_SID_CAN_MSG txmsg15_ID195h;  //Target speed for reel 1
//        MY_SID_CAN_MSG txmsg16_ID196h;  //Target speed for reel 2
//        MY_SID_CAN_MSG txmsg17_ID197h;  //Target speed for reel 3
//        MY_SID_CAN_MSG txmsg18_ID198h;  //Coordinates of the target reel 0
//        MY_SID_CAN_MSG txmsg19_ID199h;  //Coordinates of the target reel 1
//        MY_SID_CAN_MSG txmsg20_ID200h;  //Coordinates of the target reel 2
//        MY_SID_CAN_MSG txmsg21_ID201h;  //Coordinates of the target reel 4
//        MY_SID_CAN_MSG txmsg22_ID288h;  //Reels Angles
//        MY_SID_CAN_MSG txmsg23_ID290h;  //Coordinates of MLS
//        MY_SID_CAN_MSG txmsg24_ID291h;  //Angles for START not Allowed
//        MY_SID_CAN_MSG txmsg25_ID292h;  //Base Angle
//        MY_SID_CAN_MSG txmsg26;          //Reserv
//        MY_SID_CAN_MSG txmsg27;          //Reserv
//        MY_SID_CAN_MSG txmsg28;          //Reserv
//        MY_SID_CAN_MSG txmsg29;          //Reserv
//        MY_SID_CAN_MSG txmsg30;          //Reserv
//        MY_SID_CAN_MSG txmsg31;          //Reserv
//        MY_SID_CAN_MSG txmsg32;          //Reserv
//    };
//} SNR2SYS;//FROM_TCPIP_DATA;

//typedef union
//{
//    u_int8_t all[352];
//    struct {
//        MY_SID_CAN_MSG m044modeLCS;             // Mode LCS
//        MY_SID_CAN_MSG m05xStart[4];            // Command START for reel 0...3
//        MY_SID_CAN_MSG m18xPrep[4];             // Prepare for START for reel 0...3
//        MY_SID_CAN_MSG m19xOptn[4];             // Aimpoint Options reel 0...3
//        MY_SID_CAN_MSG m19xSpeed[4];            // Target speed for reel 0...3
//        MY_SID_CAN_MSG m19xCoord[4];            // Coordinates of the target reel 0
//        MY_SID_CAN_MSG m288angles;              // Reels Angles
//        MY_SID_CAN_MSG m290coordMLS;            // Coordinates of MLS
//        MY_SID_CAN_MSG m291restrictedAngles;    // Angles for START not Allowed
//        MY_SID_CAN_MSG m292baseAngle;           // Base Angle
//        MY_SID_CAN_MSG mRes[7];                 // Reserv
//    };
//} SNR2SYS;//FROM_TCPIP_DATA;

typedef union
{
    u_int8_t all[352];
    struct {
        MY_SID_CAN_MSG m044modeLCS;             // Mode LCS
        MY_SID_CAN_MSG m05xStart[4];            // Command START for reel 0...3
        MY_SID_CAN_MSG m18xPrep[4];             // Prepare for START for reel 0...3
        MY_SID_CAN_MSG m19xOptn[4];             // Aimpoint Options reel 0...3
        MY_SID_CAN_MSG m19xSpeed[4];            // Target speed for reel 0...3
        MY_SID_CAN_MSG m19xCoord[4];            // Coordinates of the target reel 0...3
        MY_SID_CAN_MSG m20xSpherCoord[4];       // Spherical Coordinates of the target reel 0...3
        MY_SID_CAN_MSG m288angles;              // Reels Angles
        MY_SID_CAN_MSG m290coordMLS;            // Coordinates of MLS
        MY_SID_CAN_MSG m291restrictedAngles;    // Angles for START not Allowed
        MY_SID_CAN_MSG m292baseAngle;           // Base Angle
        MY_SID_CAN_MSG mRes[3];                 // Reserv
    };
} SNR2SYS;//FROM_TCPIP_DATA;


typedef union
{
    u_int8_t all[88];
    struct {
        MY_SID_CAN_MSG m040commonData;      //Common Data
        MY_SID_CAN_MSG m10xMissileInfo[4];  //M0...3 Data
        MY_SID_CAN_MSG mRes[3];             //Reserv

};
} SYS2SNR;//TO_TCPIP_DATA;


//typedef union
//{
//    u_int8_t all[88];
//    struct {
//        MY_SID_CAN_MSG rxmsg1_ID040h;  //Common Data
//        MY_SID_CAN_MSG rxmsg2_ID100h;  //M0 Data
//        MY_SID_CAN_MSG rxmsg3_ID101h;  //M1 Data
//        MY_SID_CAN_MSG rxmsg4_ID102h;  //M2 Data
//        MY_SID_CAN_MSG rxmsg5_ID103h;  //M3 Data
//        MY_SID_CAN_MSG rxmsg6;         //Reserv
//        MY_SID_CAN_MSG rxmsg7;         //Reserv
//        MY_SID_CAN_MSG rxmsg8;         //Reserv

//};
//} SYS2SNR;//TO_TCPIP_DATA;




#pragma pack(pop)


#endif // SYS

