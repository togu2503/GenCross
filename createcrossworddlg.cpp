#include "createcrossworddlg.h"
#include "ui_createcrossworddlg.h"

CreateCrosswordDlg::CreateCrosswordDlg(QWidget *parent) :
    QDialog(parent), m_Width(0), m_Height(0),
    ui(new Ui::CreateCrosswordDlg)
{
    ui->setupUi(this);
}

CreateCrosswordDlg::~CreateCrosswordDlg()
{
    delete ui;
}

Board CreateCrosswordDlg::CreateBoard()
{
    exec();
    return Board(GetHeight(),GetWidth());
}

unsigned int CreateCrosswordDlg::GetHeight()
{
    return m_Height;
}

unsigned int CreateCrosswordDlg::GetWidth()
{
    return m_Width;
}

void CreateCrosswordDlg::SetHeight(unsigned int height)
{
    m_Height = height;
    return;
}

void CreateCrosswordDlg::SetWidth(unsigned int width)
{
    m_Width = width;
}

void CreateCrosswordDlg::on_WidthSpinBox_valueChanged(int arg1)
{
    if(arg1<0)
        ui->WidthSpinBox->setValue(0);
    SetWidth(arg1);
}

void CreateCrosswordDlg::on_HeightSpinBox_valueChanged(int arg1)
{
    SetHeight(arg1);
}
