#ifndef LENGTHDIALOG_H
#define LENGTHDIALOG_H

#include <QDialog>

namespace Ui {
class LengthDialog;
}

class LengthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LengthDialog(QWidget *parent = nullptr);

    bool ShowDialog(int* min, int* max, int* seed);

    ~LengthDialog();


private slots:

    void on_minValue_valueChanged(int arg1);

    void on_maxValue_valueChanged(int arg1);

    void on_Seed_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::LengthDialog *ui;

    bool m_bAccepted = false;
};

#endif // LENGTHDIALOG_H
