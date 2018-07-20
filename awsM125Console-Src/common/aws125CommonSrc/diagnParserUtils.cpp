#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::setIndicatorColor(QLabel *indicatorLabel, bool enabled, int indicatorType, unsigned char stateIndex)
{
    //bool res;

    if(enabled)
    {
        if((indicatorType == IndicatorIspravnost || indicatorType == IndicatorVkluchenia) && stateIndex == 0x01)
        {
            indicatorLabel->setPixmap(lightGreenPixmap);
        }
        else
        {
            if(indicatorType == IndicatorIspravnost && stateIndex == 0x00 || indicatorType == IndicatorNeIsprvnst && stateIndex == 0x01)
            {
                indicatorLabel->setPixmap(lightRedPixmap);
            }
            else
            {
                if(indicatorType == IndicatorNeIsprvnst && stateIndex == 0x00)
                    indicatorLabel->setPixmap(darkRedPixmap);
                else
                    indicatorLabel->setPixmap(darkGreenPixmap);
            }
        }
    }
    else
    {
        //res =
        indicatorLabel->setProperty("pixmap", QVariant());
    }

    //qDebug()<<"setIndicatorColor: "<<indicatorLabel->objectName()<<"enabled:"<<enabled<<"res:"<<res;
}


void MainWindow::setTemperature(unsigned char inputCh, QLabel *label)
{
    label->resize(4 + (10.0 + 110.0 / 255 * inputCh) / 132.0 * 159, label->height());
}


//void MainWindow::set6IndicatorColors(unsigned char inputCh, QLabel lbl0, QLabel lbl1, QLabel lbl2, QLabel lbl3, QLabel lbl4, QLabel lbl5)
//{
//    unsigned char maska0 = 0x01;
//    unsigned char maska1 = maska0 << 1;
//    unsigned char maska2 = maska1 << 1;
//    unsigned char maska3 = maska2 << 1;
//    unsigned char maska4 = maska3 << 1;
//    unsigned char maska5 = maska4 << 1;
//}


void MainWindow::fourVoltages(unsigned char inputCh, int startQFrameInd)
{
    unsigned char maskaA = 0x03;
    unsigned char maskaB = maskaA << 2;
    unsigned char maskaC = maskaB << 2;
    unsigned char maskaD = maskaC << 2;

    unsigned char resA = inputCh & maskaA;
    unsigned char resB = (inputCh & maskaB)>>2;
    unsigned char resC = (inputCh & maskaC)>>4;
    unsigned char resD = (inputCh & maskaD)>>6;

    QString c0 = "64";
    QString c1 = "255";

    //qDebug()<<"inputCh:"<<inputCh<<"resA:"<<resA<<"resB:"<<resB<<"resC:"<<resC;

    for(int i = 0; i < 4; i ++)
    {
        unsigned char nextVoltageValue;

        switch(i)
        {
            case 0: nextVoltageValue = resA; break;
            case 1: nextVoltageValue = resB; break;
            case 2: nextVoltageValue = resC; break;
            case 3: nextVoltageValue = resD;
        }

        QString r0, g1, g2, r3;

        switch(nextVoltageValue)
        {
            case 0: r0 = c1; g1 = c0; g2 = c0; r3 = c0; break;
            case 1: r0 = c0; g1 = c1; g2 = c0; r3 = c0; break;
            case 2: r0 = c0; g1 = c0; g2 = c1; r3 = c0; break;
            case 3: r0 = c0; g1 = c0; g2 = c0; r3 = c1; break;
            default: r0 = c0; g1 = c0; g2 = c0; r3 = c0;
        }

        QString stlStr = QString("QFrame{border-bottom: 1px solid #AAA099; border-top: 1px solid #222222; border-right: 1px solid #AAA099; border-left: 1px solid #444444;");

        stlStr += "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,";

        stlStr += "stop:0    rgba(" + r3 + ",28,0,0), stop:0.07 rgba(" + r3 + ",28,0,255), stop:0.23 rgba(" + r3 + ",28,0,255), stop:0.30 rgba(" + r3 + ",28,0,0),";
        stlStr += "stop:0.30 rgba(0," + g2 + ",0,0), stop:0.37 rgba(0," + g2 + ",0,255), stop:0.43 rgba(0," + g2 + ",0,255), stop:0.5 rgba(0," + g2 + ",0,0),";
        stlStr += "stop:0.50 rgba(0," + g1 + ",0,0), stop:0.57 rgba(0," + g1 + ",0,255), stop:0.63 rgba(0," + g1 + ",0,255), stop:0.7 rgba(0," + g1 + ",0,0),";
        stlStr += "stop:0.7  rgba(" + r0 + ",28,0,0), stop:0.77 rgba(" + r0 + ",28,0,255), stop:0.93 rgba(" + r0 + ",28,0,255), stop:1.00 rgba(" + r0 + ",28,0,0));";

        stlStr += "};";

        ////////////////////////voltageframes[startQFrameInd + i]->setStyleSheet(stlStr);
    }
}


void MainWindow::ABCphases(unsigned char inputCh, QFrame *frameA, QFrame *frameB, QFrame *frameC)
{
    unsigned char maskaA = 0x03;
    unsigned char maskaB = maskaA << 2;
    unsigned char maskaC = maskaB << 2;

    unsigned char resA = inputCh & maskaA;
    unsigned char resB = (inputCh & maskaB)>>2;
    unsigned char resC = (inputCh & maskaC)>>4;

    QString c0 = "64";
    QString c1 = "255";

    //qDebug()<<"inputCh:"<<inputCh<<"resA:"<<resA<<"resB:"<<resB<<"resC:"<<resC;

    for(int i = 0; i < 3; i ++)
    {
        QFrame *nextFrame;
        unsigned char nextVoltageValue;

        switch(i)
        {
            case 0: nextFrame = frameA; nextVoltageValue = resA; break;
            case 1: nextFrame = frameB; nextVoltageValue = resB; break;
            case 2: nextFrame = frameC; nextVoltageValue = resC;
        }

        QString r0, g1, g2, r3;

        switch(nextVoltageValue)
        {
            case 0: r0 = c1; g1 = c0; g2 = c0; r3 = c0; break;
            case 1: r0 = c0; g1 = c1; g2 = c0; r3 = c0; break;
            case 2: r0 = c0; g1 = c0; g2 = c1; r3 = c0; break;
            case 3: r0 = c0; g1 = c0; g2 = c0; r3 = c1; break;
            default: r0 = c0; g1 = c0; g2 = c0; r3 = c0;
        }

        QString stlStr = QString("QFrame{border-bottom: 1px solid #AAA099; border-top: 1px solid #222222; border-right: 1px solid #AAA099; border-left: 1px solid #444444;");

        stlStr += "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,";

        stlStr += "stop:0    rgba(" + r3 + ",28,0,0), stop:0.06 rgba(" + r3 + ",28,0,255), stop:0.24 rgba(" + r3 + ",28,0,255), stop:0.30 rgba(" + r3 + ",28,0,0),";
        stlStr += "stop:0.30 rgba(0," + g2 + ",0,0), stop:0.36 rgba(0," + g2 + ",0,255), stop:0.44 rgba(0," + g2 + ",0,255), stop:0.5 rgba(0," + g2 + ",0,0),";
        stlStr += "stop:0.50 rgba(0," + g1 + ",0,0), stop:0.56 rgba(0," + g1 + ",0,255), stop:0.64 rgba(0," + g1 + ",0,255), stop:0.7 rgba(0," + g1 + ",0,0),";
        stlStr += "stop:0.7  rgba(" + r0 + ",28,0,0), stop:0.76 rgba(" + r0 + ",28,0,255), stop:0.94 rgba(" + r0 + ",28,0,255), stop:1.00 rgba(" + r0 + ",28,0,0));";

        stlStr += "};";

        nextFrame->setStyleSheet(stlStr);
    }
}


