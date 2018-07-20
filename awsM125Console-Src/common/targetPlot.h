#ifndef TARGETPLOT
#define TARGETPLOT
#pragma pack(push, 1)

#include <QtGlobal>
#include <QTime>

#include "localstructures.h"

#define VRAKETI 0.600

#define R_ZemliEquiv 8400.0

#define Rad2Grad 57.29578049

#define NM2km 1.852

#define PI_4 0.78539815

#define SIN45COS45 0.707106781

typedef struct
{
    float deltaF[2];
} UV11ANGLES;

typedef struct
{
    uint32_t transactionId;
    uint8_t numTarget;
    double distance;
    double height;
    double azimuth;
    double speed;
    double kyrs;
    uint8_t otrabCY;
    uint8_t mode;
} ArtSimulatorData;


typedef struct  //  26 байт
{
    uint8_t targetN;   // номер цели в системе АРМ руководителя тренировки
    uint8_t tip;       // тип цели
    float distance;     // горизонтальная дальность цели относительно УНВ, км
    float height;       // высота цели над поверхностью Земли, км
    float azimuth;      // азимут цели относительно УНВ, градусы 0...360
    float speed;        // абсолютная скорость цели, м/с
    float kyrs;         // курс цели, градусы 0...360
    //float delay;        // отсрочка старта относительно момента запуска налёта, сек
    float jammingPower; // спектральная плотность мощности бортового передатчика активных заградительных помех в полосе приёмника СНР, Вт/МГц

} KlmTargetData;


typedef struct  //  8 + 26 x 255 = 6638 байт
{
    uint32_t transactionId;                // идентификатор сообщения (счётчик)
    uint16_t messageLength;                // длина всего сообщения в байтах
    uint8_t controlByte;                   // байт управления
                                            // бит 0 - показывать на АРМах "истинные" полные трассы имитированных целей
                                            // бит 1 - показывать на АРМах только "истинное" текущее положение имитированных целей
                                            // бит 2...7 - резерв

    uint8_t targetTotalNumber;             // общее количество целей в налёте, максимум 255

    KlmTargetData trgts[255];               // данные по целям

} KlmSimulatorData;


enum ZRKOPERATIONS {NET_DEISTVIJ, POISK, SOPROVOJDENIE_V_OBZORE, SOPROVOJDENIE, NAVEDENIE, VSTRECHA};

typedef  struct {
    u_int8_t    targetN;      //  номер цели
    u_int8_t    options;      //  действие ЗРК по цели
}ZRKTrgtOptions;

typedef struct  //
{
    uint32_t transactionId;                // идентификатор сообщения (счётчик)
    uint16_t messageLength;                // длина всего сообщения в байтах
    uint8_t controlByte;                   // резерв

    uint8_t targetTotalNumber;             // общее количество целей в налёте, максимум 255

    ZRKTrgtOptions zrkTrgtOptions[255];    // список действий ЗРК по целям, длина списка = targetTotalNumber

} ZRK2SimulatorData;


typedef struct
{
    int targetNo;    
    float azimuth360;
    float kyrs;
    float rakyrs;

    float slantRange_km;
    float groundRange_km;

    float groundSpeed_ms;
    float radialSpeed_ms;

    float elevationAngle;    
    UV11ANGLES uv11Angles;

    float radarRelativeAltitude;
    float seaLevelAltitude;
    float groundX;      //  на восток
    float groundY;      //  на север

    float kyrsovojParameter_km;
    float maxKyrsovojParameter_km;
    float rangeParametr_km;
    float bGZoniPorParam_km;
    float dGZoniPorParam_km;
    float dGZoniPyskaParametr_km;
    float rangeParametrTV_km;
    float podletnoeVrema;

} TargetParams;

#pragma pack(pop)
#endif // TARGETPLOT
