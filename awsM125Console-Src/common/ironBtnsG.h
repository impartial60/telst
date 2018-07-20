#ifndef IRONBTNSG
#define IRONBTNSG

#pragma pack(push,1)
typedef struct
{
    bool vkluchen;
    bool imitator;
    bool FK;
    bool antenna_eqv;
    bool izlychenie;

} REJIM_RABOTI_UNV;
#pragma pack(pop)


#pragma pack(push,1)

//enum ironBtnsRejimPoiska {krygovojPoisk, bolshojSectornijPoisk, malijSectornijPoisk, poiskVikl};
enum ironBtnsRejimZahvata {rejimZahvataASD, rejimZahvataASFD, rejimZahvataAS_RSDV, rejimZahvataASF, rejimZahvataRSF, rejimZahvataZahvat};
enum ironBtnsRejimRabotiSNR {rejimRabotiSNR_UNV, rejimRabotiSNR_UNV_TOV, rejimRabotiSNR_TOV};
enum ironBtnsErrors {ironButtonsNoError, tooMachButtonsArePressed, attempt2DecreaseMinimumLitera, attempt2IncreaseMaximumLitera, unknownError};
enum ironBtnsKeyDogon {dogonZero, dogonPlus, dogonMinus};

typedef struct
{
    bool newCommand;
    bool isHoldingDown;
    u_int8_t buttonsErrorCodes;

    u_int8_t rejimPoiska;
    u_int8_t rejimZahvata;
    u_int8_t litera;
    u_int8_t rejimRabotiSNR;
    REJIM_RABOTI_UNV rejimRabotiUNV;
    bool privod;
    bool UV10;
    bool uv11;
    bool GSh;


} IRON_BUTTONS_G;
#pragma pack(pop)


#endif // IRONBTNSG

