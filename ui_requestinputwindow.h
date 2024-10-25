/********************************************************************************
** Form generated from reading UI file 'requestinputwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REQUESTINPUTWINDOW_H
#define UI_REQUESTINPUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RequestInputWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QFrame *line_2;
    QLabel *lbl;
    QFrame *line;
    QPushButton *pB_Visit;
    QPushButton *pB_DropParcel;
    QPushButton *pB_FoodDelivery;
    QPushButton *pB_Services;
    QPushButton *pB_PickUp;
    QPushButton *pB_OtherReason;
    QPushButton *pB_Back;

    void setupUi(QDialog *RequestInputWindow)
    {
        if (RequestInputWindow->objectName().isEmpty())
            RequestInputWindow->setObjectName(QString::fromUtf8("RequestInputWindow"));
        RequestInputWindow->setWindowModality(Qt::NonModal);
        RequestInputWindow->resize(460, 340);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(230, 243, 213, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        RequestInputWindow->setPalette(palette);
        RequestInputWindow->setAutoFillBackground(false);
        RequestInputWindow->setStyleSheet(QString::fromUtf8(""));
        RequestInputWindow->setSizeGripEnabled(false);
        RequestInputWindow->setModal(true);
        verticalLayoutWidget = new QWidget(RequestInputWindow);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 441, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        line_2 = new QFrame(verticalLayoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        lbl = new QLabel(verticalLayoutWidget);
        lbl->setObjectName(QString::fromUtf8("lbl"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbl->sizePolicy().hasHeightForWidth());
        lbl->setSizePolicy(sizePolicy);
        lbl->setMinimumSize(QSize(0, 25));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        lbl->setFont(font);
        lbl->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(lbl);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        line->setFont(font1);
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);

        verticalLayout->addWidget(line);

        pB_Visit = new QPushButton(verticalLayoutWidget);
        pB_Visit->setObjectName(QString::fromUtf8("pB_Visit"));
        sizePolicy.setHeightForWidth(pB_Visit->sizePolicy().hasHeightForWidth());
        pB_Visit->setSizePolicy(sizePolicy);
        pB_Visit->setMinimumSize(QSize(350, 0));
        pB_Visit->setMaximumSize(QSize(500, 16777215));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Quicksand Medium"));
        font2.setPointSize(14);
        pB_Visit->setFont(font2);
        pB_Visit->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_Visit);

        pB_DropParcel = new QPushButton(verticalLayoutWidget);
        pB_DropParcel->setObjectName(QString::fromUtf8("pB_DropParcel"));
        sizePolicy.setHeightForWidth(pB_DropParcel->sizePolicy().hasHeightForWidth());
        pB_DropParcel->setSizePolicy(sizePolicy);
        pB_DropParcel->setMinimumSize(QSize(350, 0));
        pB_DropParcel->setMaximumSize(QSize(500, 16777215));
        pB_DropParcel->setFont(font2);
        pB_DropParcel->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_DropParcel);

        pB_FoodDelivery = new QPushButton(verticalLayoutWidget);
        pB_FoodDelivery->setObjectName(QString::fromUtf8("pB_FoodDelivery"));
        sizePolicy.setHeightForWidth(pB_FoodDelivery->sizePolicy().hasHeightForWidth());
        pB_FoodDelivery->setSizePolicy(sizePolicy);
        pB_FoodDelivery->setMinimumSize(QSize(350, 0));
        pB_FoodDelivery->setMaximumSize(QSize(500, 16777215));
        pB_FoodDelivery->setFont(font2);
        pB_FoodDelivery->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_FoodDelivery);

        pB_Services = new QPushButton(verticalLayoutWidget);
        pB_Services->setObjectName(QString::fromUtf8("pB_Services"));
        sizePolicy.setHeightForWidth(pB_Services->sizePolicy().hasHeightForWidth());
        pB_Services->setSizePolicy(sizePolicy);
        pB_Services->setMinimumSize(QSize(350, 0));
        pB_Services->setMaximumSize(QSize(500, 16777215));
        pB_Services->setFont(font2);
        pB_Services->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_Services);

        pB_PickUp = new QPushButton(verticalLayoutWidget);
        pB_PickUp->setObjectName(QString::fromUtf8("pB_PickUp"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pB_PickUp->sizePolicy().hasHeightForWidth());
        pB_PickUp->setSizePolicy(sizePolicy1);
        pB_PickUp->setMinimumSize(QSize(350, 0));
        pB_PickUp->setMaximumSize(QSize(500, 16777215));
        pB_PickUp->setFont(font2);
        pB_PickUp->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_PickUp);

        pB_OtherReason = new QPushButton(verticalLayoutWidget);
        pB_OtherReason->setObjectName(QString::fromUtf8("pB_OtherReason"));
        sizePolicy1.setHeightForWidth(pB_OtherReason->sizePolicy().hasHeightForWidth());
        pB_OtherReason->setSizePolicy(sizePolicy1);
        pB_OtherReason->setMinimumSize(QSize(350, 0));
        pB_OtherReason->setMaximumSize(QSize(500, 16777215));
        pB_OtherReason->setFont(font2);
        pB_OtherReason->setStyleSheet(QString::fromUtf8("color:rgb(0, 170, 0)"));

        verticalLayout->addWidget(pB_OtherReason);

        pB_Back = new QPushButton(verticalLayoutWidget);
        pB_Back->setObjectName(QString::fromUtf8("pB_Back"));
        sizePolicy1.setHeightForWidth(pB_Back->sizePolicy().hasHeightForWidth());
        pB_Back->setSizePolicy(sizePolicy1);
        pB_Back->setMinimumSize(QSize(350, 0));
        pB_Back->setMaximumSize(QSize(500, 16777215));
        pB_Back->setFont(font2);
        pB_Back->setStyleSheet(QString::fromUtf8("color:rgb(255, 0, 0)"));

        verticalLayout->addWidget(pB_Back);


        retranslateUi(RequestInputWindow);

        QMetaObject::connectSlotsByName(RequestInputWindow);
    } // setupUi

    void retranslateUi(QDialog *RequestInputWindow)
    {
        RequestInputWindow->setWindowTitle(QCoreApplication::translate("RequestInputWindow", "Select Your Purpose", nullptr));
        lbl->setText(QCoreApplication::translate("RequestInputWindow", " Please select your purpose... ", nullptr));
        pB_Visit->setText(QCoreApplication::translate("RequestInputWindow", "Visit", nullptr));
        pB_DropParcel->setText(QCoreApplication::translate("RequestInputWindow", "Drop Parcel(s)", nullptr));
        pB_FoodDelivery->setText(QCoreApplication::translate("RequestInputWindow", "Food Delivery", nullptr));
        pB_Services->setText(QCoreApplication::translate("RequestInputWindow", "Services (Plumbers, Electricians, etc)", nullptr));
        pB_PickUp->setText(QCoreApplication::translate("RequestInputWindow", "Pick Up Item(s)", nullptr));
        pB_OtherReason->setText(QCoreApplication::translate("RequestInputWindow", "Other Reason", nullptr));
        pB_Back->setText(QCoreApplication::translate("RequestInputWindow", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RequestInputWindow: public Ui_RequestInputWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQUESTINPUTWINDOW_H
