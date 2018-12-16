#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QThread"
#include "serialcomm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void setSerialComms(QString);
    void sendCmd(SerialComm::Command, uint8_t, uint16_t);
    void connectClicked();
    void disconnectClicked();

public slots:
    void serialCommsSetupDone(QString);
    void doneConnecting(void);
    void doneDisconnecting(void);
    void logMessage(QString message);

private slots:
    void on_targetSpeedSlider_valueChanged(int value);

    void on_targetSpeedSpinBox_valueChanged(double arg1);

    void on_loopControlComboBox_currentIndexChanged(int index);

    void on_checkBox_toggled(bool checked);

    void on_setupPushButton_clicked();

    void on_connectPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QThread *serialThread;
    bool next_connect;
};

#endif // MAINWINDOW_H
