#ifndef ZRKSERVICEINFO
#define ZRKSERVICEINFO

#pragma pack(push,1)

enum RejimRabotiZRK {VIKLUCHEN, BR, FK, TR_K, TR_A, RR};


typedef struct
{
}
ZRK2VKP;


typedef struct
{
    float x_km;
    float y_km;
    float h_km;

    float vx_ms;
    float vy_ms;
    float vh_ms;
}
COORD_YHX;

typedef struct
{
    int16_t  integerDegr;    // широта, целые градусы
    uint8_t  integerMin;     // широта, целые минуты
    uint8_t  integerSec;     // широта, целые секунды
    uint8_t  integerSec100;  // широта, сотые доли секунды

} geoCoordinate;

typedef struct
{
    geoCoordinate geoY;   // широта
    geoCoordinate geoX;   // долгота
    int16_t seaLevelH; // высота над уровнем моря фазового центра антенны СНР с учётом горки, целые метры с

} geoPosition;


typedef struct
{
    bool    commandInfoExist;       // наличиe всего эього сегмента данных
    bool    targetInfoExist;       // FX - признак наличия информации о цели
    uint8_t NomK;     // НомК - номер целевого канала
    bool    CU;       // ЦУ - признак выдачи ЦУ (1)
    bool    OCU;      // ОЦУ - отмена ЦУ (1)
    bool    ZP;       // ЗП - запрет пуска (1)
    bool    ZPVS;     // ЗПВС - запрет пуска навстречу (1)
    bool    ZPVD;     // ЗПВД - запрет пуска вдогон (1)
    bool    R_IS;     // Р ИС - по цели работают истребители (1)
    bool    R_ZRK;    // Р ЗРК - по цели работает другой ЗРК(1)
    uint16_t NceliKP; // Номер цели КП
} astrx125_050;


typedef struct
{    
    int sizeInfo125;

    uint8_t fspec1;
    uint8_t fspec2;

    uint8_t sac;
    uint8_t sic;
    uint8_t Nzrk;

    uint8_t komandaYprZRK030[2];
    uint8_t yprRaketami040[6];
    astrx125_050 komandiPoCelevomyKanaly1;

    int cyStatus;

    //bool targetInfoExist;
    uint8_t header050Chanal1[11];

    COORD_YHX coordYHX;

    uint16_t cmplxTrgtN[3];
    COORD_YHX cmplxYHX[3];

    bool sectorOtvetstvennostiNaznachit;
    float sectorOtvetstvennostiStart;
    float sectorOtvetstvennostiEnd;
}
VKP2ZRK;

typedef struct
{
    int transActionId = 0;
    bool isTrack;

    uint16_t targetNo;

    int sizeInfoP18;

    uint8_t sac;
    uint8_t sic;

    float timeCode;

    // Х А Р А К Т Е Р И С Т И К И   Т Р А Е К Т О Р И И
    float coordX_NM;
    float coordY_NM;
    float speed_NM_s;
    float azimuth360;
    float kyrs360;

    // Х А Р А К Т Е Р И С Т И К И   П Л О Т А
    float dalnostNM;

}
P18_2ZRK;

#pragma pack(pop)


#endif // ZRKSERVICEINFO

