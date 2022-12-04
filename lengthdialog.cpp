#include "lengthdialog.h"
#include <algorithm>
#include "ui_lengthdialog.h"

LengthDialog::LengthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LengthDialog)
{
    ui->setupUi(this);
}

LengthDialog::~LengthDialog()
{
    delete ui;
}

void LengthDialog::on_minValue_valueChanged(int arg1)
{
    ui->minValue->setValue(std::clamp(arg1, 3, ui->maxValue->value()));
}

bool LengthDialog::ShowDialog(int* min, int* max, int* seed)
{
    exec();

    if(m_bAccepted)
    {
        *min = ui->minValue->value();
        *max = ui->maxValue->value();
        *seed = ui->Seed->value();
    }
    return m_bAccepted;
}

void LengthDialog::on_maxValue_valueChanged(int arg1)
{
    ui->maxValue->setValue(std::clamp(arg1, ui->minValue->value(), 10));
}


void LengthDialog::on_Seed_valueChanged(int arg1)
{
    ui->Seed->setValue(arg1 > 0 ? arg1 : 0);
}


void LengthDialog::on_buttonBox_accepted()
{
    m_bAccepted = true;
}

