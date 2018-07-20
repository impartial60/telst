#ifndef LAUNCHERINFO_H
#define LAUNCHERINFO_H

#include <QDebug>
#include <QTime>

#pragma pack(push,1)

#define  MAXPY 4

#define  MAXMISSILE 4

#define  TMAXOSTIVANIE_V_ISKHODNOM 900

#define  TMAXPEREHOD_ISKHODNOE_NAKAL 120

#define  TMAXNAKAL 7200

#define  TMAXPEREHOD_NAKAL_GOTOVA 10

#define  TMAXGOTOVA 240

#define  TMAXNAVEDENIE 70


struct MissileStatus
{
    enum Status {RAKETA_OTSYTSTVIET, ISKHODNOE, PEREHOD_ISKHODNOE_NAKAL, PEREHOD_ISKHODNOE_GOTOVA, NAKAL, PEREHOD_NAKAL_GOTOVA, GOTOVA, NAVEDENIE, NEISPRAVNA};
};

enum MissileCommands {NO_COMMAND, VKLYCHIT_ISKHODNOE, VKLYCHIT_NAKAL, VKLYCHIT_GOTOVA, PYSK};

typedef struct
{
    public:

    MissileStatus::Status missileStatus;

    float t_skolko_ostalos_perehod_iskhodnoe_nakal_sec;
    int time_vklychenie_nakala_milisec;

    float t_obratnogo_otcheta_nahojdenia_v_nakale_sec;
    int time_vklychenie_obratnogo_otscheta_nakala_milisec;

    float t_perehod_nakal_podgotovka_sec;
    int time_vklychenie_podgotovka_milisec;

    float t_obratnogo_otcheta_nahojdenia_v_gotova_sec;
    int time_vklychenie_obratnogo_otscheta_gotova_milisec;

    float t_navedenie_sec;
    int time_start_navedenie_milisec;

    float t_obratnogo_otcheta_ostivanie_v_iskhodnom_sec;
    int time_vklychenie_obratnogo_otscheta_ostivanie_v_iskhodnom_milisec = -1000000;

    int counter_ostivanie_v_iskhodnom;

} MissileInfo;

typedef struct
{
    public:

    MissileInfo missileInfo[MAXMISSILE];

} LauncherInfo;


#pragma pack(pop)

#endif // LAUNCHERINFO_H
