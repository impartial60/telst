#include <QMainWindow>
#include <QString>
#include <QTime>
#include <QImage>
#include <QDebug>

#include "qlabelm.h"

#ifndef SCALEINFO
#define SCALEINFO


class SCALE_INFO  :  public //QMainWindow,
        qLabelM
{
    //Q_OBJECT

    public:

    int mainScaleHoverX;
    int detailScaleHoverX;    

    QImage azmtMainScalePicture;
    QImage azmtSmallScalePicture;

    QImage elevMainScalePicture;
    QImage elevSmallScalePicture;

    QPixmap pixmapScale360;
    QPixmap pixmapScale3600;
    QPixmap pixmapElevationScale87;
    QPixmap pixmapElevationScale870;

    SCALE_INFO()//Ui::qLabelM *qM)
    {
        azmtMainScalePicture =  QImage(":/AzScales/resgui/AzimuthScales/g360.png", 0);
        azmtSmallScalePicture = QImage(":/AzScales/resgui/AzimuthScales/gLong.png", 0);

        elevMainScalePicture =  QImage(":/AzScales/resgui/AzimuthScales/gElev87.png", 0);
        elevSmallScalePicture =  QImage(":/AzScales/resgui/AzimuthScales/gElev870.png", 0);

        pixmapScale360 = QPixmap(1130,40);
        pixmapScale3600 = QPixmap(596,30);

        pixmapElevationScale87 = QPixmap(40, 480);
        pixmapElevationScale870 = QPixmap(30, 596);

        //connect(qM, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
	}

//    void MouseMoveScales(int hovMain, int hovDelt)
//    {
//        detailScaleHoverX = hovDelt;
//        mainScaleHoverX = hovMain;
//        qDebug()<<"hovMain:"<<hovMain;

//    }

};


#endif
