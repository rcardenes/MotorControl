#include "selectserialdialog.h"
#include "ui_selectserialdialog.h"
#include <QSerialPortInfo>
#include <QStringListModel>
#include <QPushButton>

SelectSerialDialog::SelectSerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSerialDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->deviceNameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

SelectSerialDialog::~SelectSerialDialog()
{
    delete ui;
}

void
SelectSerialDialog::populate()
{
    QStringList list;

    for(auto info : QSerialPortInfo::availablePorts())
    {
        list.append(info.portName());
    }

    ui->deviceNameListView->setModel(new QStringListModel(list));
}

QString
SelectSerialDialog::getSelectedName()
{
    QModelIndex index(ui->deviceNameListView->currentIndex());
    return index.data().toString();
}

void SelectSerialDialog::on_deviceNameListView_clicked(const QModelIndex &index)
{
    QPushButton *ok = ui->buttonBox->button(QDialogButtonBox::Ok);

    ok->setEnabled(index.isValid());
}
