#ifndef CREATECROSSWORDDLG_H
#define CREATECROSSWORDDLG_H

#include <QDialog>
#include "BoardBuilder.h"

namespace Ui {
class CreateCrosswordDlg;
}

class CreateCrosswordDlg : public QDialog
{
    Q_OBJECT

public:

    Board CreateBoard();

    explicit CreateCrosswordDlg(QWidget *parent = nullptr);
    ~CreateCrosswordDlg();
    void SetWidth(unsigned int width);
    void SetHeight(unsigned int height);
    unsigned int GetWidth();
    unsigned int GetHeight();



private slots:

    void on_HeightSpinBox_valueChanged(int arg1);

    void on_WidthSpinBox_valueChanged(int arg1);

private:
    unsigned int m_Width;
    unsigned int m_Height;
    Ui::CreateCrosswordDlg *ui;
};

#endif // CREATECROSSWORDDLG_H
