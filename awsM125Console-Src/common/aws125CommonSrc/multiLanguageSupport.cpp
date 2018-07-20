#include <QtGui>
#include <QMainWindow>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include "../../aws125CommonInclude/commonEnum.h"

//#pragma once

//enum WidgetTypes{GroupBoxWidget, PushButtonWidget, CheckBoxWidget, RadioButtonWidget, LabelWidget, WindowWidget};

void setCaption(QWidget *widget, int widgetType, int lngInd, QString textR, QString textE, QString textP)
{
    QString caption;

    switch(lngInd)
    {
        case english: caption = textE; break;
        //case russian: caption = textR; break;
        case russian: caption = QString::fromUtf8(textR.toStdString().c_str()); break;
        case portuguese: caption = textP; break;
        default: caption = "unKnownCaption";
    }

    switch(widgetType)
    {
        case GroupBoxWidget: ((QGroupBox*)widget)->setTitle(caption); break;
        case PushButtonWidget: ((QPushButton*)widget)->setText(caption); break;
        case CheckBoxWidget: ((QCheckBox*)widget)->setText(caption); break;
        case RadioButtonWidget: ((QRadioButton*)widget)->setText(caption); break;
        case LabelWidget: ((QLabel*)widget)->setText(caption); break;
        case WindowWidget: ((QMainWindow*)widget)->setWindowTitle(caption); break;

        default: qDebug()<<"setCaption unKnownwidgetType:"<<widgetType;
    }
}


//void MainWindow::passwordLanguageChange(int lng)
//{
//    ui->labelPassword->setAlignment(Qt::AlignVCenter);

//    if(fullAccess)
//    {
//        setCaption(ui->pushButtonLogInLogOut, PushButtonWidget, lng, "Выйти", "Log Out", "Sair");
//        setCaption(ui->labelPassword, LabelWidget, lng, "     полный доступ включён", "     full access permitted", "                 acesso permitido");
//    }
//    else
//    {
//        setCaption(ui->pushButtonLogInLogOut, PushButtonWidget, lng, "Войти", "Login", "Login");
//        setCaption(ui->labelPassword, LabelWidget, lng, "    пароль:", "   password:", "palavra-passe:");
//    }
//}


//void MainWindow::on_pushButtonFullScreen_clicked()
//{
//    //this->setWindowState(Qt::WindowFullScreen);
//    //this->setWindowState(Qt::window::WindowMaximized);
//    QString strNormalWin[3] = {"Window mode", "Оконный режим", "Modo de janela"};
//    QString strFullScreen[3] = {"Full screen", "Полный экран", "Modo de tela cheia"};

//    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
//    if(this->windowState() == Qt::WindowFullScreen)
//    {
//        ui->frameWindowTitle->move(0, 0);
//        ui->frameMain->move(0, 28);
//        ui->pushButtonFullScreen->setText(QString::fromUtf8(strNormalWin[settings->languageIndex].toStdString().c_str()));
//    }
//    else
//    {
//        ui->frameWindowTitle->move(0, -30);
//        ui->frameMain->move(0, 0);
//        ui->pushButtonFullScreen->setText(QString::fromUtf8(strFullScreen[settings->languageIndex].toStdString().c_str()));
//    }
//}
