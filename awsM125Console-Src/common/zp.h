#ifndef ZP_H
#define ZP_H

#include <QDebug>
#include "targetPlot.h"

class ZonaPorazhRanges
{
    public:
    float minValue_km;
    float maxValue_km;
    float maxParams;
};

enum {innerBorder, outerBorder};
enum {h0025 = 0, h1_9 = 1, h14 = 2, h20 = 3, h25 = 4};

static class ZP_PARAMS
{
    public:
    float h[5];
    float l[5][5];
    float p[5][5];

    ZP_PARAMS()
    {
        h[h25] = 25.0;
        l[h25][0] = 20.0;   l[h25][1] =  7.5;   l[h25][2] = -0.1;   l[h25][3] =  0.0;   l[h25][4] =  0.0;
        p[h25][0] = 0.0;    p[h25][1] = 0.0;    p[h25][2] = 0.0;    p[h25][3] = 13.5;   p[h25][4] =  9.0;

        h[h20] = 20.0;
        l[h20][0] = 35.0;   l[h20][1] = 6.0;    l[h20][2] = -9.2;   l[h20][3] = 0.0;    l[h20][4] = 0.0;
        p[h20][0] = 0.0;    p[h20][1] = 0.0;    p[h20][2] = 0.0;    p[h20][3] = 20.7;   p[h20][4] = 7.0;

        h[h14] = 14.0;
        l[h14][0] = 42.0;   l[h14][1] = 4.5;    l[h14][2] = -15.0;  l[h14][3] = 0.0;    l[h14][4] = 0.0;
        p[h14][0] = 0.0;    p[h14][1] = 0.0;    p[h14][2] = 0.0;    p[h14][3] = 30.0;   p[h14][4] = 6.0;

        h[h1_9] = 1.9;
        l[h1_9][0] = 41.1;  l[h1_9][1] = 3.5;   l[h1_9][2] = -14.5; l[h1_9][3] = 0.0;   l[h1_9][4] = 0.0;
        p[h1_9][0] = 0.0;   p[h1_9][1] = 0.0;   p[h1_9][2] = 0.0;   p[h1_9][3] = 29.0;  p[h1_9][4] = 5.5;

        h[h0025] = 0.025;
        l[h0025][0] = 20.5; l[h0025][1] = 3.5;  l[h0025][2] = -7.5; l[h0025][3] = 0.0;  l[h0025][4] = 0.0;
        p[h0025][0] = 0.0;  p[h0025][1] = 0.0;  p[h0025][2] = 0.0;  p[h0025][3] = 16.0; p[h0025][4] = 5.0;
   }

}zpParams;


class ZP
{
    public:

    static ZonaPorazhRanges glybina_ZP(float prm_km, float altitude)
    {
        int outerIndex = h1_9;
        int innerIndex = h0025;

        if (altitude > 20.0)
        {
            outerIndex = h20;
            innerIndex = h25;
        }
        else
            if(altitude > 14.0)
            {
                outerIndex = h14;
                innerIndex = h20;
            }
            else
                if(altitude > 1.9)
                {
                    outerIndex = h1_9;
                    innerIndex = h14;
                }

        ZonaPorazhRanges retRanges;

        float ll[5];
        ll[3] = 0.0;
        ll[4] = 0.0;

        for(int i = 0; i < 3; i++)
        {
            float k = (zpParams.l[outerIndex][i] - zpParams.l[innerIndex][i]) / (zpParams.h[outerIndex] - zpParams.h[innerIndex]);

            ll[i] = k * (altitude - zpParams.h[innerIndex]) + zpParams.l[innerIndex][i];

            //qDebug()<<"ZonaPorazhRanges2 ll:"<<ll[0]<<ll[1]<<ll[2];
        }

        float pp[5];
        pp[0] = 0.0;
        pp[1] = 0.0;
        pp[2] = 0.0;

        for(int i = 3; i < 5; i++)
        {
            float k = (zpParams.p[outerIndex][i] - zpParams.p[innerIndex][i]) / (zpParams.h[outerIndex] - zpParams.h[innerIndex]);

            pp[i] = k * (altitude - zpParams.h[innerIndex]) + zpParams.p[innerIndex][i];

            //qDebug()<<"ZonaPorazhRanges2 pp:"<<pp[3]<<pp[4];
        }

        retRanges.maxParams = pp[3];

        //////////////////////////////////retRanges.maxHeightOnCurrentParam = zonaPorazh_05_10.minHeight_km;

        // Д А Л Ь Н Я Я   Г Р А Н И Ц А   З О Н Ы   П О Р А Ж Е Н И Я

        float pb = prm_km / pp[3];

        if(fabs(pb) <= 1.0)
        {
            retRanges.maxValue_km = sqrt(1 - pb * pb) * ll[0];

            // Б Л И Ж Н Я Я   Г Р А Н И Ц А   З О Н Ы   П О Р А Ж Е Н И Я

            if(fabs(prm_km) > pp[4])
            {
                retRanges.minValue_km = sqrt(1 - pb * pb) * ll[2];
            }
            else
            {
                pb = prm_km / pp[4];
                retRanges.minValue_km = sqrt(1 - pb * pb) * ll[1];
            }
        }
        else    //  за пределами зоны
        {
            retRanges.minValue_km = -1001;
            retRanges.maxValue_km = -1001;
        }

        //qDebug()<<"ZonaPorazhRanges2   altit:"<<altit<<"prm_km:"<<prm_km<<"pp[3]:"<<pp[3]<<"pb:"<<pb<<"ll[0]:"<<ll[0]<<"retRanges.minValue_km:"<<retRanges.minValue_km<<"maxValue_km:"<<retRanges.maxValue_km;
        //qDebug()<<"glybina_ZP prm_km:"<<prm_km<<"altit:"<<altit;
        return retRanges;
    }
};

#endif // ZP_H
