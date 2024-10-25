/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_9;
    QLabel *lbl_OnOffline;
    QSpacerItem *horizontalSpacer;
    QLabel *lbl_Instr;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lbl_Streaming;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *gBox_Controls;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pB_RequestPermission;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pB_RingDoorBell;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pB_LeaveMessage;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *opencvFrame;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1061, 703);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(218, 248, 254, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::ForceTabbedDocks);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setEnabled(true);
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 1041, 661));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_9 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        lbl_OnOffline = new QLabel(verticalLayoutWidget);
        lbl_OnOffline->setObjectName(QString::fromUtf8("lbl_OnOffline"));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        lbl_OnOffline->setFont(font);

        horizontalLayout->addWidget(lbl_OnOffline);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lbl_Instr = new QLabel(verticalLayoutWidget);
        lbl_Instr->setObjectName(QString::fromUtf8("lbl_Instr"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbl_Instr->sizePolicy().hasHeightForWidth());
        lbl_Instr->setSizePolicy(sizePolicy);
        lbl_Instr->setMinimumSize(QSize(0, 35));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        lbl_Instr->setFont(font1);
        lbl_Instr->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbl_Instr);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        lbl_Streaming = new QLabel(verticalLayoutWidget);
        lbl_Streaming->setObjectName(QString::fromUtf8("lbl_Streaming"));
        lbl_Streaming->setFont(font);
        lbl_Streaming->setStyleSheet(QString::fromUtf8("color:rgb(0, 0, 212)"));

        horizontalLayout->addWidget(lbl_Streaming);

        horizontalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(138, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(5, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        gBox_Controls = new QGroupBox(verticalLayoutWidget);
        gBox_Controls->setObjectName(QString::fromUtf8("gBox_Controls"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gBox_Controls->sizePolicy().hasHeightForWidth());
        gBox_Controls->setSizePolicy(sizePolicy1);
        gBox_Controls->setMinimumSize(QSize(200, 0));
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setUnderline(false);
        font2.setWeight(75);
        gBox_Controls->setFont(font2);
        gBox_Controls->setContextMenuPolicy(Qt::DefaultContextMenu);
        gBox_Controls->setAutoFillBackground(false);
        gBox_Controls->setAlignment(Qt::AlignCenter);
        verticalLayoutWidget_2 = new QWidget(gBox_Controls);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 30, 181, 551));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(10, 0, 10, 0);
        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        pB_RequestPermission = new QPushButton(verticalLayoutWidget_2);
        pB_RequestPermission->setObjectName(QString::fromUtf8("pB_RequestPermission"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pB_RequestPermission->sizePolicy().hasHeightForWidth());
        pB_RequestPermission->setSizePolicy(sizePolicy2);
        pB_RequestPermission->setMinimumSize(QSize(115, 70));
        pB_RequestPermission->setFont(font2);
        pB_RequestPermission->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 125)"));

        verticalLayout_2->addWidget(pB_RequestPermission);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        pB_RingDoorBell = new QPushButton(verticalLayoutWidget_2);
        pB_RingDoorBell->setObjectName(QString::fromUtf8("pB_RingDoorBell"));
        sizePolicy2.setHeightForWidth(pB_RingDoorBell->sizePolicy().hasHeightForWidth());
        pB_RingDoorBell->setSizePolicy(sizePolicy2);
        pB_RingDoorBell->setMinimumSize(QSize(115, 70));
        pB_RingDoorBell->setFont(font2);
        pB_RingDoorBell->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 125)"));

        verticalLayout_2->addWidget(pB_RingDoorBell);

        verticalSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        pB_LeaveMessage = new QPushButton(verticalLayoutWidget_2);
        pB_LeaveMessage->setObjectName(QString::fromUtf8("pB_LeaveMessage"));
        sizePolicy2.setHeightForWidth(pB_LeaveMessage->sizePolicy().hasHeightForWidth());
        pB_LeaveMessage->setSizePolicy(sizePolicy2);
        pB_LeaveMessage->setMinimumSize(QSize(115, 70));
        pB_LeaveMessage->setFont(font2);
        pB_LeaveMessage->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 125)"));

        verticalLayout_2->addWidget(pB_LeaveMessage);

        verticalSpacer_6 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_6);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(2, 2);
        verticalLayout_2->setStretch(4, 2);
        verticalLayout_2->setStretch(6, 1);

        verticalLayout_4->addWidget(gBox_Controls);


        horizontalLayout_3->addLayout(verticalLayout_4);

        horizontalSpacer_7 = new QSpacerItem(3, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        opencvFrame = new QLabel(verticalLayoutWidget);
        opencvFrame->setObjectName(QString::fromUtf8("opencvFrame"));
        sizePolicy2.setHeightForWidth(opencvFrame->sizePolicy().hasHeightForWidth());
        opencvFrame->setSizePolicy(sizePolicy2);
        opencvFrame->setMinimumSize(QSize(320, 240));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(225, 240, 249, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        opencvFrame->setPalette(palette1);
        opencvFrame->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(opencvFrame);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 100);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer_8 = new QSpacerItem(248, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_8);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 100);
        verticalLayout_3->setStretch(2, 1);

        horizontalLayout_3->addLayout(verticalLayout_3);

        horizontalSpacer_6 = new QSpacerItem(5, 268, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 1);
        horizontalLayout_3->setStretch(2, 1);
        horizontalLayout_3->setStretch(3, 100);
        horizontalLayout_3->setStretch(4, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(98, 4, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer_2);

        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1061, 27));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lbl_OnOffline->setText(QString());
        lbl_Instr->setText(QCoreApplication::translate("MainWindow", "Instruction", nullptr));
        lbl_Streaming->setText(QString());
        gBox_Controls->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        pB_RequestPermission->setText(QCoreApplication::translate("MainWindow", "Request \n"
"Permission", nullptr));
        pB_RingDoorBell->setText(QCoreApplication::translate("MainWindow", "Ring \n"
"Door Bell", nullptr));
        pB_LeaveMessage->setText(QCoreApplication::translate("MainWindow", "Leave message\n"
"(unimplemented)", nullptr));
        opencvFrame->setText(QCoreApplication::translate("MainWindow", "Camera", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
