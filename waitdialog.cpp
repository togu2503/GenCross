#include "waitdialog.h"
#include "ui_waitdialog.h"
#include <QThread>

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    ui->setupUi(this);
}

void WaitDialog::ExecFunction(QThread* thread)
{
    m_bIsTerminated = false;
    m_Thread = thread;
    m_Thread ->start();
    QObject::connect(
                m_Thread , &QThread::finished,[&](){this->close();});
    exec();
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::StopProcess()
{
    if(!m_bIsTerminated)
    {
        m_Thread->terminate();
        m_bIsTerminated = true;
        this->setModal(false);
        this->clearFocus();
        this->close();
    }
}

void WaitDialog::on_pushButton_clicked()
{
    StopProcess();
}

