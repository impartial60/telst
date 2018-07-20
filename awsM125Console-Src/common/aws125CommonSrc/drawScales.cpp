#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"

const int scaleHeight = 34;
const int scaleWidth = 34;
extern float convert360angle2PlusMinus180(float dltAz360);

void MainWindow::drawAzimuthScales(float azCur360)
{
    ui->azimuthLabel->setText(QString::number(azCur360, 'f', 2) + QString::fromUtf8("°"));

    float drctrAz180 = convert360angle2PlusMinus180(console2User.snrInfo.directrisaAzimuth);
    float crntAz180 = convert360angle2PlusMinus180(console2User.snrInfo.currentAzimuth);
    float azBlue180 = convert360angle2PlusMinus180(drctrAz180 - crntAz180);

    drawScale360(drctrAz180, azBlue180);
    drawScale3600(drctrAz180, azBlue180);
}


void MainWindow::drawElevationScale870(float elevationAngleCur)
{
    QPainter painter(&scaleInfo->pixmapElevationScale870);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, scaleInfo->pixmapElevationScale870.width(), scaleInfo->pixmapElevationScale870.height());

    int srcY = (int)(0.5 * scaleInfo->pixmapElevationScale870.height() - 30.0 * (88.0 - elevationAngleCur));

    painter.drawImage(0, srcY, scaleInfo->elevSmallScalePicture);

    int scaleCenterY = scaleInfo->pixmapElevationScale870.height() / 2 + 0.5 ;/////// - 0.5;// + 0.5;

    if(ui->elevationScale87->visibleMarker || ui->elevationScale870->visibleMarker)
    {
        int scrAzH = scaleInfo->pixmapElevationScale870.height() / 2 - scaleInfo->detailScaleHoverX;
        //int scrAzH = heightAz / 2 + arm->detailScaleHoverX;

        QPen pen2H(QColor(64,255,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen2H);
        painter.drawLine(0, scrAzH, 29, scrAzH);//ui->azimuthScale360->height() - 10);
        QPen penH(Qt::green);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(penH);
        painter.drawLine(0, scrAzH, 29, scrAzH);// ui->azimuthScale360->height());
    }

    QPen pen2(QColor(255,64,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen2);
    painter.drawLine(0, scaleCenterY, 19, scaleCenterY);//ui->azimuthScale360->height() - 10);
    QPen pen(Qt::red);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(0, scaleCenterY, ui->azimuthScale360->width(), scaleCenterY);

    ui->elevationScale870->setPixmap(scaleInfo->pixmapElevationScale870);
}


void MainWindow::drawElevationScale87(float elevationAngleCur)
{
    ui->elevAngleLabel->setText(QString::number(elevationAngleCur, 'f', 2) + QString::fromUtf8("°"));

    QPainter painter(&scaleInfo->pixmapElevationScale87);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, scaleInfo->pixmapElevationScale87.width(), scaleInfo->pixmapElevationScale87.height());

    int srcY = (int)(0.5 * scaleInfo->pixmapElevationScale87.height() - 3.0 * (100.0 - elevationAngleCur));

    painter.drawImage(0, srcY, scaleInfo->elevMainScalePicture);

    int scaleCenterY = scaleInfo->pixmapElevationScale87.height() / 2 + 0.5 ;/////// - 0.5;// + 0.5;


    if(ui->elevationScale87->visibleMarker || ui->elevationScale870->visibleMarker)
    {
        int scrAzH = scaleInfo->pixmapElevationScale87.height() / 2 - scaleInfo->mainScaleHoverX;
        //int scrAzH = heightAz / 2 + arm->detailScaleHoverX;

        QPen pen2H(QColor(64,255,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen2H);
        painter.drawLine(0, scrAzH, 29, scrAzH);//ui->azimuthScale360->height() - 10);
        QPen penH(Qt::green);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(penH);
        painter.drawLine(0, scrAzH, 29, scrAzH);// ui->azimuthScale360->height());
    }

    QPen pen2(QColor(255,64,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen2);
    painter.drawLine(0, scaleCenterY, 19, scaleCenterY);//ui->azimuthScale360->height() - 10);
    QPen pen(Qt::red);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(0, scaleCenterY, ui->azimuthScale360->width(), scaleCenterY);

    ui->elevationScale87->setPixmap(scaleInfo->pixmapElevationScale87);
}


void MainWindow::drawScale360(float drctrAz, float azBlue)
{
    //qDebug()<<"drawScale360 directrisaAzimuth:"<<shMemC2U.snrInfo.directrisaAzimuth<<"azCur:"<<azCur<<"currentAzimuth:"<<shMemC2U.snrInfo.currentAzimuth<<"azCurScan:"<<azCurScan;

    QPainter painter(&scaleInfo->pixmapScale360);

    int widthBmpScale = 1080;
    int widthAz = scaleInfo->pixmapScale360.width();

    int srcX = (int)(0.5 * (widthBmpScale - widthAz) + 3.0 * drctrAz);// - + 0.5;

    QRect target(0, 0, widthBmpScale - srcX, scaleHeight);
    QRect source(srcX, 0, widthBmpScale - srcX, scaleHeight);
    painter.drawImage(target, scaleInfo->azmtMainScalePicture, source);
    //printf("srcX: %f\n", srcX);

    if(srcX < 0)
    {
        QRect target2(0, 0, - srcX, scaleHeight);
        QRect source2(widthBmpScale + srcX, 0, -srcX, scaleHeight);
        painter.drawImage(target2, scaleInfo->azmtMainScalePicture, source2);
    }

    if(srcX > widthBmpScale - widthAz)
    {
        QRect target2(widthBmpScale - srcX, 0, widthAz - widthBmpScale + srcX, scaleHeight);
        QRect source2(0, 0, widthAz - widthBmpScale + srcX, scaleHeight);
        painter.drawImage(target2, scaleInfo->azmtMainScalePicture, source2);
    }

    if(ui->azimuthScale360->visibleMarker || ui->azimuthScale3600->visibleMarker)
    {
        int scrAzH = widthAz / 2 + scaleInfo->mainScaleHoverX;//+  azH * 3 - 0.5 ;// + 0.5;
        QPen pen2H(QColor(64,255,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen2H);
        painter.drawLine(scrAzH, 0, scrAzH, 19);
        QPen penH(Qt::green);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(penH);
        painter.drawLine(scrAzH, 0, scrAzH, scaleHeight - 1);
    }

    int scaleCenterX = widthAz / 2 + 0.5 ;

    int blueX = scaleCenterX - 3.0 * azBlue;
    painter.setPen(QPen(QColor(0, 64, 255, 144), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    painter.drawLine(blueX, 0, blueX, 22);
    painter.setPen(QPen(QColor(0, 64, 255, 255)));
    painter.drawLine(blueX, 0, blueX, 30);

    QPen pen(Qt::red);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(scaleCenterX, 0, scaleCenterX, ui->azimuthScale360->height());

    ui->azimuthScale360->setPixmap(scaleInfo->pixmapScale360);
}


void MainWindow::drawScale3600(float drctrAz, float azBlue)
{
    float azCurScan = console2User.snrInfo.currentAzimuth;//azCur;
    if(azCurScan > 180)
        azCurScan -= 360;

    QPainter painter(&scaleInfo->pixmapScale3600);

    int widthBmpScale = 10800;
    int widthAz = scaleInfo->pixmapScale3600.width();

    int srcX = 0.5 * (widthBmpScale - widthAz) + 30 * drctrAz;// - + 0.5;

    QRect target(0, 0, widthBmpScale - srcX, scaleHeight);
    QRect source(srcX, 0, widthBmpScale - srcX, scaleHeight);
    painter.drawImage(target, scaleInfo->azmtSmallScalePicture, source);
    //printf("srcX: %f\n", srcX);

    if(srcX < 0)
    {
        QRect target2(0, 0, - srcX, scaleHeight);
        QRect source2(widthBmpScale + srcX, 0, -srcX, scaleHeight);
        painter.drawImage(target2, scaleInfo->azmtSmallScalePicture, source2);
    }

    if(srcX > widthBmpScale - widthAz)
    {
        QRect target2(widthBmpScale - srcX, 0, widthAz - widthBmpScale + srcX, scaleHeight);
        QRect source2(0, 0, widthAz - widthBmpScale + srcX, scaleHeight);
        painter.drawImage(target2, scaleInfo->azmtSmallScalePicture, source2);
    }

    if(ui->azimuthScale3600->visibleMarker || ui->azimuthScale360->visibleMarker)
    {
        int scrAzH = widthAz / 2 + scaleInfo->detailScaleHoverX;

        QPen pen2H(QColor(64,255,64,64), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen2H);
        painter.drawLine(scrAzH, 0, scrAzH, 19);
        QPen penH(Qt::green);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(penH);
        painter.drawLine(scrAzH, 0, scrAzH, scaleHeight - 1);
    }

    int scaleCenterX = widthAz / 2 + 0.5;

    int blueX = scaleCenterX - 30.0 * azBlue;
    painter.setPen(QPen(QColor(0, 64, 255, 144), 21, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    painter.drawLine(blueX, 0, blueX, 19);
    painter.setPen(QPen(QColor(0, 64, 255, 255)));
    painter.drawLine(blueX, 0, blueX, 28);

    QPen pen(Qt::red);//, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(scaleCenterX, 0, scaleCenterX, ui->azimuthScale3600->height());

    ui->azimuthScale3600->setPixmap(scaleInfo->pixmapScale3600);
}
