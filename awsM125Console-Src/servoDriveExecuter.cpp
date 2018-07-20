#include "extern.h"
#include "srcKram/Drive125.h"

//#define real_work

//--------------------------ADD Kramarenko A.V.-----19.03.2018-----------------------
extern  Drive125 *drive_unit;
//----------------------------------------------------------------------------------------

extern float convert360angle2PlusMinus180(float dltAz360);
extern void azimuthPakageDetector();

bool unvClockwiseScanDirection = true;

float directFactor = 1.0;

int cntr = 0;

extern int dCntr;

//int komJustExecuteCounter[] = {-1,-1};

void servoDriveExecuter()
{
    //qDebug()<<"servoDriveExecuter ********** settings->servoDriveDiagnostic:"<<settings->servoDriveDiagnostic;

    //printf("printf servoDriveExecuter: %d\n", settings->servoDriveDiagnostic);

    cntr++;

    if(snrInfoG.rejimRabotiZRK == RejimRabotiZRK::TR_A)
        snrInfoG.targetIndexCY = oficerNaved2Console.guidanceOfficerCmds.targetIndexCY;
//    else
//        snrInfoG.simulatorTargetIndexCY = 0;

    snrInfoG.rejimSoprovojdeniaSNR = oficerNaved2Console.guidanceOfficerCmds.rejimSoprovojdeniaSNR;

    if(snrInfoG.rejimPoiskaSNR == POISK_BSP)
        snrInfoG.azimuthScanSectorHalfWidth = 5.0;
    else
        if(snrInfoG.rejimPoiskaSNR == POISK_MSP)
            snrInfoG.azimuthScanSectorHalfWidth = 2.5;

//    if(snrInfoG.rejimPoiskaSNR != RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT && snrInfoG.rejimPoiskaSNR != RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE)//if(snrInfoG.zahvat == false)
//    {
//        for(int i = 0; i < 2; i++)
//        {
//            old_state_F[i].Position[0] = 0.0;
//            /////////////////////////////snrInfoG.handWheelFSpeed[i] = 0.0;
//        }
//    }

    //  А З И М У Т   А З И М У Т   А З И М У Т   А З И М У Т   А З И М У Т
    //qDebug()<<"servoDriveExecuter: komKomG.kanalSvoboden:"<<komKomG.kanalSvoboden<<"rejimRabotiZRK:"<<komKomG.rejimRabotiZRK;

    if(snrInfoG.remoteControlZRK) //if(shMemC2U.commander2Guidance.komKom.remoteControlZRK) //  РАБОТА ПО ЦУ ОТ КП
    {
        if(vkp2zrkG.komandiPoCelevomyKanaly1.commandInfoExist)
        {
            if(vkp2zrkG.komandiPoCelevomyKanaly1.OCU)
            {
                snrInfoG.rejimPoiskaSNR = POISK_VIKLUCHEN;

                qDebug()<<"servoDriveExecuter: metka 1 - отмена ЦУ"<<"cntr:"<<cntr<<"CU:"<<vkp2zrkG.komandiPoCelevomyKanaly1.CU<<"OCU:"<<vkp2zrkG.komandiPoCelevomyKanaly1.OCU;
            }

            if(vkp2zrkG.komandiPoCelevomyKanaly1.CU)
            {
                if(snrInfoG.rejimPoiskaSNR == POISK_VIKLUCHEN)
                {
                    snrInfoG.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT;
                }
                else
                {
                    if(vkp2zrkG.komandiPoCelevomyKanaly1.targetInfoExist)
                    {
                        float azCU_grad = atan2(vkp2zrkG.coordYHX.x_km, vkp2zrkG.coordYHX.y_km) * Rad2Grad;

                        float deltaAz = azCU_grad - snrInfoG.currentAzimuth;

                        if(deltaAz > 180)
                            deltaAz -= 360.0;

                        if(deltaAz < -180)
                            deltaAz += 360.0;

                drive_unit->unv->device_setpos_az(drive_unit->unv->device_getpos_az()+ deltaAz);

                        float deltaMax = 2.5;

                        if(fabs(deltaAz) < deltaMax)
                            snrInfoG.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE;
                        else
                            snrInfoG.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT;

//                        qDebug()<<"servoDriveExecuter: metka 3 cntr:"<<cntr<<"azCU_grad:"<<azCU_grad<<"currentAzimuth"<<snrInfoG.currentAzimuth<<"deltaAz:"<<deltaAz
//                               <<"snrInfoG.rejimPoiskaSNR:"<<snrInfoG.rejimPoiskaSNR<<"targetInfoExist:"<<vkp2zrkG.komandiPoCelevomyKanaly1.targetInfoExist
//                              <<"coord:"<<vkp2zrkG.coordYHX.y_km<<vkp2zrkG.coordYHX.x_km<<"CU:"<<vkp2zrkG.komandiPoCelevomyKanaly1.CU<<"OCU:"<<vkp2zrkG.komandiPoCelevomyKanaly1.OCU;
                    }
                }
            }
        }
    }
    else    // М Е С Т Н О Е   У П Р А В Л Е Н И Е   З Р К
    {
        // О Т Р А Б О Т К А   К Л И К А   М Ы Ш И   П О   Ш К А Л А М   кроме режима захвата и сопровождения
        if(snrInfoG.rejimPoiskaSNR != POISK_PEREKLYCHEN_V_ZAHVAT && snrInfoG.rejimPoiskaSNR != POISK_PEREKLYCHEN_V_SOPROVOJDENIE)
        {
            if(oficerNaved2Console.guidanceOfficerCmds.servoDriveExecute[AZIMUTH])    // отработка клика мыши по шкале азимута
            {
                //if(snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_VIKLUCHEN)
                    //p1.pos_cmd = p1.curr_pos + oficerNaved2Console.guidanceOfficerCmds.servoDriveDelta[AZIMUTH];
                //else
                    //snrInfoG.directrisaAzimuth += oficerNaved2Console.guidanceOfficerCmds.servoDriveDelta[AZIMUTH];

                drive_unit->unv->device_setpos_az(convert360angle2PlusMinus180(snrInfoG.directrisaAzimuth + oficerNaved2Console.guidanceOfficerCmds.servoDriveDelta[AZIMUTH]));   //  2018-05-08
                snrInfoG.directrisaAzimuth = drive_unit->unv->device_getpos_az();                                                                                                    //  2018-05-08
            }

            if(komKomM.srv[AZIMUTH].executeCounter % 10 > 0)    // отработка клика мыши по шкале азимута командиром
            {
                if(komKomM.srv[AZIMUTH].executeCounter / 10 > snrInfoG.komJustExecuteCounter[AZIMUTH])  // это действительно новый клик
                {                    
                    snrInfoG.komJustExecuteCounter[AZIMUTH] = komKomM.srv[AZIMUTH].executeCounter / 10;

//                    if(snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_VIKLUCHEN)
//                    {
//                        p1.pos_cmd = p1.curr_pos + komKomM.srv[AZIMUTH].delta;
//                    }
//                    else
//                        snrInfoG.directrisaAzimuth += komKomM.srv[AZIMUTH].delta;

                    drive_unit->unv->device_setpos_az(convert360angle2PlusMinus180(snrInfoG.directrisaAzimuth + komKomM.srv[AZIMUTH].delta));  //  2018-05-08
                    snrInfoG.directrisaAzimuth = drive_unit->unv->device_getpos_az();                                                             //  2018-05-08
                }
                else
                    if(komKomM.srv[AZIMUTH].executeCounter / 10 < snrInfoG.komJustExecuteCounter[AZIMUTH])   // случай когда АРМ командира был перезапущен
                        snrInfoG.komJustExecuteCounter[AZIMUTH] = komKomM.srv[AZIMUTH].executeCounter / 10 - 1;
            }

            if(oficerNaved2Console.guidanceOfficerCmds.servoDriveExecute[ELEVANG])    // отработка клика мыши по угломестной шкале
            {
                drive_unit->unv->device_setpos_elv(drive_unit->unv->device_getpos_elv() + oficerNaved2Console.guidanceOfficerCmds.servoDriveDelta[ELEVANG]);
            }

            if(komKomM.srv[ELEVANG].executeCounter % 10 > 0)    // отработка клика мыши по угломестной шкале командиром
            {
                if(komKomM.srv[ELEVANG].executeCounter / 10 > snrInfoG.komJustExecuteCounter[ELEVANG])  // это действительно новый клик
                {                    
                    snrInfoG.komJustExecuteCounter[ELEVANG] = komKomM.srv[ELEVANG].executeCounter / 10;
                    drive_unit->unv->device_setpos_elv(drive_unit->unv->device_getpos_elv() + komKomM.srv[ELEVANG].delta);
                }
                else
                    if(komKomM.srv[ELEVANG].executeCounter / 10 < snrInfoG.komJustExecuteCounter[ELEVANG])   // случай когда АРМ командира был перезапущен
                        snrInfoG.komJustExecuteCounter[ELEVANG] = komKomM.srv[ELEVANG].executeCounter / 10 - 1;
            }
        }

        if(oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_VIKLUCHEN)  // старая или новая команда офицера наведения выключить и поиск и сопровождение
        {
            if(snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP ||      // сканирования было включено,
               snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)        // выключаем сканирование и возвращаем УНВ на директрису
            {
                drive_unit->unv->device_setpos_az(snrInfoG.directrisaAzimuth);
            }
        }
        else    // офицер наведения включил или поиск или захват
        {
            if(oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)  // офицер наведения включил поиск
            {
                if(snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || snrInfoG.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP) //if(currentUNVScanIsOn)  // сканирование продолжается
                {
                    drive_unit->unv->device_setpos_az(snrInfoG.directrisaAzimuth + directFactor * snrInfoG.azimuthScanSectorHalfWidth);

                    if(fabs(convert360angle2PlusMinus180(drive_unit->unv->device_getpos_az() - snrInfoG.currentAzimuth)) < 0.1)  // дошли до границы сектора - возвращаемся
                    {
                        unvClockwiseScanDirection = !unvClockwiseScanDirection;

                        azimuthPakageDetector();
                        dCntr = 0;

                        directFactor = 1.0;
                        if(unvClockwiseScanDirection == false)
                            directFactor = -1.0;
                    }
                }
                else    // сканирование начинается
                {
                    ////////////////////   //  2018-05-08   //  2018-05-08  //  2018-05-08            snrInfoG.directrisaAzimuth = snrInfoG.currentAzimuth;  // при включении сканирования запоминаем директрису

                    unvClockwiseScanDirection = true;
                    drive_unit->unv->device_setpos_az(convert360angle2PlusMinus180(snrInfoG.directrisaAzimuth + snrInfoG.azimuthScanSectorHalfWidth));    //  2018-05-08
                }
            }
            else    // офицер наведения включил захват, в режиме тренажа при АС сопровождение вкючится само, когда сервопривод отработает азимут
            {
                if(oficerNaved2Console.guidanceOfficerCmds.targetIndexCY >= 0 && oficerNaved2Console.guidanceOfficerCmds.targetIndexCY < MAXTOCKANUM)   //  эта проверка на тот случай, если simulatorTargetIndexCY == -1,
                {                                                                                                                                   //  так быть не должно никогда

                    if(snrInfoG.rejimSoprovojdeniaSNR == SOPROVOJDENIE_AS ||            // в режиме тренажа АС по углам
                            snrInfoG.rejimPoiskaSNR == POISK_PEREKLYCHEN_V_ZAHVAT)      // и отработка ЦУ по углам в РС без ошибок
                    {
                        drive_unit->unv->device_setpos_az(snrInfoG.nextTocka[oficerNaved2Console.guidanceOfficerCmds.targetIndexCY].azimuth360);
                        drive_unit->unv->device_setpos_elv(snrInfoG.nextTocka[oficerNaved2Console.guidanceOfficerCmds.targetIndexCY].elevationAngle);
                    }
                    else    // РС по углам ведется по штурвалам операторов
                    {
                        float dAzimSpeed = snrInfoG.handWheelFSpeed[0] * SIN45COS45 +  snrInfoG.handWheelFSpeed[1] * SIN45COS45;    //  штатный режим
                        float dElAngSpeed = snrInfoG.handWheelFSpeed[0] * SIN45COS45 - snrInfoG.handWheelFSpeed[1] * SIN45COS45;    //  штатный режим

                        //float dElAngSpeed = 0; float dAzimSpeed = 0.01 * shMemU2C.guidanceOfficerCmds.integerValue4Test;    //  тест с помощью слайдера

                        drive_unit->unv->device_setpos_az(drive_unit->unv->device_getpos_az() + dAzimSpeed);    //  штатный режим
                        drive_unit->unv->device_setpos_elv(drive_unit->unv->device_getpos_elv() + dElAngSpeed);   //  штатный режим
                    }

                    float deltaMax = 0.3;
                    float deltaAz = snrInfoG.nextTocka[oficerNaved2Console.guidanceOfficerCmds.targetIndexCY].azimuth360 - snrInfoG.currentAzimuth;
                    if(deltaAz > 180)
                        deltaAz -= 360.0;

                    float deltaElAngle = snrInfoG.nextTocka[oficerNaved2Console.guidanceOfficerCmds.targetIndexCY].elevationAngle - snrInfoG.currentDirectrisaElevationAngle;

                    if(fabs(deltaAz) <= deltaMax && fabs(deltaElAngle) <= deltaMax)
                    {
                        snrInfoG.rejimPoiskaSNR = POISK_PEREKLYCHEN_V_SOPROVOJDENIE;
                    }
                }
            }
        }
    }

    if(oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR != POISK_PEREKLYCHEN_V_ZAHVAT ||     // после отработки команды синхронизируем snrInfo с guidanceOfficerCmds
      (oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR == POISK_PEREKLYCHEN_V_ZAHVAT &&     // кроме переключения в сопровождение, которое происходит автоматически
       snrInfoG.rejimPoiskaSNR != POISK_PEREKLYCHEN_V_SOPROVOJDENIE))
    {
        snrInfoG.rejimPoiskaSNR = oficerNaved2Console.guidanceOfficerCmds.rejimPoiskaSNR;
    }


    //  А З И М У Т
 //   p1.max_acc = 10;
 //   p1.max_vel = 10;

    //  У Г О Л   М Е С Т А
 //   p2.max_acc = 10;
 //   p2.max_vel = 10;

    if(drive_unit->unv->device_getpos_elv()/*p2.pos_cmd>84*/ > 72.0)
        drive_unit->unv->device_setpos_elv(72.0)/*p2.pos_cmd = 84.0*/;

    if(drive_unit->unv->device_getpos_elv()/*p2.pos_cmd*/ < -2.0)
        drive_unit->unv->device_setpos_elv(/*p2.pos_cmd =*/ -2.0);


//    if(komKomM.servoDriveSimulation == false)   // Р А Б О Т А   С   Р Е А Л Ь Н Ы М   С Е Р В О П Р И В О Д О М
//    drive_unit->unv.set_mode(drive_unit->unv.combat);
//    else
//    drive_unit->unv.set_mode(drive_unit->unv.training);

    //-------------------------------------------------------------------

    snrInfoG.currentAzimuth = drive_unit->unv->device_getpos_az();

    int popravka_na_oboroti = snrInfoG.currentAzimuth / 360.0;
    snrInfoG.currentAzimuth -= 360.0 * popravka_na_oboroti;

    if(snrInfoG.currentAzimuth < 0)
        snrInfoG.currentAzimuth += 360.0;

    if(snrInfoG.currentAzimuth >= 360.0)
        snrInfoG.currentAzimuth -= 360.0;    

    snrInfoG.currentDirectrisaElevationAngle = drive_unit->unv->device_getpos_elv();

    //if(settings->servoDriveDiagnostic == 12345)
      //  qDebug()<<"servoDriveExecuter settings->servoDriveDiagnostic:"<<settings->servoDriveDiagnostic;
}

