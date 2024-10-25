#ifndef REQUESTINPUTWINDOW_H
#define REQUESTINPUTWINDOW_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class RequestInputWindow;
}

/**
 * @brief A pop-up window for visitor to select visiting purpose
 */
class RequestInputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RequestInputWindow(QWidget *parent = nullptr);
    ~RequestInputWindow();

Q_SIGNALS:
    void buttonPressed(QString _purpose);
    void Done();

private Q_SLOTS:
    void on_pB_Visit_clicked();

    void on_pB_DropParcel_clicked();

    void on_pB_FoodDelivery_clicked();

    void on_pB_Services_clicked();

    void on_pB_PickUp_clicked();

    void on_pB_OtherReason_clicked();

    void on_pB_Back_clicked();

private:
    Ui::RequestInputWindow *ui;
    QString purpose = "";
    QSignalMapper *const signalMapper;
};

#endif // REQUESTINPUTWINDOW_H
