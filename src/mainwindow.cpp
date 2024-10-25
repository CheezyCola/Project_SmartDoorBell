#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mRequestInputWindow(new RequestInputWindow(this))
    , mOpenCV_videoCapture(new MyVideoCapture(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("Smart Door Bell"));
    setWindowModality(Qt::ApplicationModal);

    ui->centralwidget->setLayout(ui->verticalLayout);
    ui->gBox_Controls->setLayout(ui->verticalLayout_2);

    mRequestInputWindow->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);

    msgBox->setStyleSheet("QLabel{min-width: 500px; font-size: 24px;}");

    // connect signals from Reuqest Input Window ui
    connect(mRequestInputWindow, &RequestInputWindow::buttonPressed, this, &MainWindow::setLBL);
    connect(mRequestInputWindow, &RequestInputWindow::Done, this, &MainWindow::showMainWindow);

    // connect signals from main window ui
    connect(mOpenCV_videoCapture, &MyVideoCapture::NotInRequest, this, [&]()
    {
        ui->pB_RequestPermission->setText("Request \nPermission");
    });
    connect(mOpenCV_videoCapture, &MyVideoCapture::displayHint, this, [&]()
    {
        ui->lbl_Instr->setText(mOpenCV_videoCapture->GetHint());
    });
    connect(mOpenCV_videoCapture, &MyVideoCapture::EXIT, this, [&]()
    {
        ui->menubar->setVisible(true);
        popMessageBox(mOpenCV_videoCapture->ErrorMsg);
        Q_EMIT exited();
    });
    connect(mOpenCV_videoCapture, &MyVideoCapture::newPixmapCapture, this, [&]()
    {
        ui->opencvFrame->setPixmap(mOpenCV_videoCapture->pixmap().scaled(ui->opencvFrame->width(),ui->opencvFrame->height(),Qt::KeepAspectRatio));
    });

    connect(mOpenCV_videoCapture, &MyVideoCapture::OnlineMode, this, &MainWindow::setOnlineMode);
    connect(mOpenCV_videoCapture, &MyVideoCapture::enableButtons, this, &MainWindow::setEnableButtons);
    connect(mOpenCV_videoCapture, &MyVideoCapture::Streaming, this, &MainWindow::setStreaming);
    connect(mOpenCV_videoCapture, &MyVideoCapture::showDoorBellFunctions, this, &MainWindow::setShowDoorBellFunctions);

    // On camera
    mOpenCV_videoCapture->start(QThread::HighPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete msgBox;
    mOpenCV_videoCapture->terminate();
    delete mOpenCV_videoCapture;
    delete mRequestInputWindow;
}

void MainWindow::showMainWindow()
{
    mRequestInputWindow->hide();
}

void MainWindow::setLBL(QString str)
{
    mOpenCV_videoCapture->Request = str.toStdString();
    ui->pB_RequestPermission->setText("Cancel \nRequest");
    if (mOpenCV_videoCapture->ToRequest == false){
        mOpenCV_videoCapture->ToRequest = true;
        ui->lbl_Instr->setText("Looking for a face to capture for request.");
    }
}

void MainWindow::on_pB_RequestPermission_clicked()
{
    if (ui->pB_RequestPermission->text() == "Request \nPermission"){
        mRequestInputWindow->show();
    }
    else if (ui->pB_RequestPermission->text() == "Cancel \nRequest"){
        mOpenCV_videoCapture->cancelRequest = true;
        ui->pB_RequestPermission->setText("Request \nPermission");
    }
}

void MainWindow::on_pB_RingDoorBell_clicked()
{
    if (!mOpenCV_videoCapture->ring) mOpenCV_videoCapture->ring = true;
}

void MainWindow::popMessageBox(QString msg)
{
    msgBox->setText(msg);
    msgBox->exec();

}

void MainWindow::setEnableButtons(){
    if (mOpenCV_videoCapture->BtnEnabled){
        ui->pB_RequestPermission->setEnabled(true);
        ui->pB_RingDoorBell->setEnabled(true);
        ui->pB_LeaveMessage->setEnabled(true);
    }
    else{
        ui->pB_RequestPermission->setEnabled(false);
        ui->pB_RingDoorBell->setEnabled(false);
        ui->pB_LeaveMessage->setEnabled(false);
    }
}

void MainWindow::setOnlineMode(){
    if (mOpenCV_videoCapture->Online){
        ui->lbl_OnOffline->setText("Online");
        ui->lbl_OnOffline->setStyleSheet("QLabel {color:rgb(0,255,0)}");
        ui->pB_RequestPermission->setEnabled(true);
    }
    else{
        ui->lbl_OnOffline->setText("Offline");
        ui->lbl_OnOffline->setStyleSheet("QLabel {color:rgb(255,0,0)}");
        ui->pB_RequestPermission->setEnabled(false);
    }
}

void MainWindow::setStreaming(){
    if (mOpenCV_videoCapture->stream_request){
        ui->lbl_Streaming->setText("Streaming");
    }
    else{
        ui->lbl_Streaming->setText(" ");
    }
}

void MainWindow::setShowDoorBellFunctions(){
    if (mOpenCV_videoCapture->showDBFunc){
        popMessageBox(QString("Stranger detected!!!\n Please help yourself with the menu."));
        ui->gBox_Controls->setVisible(true);
    }
    else{
        ui->gBox_Controls->setVisible(false);
    }
}

void MainWindow::on_pB_LeaveMessage_clicked()
{
    popMessageBox(QString("Function is not supported at the moment!"));
}
