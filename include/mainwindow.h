#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "myvideocapture.h"
#include "requestinputwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief QT MainWindow of Smart Door Bell UI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    /**
     * @brief exited: Signal to call for program termination
     */
    void exited();

private Q_SLOTS:
    void on_pB_RequestPermission_clicked();
    void on_pB_RingDoorBell_clicked();

    void on_pB_LeaveMessage_clicked();

public Q_SLOTS:
    void showMainWindow();
    void setLBL(QString str);
    void setEnableButtons();
    /**
     * @brief setOnlineMode: Set the label text to display current mode of online or offline
     */
    void setOnlineMode();
    /**
     * @brief setStreaming: Set the label text to display current mode of streaming (on or off)
     */
    void setStreaming();
    /**
     * @brief setShowDoorBellFunctions: Show the menu panel for visitor
     */
    void setShowDoorBellFunctions();

private:
    Ui::MainWindow *ui;
    QMessageBox *msgBox = new QMessageBox();

    RequestInputWindow *const mRequestInputWindow;
    MyVideoCapture *const mOpenCV_videoCapture;
    void popMessageBox(QString msg);
};
#endif // MAINWINDOW_H
