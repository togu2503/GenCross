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

    void ExecFunction(QThread * thread);

    void StopProcess();

    explicit WaitDialog(QWidget *parent = nullptr);
    ~WaitDialog();

private slots:
    void on_pushButton_clicked();


    void on_WaitDialog_finished(int result);

    void on_WaitDialog_rejected();

    void on_WaitDialog_destroyed();

private:
    Ui::WaitDialog *ui;

    QThread* m_Thread;

    bool m_bIsTerminated = false;
};

#endif // WAITDIALOG_H
