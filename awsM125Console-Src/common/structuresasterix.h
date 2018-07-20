#pragma once
#ifndef STRUCTURESASTERIX_H
#define STRUCTURESASTERIX_H
#pragma pack(push, 1)

#include <QtGlobal>
#include <QTime>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////             ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////   ASTERIX   ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////             ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union {
    uint16_t Word;
    uint8_t  Byte[2];
} uWord;

typedef union {
    uint8_t   Bytes[4];
    int32_t   Int;
    uint32_t  UInt;
} uDWord;

// структура для геокоординат в протоколе Астерикс126
typedef struct {
    uint8_t  Coord[3];
} sAterix_Geo;

// координаты WGS, используемые в Asterix
typedef struct
{   // Отрицательные значения записываются в дополнительном коде.
    sAterix_Geo  Lon;          // долгота в WGS 84, ц.м.р.=180°/(2^23)
    sAterix_Geo  Lat;          // широта в WGS 84, ц.м.р.=180°/(2^23)
    uint16_t     Height;       // высота в WGS 84, ц.м.р.=1м
} sAsterix_WGS_Coordinates;

typedef struct
{
    int16_t North;              // север
    int16_t Height;             // высота
    int16_t East;               // восток
} sAsterix12xCoordinates; // прямоугольная система координат в протоколе Asterix

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  I125  /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================== I125/020 Сектор ответственности ========================================================
typedef struct
{
    // сектор ответственности
    #define KC125_Start    0b01
    #define KC125_Stop     0b10

    unsigned        KC:2;            /* KC:
                                      * KC_Start    0b01 - назначить сектор ответственности
                                      * KC_Stop     0b10 - отменить*/
    unsigned        :6;
    uint8_t         Azimuth_Start;      // азимут начала сектора ответственности и по часовой...
    uint8_t         Azimuth_End;        // ...азимут конца сектора ответственности (ц.м.р.=360°/256)
} sAsterix125_020_Sector;

// ===================================== I125/030 Команда на ЗРК ================================================================
typedef struct
{
    // коды команд на ЗРК
    #define CCtoZRK125_Ready3          0b0011
    #define CCtoZRK125_Ready2          0b0010
    #define CCtoZRK125_Ready1          0b0001
    #define CCtoZRK125_Ready1quick     0b0100
    #define CCtoZRK125_AT              0b0101
    #define CCtoZRK125_KT              0b0110
    #define CCtoZRK125_KFK             0b0111
    #define CCtoZRK125_CombatWork      0b1000
    #define CCtoZRK125_RegControl      0b1001
    #define CCtoZRK125_SetTrip         0b1010
    #define CCtoZRK125_SetTransport    0b1011
    #define CCtoZRK125_NoCommand       0b0000

    unsigned    CC:4;   /* CC - код команды:
                         * CCtoZRK125_Ready3       0b0011 - привести ЗРК в готовность 3
                         * CCtoZRK125_Ready2       0b0010 - привести ЗРК в готовность 2
                         * CCtoZRK125_Ready1       0b0001 - привести ЗРК в готовность 1
                         * CCtoZRK125_Ready1quick  0b0100 - привести ЗРК в готовность 1 ускоренно
                         * CCtoZRK125_AT           0b0101 - провести автономный тренаж (АТ)
                         * CCtoZRK125_KT           0b0110 - провести коплексный тренаж (КС)
                         * CCtoZRK125_KFK          0b0111 - провести комплексный контроль функционирования (КФК)
                         * CCtoZRK125_CombatWork   0b1000 - боевая работа
                         * CCtoZRK125_RegControl   0b1001 - регламентный контроль
                         * CCtoZRK125_SetTrip      0b1010 - перевести ЗРК в походное положение
                         * CCtoZRK125_SetTransport 0b1011 - перевести ЗРК в транспортное положение
                         * CCtoZRK125_NoCommand    0b0000 - нет команды */
    unsigned        Rez:1;              // Рез - перевести в резерв
    unsigned        UvKp:1;             // УВКП - перейти на управление вышестоящего КП (1)
    unsigned        M:1;                // М - мощность излучения (1..100%, 0 - пониженная)
    unsigned        Z:1;                // З - запрет излучения на антенну (1-запрет, 0-нет запрета)
    unsigned        :3;
    unsigned        RC:3;               /* РЦ - регламент целераспредения. указывает количество каналов (от 1 до 4)
                                         * выделяемых для автономной работы, остальные каналы выделяются
                                         * для отработки ЦУ от ПУ. Если все каналы отданы для автономной работы,
                                         * то это команда на автономную работу. */
    unsigned        ZS:1;               // ЗС - запрет излучения на антенну в секторе (1-запрет, 0-нет запрета)
    unsigned        :1;
} sAsterix125_030_CommandZRK;

// ===================================== I125/040 Управление ракетами ==============================================
typedef struct
{   // Количество равное нулю не несет информации.
    unsigned        KRP_R:4;            // КРП_Р - кол-во ракет, кот. нужно поставить на подготовку разово
    unsigned        KRP_N:3;            // КРП_Н - кол-во ракет, кот. нужно поставить на подготовку непрерывно
    unsigned        OG:1;               // ОГ: 1 - ограничить кол-во расхода ракет, 0 - нет ограничения
} sAsterix125_040_RC_1    ;

typedef struct
{   // Количество равное нулю не несет информации.
    unsigned        OP_R:4;             // ОП_Р - кол-во ракет, кот. нужно отменить подготовку разово
    unsigned        OP_N:4;             // ОП_Н - кол-во ракет, кот. нужно отменить подготовку непрерывно
} sAsterix125_040_RC_2;

typedef struct
{
    sAsterix125_040_RC_1    Rockets27D_1;
    sAsterix125_040_RC_1    Rockets27DM1_1;
    sAsterix125_040_RC_1    Rockets27DM2_1;

    sAsterix125_040_RC_2    Rockets27D_2;
    sAsterix125_040_RC_2    Rockets27DM1_2;
    sAsterix125_040_RC_2    Rockets27DM2_2;
} sAsterix125_040_RocketControl;

// ===================================== I125/(050,060,070,080) Команды по целевому каналу 1..4 ====================================================
typedef struct
{
    unsigned    FX:1;       // FX - признак наличия информации о цели
    unsigned    NomK:3;     // НомК - номер целевого канала
    unsigned    CU:1;       // ЦУ - признак выдачи ЦУ (1)
    unsigned    OCU:1;      // ОЦУ - отмена ЦУ (1)
    unsigned    ZP:1;       // ЗП - запрет пуска (1)
    unsigned    ZPVS:1;     // ЗПВС - запрет пуска навстречу (1)
    unsigned    ZPVD:1;     // ЗПВД - запрет пуска вдогон (1)
    unsigned    R_IS:1;     // Р ИС - по цели работают истребители (1)
    unsigned    R_ZRK:1;    // Р ЗРК - по цели работает другой ЗРК(1)
    unsigned    :5;
} sAsterix125_050_CommandCC_Header;

typedef struct
{
    // Если номер цели равен 0, значит номера нет
    uint16_t NumTargetKP;    // Номер цели КП
    uint8_t  NumTargetZRK;   // Номер цели ЗРК

    struct
    {
        unsigned    :1;
        unsigned    Prior:1;    // Приор - приоритетная
        unsigned    ManS:1;     // МанС - манёвр по скорости
        unsigned    ManV:1;     // МанВ - манёвр по высоте
        unsigned    ManK:1;     // МанК - манёвр по курсу
        unsigned    PAP:1;      // ПАП - постановщик помех для РЛС ПВО
        unsigned    MR:1;       // МР - малоразмерная
        unsigned    BR:1;       // БР - большеразмерная

        // характеристика воздушного объекта
        #define HVO125_NoInfo          0b0000
        #define HVO125_Friend          0b0001
        #define HVO125_Enemy           0b0010
        #define HVO125_Unidentified    0b0011
        #define HVO125_Reserve         0b0100

        unsigned    HVO:4;  /* ХВО - характеристика воздушного объекта:
                             * HVO125_NoInfo       0b0000 - нет информации
                             * HVO125_Friend       0b0001 - свой
                             * HVO125_Enemy        0b0010 - чужой (противник)
                             * HVO125_Unidentified 0b0011 - неопознанный
                             * HVO125_Reserve      0b0100 - резерв */
        // Тип цели
        #define Type125_NoInfo         0b0000
        #define Type125_Drone          0b0001
        #define Type125_Helicopter     0b0010
        #define Type125_Aerodynamic    0b0011
        #define Type125_Reserve1       0b0101
        #define Type125_Reserve2       0b0110
        #define Type125_Reserve3       0b0111

        unsigned    Type:4; /* ТипЦ - Тип цели:
                             * Type125_NoInfo      0b0000 - нет информации;
                             * Type125_Drone       0b0001 - крылатая ракета (беспилотник);
                             * Type125_Helicopter  0b0010 - вертолёт;
                             * Type125_Aerodynamic 0b0011 - аэродинамическая;
                             * Type125_Reserve1    0b0101 - резерв;
                             * Type125_Reserve2    0b0110 - резерв
                             * Type125_Reserve3    0b0111 - резерв */
    } TargetAttribute; // Признаки цели (задаются единицей)

    struct
    {
        unsigned        :8;

        // Результаты ОП
        #define RezOP125_NotConducted  0b00
        #define RezOP125_respond       0b01
        #define RezOP125_NotRespond    0b10
        #define RezOP125_Reserve       0b00

        unsigned    RezOP:4;    /* Рез ОП - Результаты ОП:
                                 * RezOP125_NotConducted   0b00 - не проводилось;
                                 * RezOP125_respond        0b01 - отвечает;
                                 * RezOP125_NotRespond     0b10 - не отвечает
                                 * RezOP125_Reserve        0b11 - резерв */

        unsigned    :4;
    } TargetIdentification; // опознавание

    struct
    {
        unsigned    :1;
        unsigned    PravG:3;    // ПравГ - правило обстрела групповой цели
        unsigned    KolG:3;     // КолГ - количество целей в группе
        unsigned    PG:1;       // ПГ - признак групповой цели:   1 - групповая,  0 - не групповая
    } GroupTarget; // групповая цель

    struct
    {
        // Вид помехи ЗРК (по данным ПУ)
        #define Kind125_NoInfo  0b0000
        #define Kind125_AZNP    0b0001
        #define Kind125_PPCHNP  0b0010
        #define Kind125_PPCHBP  0b0011
        #define Kind125_IOP     0b0100
        #define Kind125_NP      0b0101
        #define Kind125_PP      0b0110

        unsigned    Kind:4;     /* Вид помехи ЗРК (по данным ПУ):
                                 * Kind125_NoInfo  0b0000 - нет информации
                                 * Kind125_AZNP    0b0001 - АКТИВНАЯ ЗАГРАДИТЕЛЬНАЯ НЕПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                 * Kind125_PPCHNP  0b0010 - ПРИЦЕЛЬНАЯ ПО ЧАСТОТЕ НЕПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                 * Kind125_PPCHBP  0b0011 - ПРИЦЕЛЬНАЯ ПО ЧАСТОТЕ БЫСТРОПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                 * Kind125_IOP     0b0100 - ИМПУЛЬСНЫЕ ОТВЕТНЫЕ ПОМЕХИ
                                 * Kind125_NP      0b0101 - НЕСИНХРОННЫЕ ПОМЕХИ
                                 * Kind125_PP      0b0110 - ПАССИВНЫЕ ПОМЕХИ */

        // интенсивность помехи
        #define Power125_NoInfo    0b00
        #define Power125_Low       0b01
        #define Power125_Strong    0b10

        unsigned    Power:2;    /* Ин - интенсивность помехи:
                                 * Power125_NoInfo  0b00 - нет информации по интенсивности
                                 * Power125_Low     0b01 - слабая
                                 * Power125_Strong  0b10 - сильная */

        // Наличие помех (получено от РЛС)
        #define Existence125_NotDetected           0b00
        #define Existence125_ActiveStationary      0b01
        #define Existence125_ActiveNonStationary   0b10
        #define Existence125_Asynchronous          0b11

        unsigned    Existence:2;    /* НП - наличие помех (получено от РЛС):
                                     * Existence125_NotDetected            0b00 - не обнаружено
                                     * Existence125_ActiveStationary       0b01 - активные стационарные (АП)
                                     * Existence125_ActiveNonStationary    0b10 - активные нестационарные (НАП)
                                     * Existence125_Asynchronous           0b11 - несинхронные (НИП) */
    } Jamming; // помеха

    sAsterix12xCoordinates TargetCoordinates; // Координаты Север, Высота, Восток относительно ЗРК, ц.м.р.= 50м, отрицательные в дополнительном коде.
    sAsterix12xCoordinates TargetSpeed;       // Скорости по координатам с ц.м.р. 1 м/сек, отрицательные в дополнительном коде.
} sAsterix125_050_CommandCC_Body;

typedef struct
{
    sAsterix125_050_CommandCC_Header    Header;
    sAsterix125_050_CommandCC_Body      Body;
} sAsterix125_050_CommandCC;                // Структуры 060, 070, 080 идентичны структуре 050

// ======================================= I125/(091,092,093) Цель комплексного тренажа ===================================================
typedef struct
{
    uint16_t            Num;                // Номер цели

    sAsterix12xCoordinates Coordinates;        // Координаты Север, Высота, Восток относительно ЗРК, ц.м.р. 50м, отрицательные в дополнительном коде.
    sAsterix12xCoordinates Speed;              // Скорости по координатам с ц.м.р. 1 м/сек, отрицательные в дополнительном коде.
} sAsterix125_091_ComplexTrainingTarget; // Структуры 092, 093 идентичны структуре 091

// ================================ FSPEC I125 ======================================================================
typedef union
{
    uint8_t FSPEC1[2];                       // весь FSPEC побайтно

    struct {
        unsigned FX1:1;                     // FX Индикатор расширения поля
        unsigned :2;
        unsigned RocketControl:1;           // I125/040 управление ракетами
        unsigned CommandZRK:1;              // I125/030 команда на ЗРК
        unsigned Sector:1;                  // I125/020 сектор ответственности
        unsigned NumZRK:1;                  // I125/015 номер ЗРК
        unsigned Id:1;                      // I125/010 идентификатор источника данных

        unsigned FX2:1;                     // FX Индикатор расширения поля
        unsigned ComplexTrainingTarget3:1;  // I125/093 цель №3 комплексного тренажа
        unsigned ComplexTrainingTarget2:1;  // I125/092 цель №2 комплексного тренажа
        unsigned ComplexTrainingTarget1:1;  // I125/091 цель №1 комплексного тренажа
        unsigned CommandCC4:1;              // I125/080 команды по целевому каналу 4
        unsigned CommandCC3:1;              // I125/070 команды по целевому каналу 3
        unsigned CommandCC2:1;              // I125/060 команды по целевому каналу 2
        unsigned CommandCC1:1;              // I125/050 команды по целевому каналу 1

    } Bits;     // весь FSPEC побитно
} uAsterix125_FSPEC;

// ============================== Asterix I125 весь =========================================================
typedef struct
{
    uAsterix125_FSPEC                       FSPEC1;                  // FSPEC
    uint16_t                                Id;                     // I125/010 Идентификатор источника данных (ПУ)
    uint8_t                                 NumZRK;                 // I125/015 Номер ЗРК
    sAsterix125_020_Sector                  Sector;                 // I125/020 Сектор ответственности
    sAsterix125_030_CommandZRK              CommandZRK;             // I125/030 Команда на ЗРК
    sAsterix125_040_RocketControl           RocketControl;          // I125/040 Управление ракетами
    sAsterix125_050_CommandCC               CommandCC[4];           // I125/(050,060,070,080)Команды по целевому каналу 1..4
    sAsterix125_091_ComplexTrainingTarget   ComplexTrainingTarget[3];//I125/(091,092,093)Цель №1..3 комплексного тренажа
} sAsterix125;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  I126  /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================== I126/030 Состояние ЗРК ======================================================
typedef struct
{
    // Наличие помех (получено от РЛС)
    #define ReadyState126_Ready3               0b0011
    #define ReadyState126_Ready2               0b0010
    #define ReadyState126_Ready1               0b0001
    #define ReadyState126_RegControl           0b0100
    #define ReadyState126_Inclusion            0b0101
    #define ReadyState126_CombatWork           0b1001
    #define ReadyState126_ZRKToAutoTraining    0b1010
    #define ReadyState126_ZRKToIntegratedSim   0b1011
    #define ReadyState126_ZRKToIntegratedCMO   0b1100
    #define ReadyState126_Trip                 0b1101
    #define ReadyState126_Transport            0b1110

    unsigned    ReadyState:4;   /* Готовность:
                                 * ReadyState126_Ready3                0b0011 - ЗРК в готовности 3
                                 * ReadyState126_Ready2                0b0010 - ЗРК в готовности 2
                                 * ReadyState126_Ready1                0b0001 - ЗРК в готовности 1
                                 * ReadyState126_RegControl            0b0100 - регламентный контроль
                                 * ReadyState126_Inclusion             0b0101 - идет включение
                                 * ReadyState126_CombatWork            0b1001 - боевая работа
                                 * ReadyState126_ZRKToAutoTraining     0b1010 - ЗРК в режиме автономного тренажа
                                 * ReadyState126_ZRKToIntegratedSim    0b1011 - ЗРК в режиме комплексного тренажа
                                 * ReadyState126_ZRKToIntegratedCMO    0b1100 - ЗРК в режиме комплексного контроля функционирования
                                 * ReadyState126_Trip                  0b1101 - перевожу ЗРК в походное положение
                                 * ReadyState126_Transport             0b1110 - перевожу ЗРК в транспортное положение */
    unsigned    :4;
    unsigned    Mode:3;                     // Режим
    unsigned    KICK:3;                     // КИЦК – количество исправных целевых каналов
    unsigned    Rez:1;                      // Рез – ЗРК в резерве (1)
    unsigned    UVKP:1;                     // УВКП – ЗРК на управлении от вышестоящего КП (1)

    unsigned    KIRK:4;                     // КИРК - количество исправных ракетных каналов
    unsigned    :1;
    unsigned    AR:1;                       // АР – автономная работа: 1 - ЗРК находится в режиме автономных боевых действий в соответствии с регламентом целераспределения  и параметрами сектора ответственности, 0 – ЗРК не находится в режиме автономных боевых действий
    unsigned    Iz:1;                       // Из – Излучение в эфир (на антенну) (1-включено,  0-выключено)
    unsigned    M:1;                        // М - мощность излучения (1 - 100%, 0 – пониженная мощность)
} sAsterix126_030_sStateOfZRK;

typedef struct
{
    unsigned    KRP:4;                      // КРП – количество ракет включенных на подготовку (ракеты, которые переводятся из  «готовность №2» в «готовность №1»)
    unsigned    KR:4;                       // КР - общее количество ракет (боезапас) данного типа в ЗРК
    unsigned    KRG2:4;                     // КРГ2 – количество ракет в готовности №2 (будут готовы к немедленному пуску только после их включения на подготовку в гот.1 через 0,5 мин)
    unsigned    KRG1:4;                     // КРГ1 – количество ракет в готовности №1 (готовые к немедленному пуску)
} sAsterix126_RocketInfo_SingleType;

// =============================  I126/040 Информация о ракетах на ЗРК =============================================
typedef struct
{
    sAsterix126_RocketInfo_SingleType Rocket27D;
    sAsterix126_RocketInfo_SingleType Rocket27DM1;
    sAsterix126_RocketInfo_SingleType Rocket27DM2;
} sAsterix126_040_RocketInfo;

// =============================== I126/050 Состояние целевого канала ==================================================
typedef struct
{
    unsigned FX:1;      // FX - признак наличия информации о цели
    unsigned NomK:3;    // НомК - номер целевого канала

    // Фаза работы ЗРК и состояние целевого канала
    #define CK126_FreeChannel             0b0000
    #define CK126_CUAccepted              0b0001
    #define CK126_TrackingMode1           0b0011
    #define CK126_TrackingMode2           0b0100
    #define CK126_TargetInZone            0b0101
    #define CK126_RefusingCU              0b0110
    #define CK126_BrokenChannel           0b0111
    #define CK126_ReproductionChannel     0b1000
    #define CK126_MTInGuaranteedZone      0b1001


    unsigned CK:4;  /* СК - Фаза работы ЗРК и состояние целевого канала:
                     * CK126_FreeChannel           0b0000 - канал свободен
                     * CK126_CUAccepted            0b0001 - ЦУ принято, ЗРК в поиске
                     * CK126_TrackingMode1         0b0011 - сопровождение в режиме 1
                     * CK126_TrackingMode2         0b0100 - сопровождение в режиме 2
                     * CK126_TargetInZone          0b0101 - цель в зоне пуска
                     * CK126_RefusingCU            0b0110 - отказ от ЦУ
                     * CK126_BrokenChannel         0b0111 - канал неисправен
                     * CK126_ReproductionChannel   0b1000 - канал на размножении
                     * CK126_MTInGuaranteedZone    0b1001 - маневрирующая цель в гарантированной зоне пуска        */
} sAsterix126_050_StateOfChannel_Header; // Заголовок блока данных

typedef struct
{
    struct
    {
        unsigned    :1;
        unsigned    PC:1;   // ПЦ - поражение цели: 1 - цель поражена, 0 - цель не поражена.
        unsigned    VO:1;   // ВО - вид огня: 0 - очередь, 1 - залп.
        unsigned    MN:1;   // МН - метод наведения: 0 - метод №1 (трехточка), 1 - метод №2 (половинное спрямление)
        unsigned    NR:1;   // НР - наведение ракеты: 1 - ракета захвачена и управляется, 0 - ракета не управляется
        unsigned    P:1;    // П  - пуск: 0 - пуска нет, 1 - пуск есть

        // тип ракеты
        #define TR126_NoInfo   0b00
        #define TR126_27D      0b01
        #define TR126_27DM1    0b10
        #define TR126_27DM2    0b11

        unsigned    TR:2;   /* ТР - тип ракеты :
                             * TR126_NoInfo    0b00 - нет информации (соответствующие байты содержат нулевую информацию)
                             * TR126_27D       0b01 - 27Д
                             * TR126_27DM1     0b10 - 27ДМ1
                             * TR126_27DM2     0b11 - 27ДМ2 */
        unsigned    :8;

        uint8_t  GuidanceTimeFromStart;      // Время наведения от старта ц.м.р. 1 сек
        uint8_t  RestOfTimeBeforeMeeting;    // Остаток времени до встречи  ц.м.р. 1 сек

    } ResultFireRockets[2]; // результаты стрельбы первой и второй ракетой соответственно

    // Если номер цели равен 0, значит номера нет
    uint16_t NumTargetKP;    // Номер цели КП
    uint8_t  NumTargetZRK;   // Номер цели ЗРК

    struct
    {
        unsigned    :1;
        unsigned    Prior:1;    // Приор - приоритетная
        unsigned    ManS:1;     // МанС - маневр по скорости
        unsigned    ManV:1;     // МанВ - маневр по высоте
        unsigned    ManK:1;     // МанК - маневр по курсу
        unsigned    MV:1;       // МВ - маловысотная
        unsigned    MR:1;       // МР - малоразмерная
        unsigned    BR:1;       // БР - большеразмерная

        // характеристики воздушного объекта
        #define HVO126_NoInfo                  0b0000
        #define HVO126_RequestPlane            0b0001
        #define HVO126_Enemy                   0b0010
        #define HVO126_Target                  0b0011
        #define HVO126_ConflictOGP             0b0100
        #define HVO126_FighterInterceptor      0b0101
        #define HVO126_ControlPlane            0b0110
        #define HVO126_Friend                  0b0111
        #define HVO126_FlightViolatorFriend    0b1001
        #define HVO126_FlightViolatorEnemy     0b1010
        #define HVO126_UnauthorizedTakeoff     0b1011
        #define HVO126_TrespasserFriend        0b1100
        #define HVO126_TrespasserEnemy         0b1101
        #define HVO126_TerrorCapturedPlane     0b1110


        unsigned    HVO:4;  /* ХВО - характеристики воздушного объекта:
                             * HVO126_NoInfo                   0b0000 - нет информации
                             * HVO126_RequestPlane             0b0001 - самолет по заявке
                             * HVO126_Enemy                    0b0010 - чужой
                             * HVO126_Target                   0b0011 - цель
                             * HVO126_ConflictOGP              0b0100 - конфликт ОГП
                             * HVO126_FighterInterceptor       0b0101 - истребитель перехватчик
                             * HVO126_ControlPlane             0b0110 - контрольный самолет
                             * HVO126_Friend                   0b0111 - свой
                             * HVO126_FlightViolatorFriend     0b1001 - нарушитель режима полета СВОЙ
                             * HVO126_FlightViolatorEnemy      0b1010 - нарушитель режима полета ЧУЖОЙ
                             * HVO126_UnauthorizedTakeoff      0b1011 - несанкционированный взлет
                             * HVO126_TrespasserFriend         0b1100 - нарушитель гос. границы СВОЙ
                             * HVO126_TrespasserEnemy          0b1101 - нарушитель гос. границы ЧУЖОЙ
                             * HVO126_TerrorCapturedPlane      0b1110 - самолет захвачен террористами */

        // Тип цели
        #define Type126_NoInfo         0b0000
        #define Type126_CruiseMissile  0b0001
        #define Type126_Drone          0b0010
        #define Type126_Aerodynamic    0b0011
        #define Type126_PAP            0b0100
        #define Type126_PAPBarracker   0b0101
        #define Type126_Helicopter     0b0110
        #define Type126_Ground         0b0111
        #define Type126_Ballistic      0b1000
        #define Type126_BearingForPAP  0b1001
        #define Type126_TrainingGoal   0b1010

        unsigned    Type:4; /* ТипЦ - тип цели:
                             * Type126_NoInfo          0b0000 - нет информации
                             * Type126_CruiseMissile   0b0001 - крылатая ракета
                             * Type126_Drone           0b0010 - БПЛА
                             * Type126_Aerodynamic     0b0011 - аэродинамическая
                             * Type126_PAP             0b0100 - ПАП
                             * Type126_PAPBarracker    0b0101 - ПАП баражор
                             * Type126_Helicopter      0b0110 - Вертолет
                             * Type126_Ground          0b0111 - наземная
                             * Type126_Ballistic       0b1000 - баллистическая
                             * Type126_BearingForPAP   0b1001 - пеленг на ПАП
                             * Type126_TrainingGoal    0b1010 - тренажная цель   */

        struct
        {
            // ВидОП
            #define ModeOP126_NoInfo               0b0000
            #define ModeO126P_IdentificationMode1  0b0001
            #define ModeO126P_IdentificationMode2  0b0010
            #define ModeOP126_IdentificationMode3  0b0011
            #define ModeOP126_IdentificationMode4  0b0100

            unsigned    ModeOP:4;   /* ВидОП:
                                     * ModeOP126_NoInfo                0b0000 - нет информации
                                     * ModeOP126_IdentificationMode1   0b0001 - опознавание в режиме №1
                                     * ModeOP126_IdentificationMode2   0b0010 - опознавание в режиме №2
                                     * ModeOP126_IdentificationMode3   0b0011 - опознавание в режиме №3
                                     * ModeOP126_IdentificationMode4   0b0100 - опознавание в режиме №4  */

            // ВКЛ
            #define Vkl126_NoInfo  0b00
            #define Vkl126_On      0b01
            #define Vkl126_Off     0b10

            unsigned    Vkl:2;  /* ВКЛ:
                                 * Vkl126_NoInfo   0b00 - нет информации
                                 * Vkl126_On       0b01 - аппаратура ОП включена, опознавание производится,
                                 * Vkl126_Off      0b10 - аппаратура ОП включена, опознавание не производится, */

            // ИспрОП
            #define IsprOP126_NoInfo       0b00
            #define IsprOP126_Serviceable  0b01
            #define IsprOP126_Defective    0b10

            unsigned    IsprOP:2;   /* ИспрОП:
                                     * IsprOP126_NoInfo        0b00 - нет информации
                                     * IsprOP126_Serviceable   0b01 - Аппаратура ОП исправна
                                     * IsprOP126_Defective     0b10 - неисправна */

            // РезОП
            #define RezOP126_NoInfo    0b0000
            #define RezOP126_Friend    0b0001
            #define RezOP126_Enemy     0b0010

            unsigned    RezOP:4;    /* РезОП:
                                     * RezOP126_NoInfo 0b0000 - нет информации
                                     * RezOP126_Friend 0b0001 - СВОЙ
                                     * RezOP126_Enemy  0b0010 - ЧУЖОЙ */

            unsigned    :4;
        } TargetRecognition;    // опознавание

        struct
        {
            unsigned    :1;
            unsigned    PravG:3;    // ПравГ - правило обстрела групповой цели
            unsigned    KolG:3;     // КолГ - количество целей в группе
            unsigned    PG:1;       // ПГ - признак групповой цели:   1 - групповая,  0 - не групповая

        } GroupTarget;

        struct
        {
            // Вид помехи
            #define Kind126_NoInfo  0b0000
            #define Kind126_AZNP    0b0001
            #define Kind126_PPCHNP  0b0010
            #define Kind126_PPCHBP  0b0011
            #define Kind126_IOP     0b0100
            #define Kind126_NP      0b0101
            #define Kind126_PP      0b0110

            unsigned    Kind:4;     /* Вид помехи:
                                     * Kind126_NoInfo  0b0000 - нет информации
                                     * Kind126_AZNP    0b0001 - АКТИВНАЯ ЗАГРАДИТЕЛЬНАЯ НЕПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                     * Kind126_PPCHNP  0b0010 - ПРИЦЕЛЬНАЯ ПО ЧАСТОТЕ НЕПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                     * Kind126_PPCHBP  0b0011 - ПРИЦЕЛЬНАЯ ПО ЧАСТОТЕ БЫСТРОПЕРЕСТРАИВАЕМАЯ ПОМЕХА
                                     * Kind126_IOP     0b0100 - ИМПУЛЬСНЫЕ ОТВЕТНЫЕ ПОМЕХИ
                                     * Kind126_NP      0b0101 - НЕСИНХРОННЫЕ ПОМЕХИ
                                     * Kind126_PP      0b0110 - ПАССИВНЫЕ ПОМЕХИ */

            unsigned    Power:1;    // Ин - интенсивность помехи:  1 - сильная, 0 - слабая
            unsigned    :3;
        } Jamming;  // помеха в канале

        sAsterix12xCoordinates Coordinates;   // Координаты Север,  Высота, Восток  относительно ЗРК, ц.м.р. 50м, отрицательные в дополнительном коде.
        sAsterix12xCoordinates Speed;         // Скорости по координатам с ц.м.р. 1 м/сек, отрицательные в дополнительном коде.
    } TargetCharacteristics;    // характеристики цели
} sAsterix126_050_StateOfChannel_Body;

typedef struct
{
    sAsterix126_050_StateOfChannel_Header   Header;
    sAsterix126_050_StateOfChannel_Body     Body;
} sAsterix126_050_StateOfChannel;

// ========================== FSPEC I126 ======================================================================
typedef union
{
    uint8_t         FSPEC1[2];                  // весь FSPEC побайтно

    struct
    {
        unsigned    FX1:1;                     // FX Индикатор расширения поля
        unsigned    :1;
        unsigned    CoordinatesZRK:1;          // I126/045 Координаты ЗРК
        unsigned    RocketInfo:1;              // I126/040 Информация о ракетах
        unsigned    StateOfZRK:1;              // I126/030 Состояние ЗРК
        unsigned    BisectorWorkingSector:1;   // I126/020 Биссектриса рабочего сектора
        unsigned    NumZRK:1;                  // I126/015 Номер ЗРК
        unsigned    Id:1;                      // I126/010 Идентификатор пункта управления (ПУ)

        unsigned    FX2:1;                     // FX Индикатор расширения поля
        unsigned    :3;
        unsigned    StateOfChannel4:1;         // I126/080 Фаза работы ЗРК и состояние целевого канала 4
        unsigned    StateOfChannel3:1;         // I126/070 Фаза работы ЗРК и состояние целевого канала 3
        unsigned    StateOfChannel2:1;         // I126/060 Фаза работы ЗРК и состояние целевого канала 2
        unsigned    StateOfChannel1:1;         // I126/050 Фаза работы ЗРК и состояние целевого канала 1

    } Bits;   // весь FSPEC побитно
} uAsterix126_FSPEC;

// =============================== Asterix I126 весь ==========================================================
typedef struct
{
    uAsterix126_FSPEC               FSPEC1;                  // FSPEC
    uint16_t                        Id;                     // I126/010 Идентификатор источника данных (ПУ)
    uint8_t                         NumZRK;                 // I126/015 Номер ЗРК
    uint8_t                         BisectorWorkingSector;  // I126/020 Биссектриса рабочего сектора
    sAsterix126_030_sStateOfZRK     StateOfZRK;             // I126/030 Состояние ЗРК
    sAsterix126_040_RocketInfo      RocketInfo;             // I126/040 Информация о ракетах на ЗРК
    sAsterix_WGS_Coordinates        CoordinatesZRK;         // I126/045 Положение источника данных
    sAsterix126_050_StateOfChannel  StateOfChannel[4];      // I126/050 Состояние целевого канала
} sAsterix126;


typedef struct
{
    uint8_t     cat;
    uint16_t    messageLength;
    uint8_t     FSPEC1;
    uint8_t     FSPEC2;

    uint16_t    Id;                     // I126/010 Идентификатор источника данных (ПУ)
    uint8_t     NumZRK;                 // I126/015 Номер ЗРК
    uint8_t     BisectorWorkingSector;  // I126/020 Биссектриса рабочего сектора

    uint8_t     sostZRK[3];             // I126/030 Состояние ЗРК
    uint8_t     missileInfo[6];         // I126/040 Информация о ракетах

    uint8_t     coordsZRK[8];           // I126/045 Координаты ЗРК

    uint8_t     targetInfo1[30];        // I126/050 Фаза работы ЗРК и состояние целевого канала 1

}PROTOKOL_FULL_126;


typedef struct
{
    uint8_t cat;
    uint16_t messageLength;
    uint8_t FSPEC1;

}HEADER_125;



//typedef struct
//{
//    uint16_t                        Id;                     //
//    uint8_t                         NumZRK;                 // I126/015 Номер ЗРК

//    uint8_t sectorOtvetstvennostiNaznachit;
//    uint8_t sectorOtvetstvennostiStart;
//    uint8_t sectorOtvetstvennostiEnd;

//}PROTOKOL_125;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  I001  /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union
{
    uint8_t     MessageAboutTarget[2];

    struct
    {
        unsigned    FX:1;                           // FX Индикатор расширения поля
        unsigned    RAB:1;                          // Точка или трек от контрольного ответчика
        unsigned    SPI:1;                          // Наличие импульса SPI
        unsigned    ANT:1;                          // Сообщение об отметке получено с первой антенны

        #define     SSR_PSR_None    0b00
        #define     SSR_PSR_N1      0b01
        #define     SSR_PSR_N2      0b10
        #define     SSR_PSR_Cmb     0b11
        unsigned    SSR_PSR:2;                      /* Радиолокационное обнаружение на последнем обзоре:
                                                     * SSR_PSR_None    0b00 - Нет обнаружения
                                                     * SSR_PSR_N1      0b01 - Обнаружение только по первичному каналу
                                                     * SSR_PSR_N2      0b10 - Обнаружение только по вторичному каналу
                                                     * SSR_PSR_Cmb     0b11 - Комбинированное обнаружение по первичному и вторичному каналам */
        unsigned    SIM:1;                          // 0 - Фактическая отметка или трек, 1 - Имитированная точка или трек
        unsigned    TYP:1;                          // 0 - Отметка, 1 - Трек


        unsigned    :1;

        #define     ZCode_Unknown   0b00
        #define     ZCode_Low       0b10
        #define     ZCode_High      0b11
        unsigned    ZCode:2;                        /* Код зоны:
                                                     * ZCode_Unknown   0b00 - Неизвестно
                                                     * ZCode_Low       0b10 - Отметка обнаружена в нижней зоне углов места
                                                     * ZCode_High      0b11 - Отметка обнаружена в верхней зоне углов места */
        unsigned    MI:1;                           // Идентификация военного объекта
        unsigned    ME:1;                           // Сигнал бедствия от военного воздушного судна

        #define     DS_Default              0b00
        #define     DS_IllegalIntrusion     0b01
        #define     DS_RadioFailure         0b10
        #define     DS_Glitch               0b11
        unsigned    DS1_DS2:2;                      /* DS_Default              0b00 - По умолчанию
                                                     * DS_IllegalIntrusion     0b01 - Незаконное вторжение (код 7500)
                                                     * DS_RadioFailure         0b10 - Отказ радиосвязи (код 7600)
                                                     * DS_Glitch               0b11 - Авария (код 7700) */
        unsigned    TST:1;                          // 0 по умолчанию (???)
    } Bits;
} sAsterix001_020_MessageAboutTarget;

typedef struct
{
    uint16_t        Distance;   // LSB = 1/128 морской мили. Дальность максимальная = 512 м. миль.
    uint16_t        Azimuth;    // LSB = 360°/(2^16).
} sAsterix001_040_PolarCoordinates;

typedef struct
{
    int16_t         X,Y;    // LSB = 1/64 м. мили (Отрицательные значения X,Y записываются в дополнительном коде.)
} sAsterix001_042_CartesianCoordinates;

typedef union
{
    uint16_t        AllCode;

    struct
    {
        unsigned    D:3;    // код режима в восьмеричном виде
        unsigned    C:3;    // код режима в восьмеричном виде
        unsigned    B:3;    // код режима в восьмеричном виде
        unsigned    A:3;    // код режима в восьмеричном виде
        unsigned    :1;
        unsigned    L:1;    // 0 - код режима полученный от ответчика, 1 - код режима сглаженный в местном устройстве
        unsigned    G:1;    // 0 - по умолчанию, 1 - искаженный код ответа
        unsigned    V:1;    // 0 - код достоверный, 1 - код недостоверный

    } Bits;
} uAsterix001_050_070_Code;

typedef union
{
    uint16_t        AllCode;

    struct
    {
        unsigned    Height:14;
        unsigned    G:1;
        unsigned    V:1;
    } Bits;
} uAsterix001_090_CodeModeC;

typedef struct
{
    unsigned        :5;
    unsigned        MAN:1;      // 0 - по умолчанию, 1 - признак маневра
    unsigned        RAD:1;      // 0 - траектория от первичного радиолокатора, 1 - Траектория от вторичного радиолокатора или комбинированная
    unsigned        :1;
} sAsterix001_170_TrajectoryStatus;

typedef struct
{
    uint16_t    Velocity;       // Скорость, LSB = 2^(-14) морских миль в секунду (м.миль/с)
    uint16_t    Course;         // Курс, LSB = 360°/(2^16)
} sAsterix001_200_VelocityVector;

typedef struct
{
    unsigned    :1;
    unsigned    PassCounter:3;  // Количество пропусков
    unsigned    NewTrack:1;     // Новая трасса (0 - нет, 1 - есть)
    unsigned    Refurbished:1;  // Восстановленные трассы (0 - нет, 1 - есть)
    unsigned    Quality:1;      // качество
    unsigned    :1;
} sAsterix001_210_TrajectoryQuality;

typedef union
{
    uint16_t    AllHeight;

    struct
    {
        unsigned    Height:14;  /* высота выше уровня моря, LSB = 25 футов. Биты знаковые (!!!!!!)
                                 * Отрицательные значения высоты выражаются вдополнительной форме,
                                 * старший бит устанавливается в 0 для положительных значений
                                 * и в 1 – для отрицательных значений величины */

        #define     HS_Default          0b00
        #define     HS_3Coord           0b01
        #define     HS_RadioAltimeter   0b10
        unsigned    HS:2;       /* Признак высоты
                                 * HS_Default          0b00 - по умолчанию
                                 * HS_3Coord           0b01 - Высота трехкоординатной РЛС
                                 * HS_RadioAltimeter   0b10 - Высота от радиовысотомера */
    } Bits;
} uAsterix001_Height;

typedef struct
{
    unsigned    B:2;
    unsigned    A:3;
    unsigned    :3;
} sAsterix001_CodeMode1;

typedef struct
{
    uint8_t             Length;     // Длина
    uAsterix001_Height  Height;
    sAsterix001_CodeMode1          CodeMode1;  // Код Режима 1 в восьмеричной форме
} sAsterix001_SDIplot;

typedef struct
{
    uint8_t                         Length;     // Длина сообщения в байтах. Значение равно 7 или 14
    unsigned                        :24;
    uAsterix001_Height              Height;
    sAsterix001_CodeMode1           CodeMode1;
    char                            Callsign[7];    // позывной, байты нужно "переворачивать"
} sAsterix001_SDItrack;

// ========================== FSPEC I001 ===================================================================
typedef union
{
    uint8_t         FSPEC1[3];                       // весь FSPEC побайтно

    struct
    {
        unsigned    FX1:1;                          // FX Индикатор расширения поля
        unsigned    PartTime:1;                     // I001/141 Неполное время суток
        unsigned    :1;
        unsigned    CodeModeC:1;                    // I001/090 Код высоты в режиме С
        unsigned    CodeMode3A:1;                   // I001/070 Код опознавания в режиме 3/А
        unsigned    PolarCoordinates:1;             // I001/040 Измеренные полярные координаты
        unsigned    MessageAboutTarget:1;           // I001/020 Описание сообщения о цели
        unsigned    Id:1;                           // I001/010 Идентификатор источника данных

        unsigned    FX2:1;                          // FX Индикатор расширения поля
        unsigned    WarningError:1;                 // I001/030 Предупреждение/ошибка
        unsigned    :5;
        unsigned    CodeMode2:1;                    // I001/050 Код опознавания режима 2

        unsigned    FX3:1;                          // FX Индикатор расширения поля
        unsigned    :1;
        unsigned    SDIplot:1;                      // Специальный элемент данных  первичных отметок
        unsigned    :5;

    } BitsPlot;     // весь FSPEC побитно (отметки)

    struct
    {
        unsigned    FX1:1;                          // FX Индикатор расширения поля
        unsigned    CodeMode3A:1;                   // I001/070 Код опознавания в режиме 3/А
        unsigned    VelocityVector:1;               // I001/200 Вычисленный вектор курсовой скорости
        unsigned    CartesianCoordinates:1;         // I001/042 Рассчитанные декартовы координаты
        unsigned    :1;
        unsigned    TrackNumber:1;                  // I001/161 Номер траектории
        unsigned    MessageAboutTarget:1;           // I001/020 Описание сообщения о цели
        unsigned    Id:1;                           // I001/010 Идентификатор источника данных

        unsigned    FX2:1;                          // FX Индикатор расширения поля
        unsigned    TrajectoryQuality:1;            // I001/210 Качество траектории
        unsigned    TrajectoryStatus:1;             // I001/170 Статус траектории
        unsigned    :3;
        unsigned    PartTime:1;                     // I001/141 Неполное время суток
        unsigned    CodeModeC:1;                    // I001/090 Код высоты в режиме С

        unsigned    FX3:1;                          // FX Индикатор расширения поля
        unsigned    :1;
        unsigned    SDItrack:1;                     // Специальный элемент данных трассы
        unsigned    :4;
        unsigned    CodeMode2:1;                    // I001/050 Код опознавания режима 2

    } BitsTrack;    // весь FSPEC побитно (трассы);
} uAsterix001_FSPEC;

// =============================== Asterix I001 весь ==========================================================
typedef struct
{
    uAsterix001_FSPEC                       FSPEC1;                  // FSPEC
    uint16_t                                Id;                     // I001/010 Идентификатор источника данных
    sAsterix001_020_MessageAboutTarget      MessageAboutTarget;     // I001/020 Описание сообщения о цели
    uint8_t                                 WarningError;           // I001/030 Предупреждения и ошибки (Все биты данного байта устанавливаются в 0)
    sAsterix001_040_PolarCoordinates        PolarCoordinates;       // I001/040 Измеренные полярные координаты
    sAsterix001_042_CartesianCoordinates    CartesianCoordinates;   // I001/042 Вычисленные декартовы координаты
    uAsterix001_050_070_Code                CodeMode2;              // I001/050 Код Режима 2 в восьмеричном виде
    uAsterix001_050_070_Code                CodeMode3A;              // I001/070 Код опознавания Режима 3/А
    uAsterix001_090_CodeModeC               CodeModeC;              // I001/090 Код высоты в режиме С
    uint16_t                                PartTime;               // I001/141 Неполное время суток (LSB = 1/128 c)
    uint16_t                                TrackNumber;            // I001/161 Номер траектории
    sAsterix001_170_TrajectoryStatus        TrajectoryStatus;       // I001/170 Статус траектории
    sAsterix001_200_VelocityVector          VelocityVector;         // I001/200 Вектор курсовой скорости
    sAsterix001_210_TrajectoryQuality       TrajectoryQuality;      // I001/210 Качество траектории сопровождения
    sAsterix001_SDIplot                     SDIplot;                // I001 SDIplot Специальный элемент данных об отметке (SDI Plot)
    sAsterix001_SDItrack                    SDItrack;               // I001 SDItrack Специальный элемент данных о трассе (SDI Track)
} sAsterix001;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  I002  /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    struct
    {
        unsigned    FX:1;       // 0 - Конец первичного подполя, 1 - Расширение Первичного подполя в следующий октет.
        unsigned    :2;
        unsigned    SSR:1;      // Состояние ВРЛ источника (0 - нет, 1 - есть)
        unsigned    PSR:1;      // Состояние ПРЛ источника --//--
        unsigned    RS:1;       // Состояние РЛС --//--
        unsigned    :1;
        unsigned    COM:1;      // Общая часть. --//--

    } Header;

    struct
    {
        unsigned    :3;
        unsigned    MSC:1;
        unsigned    OVL:1;
        unsigned    ChAB:2;
        unsigned    ANT:1;

    } SSR; // все биты всегда устанавливаются в 0

    struct
    {
        unsigned    :1;

        #define     WorkMode_0_6        0b00
        #define     WorkMode_03_36      0b01
        #define     WorkMode_06_6_30    0b10
        #define     WorkMode_0_3        0b11
        unsigned    WorkMode:2; /* Режим работы РЛС:
                                 * WorkMode_0_6        0b00 - зона обзора по углу места 0°-6° на каждом обороте
                                 * WorkMode_03_36      0b01 - зона обзора по углу места меняется через оборот: 0°-3° и 3°-6°
                                 * WorkMode_06_6_30    0b10 - зона обзора по углу места меняется через оборот: 0°-6° и 6°-30°
                                 * WorkMode_0_3        0b11 - зона обзора по углу места 0°-3° на каждом обороте */
        unsigned    MSC:1;      // Состояние системы контроля. Всегда устанавливается в 0
        unsigned    OVL:1;      // Условие перезагрузки. Всегда устанавливается в 0
        unsigned    ChAB:2;     // Селекция канала А/В. Всегда устанавливается 0b01
        unsigned    ANT:1;      // Селекция антенны. Всегда устанавливается в 0

    } PSR; // Структура подполя PSR: Информация о состоянии ПРЛ источника.

    struct
    {
        unsigned    :1;
        unsigned    Condition:3;// Состояние РЛС: 0 - исправна, 1 - неисправна
        unsigned    :4;

    } RS;

    struct
    {
        unsigned    :1;
        unsigned    TSV:1;      // Достоверность источника времени. 0 - достоверен, 1 - нет
        unsigned    MSC:1;      // Состояние подключения системы контроля. 0 - Система контроля подключена
        unsigned    OVL_XMT:1;  // Наличие перегрузки системы передачи. 0 - Нет перегрузки, 1 - Есть перегрузка
        unsigned    OVL_RDP:1;  // Индикатор перегрузки RDP. 0 - Нет перегрузки, 1 - Есть перегрузка
        unsigned    RDPR:1;     // Запуск/перезапуск RDP. 0 - По умолчанию, 1 - Запуск RDP
        unsigned    RDPC:1;     // Статус выбора цепи RDP. 0 - Выбран RDP 1
        unsigned    NOGO:1;     // Операционное состояние системы. 0 - РЛС работает, 1 - Данные от РЛС не должны использоваться

    } COM;
} sAsterix002_RLS_Config; // работает как FSPEC. бит выставлен - добавляем структуру в пакет

// ========================== FSPEC I002 ===================================================================
typedef union
{
    uint8_t         FSPEC1[2];                       // весь FSPEC побайтно

    struct
    {
        unsigned    FX1:1;                          // FX Индикатор расширения поля
        unsigned    :1;
        unsigned    RLS_Config:1;                   // I002/050 Конфигурация РЛС
        unsigned    SpeedRotate:1;                  // I002/041 Период вращения антенны
        unsigned    Time:1;                         // I002/030 Полное время дня
        unsigned    NumSector:1;                    // I002/020 Номер сектора
        unsigned    TypeMessage:1;                  // I002/000 Тип сообщения
        unsigned    Id:1;                           // I002/010 Идентификатор источника данных

        unsigned    FX2:1;                          // FX Индикатор расширения поля
        unsigned    :3;
        unsigned    Position:1;                     // I002/120 Положение источника данных (высота, широта, долгота)
        unsigned    :3;

    } Bits;    // весь FSPEC побитно
} uAsterix002_FSPEC;

// =============================== Asterix I002 весь ==========================================================
typedef struct
{
    uAsterix002_FSPEC                       FSPEC1;                  // FSPEC
    uint16_t                                Id;                     // I002/010 Идентификатор источника данных
    uint8_t                                 TypeMessage;            /* I002/000 Тип сообщения
                                                                     * 001 – сообщение о метке севера. Передаётся только при пересечении метки севера.
                                                                     * 002 – сообщение о пересечении сектора (вырабатываются, когда антенна пересекает соответствующий азимут (22.5, 45, …).
                                                                     * При пересечении метки севера не выдаётся. */
    uint8_t                                 NumSector;              // I002/020 Номер сектора. LSB = 360°/(2^8)
    uint8_t                                 Time[3];                // I002/030 Время дня, 3 байта, LSB = 1/128c
    uint16_t                                SpeedRotate;            // I002/041 Период вращения антенны, LSB = 1/128c
    sAsterix002_RLS_Config                  RLS_Config;             // I002/050 Конфигурация РЛС
    sAsterix_WGS_Coordinates                Position;               // I002/120 Положение источника данных (высота, широта, долгота)
} sAsterix002;

#pragma pack(pop)
#endif // STRUCTURESASTERIX_H
