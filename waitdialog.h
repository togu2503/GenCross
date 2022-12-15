#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>

namespace Ui {
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

public:

    explicit WaitDialog(QWidget *parent = nullptr);
    ~WaitDialog();

    void ExecFunction(QThread * thread);

    void StopProcess();

private slots:
    void on_pushButton_clicked();
    void on_WaitDialog_rejected();

private:
    Ui::WaitDialog *ui;

    QThread* m_Thread;

    bool m_bIsTerminated = false;
};

#endif // WAITDIALOG_H
