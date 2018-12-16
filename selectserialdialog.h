#ifndef SELECTSERIALDIALOG_H
#define SELECTSERIALDIALOG_H

#include <QDialog>

namespace Ui {
class SelectSerialDialog;
}

class SelectSerialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSerialDialog(QWidget *parent = nullptr);
    ~SelectSerialDialog();
    void populate();
    QString getSelectedName();

private slots:
    void on_deviceNameListView_clicked(const QModelIndex &index);

private:
    Ui::SelectSerialDialog *ui;
};

#endif // SELECTSERIALDIALOG_H
