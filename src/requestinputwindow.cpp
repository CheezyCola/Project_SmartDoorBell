#include "requestinputwindow.h"
#include "ui_requestinputwindow.h"

RequestInputWindow::RequestInputWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RequestInputWindow),
    signalMapper(new QSignalMapper(this))
{
    ui->setupUi(this);

    //connect signals
    connect(ui->pB_Visit, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_DropParcel, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_FoodDelivery, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_Services, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_PickUp, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_OtherReason, &QPushButton::clicked, this, &RequestInputWindow::Done);
    connect(ui->pB_Back, &QPushButton::clicked, this, &RequestInputWindow::Done);

    connect(ui->pB_Visit, SIGNAL(clicked()), signalMapper,SLOT(map()));
    connect(ui->pB_DropParcel, SIGNAL(clicked()), signalMapper,SLOT(map()));
    connect(ui->pB_FoodDelivery, SIGNAL(clicked()), signalMapper,SLOT(map()));
    connect(ui->pB_Services, SIGNAL(clicked()), signalMapper,SLOT(map()));
    connect(ui->pB_PickUp, SIGNAL(clicked()), signalMapper,SLOT(map()));
    connect(ui->pB_OtherReason, SIGNAL(clicked()), signalMapper,SLOT(map()));

    connect(signalMapper, &QSignalMapper::mappedString, this, &RequestInputWindow::buttonPressed);
}

RequestInputWindow::~RequestInputWindow()
{
    delete ui;
}

void RequestInputWindow::on_pB_Visit_clicked()
{
    purpose = "To visit someone";
    signalMapper->setMapping(ui->pB_Visit, purpose);
}

void RequestInputWindow::on_pB_DropParcel_clicked()
{
    purpose = "To drop or handover a parcel";
    signalMapper->setMapping(ui->pB_DropParcel, purpose);
}

void RequestInputWindow::on_pB_FoodDelivery_clicked()
{
    purpose = "To deliver food";
    signalMapper->setMapping(ui->pB_FoodDelivery, purpose);
}

void RequestInputWindow::on_pB_Services_clicked()
{
    purpose = "To provide service";
    signalMapper->setMapping(ui->pB_Services, purpose);
}

void RequestInputWindow::on_pB_PickUp_clicked()
{
    purpose = "To pick up something";
    signalMapper->setMapping(ui->pB_PickUp, purpose);
}

void RequestInputWindow::on_pB_OtherReason_clicked()
{
    purpose = "Other reason";
    signalMapper->setMapping(ui->pB_OtherReason, purpose);
}

void RequestInputWindow::on_pB_Back_clicked()
{
    purpose = "";
}
