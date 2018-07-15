#include "extern.h"
/*
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
*/

void rsTrgtPrmsCalculator()
{
    int indCY = snrInfoG.targetIndexCY;
    if(indCY < 0 || indCY >= MAXTOCKANUM)
        return;

    if(snrInfoG.rejimSoprovojdeniaSNR == SOPROVOJDENIE_AS)
        memcpy(&snrInfoG.nextTockaRS[0], &snrInfoG.nextTocka[snrInfoG.targetIndexCY], sizeof(TargetParams));
    else
    {
//        qDebug()<<"rsTrgtPrmsCalculator groundRange_km:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundRange_km
//                                                        <<snrInfoG.nextTockaRS[0].groundRange_km;

        if(fabs(snrInfoG.nextTocka[snrInfoG.targetIndexCY].groundRange_km - snrInfoG.nextTockaRS[0].groundRange_km) > 0.2 )// ||
           //snrInfoG.currentAzimuth != snrInfoG.nextTockaRS[0].azimuth360)   //  если есть приращение дальности или азимута
        {                                                                                   //  сдивигаем на один шаг историю о цели
            for(int i = MAXTOCKANUM - 1; i > 0; i--)
            {
                memcpy(&snrInfoG.nextTockaRS[i], &snrInfoG.nextTockaRS[i - 1], sizeof(TargetParams));
            }

            snrInfoG.nextTockaRS[0].targetNo = snrInfoG.nextTocka[indCY].targetNo;
            snrInfoG.nextTockaRS[0].slantRange_km = snrInfoG.nextTocka[indCY].slantRange_km;

            snrInfoG.nextTockaRS[0].azimuth360 = snrInfoG.currentAzimuth;   //snrInfoG.nextTocka[indCY].azimuth360;
            snrInfoG.nextTockaRS[0].elevationAngle = snrInfoG.currentDirectrisaElevationAngle;  //snrInfoG.nextTocka[indCY].elevationAngle;
            snrInfoG.nextTockaRS[0].groundRange_km = snrInfoG.nextTocka[indCY].slantRange_km * cos(snrInfoG.nextTockaRS[0].elevationAngle / Rad2Grad);

            //        qDebug()<<"L blinckingTimerTick  nextTochkaL.rangeParametr_km:"<<nextTochkaL.rangeParametr_km<<shMemC2U.commander2Launcher.snrInfo.nextTockaRS[0].rangeParametr_km
            //                <<"nextTochkaL.kyrsovojParameter_km:"<<nextTochkaL.kyrsovojParameter_km<<shMemC2U.commander2Launcher.snrInfo.nextTockaRS[0].kyrsovojParameter_km;

    //        qDebug()<<"rsTrgtPrmsCalculator nextTocka.azimuth360:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].azimuth360<<snrInfoG.nextTockaRS[0].azimuth360
    //                <<"nextTochka.elevationAngle:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].elevationAngle<<snrInfoG.nextTockaRS[0].elevationAngle;

            float az180 = snrInfoG.nextTockaRS[0].azimuth360;
            if(az180 > 180.0)
                az180 -= 360.0;

            snrInfoG.nextTockaRS[0].groundX = snrInfoG.nextTockaRS[0].groundRange_km * sin(az180 / Rad2Grad);   //snrInfoG.nextTocka[indCY].groundX;
            snrInfoG.nextTockaRS[0].groundY = snrInfoG.nextTockaRS[0].groundRange_km * cos(az180 / Rad2Grad);   //snrInfoG.nextTocka[indCY].groundY;

    //        qDebug()<<"rsTrgtPrmsCalculator nextTocka.groundX:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundX<<snrInfoG.nextTockaRS[0].groundX
    //                <<"nextTochka.groundY:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundY<<snrInfoG.nextTockaRS[0].groundY;

            float kyrsTemp;

            if(snrInfoG.nextTockaRS[0].azimuth360 != snrInfoG.nextTockaRS[1].azimuth360) // общий случай - курсовой параметр не нулевой
            {
                if(snrInfoG.nextTockaRS[0].groundX != snrInfoG.nextTockaRS[1].groundX || snrInfoG.nextTockaRS[0].groundY != snrInfoG.nextTockaRS[1].groundY)
                    kyrsTemp = atan2(snrInfoG.nextTockaRS[0].groundX - snrInfoG.nextTockaRS[1].groundX,
                                     snrInfoG.nextTockaRS[0].groundY - snrInfoG.nextTockaRS[1].groundY) * Rad2Grad;
                else
                    if(snrInfoG.nextTockaRS[0].groundX != snrInfoG.nextTockaRS[2].groundX || snrInfoG.nextTockaRS[0].groundY != snrInfoG.nextTockaRS[2].groundY)
                        kyrsTemp = atan2(snrInfoG.nextTockaRS[0].groundX - snrInfoG.nextTockaRS[2].groundX,
                                         snrInfoG.nextTockaRS[0].groundY - snrInfoG.nextTockaRS[2].groundY) * Rad2Grad;
                    else
                        if(snrInfoG.nextTockaRS[0].groundX != snrInfoG.nextTockaRS[3].groundX || snrInfoG.nextTockaRS[0].groundY != snrInfoG.nextTockaRS[3].groundY)
                            kyrsTemp = atan2(snrInfoG.nextTockaRS[0].groundX - snrInfoG.nextTockaRS[3].groundX,
                                             snrInfoG.nextTockaRS[0].groundY - snrInfoG.nextTockaRS[3].groundY) * Rad2Grad;


/*2018-02-23            qDebug()<<"rsTrgtPrmsCalculator handWheelFSpeed:"<<snrInfoG.handWheelFSpeed[0]<<"groundX:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundX<<snrInfoG.nextTockaRS[0].groundX
                        <<"groundY:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundY<<snrInfoG.nextTockaRS[0].groundY
                         // "rejimPoiskaSNR:"<<snrInfoG.rejimPoiskaSNR<<"rejimSoprovojdeniaSNR:"<<snrInfoG.rejimSoprovojdeniaSNR
                        <<"nextTocka.kyrs:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].kyrs<<snrInfoG.nextTockaRS[0].kyrs;
                        //<<"nextTochka.rakyrs:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].rakyrs<<snrInfoG.nextTockaRS[0].rakyrs;
*/
            }
            else    // частный случай - курсовой параметр = 0
            {
                if(snrInfoG.nextTockaRS[0].groundRange_km > snrInfoG.nextTockaRS[1].groundRange_km) // цель удаляется
                    kyrsTemp = snrInfoG.nextTockaRS[0].azimuth360;
                else
                    kyrsTemp = snrInfoG.nextTockaRS[0].azimuth360 - 180.0;//snrInfoG.nextTockaRS[1].kyrs;   //  цель приближается
            }

    //        if(snrInfoG.nextTockaRS[0].groundX != snrInfoG.nextTockaRS[1].groundX || snrInfoG.nextTockaRS[0].groundY != snrInfoG.nextTockaRS[1].groundY)
    //            kyrsTemp = atan2(snrInfoG.nextTockaRS[0].groundX - snrInfoG.nextTockaRS[1].groundX,
    //                             snrInfoG.nextTockaRS[0].groundY - snrInfoG.nextTockaRS[1].groundY) * Rad2Grad;

            if(kyrsTemp < 0)
                kyrsTemp += 360.0;

            snrInfoG.nextTockaRS[0].kyrs = kyrsTemp;
            snrInfoG.nextTockaRS[0].rakyrs = 180.0 - snrInfoG.nextTockaRS[0].kyrs + az180;

//            qDebug()<<"rsTrgtPrmsCalculator handWheelFSpeed:"<<snrInfoG.handWheelFSpeed[0]<<"groundX:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundX<<snrInfoG.nextTockaRS[0].groundX
//                    <<"groundY:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].groundY<<snrInfoG.nextTockaRS[0].groundY
//                     // "rejimPoiskaSNR:"<<snrInfoG.rejimPoiskaSNR<<"rejimSoprovojdeniaSNR:"<<snrInfoG.rejimSoprovojdeniaSNR
//                    <<"nextTocka.kyrs:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].kyrs<<snrInfoG.nextTockaRS[0].kyrs;
//                    //<<"nextTochka.rakyrs:"<<snrInfoG.nextTocka[snrInfoG.simulatorTargetIndexCY].rakyrs<<snrInfoG.nextTockaRS[0].rakyrs;

            snrInfoG.nextTockaRS[0].radialSpeed_ms = snrInfoG.nextTocka[indCY].radialSpeed_ms;
            float cosRakyrs = cos(snrInfoG.nextTockaRS[0].rakyrs / Rad2Grad);

            if(cosRakyrs != 0)
                snrInfoG.nextTockaRS[0].groundSpeed_ms = snrInfoG.nextTockaRS[0].radialSpeed_ms / cosRakyrs;
            else
                snrInfoG.nextTockaRS[0].groundSpeed_ms = snrInfoG.nextTockaRS[0].radialSpeed_ms;

            // Т Е К У Щ И Й   К У Р С О В О Й   П А Р А М Е Т Р
            float deltaPrm = snrInfoG.nextTockaRS[0].rakyrs / Rad2Grad;
            snrInfoG.nextTockaRS[0].kyrsovojParameter_km = snrInfoG.nextTockaRS[0].groundRange_km * sin(deltaPrm);
            snrInfoG.nextTockaRS[0].rangeParametr_km = sqrt(snrInfoG.nextTockaRS[0].groundRange_km * snrInfoG.nextTockaRS[0].groundRange_km
                                                                  - snrInfoG.nextTockaRS[0].kyrsovojParameter_km * snrInfoG.nextTockaRS[0].kyrsovojParameter_km);
            ZonaPorazhRanges zpr = ZP::glybina_ZP(snrInfoG.nextTockaRS[0].kyrsovojParameter_km, snrInfoG.nextTockaRS[0].radarRelativeAltitude);
            snrInfoG.nextTockaRS[0].bGZoniPorParam_km = zpr.minValue_km;
            snrInfoG.nextTockaRS[0].dGZoniPorParam_km = zpr.maxValue_km;
            snrInfoG.nextTockaRS[0].maxKyrsovojParameter_km = zpr.maxParams;

            float dgzPor_abs = sqrt(zpr.maxValue_km * zpr.maxValue_km + snrInfoG.nextTockaRS[0].kyrsovojParameter_km * snrInfoG.nextTockaRS[0].kyrsovojParameter_km);
            float tPoliotaRaketiDGZPor = dgzPor_abs / VRAKETI;
            float groundSpeed_km_s = snrInfoG.nextTockaRS[0].groundSpeed_ms * 1.0e-3;

            snrInfoG.nextTockaRS[0].dGZoniPyskaParametr_km = zpr.maxValue_km + fabs(groundSpeed_km_s) * tPoliotaRaketiDGZPor;

        //    qDebug()<<"assignGradysnikParams: zpr->maxValue_km:"<<zpr->maxValue_km<<"kyrsovojParameter_km:"<<snrInfoG.nextTocka[ii].kyrsovojParameter_km
        //           <<"tPoliotaRaketiDGZPor:"<<tPoliotaRaketiDGZPor<<"dGZoniPyskaParametr_km:"<<snrInfoG.nextTocka[ii].dGZoniPyskaParametr_km;

            float a = groundSpeed_km_s * groundSpeed_km_s - VRAKETI * VRAKETI;

            float b =  - 2.0 * fabs(groundSpeed_km_s) * snrInfoG.nextTockaRS[0].rangeParametr_km;

            float c = snrInfoG.nextTockaRS[0].groundRange_km * snrInfoG.nextTockaRS[0].groundRange_km;

            float discr = sqrt(b * b - 4 * a * c);

            //float x1 = (- b + discr)  / 2.0 / a;
            float x2 = (- b - discr)  / 2.0 / a;

            snrInfoG.nextTockaRS[0].rangeParametrTV_km = snrInfoG.nextTockaRS[0].rangeParametr_km - x2 * groundSpeed_km_s;

            if(groundSpeed_km_s != 0.0)
            {
                snrInfoG.nextTockaRS[0].podletnoeVrema = (snrInfoG.nextTockaRS[0].rangeParametr_km - snrInfoG.nextTockaRS[0].dGZoniPyskaParametr_km) / groundSpeed_km_s;
            }
            //qDebug()<<"assignGradysnikParams ii:"<<ii<<"rangeParametr_km"<<snrInfoG.nextTocka[ii].rangeParametr_km<<"groundSpeed_km_s:"<<groundSpeed_km_s<<"podletnoeVrema:"<<snrInfoG.nextTocka[ii].podletnoeVrema;

            //delete zpr;

        }   //  если было приращение дальности или азимута
    }   //  для РС по углам
}
