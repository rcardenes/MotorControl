#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectserialdialog.h"
#include "serialcomm.h"
#include "motormodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    next_connect(true)
{
    ui->setupUi(this);

    serialThread = new QThread(this);
    SerialComm *serialComms = new SerialComm();

    connect(
                this, &MainWindow::setSerialComms,
                serialComms, &SerialComm::setupSerialPort
    );

    connect(
                serialComms, &SerialComm::setup,
                this, &MainWindow::serialCommsSetupDone
    );

    connect(
                this, &MainWindow::sendCmd,
                serialComms, &SerialComm::sendCmd
    );

    connect(
                this, &MainWindow::connectClicked,
                serialComms, &SerialComm::connectToDevice
    );

    connect(
                this, &MainWindow::disconnectClicked,
                serialComms, &SerialComm::disconnectFromDevice
    );

    connect(
                serialComms, &SerialComm::connected,
                this, &MainWindow::doneConnecting
    );

    connect(
                serialComms, &SerialComm::disconnected,
                this, &MainWindow::doneDisconnecting
    );

    connect(
                serialComms, &SerialComm::log,
                this, &MainWindow::logMessage
    );

    serialThread->start();
    serialComms->moveToThread(serialThread);
}

MainWindow::~MainWindow()
{
    serialThread->exit();
    delete ui;
}

void MainWindow::on_targetSpeedSlider_valueChanged(int value)
{
    ui->targetSpeedSpinBox->setValue(double(value) / 10);
}

void MainWindow::on_targetSpeedSpinBox_valueChanged(double value)
{
    ui->targetSpeedSlider->setValue(int(value * 10));
    emit sendCmd(SerialComm::SliderSet, SerialComm::SetTargetSpeed, MotorModel::doubleToQ15(value));
}

void MainWindow::on_loopControlComboBox_currentIndexChanged(int index)
{
    emit sendCmd(SerialComm::SliderSet, SerialComm::SetLsw, static_cast<uint16_t>(index));
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    emit sendCmd(SerialComm::SliderSet, SerialComm::SetMotorEngage, checked ? 1: 0);
}

void MainWindow::on_setupPushButton_clicked()
{
    SelectSerialDialog* dialog;

    dialog = new SelectSerialDialog;
    dialog->populate();
    dialog->setModal(true);
    if (dialog->exec() == QDialog::Accepted)
    {
        emit setSerialComms(dialog->getSelectedName());
    }
    delete dialog;
}

void
MainWindow::serialCommsSetupDone(QString message)
{
    ui->connectPushButton->setEnabled(true);
    ui->serialInfoLineEdit->setText(message);
    QString text("Serial port set up to: ");
    logMessage(text + message);
}

void MainWindow::on_connectPushButton_clicked()
{
    if (next_connect) {
        emit this->connectClicked();
    }
    else {
        emit this->disconnectClicked();
    }
}

void MainWindow::doneConnecting()
{
    ui->connectPushButton->setText("Disconnect");
    next_connect = false;
    logMessage("Connected to the device");
}

void MainWindow::doneDisconnecting()
{
    ui->connectPushButton->setText("Connect");
    next_connect = true;
    logMessage("Disonnected from the device");
}

void MainWindow::logMessage(QString message)
{
    ui->logTextBrowser->append(message);
}
