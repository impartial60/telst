#include <unistd.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <string.h>

#include <fcntl.h>

#include "extern.h"

extern uint16_t uint16_t_byte_reorder(uint16_t inputValue);
extern UV11ANGLES  operatorPovorota(float b, float e);
extern void assignGradysnikParams(int ii);
extern QString strByte125(uint8_t inputVal, QString pName8, QString pName7, QString pName6, QString pName5,
                                     QString pName4, QString pName3, QString pName2, QString pName1);

int monoTrackElapsedTime[MAXTOCKANUM];
//int monoTrackPointsCount[MAXTOCKANUM];
//int monoTrackTargetNoNo[MAXTOCKANUM];

void converterP18_2NextTocka() /////////////////// П Р Е О Б Р А З О В А Н И Е   П А Р А М Е Т Р О В   Ц Е Л И   И З   И С К Р Ы   В   nextTocka
{
    //        typedef struct        {
    //            float azimuth360;

    //            UV11ANGLES uv11Angles;

    //            float slantRange_km;
    //            float groundRange_km;

    //            float kyrs;
    //            float rakyrs;
    //            float newKyrs;
    //            float kyrs_proizv_1;

    //            float radialSpeed;
    //            float groundSpeed;

    //            float elevationAngle;
    //            float radarRelativeAltitude;
    //            float seaLevelAltitude;
    //            float groundX;      //  на восток
    //            float groundY;      //  на север

    //            bool isRealTarget;
    //            bool isNew;

    //            u_int16_t amplitude;

    //            sLPH soprovojdCoord;        // Координаты сопровождаемой СНР цели

    //        } TargetParams; // data_29; // Структура данных №29 от офицера наведения

    // какая цель ближе к ЦУ

//    float maxNeviazka = 3000;

//    int ind = 0;

//    for(int i = 0; i < 3; i++)  // ПОКА ТРИ ЦЕЛИ КОМПЛЕКСНОГО ТРЕНАЖА
//    {
//        float dx = vkp2zrkG.cmplxYHX[i].x_km - vkp2zrkG.coordYHX.x_km;
//        float dy = vkp2zrkG.cmplxYHX[i].y_km - vkp2zrkG.coordYHX.y_km;
//        float dh = vkp2zrkG.cmplxYHX[i].h_km - vkp2zrkG.coordYHX.h_km;

//        float neviazka = sqrt(dx * dx + dy * dy + dh *dh);
//        if(neviazka < maxNeviazka)
//        {
//            ind = i;
//            maxNeviazka = neviazka;
//        }
//    }

//    snrInfoG.simulatorTargetIndexCY = ind;
//    if(p18_2ZRKG.isTrack)

    bool nextTochkaIsMatched = false;
    int ind = -1;

    for(int i = 0; i < MAXTOCKANUM; i++)
    {
        if(snrInfoG.nextTocka[i].targetNo == p18_2ZRKG.targetNo)
        {
            nextTochkaIsMatched = true;
            ind = i;
            break;
        }
    }

    if(nextTochkaIsMatched == false)
    {
        for(int i = 0; i < MAXTOCKANUM; i++)
        {
            if(snrInfoG.nextTocka[i].targetNo == 0)
            {
                nextTochkaIsMatched = true;
                ind = i;
                break;
            }
        }
    }

    if(nextTochkaIsMatched == false)
    {
        int minElapsed = 1000000;

        for(int i = 0; i < MAXTOCKANUM; i++)
        {
            if(monoTrackElapsedTime[i] < minElapsed)
            {
                minElapsed = monoTrackElapsedTime[i];
                ind = i;
            }
        }
    }

    if(ind >= 0 && ind < MAXTOCKANUM)
    {
        monoTrackElapsedTime[ind] = programmStartTime.elapsed();
        snrInfoG.nextTocka[ind].targetNo = p18_2ZRKG.targetNo;

        float x_km = NM2km * p18_2ZRKG.coordX_NM;
        float y_km = NM2km * p18_2ZRKG.coordY_NM;

        float az360 = atan2(x_km, y_km) * Rad2Grad;
        if(az360 < 0.0)
            az360 += 360.0;

        snrInfoG.nextTocka[ind].azimuth360 = az360;

        snrInfoG.nextTocka[ind].radarRelativeAltitude = 12.3;  //  for test only
        snrInfoG.nextTocka[ind].seaLevelAltitude = snrInfoG.nextTocka[ind].radarRelativeAltitude + 0.001 * snrInfoG.unvGeoPosition.seaLevelH;

        snrInfoG.nextTocka[ind].groundRange_km = sqrt(x_km * x_km + y_km * y_km);

        snrInfoG.nextTocka[ind].slantRange_km =  sqrt(snrInfoG.nextTocka[ind].groundRange_km * snrInfoG.nextTocka[ind].groundRange_km +
                                                    snrInfoG.nextTocka[ind].radarRelativeAltitude * snrInfoG.nextTocka[ind].radarRelativeAltitude);

        //  м е т о д   В О Р О Н И Н А
        float deltaH = (- R_ZemliEquiv + sqrt(R_ZemliEquiv * R_ZemliEquiv + snrInfoG.nextTocka[ind].slantRange_km *
                        snrInfoG.nextTocka[ind].slantRange_km));

        float srednijUgol = 5.0;
        if(snrInfoG.nextTocka[ind].slantRange_km != 0)
        {
            float asv = (snrInfoG.nextTocka[ind].seaLevelAltitude // - 0.001 * settings->seaLevel
                         - deltaH) / snrInfoG.nextTocka[ind].slantRange_km;
            if(asv > 1)
                srednijUgol = 22.0;
            if(asv < -1)
                srednijUgol = 12.0;
            if(-1 <= asv && asv <= 1)
                srednijUgol = asin(asv) * Rad2Grad;
        }

        snrInfoG.nextTocka[ind].elevationAngle = srednijUgol;// * Rad2Grad;

        snrInfoG.nextTocka[ind].uv11Angles = operatorPovorota(snrInfoG.nextTocka[ind].azimuth360 - snrInfoG.currentAzimuth,
                                           snrInfoG.nextTocka[ind].elevationAngle - snrInfoG.currentDirectrisaElevationAngle);


        snrInfoG.nextTocka[ind].kyrs = p18_2ZRKG.kyrs360;

        float az180 = snrInfoG.nextTocka[ind].azimuth360;
        if(az180 > 180.0)
            az180 -= 360.0;
        snrInfoG.nextTocka[ind].rakyrs = 180.0 - snrInfoG.nextTocka[ind].kyrs + az180;

        snrInfoG.nextTocka[ind].groundSpeed_ms = 1000.0 * NM2km * p18_2ZRKG.speed_NM_s;
        //snrInfoG.nextTocka[i].radialSpeed = snrInfoG.nextTocka[i].groundSpeed_ms * cos(snrInfoG.nextTocka[i].rakyrs / Rad2Grad);

        snrInfoG.nextTocka[ind].groundX = x_km;
        snrInfoG.nextTocka[ind].groundY = y_km;

        assignGradysnikParams(ind);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////

    QString crdStr = "Nцели: ";

    for(int i = 0; i < MAXTOCKANUM; i++)
    {
        crdStr +=  QString::number(snrInfoG.nextTocka[i].targetNo) + " ";
//        //int ii = monoTrackPointsCount[i] - 1;

//        if(ii >= 0 && ii < MONOTRACKSIZE)
//        {
//            crdStr += "\nчисло точек: " + QString::number(ii) +
//                    " Nцели: " + QString::number(monoTrack[i][ii].targetNo) +
//            " Д: " + QString::number(monoTrack[i][ii].groundRange_km, 'f', 3) + "km" +
//            " Аз:" + QString::number(monoTrack[i][ii].azimuth360, 'f', 3) + "°" +
//            " V:" + QString::number(monoTrack[i][ii].groundSpeed_ms, 'f', 3) + "м/c" +
//            " Курс :" + QString::number(monoTrack[i][ii].kyrs, 'f', 3) + "°";
//        }
    }

    qDebug()<<crdStr;


//////////////////////////////////////////////////////////////////////////////////////////////////
  /*  int ind = 0;

    snrInfoG.nextTocka[ind].targetNo = p18_2ZRKG.targetNo;

    float x_km = NM2km * p18_2ZRKG.coordX_NM;
    float y_km = NM2km * p18_2ZRKG.coordY_NM;

    float az360 = atan2(x_km, y_km) * Rad2Grad;
    if(az360 < 0.0)
        az360 += 360.0;    

    snrInfoG.nextTocka[ind].azimuth360 = az360;

    snrInfoG.nextTocka[ind].radarRelativeAltitude = 12.3;  //  for test only
    snrInfoG.nextTocka[ind].seaLevelAltitude = snrInfoG.nextTocka[ind].radarRelativeAltitude + 0.001 * snrInfoG.unvGeoPosition.seaLevelH;

    snrInfoG.nextTocka[ind].groundRange_km = sqrt(x_km * x_km + y_km * y_km);

    snrInfoG.nextTocka[ind].slantRange_km =  sqrt(snrInfoG.nextTocka[ind].groundRange_km * snrInfoG.nextTocka[ind].groundRange_km +
                                                snrInfoG.nextTocka[ind].radarRelativeAltitude * snrInfoG.nextTocka[ind].radarRelativeAltitude);

    //  м е т о д   В О Р О Н И Н А
    float deltaH = (- R_ZemliEquiv + sqrt(R_ZemliEquiv * R_ZemliEquiv + snrInfoG.nextTocka[ind].slantRange_km *
                    snrInfoG.nextTocka[ind].slantRange_km));

    float srednijUgol = 5.0;
    if(snrInfoG.nextTocka[ind].slantRange_km != 0)
    {
        float asv = (snrInfoG.nextTocka[ind].seaLevelAltitude // - 0.001 * settings->seaLevel
                     - deltaH) / snrInfoG.nextTocka[ind].slantRange_km;
        if(asv > 1)
            srednijUgol = 22.0;
        if(asv < -1)
            srednijUgol = 12.0;
        if(-1 <= asv && asv <= 1)
            srednijUgol = asin(asv) * Rad2Grad;
    }

    snrInfoG.nextTocka[ind].elevationAngle = srednijUgol;// * Rad2Grad;

    snrInfoG.nextTocka[ind].uv11Angles = operatorPovorota(snrInfoG.nextTocka[ind].azimuth360 - snrInfoG.currentAzimuth,
                                       snrInfoG.nextTocka[ind].elevationAngle - snrInfoG.currentDirectrisaElevationAngle);

    //float vx = vkp2zrkG.cmplxYHX[i].vx_ms;
    //float vy = vkp2zrkG.cmplxYHX[i].vy_ms;

//    float kyrs360 = atan2(vx, vy) * Rad2Grad;;
//    if(kyrs360 < 0.0)
//        kyrs360 += 360.0;

    snrInfoG.nextTocka[ind].kyrs = p18_2ZRKG.kyrs360;

    float az180 = snrInfoG.nextTocka[ind].azimuth360;
    if(az180 > 180.0)
        az180 -= 360.0;
    snrInfoG.nextTocka[ind].rakyrs = 180.0 - snrInfoG.nextTocka[ind].kyrs + az180;

    snrInfoG.nextTocka[ind].groundSpeed_ms = 1000.0 * NM2km * p18_2ZRKG.speed_NM_s;
    //snrInfoG.nextTocka[i].radialSpeed = snrInfoG.nextTocka[i].groundSpeed_ms * cos(snrInfoG.nextTocka[i].rakyrs / Rad2Grad);

    snrInfoG.nextTocka[ind].groundX = x_km;
    snrInfoG.nextTocka[ind].groundY = y_km;

    assignGradysnikParams(ind);*/
}


void * ProcessThreadP18(void * temp_pt )
{
    p18_2ZRKG.sizeInfoP18 = 0;
    QString strP18;
    uint8_t arrayP18[1000];

    //float coordFactor = 1024.0 * 1024.0 * 8.0 / 180.0;

    int psockfd = -1;

    //int vsego_propyschenih_periodov = 0;

    //qDebug()<<"snrInfoG.currentAzimuth:"<<snrInfoG.currentAzimuth<<"protokol126.BisectorWorkingSector:"<<protokol126.BisectorWorkingSector;

    clientConnect(&psockfd, settings->portP18, settings->ipAddressP18, "ProcessThreadP18 initial connect");

//    int cntr = 0;

    while (threadStatus->thrStatus_P18 != threedStatus::cancel)
    {
        int uu = 0;
        memset(arrayP18, 0, sizeof(arrayP18));
        strP18 = "";

        usleep(30*1000);

//        if(++cntr % 10 == 0)
//            qDebug()<<"ProcessThreadP18 "<<cntr;

        // О Т С Ы Л К А    1 2 6   К А Т Е Г О Р И И
        //if(send(psockfd, &protokol126, size126, 0) < 0 )
          //  clientConnect(&psockfd, settings->portVKP, settings->ipAddressVKP, "VKP: reconnect");

        uint8_t headerP18[4];
        memset(headerP18, 0, 4);

        int totalReadBytes = 0;
        int vsego_propyschenih_periodov = 0;

        // П Р И Ё М   С Н А Ч А Л А   Т О Л Ь К О   З А Г О Л О В К А   0 0 1   К А Т Е Г О Р И И
        while (totalReadBytes < 4 && vsego_propyschenih_periodov <= 10)
        {
            //snrInfoG.consoleCntr ++;
            //nextReadBytes = recv(tcpSocketFileDescriptor, readBytes + totalReadBytes, sizeof(AWSG_User2Console) - totalReadBytes, 0);
            ssize_t nextReadBytes = recv(psockfd, headerP18 + totalReadBytes, 4 - totalReadBytes, 0);

            //qDebug()<<"CnvEth485 nextReadBytes:"<<nextReadBytes;

            if(nextReadBytes <= 0)
            {
                vsego_propyschenih_periodov++;
                //qDebug()<<"CnvEth485: vsego_propyschenih_periodov:"<<vsego_propyschenih_periodov;
            }
            else
            {
                vsego_propyschenih_periodov = 0;
                totalReadBytes += nextReadBytes;
            }
        }

        if(totalReadBytes < 4)
            continue;

        /////////////////////////////////////////////////////////////////read(psockfd, &headerP18, 4);

        int sizeInfoP18 = 256 * headerP18[1] + headerP18[2] - 4;

        p18_2ZRKG.sizeInfoP18 = 256 * headerP18[1] + headerP18[2] - 4;

        // П Р И Ё М   О С Т А Л Ь Н О Й   Ч А С Т И   0 0 1   К А Т Е Г О Р И И
        totalReadBytes = 0;
        vsego_propyschenih_periodov = 0;
        ////////////////size_t numReadByte = read(psockfd, arrayP18, sizeInfoP18);

        while (totalReadBytes < p18_2ZRKG.sizeInfoP18 && vsego_propyschenih_periodov <= 10)
        {
            //snrInfoG.consoleCntr ++;
            //nextReadBytes = recv(tcpSocketFileDescriptor, readBytes + totalReadBytes, sizeof(AWSG_User2Console) - totalReadBytes, 0);
            ssize_t nextReadBytes = recv(psockfd, arrayP18 + totalReadBytes, p18_2ZRKG.sizeInfoP18 - totalReadBytes, 0);

            //qDebug()<<"CnvEth485 nextReadBytes:"<<nextReadBytes;

            if(nextReadBytes <= 0)
            {
                vsego_propyschenih_periodov++;
                //qDebug()<<"CnvEth485: vsego_propyschenih_periodov:"<<vsego_propyschenih_periodov;
            }
            else
            {
                vsego_propyschenih_periodov = 0;
                totalReadBytes += nextReadBytes;
            }
        }

            //qDebug()<<"sizeInfoP18:"<<p18_2ZRKG.sizeInfoP18<<"totalReadBytes:"<<totalReadBytes;

        if((int)totalReadBytes < 0)
        {
            clientConnect(&psockfd, settings->portP18, settings->ipAddressP18, "P18: reconnect");
            continue;
        }
        else
        {
            if((int)totalReadBytes == sizeInfoP18)
            {
                snrInfoG.p18TransactionId++;

                while(uu < sizeInfoP18)
                {
                    uint8_t fspec1 = headerP18[3];   // 1-й FSPEC

                    bool fspec2Exist = false;       // по умолчанию 2-й FSPEC отсутствует
                    bool fspec3Exist = false;       // по умолчанию 3-й FSPEC отсутствует
                    uint8_t fspec2;
                    uint8_t fspec3;

                    if((fspec1 & 0x01) == 0x01)        // 2-й FSPEC есть ?
                    {
                        fspec2Exist = true;           // есть
                        fspec2 = arrayP18[uu]; // 2-й FSPEC    //0b11111110;

                        uu += 1;
                    }

                    if(fspec2Exist && ((fspec2 & 0x01) == 0x01))  // 3-й FSPEC есть ?
                    {
                        fspec3Exist = true;     // есть
                        fspec3 = arrayP18[uu];   // 2-й FSPEC    //0b11111110;

                        if((fspec3 & 0x01) == 0x01)  // учитывая, что их может быть сколько угодно, убеждаемся, что он последний
                            strP18 += "обнаружен 4-й FSPEC, которого не должно быть ";

                        uu += 1;
                    }

                    // расшифровка данных, определённая 1-м FSPEC

                    if((fspec1 & 0x80) == 0x80) // если есть I001/10 - SAC SIC (Идентификатор источника данных)
                    {
                       strP18 += QString("SAC:%1[0x%2] SIC:%3[0x%4]")
                               .arg(arrayP18[uu    ], 3, 10, QChar('0')).arg(arrayP18[uu    ], 2, 16, QChar('0'))
                               .arg(arrayP18[uu + 1], 3, 10, QChar('0')).arg(arrayP18[uu + 1], 2, 16, QChar('0'));

                       p18_2ZRKG.sac = arrayP18[uu    ];
                       p18_2ZRKG.sic = arrayP18[uu + 1];

                       uu += 2;
                    }

                    if(headerP18[0] == 0x01)    //  0x01 - 1 категория
                    {

                        if((fspec1 & 0x40) == 0x40) //  если есть I001/20 - Описание сообщения о цели
                        {
                           if((arrayP18[uu] & 0x80) == 0x80)
                               p18_2ZRKG.isTrack = true;
                           else
                               p18_2ZRKG.isTrack = false;

                           strP18 += " Сообщ.о цели" + strByte125(arrayP18[uu], "Трек(отм)", "Имит", "I", "II", "А", "SPI", "Контр", "Расш");// Байт 1

                           if(arrayP18[uu  ] & 0x01 == 0x01)
                           {
                               uu += 1;

                               strP18 += "ещё " + strByte125(arrayP18[uu], "TST", "DS1", "DS2", "ME", "MI", "Зум1", "Зум1", "FX"); // Байт 2
                           }

                           uu += 1;
                        }

                        if(p18_2ZRKG.isTrack)   // Э Т О   Т Р А Е К Т О Р И Я
                        {
                            strP18 += QString(" Траектория ");

                            if((fspec1 & 0x20) == 0x20) //  если есть I001/161 - Номер траектории
                            {
                                p18_2ZRKG.targetNo = 256 * arrayP18[uu   ] + arrayP18[uu + 1];

                                strP18 += QString("N: %1").arg(p18_2ZRKG.targetNo, 2, 10, QChar('0'));

                                uu += 2;
                            }
                            else
                                p18_2ZRKG.targetNo = 0;

                            // fspec1 & 0x10 - резерв

                            if((fspec1 & 0x08) == 0x08) //  если есть I001/42 - Вычисленные декартовы координаты
                            {
                                uint8_t byteCrd[4];

                                byteCrd[1] = arrayP18[uu + 0]; byteCrd[0] = arrayP18[uu + 1];
                                byteCrd[3] = arrayP18[uu + 2]; byteCrd[2] = arrayP18[uu + 3];

                                int16_t crdX;
                                int16_t crdY;

                                memcpy(&crdX, byteCrd     , 2);
                                memcpy(&crdY, byteCrd +  2, 2);

                                p18_2ZRKG.coordX_NM = 1.0 / 64 * crdX;
                                p18_2ZRKG.coordY_NM = 1.0 / 64 * crdY;

                                strP18 += QString(" X: %1NM [0x%2%3]").arg(QString::number(p18_2ZRKG.coordX_NM, 'f', 2)).arg(arrayP18[uu    ], 2, 16, QChar('0')).arg(arrayP18[uu + 1], 2, 16, QChar('0'));
                                strP18 += QString(" Y: %1NM [0x%2%3]").arg(QString::number(p18_2ZRKG.coordY_NM, 'f', 2)).arg(arrayP18[uu + 2], 2, 16, QChar('0')).arg(arrayP18[uu + 3], 2, 16, QChar('0'));

                               uu += 4;
                            }

                            if((fspec1 & 0x04) == 0x04) //  если есть I001/200 - Вектор курсовой скорости
                            {
                                int speedInt = 256 * arrayP18[uu   ] + arrayP18[uu + 1];
                                p18_2ZRKG.speed_NM_s = 1.0 / 128 / 128 * speedInt;  // 1852 метра в морской миле

                                strP18 += QString(" V: %1NM/s [0x%2%3]").arg(QString::number(p18_2ZRKG.speed_NM_s, 'f', 2)).arg(arrayP18[uu], 2, 16, QChar('0')).arg(arrayP18[uu + 1], 2, 16, QChar('0'));

                                int kyrsInt = 256 * arrayP18[uu + 2] + arrayP18[uu + 3];
                                p18_2ZRKG.kyrs360 = 360.0 / 256 / 256 * kyrsInt;

                                strP18 += QString(" Kyrs: %1° [0x%2%3]").arg(QString::number(p18_2ZRKG.kyrs360, 'f', 2)).arg(arrayP18[uu + 2], 2, 16, QChar('0')).arg(arrayP18[uu + 3], 2, 16, QChar('0'));

                               uu += 4;

                            }

                            if((fspec1 & 0x10) == 0x10) //  если есть I001/70 - Код Режима 3/A
                            {
                                uu += 2;
                            }

                            if(fspec2Exist)
                            {
                                if((fspec2 & 0x80) == 0x80) //  если есть I001/90  Код высоты в режиме C
                                {
                                    //vkp2zrkG.komandiPoCelevomyKanaly1.commandInfoExist = true;
                                    uu += 2;
                                }

                                if((fspec2 & 0x40) == 0x40) //  если есть I001/141 - Неполное время суток
                                {
                                    p18_2ZRKG.timeCode = 1.0 / 128 * (256 * arrayP18[uu   ] + arrayP18[uu + 1]);

                                    strP18 += " T: " + QString::number(p18_2ZRKG.timeCode, 'f', 3);

                                    uu += 2;
                                }

                                // fspec2 & 0x20 - резерв
                                // fspec2 & 0x10 - резерв
                                // fspec2 & 0x08 - резерв

                                if((fspec2 & 0x04) == 0x04) //  если есть I001/170 - Статус траектории
                                {
                                    strP18 += "Статус: " + strByte125(arrayP18[uu], "CON", "RAD", "MAN", "DOU", "RDPC", "Res", "GNO", "FX");

                                    uu += 1;
                                }

                                if((fspec2 & 0x02) == 0x02) //  если есть I001/210 - Качество траектории сопровождения
                                {
                                    strP18 += "Качество: " + strByte125(arrayP18[uu], "-", "Кач", "Восст", "Нов", "Пр1", "Пр2", "Пр3", "FX");

                                    uu += 1;
                                }
                            }

                            if(fspec3Exist)
                            {
                                if((fspec3 & 0x80) == 0x80) //  если есть I001/50 - Код Режима 2 в восьмиричном виде
                                {
                                    strP18 += QString(" Код Р2: 0x%1%2").arg(arrayP18[uu], 2, 16, QChar('0')).arg(arrayP18[uu + 1], 2, 16, QChar('0'));
                                    uu += 2;
                                }

                                // fspec3 & 0x40 - резерв
                                // fspec3 & 0x20 - резерв
                                // fspec3 & 0x10 - резерв
                                // fspec3 & 0x08 - резерв

                                if((fspec3 & 0x04) == 0x04) //  если есть SDI Track - Специальный элемент данных
                                {
                                    if(arrayP18[uu] == 7 || arrayP18[uu] == 14)
                                    {

                                        uu += arrayP18[uu];
                                    }
                                    else
                                        strP18 += QString(" SDI Track - неправильная длина сообщения: %1").arg(arrayP18[uu], 2, 16, QChar('0'));
                                }

                                // fspec3 & 0x02 - резерв

                            }
                        }
                        else    // Э Т О   П Л О Т
                        {
                            strP18 += QString(" Отметка ");

                            p18_2ZRKG.isTrack = false;

                            if((fspec1 & 0x20) == 0x20) //  если есть I001/40 - Измеренные полярные координаты
                            {
                                int dalnostInt = 256 * arrayP18[uu   ] + arrayP18[uu + 1];
                                p18_2ZRKG.dalnostNM = 1.0 / 128 * dalnostInt;  // 1852 метра в морской миле

                                strP18 += QString(" Д: %1NM [0x%2%3]").arg(QString::number(p18_2ZRKG.dalnostNM, 'f', 2)).arg(arrayP18[uu], 2, 16, QChar('0')).arg(arrayP18[uu + 1], 2, 16, QChar('0'));

                                int azInt = 256 * arrayP18[uu + 2] + arrayP18[uu + 3];
                                p18_2ZRKG.azimuth360 = 360.0 / 256 / 256 * azInt;

                                strP18 += QString(" Az: %1° [0x%2%3]").arg(QString::number(p18_2ZRKG.azimuth360, 'f', 2)).arg(arrayP18[uu + 2], 2, 16, QChar('0')).arg(arrayP18[uu + 3], 2, 16, QChar('0'));

                               uu += 4;
                            }

                            if((fspec1 & 0x10) == 0x10) //  если есть I001/70 - Код Режима 3/A
                            {
                                uu += 2;
                            }

                            if((fspec1 & 0x08) == 0x08) //  если есть I001/90  Код высоты в режиме C
                            {

                                uu += 2;
                            }

                            // fspec1 & 0x04 - резерв

                            if((fspec1 & 0x02) == 0x02) //  если есть I001/141 - Неполное время суток
                            {
                                p18_2ZRKG.timeCode = 1.0 / 128 * (256 * arrayP18[uu   ] + arrayP18[uu + 1]);

                                strP18 += "Время: " + QString::number(p18_2ZRKG.timeCode, 'f', 3);

                                uu += 2;
                            }

                            if(fspec2Exist)
                            {
                                if((fspec2 & 0x80) == 0x80) //  если есть I001/50 - Код Режима 2 в восьмиричном виде
                                {
                                    uu += 2;
                                }

                                // fspec2 & 0x40 - резерв
                                // fspec2 & 0x20 - резерв
                                // fspec2 & 0x10 - резерв
                                // fspec2 & 0x08 - резерв
                                // fspec2 & 0x04 - резерв

                                if((fspec1 & 0x02) == 0x02) //  если есть I001/30 - Предупреждения и ошибки
                                {
                                   strP18 += QString(" W/E Value: 0b%1").arg(arrayP18[uu], 8, 2, QChar('0'));
                                   uu += 1;
                                }
                            }

                            if(fspec3Exist)
                            {
                                // fspec3 & 0x80 - резерв
                                // fspec3 & 0x40 - резерв
                                // fspec3 & 0x20 - резерв
                                // fspec3 & 0x10 - резерв
                                // fspec3 & 0x08 - резерв

                                if((fspec3 & 0x04) == 0x04) //  если есть SDI Plot - Специальный элемент данных
                                {
                                    if(arrayP18[uu] == 3 || arrayP18[uu] == 4)
                                    {

                                        uu += arrayP18[uu];
                                    }
                                    else
                                        strP18 += QString(" SDI Plot - неправильная длина сообщения: %1").arg(arrayP18[uu], 2, 16, QChar('0'));
                                }
                                // fspec3 & 0x02 - резерв
                            }
                        }

                        //strP18 += " u:" + QString::number(uu);

                        //qDebug()<<"P18:"<<strP18;

                        // П Р Е О Б Р А З О В А Н И Е   П А Р А М Е Т Р О В   Ц Е Л И   И З   0 0 1   К А Т Е Г О Р И И   В   nextTocka
                        if(p18_2ZRKG.isTrack)
                            converterP18_2NextTocka();
                    }
                    else
                    {
                        if(headerP18[0] == 0x02)   // это 002 категория
                        {
                            if((fspec1 & 0x40) == 0x40) //  если есть I002/000 - Тип сообщения
                            {
                                if(arrayP18[uu] == 0x01)
                                    strP18 += " Метка Севера ";
                                else
                                    strP18 += " Метка Сектора ";
                                uu += 1;
                            }

                            if((fspec1 & 0x20) == 0x20) //  если есть I002/020 - Номер сектора
                            {
                                strP18 += " Nсект:" + QString::number(360.0 / 256 * arrayP18[uu], 'f', 2);
                                uu += 1;
                            }

                            if((fspec1 & 0x10) == 0x10) //  если есть I002/030 - Время дня
                            {
                                strP18 += " T:" + QString::number((arrayP18[uu] * 256 * 256 + arrayP18[uu + 1] * 256 + arrayP18[uu + 2]) / 128);
                                uu += 3;
                            }

                            if((fspec1 & 0x08) == 0x08) //  если есть I002/041 - Период вращения антенны
                            {
                                strP18 += " Период вращ." + QString::number((arrayP18[uu] * 256 + arrayP18[uu + 1]) / 128);
                                uu += 2;
                            }

                            if((fspec1 & 0x04) == 0x04) //  если есть I002/050 - Конфигурация РЛС
                            {
                                strP18 += " ******************** Конфиг:" + strByte125(arrayP18[uu], "COM", "-", "RS", "PRS", "SSR", "-", "-", "FX");// Байт 1
                                if(arrayP18[uu] & 0x01 == 0x01)
                                {
                                    uu += 1;
                                    strP18 += " " + strByte125(arrayP18[uu], "NOGO", "-", "RDPR", "OVLRDP", "OVLXMT", "-", "TSV", "-");// Байт 1
                                }
                                uu += 1;
                            }

                            if(fspec2Exist)
                            {
                                // fspec2 & 0x80 - резерв
                                // fspec2 & 0x40 - резерв
                                // fspec2 & 0x20 - резерв

                                if((fspec2 & 0x10) == 0x10) //  если есть I002/120 - Положение источника данных WGS84
                                {
                                    strP18 += " Высота: " + QString::number(arrayP18[uu] * 256 + arrayP18[uu + 1]);
                                    float factor = 180.0 / 1024 / 1024 / 8;
                                    strP18 += " Широта: " + QString::number(factor * (arrayP18[uu + 2] * 256 * 256 + arrayP18[uu + 3] * 256 + arrayP18[uu + 4]), 'f', 3);
                                    strP18 += " Долгота: " + QString::number(factor * (arrayP18[uu + 5] * 256 * 256 + arrayP18[uu + 6] * 256 + arrayP18[uu + 7]), 'f', 3);

                                    uu += 8;
                                }
                            }

                            strP18 += " uu:" + QString::number(uu) + " sizeInfoP18:" + QString::number(sizeInfoP18);

                            qDebug()<<"P18:"<<strP18;
                        }
                        else
                        {
                            qDebug()<<"P18: не понятная категория headerP18[0]"<<headerP18[0];
                        }
                    }
                } // end while(u < sizeInfoP18)
            }   // end if(numReadByte == sizeInfo125)
        }   //  totalReadBytes >= 0
    } //end while (threadStatus->thrStatus_P18 != threedStatus::cancel)  // end if(header125[0] == 0x01)   // это 001 категория


//        qDebug()<<"*** *** *** ProcessVKP: azCU_grad:"<<azCU_grad<<"currentAzimuth"<<snrInfoG.currentAzimuth<<"deltaAz:"<<deltaAz<<"cyStatus:"
//               <<snrInfoG.cyStatus<<"targetInfoExist:"<<shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.targetInfoExist
//              <<"coord:"<<shMemC2U.vkp2zrk.coordYHX.y_km<<shMemC2U.vkp2zrk.coordYHX.x_km;

//        qDebug()<<"*** *** *** ProcessVKP: snrInfo.cyStatus:"<<snrInfoG.cyStatus
//               <<"CU"<<shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.CU<<"OCU"<<shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.OCU
//               <<"targetInfoExist:"<<shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.targetInfoExist
//        <<"coord vkp2zrk: ЦУ"<<shMemC2U.vkp2zrk.coordYHX.y_km<<shMemC2U.vkp2zrk.coordYHX.x_km
//        <<"1:"<<shMemC2U.vkp2zrk.cmplxYHX[0].y_km<<shMemC2U.vkp2zrk.cmplxYHX[0].x_km
//        <<"2:"<<shMemC2U.vkp2zrk.cmplxYHX[1].y_km<<shMemC2U.vkp2zrk.cmplxYHX[1].x_km
//        <<"3:"<<shMemC2U.vkp2zrk.cmplxYHX[2].y_km<<shMemC2U.vkp2zrk.cmplxYHX[2].x_km;



    CloseSocket(psockfd);

    qDebug()<<"ProcessThreadP18 canceled";

    return NULL;
}
